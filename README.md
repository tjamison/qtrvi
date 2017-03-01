Module:
-----------------
This project is intended to be a Qtified library for JLR's RVI protocol.


Copyright:
----------------
For the moment, this project is entirely internal.
My goal is to integrate this QtRvi module into the Qt Playground and start working towards potential integration with either Qt5 mainline or Qt Automotive Suite.


Dependencies:
---------------
This project depends on the QtBase module and generally requires Qt 5.7+ to be installed.
This project will use C++11 constructs and will require a supported compiler (which is enforced by Qt 5.7)

Features:
--------------
* QRviNode client interface (both C++ and QML) for registration with a remote RVI node
* QRviServiceObject interface (both C++ and QML) for registering services with your QRviNode


Building:
---------------
This module will follow basic Qt module build steps:
```
qmake
make
make install
```

The module will be deployed to the location of the Qt SDK referenced by your qmake used during the build phase.


Timeline:
---------------
Initially this module will wrap the C implementation, rvi_lib, found here: https://github.com/GENIVI/rvi_lib

Over time I intend to implement the RVI protocol within this project where the QtRviNode can leverage the work done in the QSslSocket component.

Also, I intend to investigate a Qtified wrapper and/or interface to rvi_core, found here: https://github.com/GENIVI/rvi_core

For questions and concerns, please contact Jack Sanchez <jsanche4@jaguarlandrover.com>
