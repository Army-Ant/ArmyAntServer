#!/bin/bash

echo off

PROTOC_PATH=../external/Protobuf/bin
PROTOFILE_PATH=../proto

PROTOCPP_PATH=../res/ProtobufSource/cpp
PROTOCSHARP_PATH=../res/ProtobufSource/csharp
PROTOJAVASCRIPT_PATH=../res/ProtobufSource/javascript
PROTOPYTHON_PATH=../res/ProtobufSource/python
PROTORUBY_PATH=../res/ProtobufSource/ruby
PROTOBINARY_PATH=../res/ProtobufSource/pb


${PROTOC_PATH}/protoc -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/System/SocketHead.proto
${PROTOC_PATH}/protoc -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/System/SessionStart.proto
${PROTOC_PATH}/protoc -I=$PROTOFILE_PATH --cpp_out=$PROTOCPP_PATH --csharp_out=$PROTOCSHARP_PATH --js_out=$PROTOJAVASCRIPT_PATH --python_out=$PROTOPYTHON_PATH --ruby_out=$PROTORUBY_PATH $PROTOFILE_PATH/ArmyAntMessage/DBProxy/SqlRequest.proto

pause
