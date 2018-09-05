#!/bin/bash

echo Start to compile protobuf ...

PROTOC_PATH=../external/Protobuf/bin
PROTOFILE_PATH=../proto

PROTOCPP_PATH=../res/ProtobufSource/cpp
PROTOCSHARP_PATH=../res/ProtobufSource/csharp
PROTOJAVASCRIPT_PATH=../res/ProtobufSource/javascript
PROTOPYTHON_PATH=../res/ProtobufSource/python
PROTORUBY_PATH=../res/ProtobufSource/ruby
PROTOBINARY_PATH=../res/ProtobufSource/pb

if [ ! -d $PROTOCPP_PATH ]; then
    mkdir $PROTOCPP_PATH
fi
if [ ! -d $PROTOCSHARP_PATH ]; then
    mkdir $PROTOCSHARP_PATH
fi
if [ ! -d $PROTOJAVASCRIPT_PATH ]; then
    mkdir $PROTOJAVASCRIPT_PATH
fi
if [ ! -d $PROTOPYTHON_PATH ]; then
    mkdir $PROTOPYTHON_PATH
fi
if [ ! -d $PROTORUBY_PATH ]; then
    mkdir $PROTORUBY_PATH
fi
if [ ! -d $PROTOBINARY_PATH ]; then
    mkdir $PROTOBINARY_PATH
fi

${PROTOC_PATH}/protoc -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/System/SocketHead.proto
${PROTOC_PATH}/protoc -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/System/SessionStart.proto
${PROTOC_PATH}/protoc -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/DBProxy/SqlRequest.proto

read -rsp $'Finished !\nPress any key to exit.\n' -n 1 key
# echo $key
