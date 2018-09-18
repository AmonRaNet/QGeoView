set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Gui
    Network
    Widgets
)

add_executable(qgeoview-demo
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    demoitem.h
    demoitem.cpp
    selectordialog.h
    selectordialog.cpp
    samples/widgets.cpp
    samples/widgets.h
    samples/background.cpp
    samples/background.h
    samples/mouse.cpp
    samples/mouse.h
    samples/items.cpp
    samples/items.h
    samples/flags.cpp
    samples/flags.h
    samples/customtiles.cpp
    samples/customtiles.h
    samples/utilities.cpp
    samples/utilities.h
    samples/waveanimation.cpp
    samples/waveanimation.h
    samples/ellipse.cpp
    samples/ellipse.h
    samples/rectangle.cpp
    samples/rectangle.h
    samples/placemark.cpp
    samples/placemark.h
    samples/mytiles.cpp
    samples/mytiles.h
    samples/mytile.cpp
    samples/mytile.h
)

target_link_libraries(qgeoview-demo
    PRIVATE
        Qt5::Core
        Qt5::Network
        Qt5::Gui
        Qt5::Widgets
        QGeoView
)
