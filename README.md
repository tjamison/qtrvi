Purpose:
-----------------
Qt wrapper to create an event driven interface for an application developer to easily integrate GENIVI's RVI protocol.

Copyright and Authors:
----------------
(C) 2016 Jaguar Land Rover - All rights reserved.

Jack Sanchez - <jsanche4@jaguarlandrover.com>
Tatiana Jamison - <tjamison@jaguarlandrover.com>

TODO: Clarify licensing beyond the above...

Dependencies:
---------------
Requires the Qt framework (5.6+) to be installed and accessible in the environment
Requires rvi_lib <https://github.com/GENIVI/rvi_lib> to be installed
C++11 compliant compiler

Features:
--------------
* QRviNode client interface (both C++ and QML) for registration with a remote RVI node
* QRviServiceObject interface (both C++ and QML) for registering services with your QRviNode


Building:
---------------
If rvi_lib is accessible through default environment variable paths, then this module will follow basic Qt module build steps:
```
qmake
make
make install
```

If you have installed rvi_lib to a custom location, you may specify the include and library paths using the following variables:
```
RVI_INCLUDE_PATH
RVI_LIB_PATH
```

The module will be deployed to the location of the Qt SDK referenced by your qmake used during the build phase.
