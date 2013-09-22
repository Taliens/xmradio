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

#include <global.h>

#ifdef X_OS_WIN
#	include <Windows.h>
#else
#	include <dlfcn.h>
#endif

#include "xlibhelper.h"

XMR_BEGIN_NS

XLibHelper::XLibHelper()
	: _handle(NULL)
{
}

XLibHelper::XLibHelper(const std::string &fileName)
	: _handle(NULL)
{
	setFile(fileName);
}

XLibHelper::~XLibHelper()
{
	unload();
}

void XLibHelper::setFile(const std::string &fileName)
{
	_fileName = fileName;
}

bool XLibHelper::load()
{
	if (_fileName.empty())
		return false;

#ifdef X_OS_WIN
	_handle = (void *)LoadLibraryA(_fileName.c_str());
#else
	_handle = dlopen(_fileName.c_str(), RTLD_LAZY);
	if (_handle == NULL)
	{
		std::string tmpFile = _fileName;
		if (tmpFile.rfind(".so") == std::string::npos)
		{
			tmpFile += ".so";
			_handle = dlopen(tmpFile.c_str(), RTLD_LAZY);
		}
		
		if (_handle == NULL && tmpFile.find("lib") != 0)
		{
			tmpFile.insert(0, "lib");
			_handle = dlopen(tmpFile.c_str(), RTLD_LAZY);
		}
	}
#endif
	return _handle != NULL;
}

bool XLibHelper::unload()
{
	if (_handle == NULL)
		return false;

#ifdef X_OS_WIN
	if (FreeLibrary(HMODULE(_handle)))
		_handle = NULL;
#else
	if (dlclose(_handle) == 0)
		_handle = NULL;
#endif

	return _handle == NULL;
}

bool XLibHelper::isLoaded() const
{
	return _handle != NULL;
}

void* XLibHelper::resolve(const char *name)
{
	if (name == NULL)
		return NULL;

	if (_handle == NULL && !load())
		return NULL;

#ifdef X_OS_WIN
	return (void *)GetProcAddress(HMODULE(_handle), name);
#else
	return dlsym(_handle, name);
#endif
}

void* XLibHelper::resolve(const std::string &fileName, const char *name)
{
	XLibHelper lib(fileName);
	return lib.resolve(name);
}

XMR_END_NS
