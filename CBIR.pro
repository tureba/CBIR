TARGET = CBIR
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -DDEBUG=1 -g -ggdb
QMAKE_LFLAGS += -larboretum
SOURCES += main.cpp \
    mainwindow.cpp \
    flowlayout.cpp \
    BD.cpp
HEADERS += mainwindow.h \
    flowlayout.h \
    BD.h
FORMS += mainwindow.ui
OTHER_FILES +=
