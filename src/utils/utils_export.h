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

#ifndef __XMRADIO_UTILS_EXPORT_H__
#define __XMRADIO_UTILS_EXPORT_H__

#include <global.h>

#ifndef XMR_UTILS_EXPORT
#	ifdef XMR_UTILS_LIB
#		define XMR_UTILS_EXPORT XMR_EXPORT
#	else
#		define XMR_UTILS_EXPORT XMR_IMPORT
#	endif
#endif

#endif // __XMRADIO_UTILS_EXPORT_H__
