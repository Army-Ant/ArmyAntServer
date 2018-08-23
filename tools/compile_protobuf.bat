@echo off

set PROTOC_PATH=..\external\Protobuf\bin
set PROTOFILE_PATH=..\proto
set PROTOCPP_PATH=..\res\ProtobufSource\cpp
set PROTOCSHARP_PATH=..\res\ProtobufSource\csharp
set PROTOJAVASCRIPT_PATH=..\res\ProtobufSource\javascript
set PROTOPYTHON_PATH=..\res\ProtobufSource\python
set PROTORUBY_PATH=..\res\ProtobufSource\ruby
set PROTOBINARY_PATH=..\res\ProtobufSource\pb


%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% --csharp_out=%PROTOCSHARP_PATH% --js_out=%PROTOJAVASCRIPT_PATH% --python_out=%PROTOPYTHON_PATH% --ruby_out=%PROTORUBY_PATH% %PROTOFILE_PATH%\ArmyAntMessage\System\SocketHead.proto
%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% --csharp_out=%PROTOCSHARP_PATH% --js_out=%PROTOJAVASCRIPT_PATH% --python_out=%PROTOPYTHON_PATH% --ruby_out=%PROTORUBY_PATH% %PROTOFILE_PATH%\ArmyAntMessage\System\SessionStart.proto

pause
