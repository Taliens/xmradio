if (WIN32)
	if(NOT LIBVLC_PATH OR NOT EXISTS "${LIBVLC_PATH}" OR NOT IS_DIRECTORY "${LIBVLC_PATH}")
		message(FATAL_ERROR "You must specify libvlc path")
	endif()
	include_directories(${LIBVLC_PATH}/include)
	link_directories(${LIBVLC_PATH}/lib)
	set(LIBVLC_LIBRARIES libvlc.lib)
	
	 file(COPY ${LIBVLC_PATH}/lib/libvlc.dll DESTINATION ${XMR_OUTPUT_PATH})
	 file(COPY ${LIBVLC_PATH}/lib/libvlccore.dll DESTINATION ${XMR_OUTPUT_PATH})
	 file(COPY ${LIBVLC_PATH}/lib/plugins DESTINATION ${XMR_OUTPUT_PATH})
else()
	PKG_CHECK_MODULES(LIBVLC REQUIRED "libvlc >= 2.0")
endif()

add_definitions(-DXMR_PLAYER_LIB)

set(backend_SRC
	xmediaplayer.h
	xmediaplayer_vlc.h
	xmediaplayer_vlc.cpp
)

include_directories(. ..)
include_directories(${LIBVLC_INCLUDE_DIRS})

add_library(xmrplayer SHARED ${backend_SRC})
target_link_libraries(xmrplayer
	${LIBVLC_LIBRARIES}
)

set(PLAYER_VERSION 1.0)

set_target_properties(xmrplayer
	PROPERTIES VERSION ${PLAYER_VERSION}
	SOVERSION 0
)

if (NOT WIN32)
	install(TARGETS xmrplayer LIBRARY DESTINATION ${libdir})
endif()