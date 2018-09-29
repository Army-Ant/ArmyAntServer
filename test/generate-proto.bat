:Compile
cd ..\scripts
call compile_protobuf.bat
cd ..\test

:Copy_Javascript
if exist proto-js (
	rd proto-js /S /Q
)
mkdir proto-js
copy ..\src\ProtobufSource\javascript\* proto-js\

:Copy_CSharp
cd ..\src\ProtobufSource\csharp

:End
pause
