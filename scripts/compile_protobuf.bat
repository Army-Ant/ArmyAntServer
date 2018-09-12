@echo off

:set_path
set PROTOC_PATH=..\external\Protobuf\bin
set PROTOFILE_PATH=..\res\proto
set PROTOSRC_BASE=..\src\ProtobufSource
set PROTOCPP_PATH=%PROTOSRC_BASE%\cpp
set PROTOCSHARP_PATH=%PROTOSRC_BASE%\csharp
set PROTOJAVASCRIPT_PATH=%PROTOSRC_BASE%\javascript
set PROTOPYTHON_PATH=%PROTOSRC_BASE%\python
set PROTORUBY_PATH=%PROTOSRC_BASE%\ruby
set PROTOBINARY_PATH=%PROTOSRC_BASE%\pb

:create_noexist_path
if not exist %PROTOCPP_PATH% (mkdir %PROTOCPP_PATH%)
if not exist %PROTOCSHARP_PATH% (mkdir %PROTOCSHARP_PATH%)
if not exist %PROTOJAVASCRIPT_PATH% (mkdir %PROTOJAVASCRIPT_PATH%)
if not exist %PROTOPYTHON_PATH% (mkdir %PROTOPYTHON_PATH%)
if not exist %PROTORUBY_PATH% (mkdir %PROTORUBY_PATH%)
if not exist %PROTOBINARY_PATH% (mkdir %PROTOBINARY_PATH%)


:execute_protoc
%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% --csharp_out=%PROTOCSHARP_PATH% --js_out=%PROTOJAVASCRIPT_PATH% --python_out=%PROTOPYTHON_PATH% --ruby_out=%PROTORUBY_PATH% %PROTOFILE_PATH%\ArmyAntMessage\System\SocketHead.proto
%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% --csharp_out=%PROTOCSHARP_PATH% --js_out=%PROTOJAVASCRIPT_PATH% --python_out=%PROTOPYTHON_PATH% --ruby_out=%PROTORUBY_PATH% %PROTOFILE_PATH%\ArmyAntMessage\System\SessionStart.proto
%PROTOC_PATH%\protoc.exe -I=%PROTOFILE_PATH% --cpp_out=%PROTOCPP_PATH% --csharp_out=%PROTOCSHARP_PATH% --js_out=%PROTOJAVASCRIPT_PATH% --python_out=%PROTOPYTHON_PATH% --ruby_out=%PROTORUBY_PATH% %PROTOFILE_PATH%\ArmyAntMessage\DBProxy\SqlRequest.proto

pause
