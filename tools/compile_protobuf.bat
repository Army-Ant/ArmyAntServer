@echo off

set PROTOC_PATH=..\external\Protobuf\bin
set PROTOFILE_PATH=..\proto
set PROTOBINARY_PATH=..\bin\pb
set PROTOCPP_PATH=..\res\ProtobufSource

if not exist ..\bin ( 
    cd ..
    mkdir bin
    cd tools
)
if not exist %PROTOBINARY_PATH% ( 
    cd ..\bin
    mkdir pb
	cd ..\tools
)

%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% %PROTOFILE_PATH%\ArmyAntMessage.System.SocketHead.proto

pause
