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

#ifndef __XMRADIO_UTILS_H__
#define __XMRADIO_UTILS_H__

#include <global.h>
#include <string>

#include "xlibhelper.h"

XMR_BEGIN_NS

//! Removing leading and trailing spaces
XMR_UTILS_EXPORT std::string stringTrim(const std::string &str);

XMR_UTILS_EXPORT std::string& stringToUpper(std::string &str);
XMR_UTILS_EXPORT std::string&  stringToLower(std::string &str);

//! List files in @folder and deal each file with @opFunc
XMR_UTILS_EXPORT void
scanFiles(const std::string &folder,
		  bool recursive,
		  void (*opFunc)(const std::string &, void *),
		  void *data);

XMR_END_NS

#endif // __XMRADIO_UTILS_H__
