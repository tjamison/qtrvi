TARGET = QtRviNode
QT = core core-private
VERSION = 0.0.1

CMAKE_MODULE_TESTS = '-'

INCLUDEPATH *= .

HEADERS += \
    qrvinode.h \
    qtrvinode_global.h \
    qrvinodemonitor_p.h \
    qrviserviceobject.h

SOURCES += \
    qrvinode.cpp \
    qrvinodemonitor.cpp \
    qrviserviceobject.cpp


INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib -lrvi

load(qt_module)
