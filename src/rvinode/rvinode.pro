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

qnx {
    message($$(QNX_TARGET))
    INCLUDEPATH += $$(QNX_TARGET)/x86/include
    LIBS += -L$$(QNX_TARGET)/x86/lib 
}
# Still unsafe for cross-compilation
#unix {
#    INCLUDEPATH += /usr/local
#    LIBS += -L/usr/local/lib
#}

LIBS += -lrvi

load(qt_module)
