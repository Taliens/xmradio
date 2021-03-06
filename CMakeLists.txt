cmake_minimum_required(VERSION 2.6)

project(xmradio)

set(version 0.5.1)

set(XMR_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${XMR_CMAKE_PATH} ${CMAKE_MODULE_PATH})

include(cmake/macro.cmake)

# Default build type set to Release
# to avoid auto build system
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

option(NLS	"Native language support"  ON)
option(WARN_AS_ERROR "Warning as error" ON)
option(BUILD_TEST "Build tests" OFF)
option(ENABLE_ASAN "Build with Address-Sanitizer if gcc >= 4.8 or clang >= 3.1" OFF)

include(CheckIncludeFiles)
include(FindPkgConfig)

check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(malloc.h HAVE_MALLOC_H)
check_include_files(X11/XF86keysym.h HAVE_MMKEYS_H)

find_program(GLIB_GENMARSHAL
	NAMES glib-genmarshal
	DOC "glib-genmarshal executable"
)

mark_as_advanced(GLIB_GENMARSHAL)

if (NLS)
	FIND_PACKAGE(Gettext REQUIRED)
endif(NLS)

if (NLS AND GETTEXT_FOUND)
	set(ENABLE_NLS TRUE)
	message(STATUS "Native language support:  YES")
else(NLS AND GETTEXT_FOUND)
	message(STATUS "Native language support:  NO")
endif(NLS AND GETTEXT_FOUND)

set(package	xmradio)
set(prefix	${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(bindir ${exec_prefix}/bin)

if (NOT DEFINED LIB_INSTALL_DIR)
	set(LIB_INSTALL_DIR ${exec_prefix}/lib)
endif()

set(libdir ${LIB_INSTALL_DIR})
set(includedir ${CMAKE_INSTALL_PREFIX}/include)
set(pkgdatadir ${prefix}/share/${package})
set(localedir ${prefix}/share/locale)
set(skindir ${pkgdatadir}/skin)
set(i18ndir ${PROJECT_SOURCE_DIR}/po)
set(uidir ${pkgdatadir}/ui)
set(plugindir ${libdir}/${package}/plugins)
set(plugindatadir ${pkgdatadir}/plugins)
set(PLUGIN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/plugins)

if (WARN_AS_ERROR)
	set(CMAKE_C_FLAGS "-Wall -Werror ${CMAKE_C_FLAGS}")
endif(WARN_AS_ERROR)

if (ENABLE_ASAN)
	if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
		execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
			OUTPUT_VARIABLE GCC_VERSION)
		if (GCC_VERSION VERSION_LESS 4.8)
			message(WARNING "Your gcc version may not use Address-Sanitizer")
		endif()
		set(CMAKE_C_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${CMAKE_C_FLAGS}")
	elseif (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
		execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
			OUTPUT_VARIABLE CLANG_VERSION)
		if (CLANG_VERSION VERSION_LESS 3.1)
			message(WARNING "Your clang version may not use Address-Sanitizer")
		endif()
		set(CMAKE_C_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${CMAKE_C_FLAGS}")
	endif()
endif(ENABLE_ASAN)

message("Build type: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE EQUAL "Debug")
	set(_DEBUG TRUE)
endif()

set(XMR_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/output)
set(EXECUTABLE_OUTPUT_PATH ${XMR_OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${XMR_OUTPUT_PATH})

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(data)
add_subdirectory(po)
add_subdirectory(src)
add_subdirectory(plugins)
if (BUILD_TEST)
	add_subdirectory(test)
endif(BUILD_TEST)
