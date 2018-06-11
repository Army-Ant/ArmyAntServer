@echo off

set PROTOC_PATH=..\external\Protobuf\bin
set PROTOFILE_PATH=..\proto
set PROTOCPP_PATH=..\res\ProtobufSource\cpp
set PROTOCSHARP_PATH=..\res\ProtobufSource\csharp
set PROTOBINARY_PATH=..\res\ProtobufSource\pb


%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% --csharp_out=%PROTOCSHARP_PATH% %PROTOFILE_PATH%\ArmyAntMessage\System\SocketHead.proto
%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% --csharp_out=%PROTOCSHARP_PATH% %PROTOFILE_PATH%\ArmyAntMessage\System\SessionStart.proto

pause
