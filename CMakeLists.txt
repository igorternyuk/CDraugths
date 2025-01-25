cmake_minimum_required(VERSION 3.21)
project(CDraughts_cmake)

set(CMAKE_CXX_STANDARD 23)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(CDraughts_cmake main.cpp alliance.h alliance.cpp alphabeta.cpp alphabeta.h app.cpp app.h view.cpp view.h game.cpp game.h board.cpp board.h rules.cpp rules.h piece.cpp piece.h move.cpp move.h tile.cpp tile.h position.h step.cpp step.h utils.cpp utils.h drawhelper.cpp drawhelper.h player.cpp player.h player_human.cpp player_human.h player_cpu.cpp player_cpu.h player_alphabeta.cpp player_alphabeta.h movestrategy.h notation.h heuristic_default.cpp heuristic_default.h evaluation_strategy.h board_armenian_tamas.cpp board_armenian_tamas.h board_ru.cpp board_ru.h board_international.cpp board_international.h board_polish.cpp board_polish.h board_checkers.cpp board_checkers.h board_canadian.cpp board_canadian.h board_brazilian.cpp board_brazilian.h board_italian.cpp board_italian.h board_portuguese.cpp board_turkish.cpp board_turkish.h board_diagonal64.cpp board_diagonal64.h board_diagonal100.cpp board_diagonal100.h board_frisian.cpp board_frisian.h gamestatus.h rules_orthogonal.cpp rules_orthogonal.h rules_international.cpp rules_international.h rules_ru.cpp rules_ru.h rules_turkish.cpp rules_armenian.cpp rules_armenian.h rules_checkers.cpp rules_checkers.h rules_frisian.cpp rules_frisian.h rules_italian.cpp rules_italian.h rules_portuguese.cpp rules_portuguese.h rules_turkish.h board_portuguese.h heuristic_diagonal.cpp heuristic_diagonal.h notation.cpp)

target_link_libraries(${PROJECT_NAME} OpenGL::GL GLUT::GLUT Threads::Threads)


