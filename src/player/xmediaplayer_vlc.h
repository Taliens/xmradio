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

#ifndef __XMRADIO_MEDIA_PLAYER_VLC_H__
#define __XMRADIO_MEDIA_PLAYER_VLC_H__

#include <vector>

#include "xmediaplayer.h"

struct libvlc_instance_t;
struct libvlc_media_player_t;

XMR_BEGIN_NS

class XMR_PLAYER_EXPORT XMediaPlayerVlc : public XMediaPlayer
{
public:
	XMediaPlayerVlc();
	~XMediaPlayerVlc();

public:
	bool open(const std::string &location, bool autoplay = true);

	bool play();
	void pause();
	void playpause();
	void resume();
	void stop();

	bool isPlaying() const;
	bool isSeekable() const;
	bool canPause() const;

	long long getTime() const;
	void setTime(long long time);

	int getLoopCount() const;
	void setLoopCount(int count);

	long long duration() const;
	
	float buffering() const;

	int getVolume() const;
	void setVolume(int volume);

	bool isMuted() const;
	void setMuted(bool muted);

	State state() const;

	void setEventCallback(callback_t callback, void *data);

	bool watch(Event type);
	void unwatch(Event type);

	std::string errorString() const;

	const char* name() const;

private:
	// libvlc_event_t <=> XMediaPlayer::Event
	Event v2x(int event);
	int x2v(Event event);
	
	void setRepeat();

private:
	static void vlc_event_callback(const struct libvlc_event_t *event, void *userdata);

private:
	std::vector<Event> *_eventList;
	callback_t _callback;
	void *_userdata;

	float _buffering;
	int _loopCount;

	libvlc_instance_t *_instance;
	libvlc_media_player_t *_player;
};

XMR_END_NS

#endif // __XMRADIO_MEDIA_PLAYER_VLC_H__
