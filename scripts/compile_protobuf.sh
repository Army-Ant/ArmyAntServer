#!/bin/bash

echo Start to compile protobuf ...

#PROTOC_PATH=../external/Protobuf/bin/protoc
PROTOC_PATH=protoc
PROTOFILE_PATH=../proto
PROTOSRC_BASE=../src/ProtobufSource

PROTOCPP_PATH=$PROTOFILE_PATH/cpp
PROTOCSHARP_PATH=$PROTOFILE_PATH/csharp
PROTOJAVASCRIPT_PATH=$PROTOFILE_PATH/javascript
PROTOPYTHON_PATH=$PROTOFILE_PATH/python
PROTORUBY_PATH=$PROTOFILE_PATH/ruby
PROTOBINARY_PATH=$PROTOFILE_PATH/pb

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

${PROTOC_PATH} -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/Common/base.proto
${PROTOC_PATH} -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/System/SocketHead.proto
${PROTOC_PATH} -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/System/SessionStart.proto
${PROTOC_PATH} -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/DBProxy/SqlRequest.proto

${PROTOC_PATH} -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/SubApps/SimpleEcho.proto

read -rsp $'Finished !\nPress any key to exit.\n' -n 1 key
# echo $key
