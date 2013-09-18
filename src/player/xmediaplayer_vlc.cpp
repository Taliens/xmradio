/*************************************************************************
**
** Copyright (C) 2013  Weitian Leung (weitianleung@gmail.com).
**
** This file is part of xmradio.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#include <vlc/vlc.h>
#include <cassert>
#include <algorithm>
#include <sstream>

#include "xmediaplayer_vlc.h"

XMR_BEGIN_NS

class VLCMediaPtr
{
public:
	VLCMediaPtr(libvlc_media_t *media)
		: _media(media)
	{}

	~VLCMediaPtr()
	{
		if (_media)
			libvlc_media_release(_media);
	}

	operator libvlc_media_t* () const
	{
		return _media;
	}

private:
	libvlc_media_t *_media;
};

XMediaPlayerVlc::XMediaPlayerVlc()
	: _callback(NULL)
	, _userdata(NULL)
	, _buffering(0.f)
	, _loopCount(0)
{
	_instance = libvlc_new(0, NULL);
	assert(_instance != NULL);

	_player = libvlc_media_player_new(_instance);
	assert(_player != NULL);
}

XMediaPlayerVlc::~XMediaPlayerVlc()
{
	while(!_eventList.empty())
		unwatch(_eventList[0]);

	stop();
	libvlc_media_player_release(_player);
	libvlc_release(_instance);
}

bool XMediaPlayerVlc::open(const std::string &location, bool autoplay /* = true */)
{
	// stop before we go further
	stop();

	VLCMediaPtr media = libvlc_media_new_location(_instance, location.c_str());
	if(media == NULL)
		return false;
	
	libvlc_media_player_set_media(_player, media);
	setRepeat();

	if (autoplay)
		return play();

	return true;
}

bool XMediaPlayerVlc::play()
{
	return libvlc_media_player_play(_player) == 0;
}

void XMediaPlayerVlc::pause()
{
	libvlc_media_player_pause(_player);
}

void XMediaPlayerVlc::playpause()
{
	if (isPlaying())
		pause();
	else
		play();
}

void XMediaPlayerVlc::resume()
{
	libvlc_media_player_set_pause(_player, 0);
}

void XMediaPlayerVlc::stop()
{
	libvlc_media_player_stop(_player);
}

bool XMediaPlayerVlc::isPlaying() const
{
	return libvlc_media_player_is_playing(_player) == 1;
}

bool XMediaPlayerVlc::isSeekable() const
{
	return libvlc_media_player_is_seekable(_player) == 1;
}

bool XMediaPlayerVlc::canPause() const
{
	return libvlc_media_player_can_pause(_player) == 1;
}

long long XMediaPlayerVlc::getTime() const
{
	return libvlc_media_player_get_time(_player);
}

void XMediaPlayerVlc::setTime(long long time)
{
	libvlc_media_player_set_time(_player, time);
}

int XMediaPlayerVlc::getLoopCount() const
{
	return _loopCount;
}

void XMediaPlayerVlc::setLoopCount(int count)
{
	_loopCount = count;
	setRepeat();
}

long long XMediaPlayerVlc::duration() const
{
	return libvlc_media_player_get_length(_player);
}

float XMediaPlayerVlc::buffering() const
{
	return _buffering;
}

int XMediaPlayerVlc::getVolume() const
{
	return libvlc_audio_get_volume(_player);
}

void XMediaPlayerVlc::setVolume(int volume)
{
	assert(volume >= 0 && volume <= 100);
	libvlc_audio_set_volume(_player, volume);
}

bool XMediaPlayerVlc::isMuted() const
{
	return libvlc_audio_get_mute(_player) == 1;
}

void XMediaPlayerVlc::setMuted(bool muted)
{
	libvlc_audio_set_mute(_player, muted ? 1 : 0);
}

XMediaPlayer::State XMediaPlayerVlc::state() const
{
	libvlc_state_t st = libvlc_media_player_get_state(_player);
	switch (st)
	{
	case libvlc_NothingSpecial:
		return Nothing;

	case libvlc_Opening:
		return Opening;

	case libvlc_Buffering:
		return Buffering;

	case libvlc_Playing:
		return Playing;

	case libvlc_Paused:
		return Paused;

	case libvlc_Stopped:
		return Stopped;
		
	default:
		return Nothing;
	}

	return Nothing;
}

void XMediaPlayerVlc::setEventCallback(callback_t callback, void *data)
{
	_callback = callback;
	_userdata = data;
}

