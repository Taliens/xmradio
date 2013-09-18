PKG_CHECK_MODULES(LIBVLC REQUIRED "libvlc >= 2.0")

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

install(TARGETS xmrplayer LIBRARY DESTINATION ${plugindir})
