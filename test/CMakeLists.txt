include_directories(../src ../src/player)

add_executable(testplayer test_player.cpp)
target_link_libraries(testplayer xmrplayer)
