cmake_minimum_required(VERSION 3.19)
project(undaunted-phase1 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(undaunted-phase1
    WIN32 MACOSX_BUNDLE
    main.cpp
    resources.qrc



    SplashScreen.h SplashScreen.cpp SplashScreen.ui
    LoginPage.h LoginPage.cpp LoginPage.ui
    MapSelectionWindow.h MapSelectionWindow.cpp MapSelectionWindow.ui
    Cell.h
    Board.h
    MapLoader.h
    MapLoader.cpp
    GameBoardWindow.h GameBoardWindow.cpp
    Board.cpp
    Game.h
    Player.h
    Game.cpp
    Player.cpp
    BFS.h
    BFS.cpp
    CellItem.h CellItem.cpp
    Unit.h Unit.cpp
    Card.h Card.cpp
    DiceSystem.h DiceSystem.cpp
    CombatSystem.h CombatSystem.cpp


)

target_link_libraries(undaunted-phase1
    PRIVATE
        Qt::Core
        Qt::Widgets
)
# Ensure maps folder is copied to the correct output directory
file(COPY ${CMAKE_SOURCE_DIR}/maps/ DESTINATION ${CMAKE_BINARY_DIR}/maps/)


include(GNUInstallDirs)

install(TARGETS undaunted-phase1
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET undaunted-phase1
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
