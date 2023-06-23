TEMPLATE = app
DEFINES += DEBUG
CONFIG += c++23
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lglut -lGLU -lGL -lGLEW -lpthread
SOURCES += \
    alliance.cpp \
    app.cpp \
    board_brazilian.cpp \
    board_draugths64.cpp \
    drawhelper.cpp \
    euristic_default.cpp \
    evaluation_strategy.cpp \
        main.cpp \
    game.cpp \
    alphabeta.cpp \
    board.cpp \
    menu.cpp \
    move.cpp \
    piece.cpp \
    player.cpp \
    player_alphabeta.cpp \
    player_cpu.cpp \
    player_human.cpp \
    rules.cpp \
    rules_checkers.cpp \
    rules_draughts64.cpp \
    rules_international.cpp \
    step.cpp \
    tile.cpp \
    utils.cpp \
    view.cpp

HEADERS += \
    alliance.hpp \
    app.hpp \
    board_brazilian.hpp \
    board_draugths64.hpp \
    drawhelper.hpp \
    euristic_default.hpp \
    evaluation_strategy.hpp \
    game.hpp \
    alphabeta.hpp \
    board.hpp \
    gamestatus.hpp \
    menu.hpp \
    move.hpp \
    movestrategy.hpp \
    piece.hpp \
    player.hpp \
    player_alphabeta.hpp \
    player_cpu.hpp \
    player_human.hpp \
    position.hpp \
    rules.hpp \
    rules_checkers.hpp \
    rules_draughts64.hpp \
    rules_international.hpp \
    step.hpp \
    tile.hpp \
    utils.hpp \
    view.hpp
