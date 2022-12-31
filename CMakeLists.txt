cmake_minimum_required(VERSION 3.10)

project(NoGo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(ccgame)
add_subdirectory(bot/cita)
add_subdirectory(bot/greedy)

add_executable(${PROJECT_NAME} main.cc board.cc bot.cc game.cc save.cc)
target_link_libraries(${PROJECT_NAME} ccgame)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})