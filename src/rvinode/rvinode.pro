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

# this is x86 dependent right now
!isEmpty($$(QNX_TARGET)) {
    message($$(QNX_TARGET))
    INCLUDEPATH += $$(QNX_TARGET)/x86/include
    LIBS += -L$$(QNX_TARGET)/x86/lib -lrvi
} else {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lrvi
}

load(qt_module)
