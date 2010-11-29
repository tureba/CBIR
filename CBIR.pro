TARGET = CBIR
TEMPLATE = app
QMAKE_CXXFLAGS_DEBUG += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -std=c++0x
SOURCES += main.cpp \
    mainwindow.cpp \
    bd.cpp \
    Extratores.cpp \
    Distancias.cpp
HEADERS += mainwindow.h \
    bd.h \
    Extratores.h \
    Distancias.h
FORMS += mainwindow.ui
OTHER_FILES += 
