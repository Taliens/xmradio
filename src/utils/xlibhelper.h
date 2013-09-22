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

#ifndef __XMRADIO_XLIB_HELPER_H__

#include <string>

#include "utils_export.h"

XMR_BEGIN_NS

class XMR_UTILS_EXPORT XLibHelper
{
public:
	explicit XLibHelper();
	explicit XLibHelper(const std::string &fileName);
	~XLibHelper();
	
	void setFile(const std::string &fileName);
	bool load();
	bool unload();
	bool isLoaded() const;

	void* resolve(const char *name);

public:
	static void* resolve(const std::string &fileName, const char *name);
	
private:
	std::string _fileName;
	void *_handle;
};

XMR_END_NS

#endif // __XMRADIO_XLIB_HELPER_H__
