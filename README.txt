Demo for the integration of an OPC-UA stack into a Qt application
=================================================================

This simple application shows how you can integrate an OPC-UA stack into a Qt application. The principles
should be easy to adapt to other projects with custom event loops.

The following sections describe how to setup your environment to start with the exercises. The exercises
themselves are then described in a the separate EXERCISES.txt

Installation Steps for the Open62541 Stack
------------------------------------------

Checkout the open62541 Stack on tag 0.3.0

mkdir build
cd build
cmake -DUA_BUILD_EXAMPLES:BOOL=ON -DUA_BUILD_TOOLS:BOOL=ON -DCMAKE_INSTALL_PREFIX=/home/user/local ..
cmake --build .
cmake --build . --target install

You also need to manually copy the open62541.h header file to the install directory.

Ua Expert
---------

Demo version of an OPC-UA browser client by Unified Automation. The application
is packaged as AppImage and has no external dependencies.

FreeOpcUa Client GUI
--------------------

This is an alternative to the Ua Expert client. Make sure you use it with Python 3.x as
there are known issues with Python 2.x (https://github.com/FreeOpcUa/opcua-client-gui/issues/16)

$ sudo apt-get install python3-pip python-pyqt5
$ pip3 install opcua-client

Then start the client

$ opcua-client &

Qt Creator
----------

Is installed through the Qt Maintenance tool that is now available under
'/opt/Qt/MaintenanceTool'. By default the Qt version 5.12 is installed but
other versions can be installed through the mentioned maintenance tool.

Start the Qt Creator IDE:

qtcreator &
