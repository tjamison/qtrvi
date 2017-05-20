
enable-tests {
    QTRVI_BUILD_PARTS = $$QT_BUILD_PARTS
    QTRVI_BUILD_PARTS *= tests
}

!unix:!qnx {
    message(Attempting build on unsupported operating system...)
    message(Currently only Linux and Qnx distributions are supported)
}

load(configure)
load(qt_parts)

OTHER_FILES += sync.profile
