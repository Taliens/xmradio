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

#ifndef __XMRADIO_MEDIA_PLAYER_H__
#define __XMRADIO_MEDIA_PLAYER_H__

#include <string>

#include "player_export.h"

XMR_BEGIN_NS

class XMR_PLAYER_EXPORT XMediaPlayer
{
public:
	virtual ~XMediaPlayer() {}

public:
	//! open a media file, this will stop current playing media
	/*! @param location local file or network media file
	 *  @param autoplay set true to auto play if succeed
	 *  @return true if everything is OK, false otherwise
	 *  @see errorString() for more details if failed
	 */
	virtual bool open(const std::string &location, bool autoplay = true) = 0;

	virtual bool play() = 0;
	virtual void pause() = 0;
	virtual void playpause() = 0;
	virtual void resume() = 0;
	virtual void stop() = 0;

	virtual bool isPlaying() const = 0;
	virtual bool isSeekable() const = 0;
	virtual bool canPause() const = 0;

	//! get/set current media time (in ms)
	virtual long long getTime() const = 0;
	virtual void setTime(long long time) = 0;

	enum Loop { Infinite = -1 };
	virtual int getLoopCount() const = 0;
	virtual void setLoopCount(int count) = 0;

	//! get current media duration (in ms)
	virtual long long duration() const = 0;
	
	//! get current media buffering percentage (from 0 to 100)
	virtual float buffering() const = 0;

	//! volume range should be in [0, 100]
	virtual int getVolume() const = 0;
	virtual void setVolume(int volume) = 0;

	virtual bool isMuted() const = 0;
	virtual void setMuted(bool muted) = 0;

	//! Player state
	enum State
	{
		Nothing = 0,
		Opening,
		Buffering,
		Playing,
		Paused,
		Stopped
	};

	//! get player state
	virtual State state() const = 0;

	//! player event
	enum Event
	{
		NoEvent = -1,
		PlayerError,
		PlayerMediaChanged,
		PlayerBuffering,
		PlayerStateChanged,
		PlayerEndReadched,
		PlayerTimeChanged
	};

	typedef void (*callback_t)(Event, void *);
	virtual void setEventCallback(callback_t callback, void *data) = 0;

	virtual bool watch(Event type) = 0;
	virtual void unwatch(Event type) = 0;

	//! return last error string if any
	virtual std::string errorString() const = 0;

	//! get the backend name
	virtual const char* name() const = 0;
};

XMR_END_NS

#endif // __XMRADIO_MEDIA_PLAYER_H__
