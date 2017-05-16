CXX_MODULE = qml
TARGET = rvinodeplugin
TARGETPATH = RviNode
IMPORT_VERSION = 1.0

QT += core

SOURCES += \
	rvinode_plugin.cpp

DISTFILES = qmldir

load(qml_plugin)
