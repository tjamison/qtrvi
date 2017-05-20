QT += gui core rvinode

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

target.path = $$[QT_INSTALL_EXAMPLES]/qtrvinode/two-simple-services
# TODO: add sources install target
INSTALLS += target
