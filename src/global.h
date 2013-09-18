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

#ifndef __XMRADIO_GLOBAL_H__
#define __XMRADIO_GLOBAL_H__

#if defined(__linux__) || defined(__linux)
#	define X_OS_LINUX
#	define X_OS_UNIX
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32_)
#	define X_OS_WIN
#elif defined(__APPLE__)
#	define X_OS_MAC
#	define X_OS_UNIX
#else
#	error "Unsupported system"
#endif

#if defined X_OS_WIN
#	define XMR_EXPORT __declspec(dllexport)
#	define XMR_IMPORT __declspec(dllimport)
#else
#	define XMR_EXPORT
#	define XMR_IMPORT
#endif

#ifdef __cplusplus
#	define XMR_NAMESPACE xmrns
#	define XMR_BEGIN_NS namespace XMR_NAMESPACE {
#	define XMR_END_NS }
#	define XMR_USE_NS using namespace XMR_NAMESPACE;
#else
#	define XMR_NAMESPACE
#	define XMR_BEGIN_NS
#	define XMR_END_NS
#	define XMR_USE_NS
#endif

#endif // __XMRADIO_GLOBAL_H__
