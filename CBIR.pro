TARGET = CBIR
TEMPLATE = app
QMAKE_CXXFLAGS_DEBUG += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -std=c++0x -DDEBUG=1 -g -ggdb
SOURCES += main.cpp \
    mainwindow.cpp \
    BD.cpp
HEADERS += mainwindow.h \
    BD.h
FORMS += mainwindow.ui
OTHER_FILES +=