cmake_minimum_required(VERSION 2.6)

project(xmradio)

set(version 1.0)

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

option(WARN_AS_ERROR "Warning as error" ON)
option(BUILD_TEST "Build tests" ON)
option(ENABLE_ASAN "Build with Address-Sanitizer if gcc >= 4.8 or clang >= 3.1" OFF)

include(CheckIncludeFiles)
if (NOT WIN32)
	include(FindPkgConfig)
endif()

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
	if (MSVC)
		set(CMAKE_C_FLAGS "/WX ${CMAKE_C_FLAGS}")
		set(CMAKE_CXX_FLAGS "/WX ${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_C_FLAGS "-Wall -Werror ${CMAKE_C_FLAGS}")
		set(CMAKE_CXX_FLAGS "-Wall -Werror ${CMAKE_CXX_FLAGS}")
	endif()
endif(WARN_AS_ERROR)

if (MSVC)
	set(WARNING_NUMBERS
		4251
	)
	foreach(n ${WARNING_NUMBERS})
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd${n}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${n}")
	endforeach()
endif()

if (ENABLE_ASAN)
	if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
		execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
			OUTPUT_VARIABLE GCC_VERSION)
		if (GCC_VERSION VERSION_LESS 4.8)
			message(WARNING "Your gcc version may not use Address-Sanitizer")
		else()
			message("Enable Address-Sanitizer")
			set(CMAKE_C_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${CMAKE_C_FLAGS}")
			set(CMAKE_CXX_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${CMAKE_CXX_FLAGS}")
		endif()
	elseif (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
		execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
			OUTPUT_VARIABLE CLANG_VERSION)
		if (CLANG_VERSION VERSION_LESS 3.1)
			message(WARNING "Your clang version may not use Address-Sanitizer")
		else()
			message("Enable Address-Sanitizer")
			set(CMAKE_C_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${CMAKE_C_FLAGS}")
			set(CMAKE_CXX_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${CMAKE_CXX_FLAGS}")
		endif()
	else()
		message("Your compiler doesn't support Address-Sanitizer feature.")
	endif()
endif(ENABLE_ASAN)

message("Build type: ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE EQUAL "Debug")
	set(_DEBUG TRUE)
endif()

set(XMR_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/xmradio)
set(EXECUTABLE_OUTPUT_PATH ${XMR_OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${XMR_OUTPUT_PATH})

if (WIN32)
	foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${XMR_OUTPUT_PATH})
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${XMR_OUTPUT_PATH})
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${XMR_OUTPUT_PATH})
	endforeach()
endif()

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

include_directories(${PROJECT_BINARY_DIR})

#add_subdirectory(data)
#add_subdirectory(po)
add_subdirectory(src)
#add_subdirectory(plugins)
if (BUILD_TEST)
	add_subdirectory(test)
endif(BUILD_TEST)