bool XMediaPlayerVlc::watch(XMediaPlayer::Event type)
{
	libvlc_event_manager_t* em = libvlc_media_player_event_manager(_player);
	if (em == NULL)
		return false;
	
	std::vector<Event>::iterator it;
	it = std::find(_eventList.begin(), _eventList.end(), type);
	// already exists
	if (it != _eventList.end())
		return false;

	if (type == PlayerStateChanged)
	{
		if (libvlc_event_attach(em, libvlc_MediaPlayerPlaying, vlc_event_callback, this) != 0)
			return false;
		if (libvlc_event_attach(em, libvlc_MediaPlayerPaused, vlc_event_callback, this) != 0)
			return false;
		if (libvlc_event_attach(em, libvlc_MediaPlayerStopped, vlc_event_callback, this) != 0)
			return false;
	}
	else
	{
		if (libvlc_event_attach(em, x2v(type), vlc_event_callback, this) != 0)
			return false;
	}
	
	_eventList.push_back(type);

	return true;
}

void XMediaPlayerVlc::unwatch(XMediaPlayer::Event type)
{
	libvlc_event_manager_t* em = libvlc_media_player_event_manager(_player);
	if (em == NULL)
		return ;

	std::vector<Event>::iterator it;
	it = std::find(_eventList.begin(), _eventList.end(), type);
	if (it == _eventList.end())
		return ;

	if (type == PlayerStateChanged)
	{
		libvlc_event_detach(em, libvlc_MediaPlayerStopped, vlc_event_callback, this);
		libvlc_event_detach(em, libvlc_MediaPlayerPaused, vlc_event_callback, this);
		libvlc_event_detach(em, libvlc_MediaPlayerPlaying, vlc_event_callback, this);
	}
	else
	{
		libvlc_event_detach(em, x2v(type), vlc_event_callback, this);
	}

	_eventList.erase(it);
}

std::string XMediaPlayerVlc::errorString() const
{
	return libvlc_errmsg();
}

const char* XMediaPlayerVlc::name() const
{
	return "VLC";
}

inline XMediaPlayer::Event XMediaPlayerVlc::v2x(int event)
{
	switch (event)
	{
	case libvlc_MediaPlayerEncounteredError:
		return PlayerError;

	case libvlc_MediaPlayerMediaChanged:
		return PlayerMediaChanged;

	case libvlc_MediaPlayerBuffering:
		return PlayerBuffering;

	case libvlc_MediaPlayerTimeChanged:
		return PlayerTimeChanged;

	case libvlc_MediaPlayerEndReached:
		return PlayerEndReadched;
		
	case libvlc_MediaPlayerPlaying:
	case libvlc_MediaPlayerPaused:	
	case libvlc_MediaPlayerStopped:
		return PlayerStateChanged;
	}

	return NoEvent;
}

inline int XMediaPlayerVlc::x2v(Event event)
{
	switch (event)
	{
	case PlayerError:
		return libvlc_MediaPlayerEncounteredError;

	case PlayerMediaChanged:
		return libvlc_MediaPlayerMediaChanged;

	case PlayerBuffering:
		return libvlc_MediaPlayerBuffering;

	case PlayerStateChanged:
		// should not convert this one
		return -1;

	case PlayerTimeChanged:
		return libvlc_MediaPlayerTimeChanged;

	case PlayerEndReadched:
		return libvlc_MediaPlayerEndReached;
		
	case NoEvent:
		return -1;
	}
	
	return -1;
}

void XMediaPlayerVlc::setRepeat()
{
	libvlc_media_t *media;
	
	media = libvlc_media_player_get_media(_player);
	if (media == NULL)
		return ;

	std::stringstream ss;
	ss << "input-repeat=" << (_loopCount == XMediaPlayer::Infinite ? -1 : _loopCount);

	libvlc_media_add_option(media, ss.str().c_str());
}

void XMediaPlayerVlc::vlc_event_callback(const libvlc_event_t *event, void *userdata)
{
	XMediaPlayerVlc *player = static_cast<XMediaPlayerVlc*>(userdata);
	assert(player);

	if (event->type == libvlc_MediaPlayerBuffering)
	{
		player->_buffering = event->u.media_player_buffering.new_cache;
	}

	Event type = player->v2x(event->type);
	if (type != NoEvent && player->_callback)
		player->_callback(type, player->_userdata);
}

XMR_END_NS
