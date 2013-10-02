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

#include <algorithm>

#include "utils.h"

#ifdef X_OS_WIN
#	include <Windows.h>
#else
#	include <dirent.h>
#endif

#ifdef X_OS_WIN
#	define PATH_SEPARATOR '\\'
#else
#	define PATH_SEPARATOR '/'
#endif

XMR_BEGIN_NS

XMR_UTILS_EXPORT std::string stringTrim(const std::string &str)
{
	static const char *whitespace = " \t";

	size_t begin = str.find_first_not_of(whitespace);
	if (begin == std::string::npos)
		return "";

	size_t end = str.find_last_not_of(whitespace);
	
	return str.substr(begin, end - begin + 1);
}

struct convUpper
{
	void operator()(char &c) { c = ::toupper(c); }
};

struct convLower
{
	void operator()(char &c) { c = ::tolower(c); }
};

XMR_UTILS_EXPORT std::string& stringToUpper(std::string &str)
{
	std::for_each(str.begin(), str.end(), convUpper());
	
	return str;
}

XMR_UTILS_EXPORT std::string& stringToLower(std::string &str)
{
	std::for_each(str.begin(), str.end(), convLower());
	
	return str;
}

#ifdef X_OS_WIN
XMR_UTILS_EXPORT void
scanFiles(const std::string &folder,
		  bool recursive,
		  void (*opFunc)(const std::string &, void *),
		  void *data)
{
	if (folder.empty() || NULL == opFunc)
		return ;

	std::string wild = folder;
	if (wild[wild.length() - 1] != PATH_SEPARATOR)
		wild.push_back(PATH_SEPARATOR);
	wild.push_back('*');

	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA(wild.c_str(), &fd);
	
	if (hFind == INVALID_HANDLE_VALUE)
		return ;

	do
	{
		std::string fullPath = folder;
		if (fullPath[fullPath.length() - 1] != PATH_SEPARATOR)
			fullPath.push_back(PATH_SEPARATOR);
		fullPath += fd.cFileName;
		
		if (strcmp(fd.cFileName, ".") == 0 ||
			strcmp(fd.cFileName, "..") == 0
			)
		{
			continue ;
		}
	
		if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (recursive)
				scanFiles(fullPath, true, opFunc, data);
		}
		else
		{
			opFunc(fullPath, data);
		}
	} while (FindNextFileA(hFind, &fd));
	
	FindClose(hFind);
}
#else // X_OS_WIN
XMR_UTILS_EXPORT void
scanFiles(const std::string &folder,
		  bool recursive,
		  void (*opFunc)(const std::string &, void *),
		  void *data)
{
	if (folder.empty() || NULL == opFunc)
		return ;

	DIR *dir = opendir(folder.c_str());
	if (NULL == dir)
		return ;

	struct dirent64 *ent;
	while((ent = readdir64(dir)))
	{
		std::string fullPath = folder;
		if (fullPath[fullPath.length() - 1] != PATH_SEPARATOR)
			fullPath.push_back(PATH_SEPARATOR);
		fullPath += ent->d_name;

		if (ent->d_type == DT_DIR)
		{
			if (recursive)
				scanFiles(fullPath, true, opFunc, data);
		}
		else
		{
			opFunc(fullPath, data);
		}
	}
	
	closedir(dir);
}
#endif // X_OS_WIN

XMR_END_NS
