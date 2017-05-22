TARGET = two-simple-services

QT += core rvinode

TEMPLATE = app

HEADERS += \
    rviserviceping.h \
    rviservicepong.h

SOURCES += \
    main.cpp \
    rviserviceping.cpp \
    rviservicepong.cpp

# need to grab the libs so we can run examples without installing
LIBS += -L../../lib

# to make QtRviNode/... style includes working without installing
INCLUDEPATH += $$PWD/../../include

target.path = $$[QT_INSTALL_EXAMPLES]/two-simple-services

sources.files = $$SOURCES $$HEADERS $$RESOURCES two-simple-services.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/two-simple-services

INSTALLS += target sources
