#!/bin/bash

echo Start to copy libraries ...

BINARY_PATH=../bin/
MYSQL_PATH=../external/mysql/mysql-connector-c++-8.0.12
AALIB_PATH=../external/ArmyAntLib/bin
BOOST_PATH=../external/boost/stage/lib64-shared

if [ ! -d $BINARY_PATH ]; then
    mkdir $BINARY_PATH
fi

cp $MYSQL_PATH/lib64/*.so $BINARY_PATH
cp $MYSQL_PATH/lib64/*.so* $BINARY_PATH
rm -rf $BINARY_PATH/libArmyAnt*.s*
cp $AALIB_PATH/*.so $BINARY_PATH
rm -rf /usr/lib/x86_64-linux-gnu/libArmyAnt*.s*
ln -d $BINARY_PATH/libArmyAnt*.s* /usr/lib/x86_64-linux-gnu/
ldconfig

echo successful


