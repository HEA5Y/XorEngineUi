cmake_minimum_required(VERSION 3.19)
project(XorEngineUI LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(XorEngineUI
    WIN32 MACOSX_BUNDLE
    README.md
    main.cpp

    gui/mainwindow.cpp
    gui/mainwindow.h

    core/libXorEngine.cpp
    core/libXorEngine.h

    core/worker/xorworker.h
    core/worker/xorworker.cpp
    core/TaskConfig.h

)

qt_add_translations(
    TARGETS XorEngineUI
    TS_FILES XorEngineUI_ru_RU.ts
)

target_link_libraries(XorEngineUI
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS XorEngineUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET XorEngineUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
