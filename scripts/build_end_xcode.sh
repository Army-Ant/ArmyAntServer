#!/bin/bash

echo Start to change dylib loading path ...

cd ..
if [ -d bin ]; then
    cd bin
    if [ -f DBProxy ]; then
        echo Changing DBProxy ...
        install_name_tool -change libmysqlcppconn.7.dylib @executable_path/libmysqlcppconn.7.dylib DBProxy
        install_name_tool -change /usr/local/lib/libArmyAntLib_mac_d.dylib @executable_path/libArmyAntLib_mac_d.dylib DBProxy
        install_name_tool -change libboost_system.dylib @executable_path/libboost_system.dylib DBProxy
        install_name_tool -change /usr/local/lib/libprotobuf.17.dylib @executable_path/libprotobuf.17.dylib DBProxy
    fi
    if [ -f ArmyAntServer ]; then
        echo Changing ArmyAntServer ...
        install_name_tool -change /usr/local/lib/libArmyAntLib_mac_d.dylib @executable_path/libArmyAntLib_mac_d.dylib ArmyAntServer
        install_name_tool -change libboost_system.dylib @executable_path/libboost_system.dylib ArmyAntServer
        install_name_tool -change /usr/local/lib/libprotobuf.17.dylib @executable_path/libprotobuf.17.dylib ArmyAntServer
    fi
    if [ -f libmysqlcppconn.7.dylib ]; then
        echo Changing libmysqlcppconn.7.dylib ...
        install_name_tool -change libssl.1.0.0.dylib @executable_path/libssl.1.0.0.dylib libmysqlcppconn.7.dylib
        install_name_tool -change libcrypto.1.0.0.dylib @executable_path/libcrypto.1.0.0.dylib libmysqlcppconn.7.dylib
    fi
    if [ -f libssl.1.0.0.dylib ]; then
        echo Changing libssl.1.0.0.dylib ...
        install_name_tool -change /Volumes/hd2/pb2/build/sb_0-28989033-1528376354.5/openssl-1.0.2o-macos10.13-x86-64bit/lib/libcrypto.1.0.0.dylib @executable_path/libcrypto.1.0.0.dylib libssl.1.0.0.dylib
    fi
    cd ..
fi
cd scripts

echo OK!