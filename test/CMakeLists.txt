include_directories(../src ../src/player)

if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	link_directories(${LIBVLC_PATH}/lib)
endif()

add_executable(testplayer test_player.cpp)
target_link_libraries(testplayer xmrplayer)
