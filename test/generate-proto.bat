:Compile
@echo off
cd ..\scripts
call compile_protobuf.bat
cd ..\test

:Copy_Javascript
if exist proto-js (
	rd proto-js /S /Q
)
mkdir proto-js
xcopy ..\src\ProtobufSource\javascript .\proto-js /E /C /Q /Y /I
if not exist libprotobuf-js (
	xcopy ..\external\Protobuf\js .\libprotobuf-js /E /C /Q /Y /I
)
if not exist libclosure-js (
	xcopy ..\external\closure-library .\libclosure-js /E /C /Q /Y /I
)
:: call python libclosure-js\closure\bin\calcdeps.py -i libprotobuf-js/ -i proto-js/ -p libclosure-js/ -o script > armyantmessage.js :: This sentence cannot work well, I will use ArmyAnt.js instead
if not exist ArmyAnt.js (
    xcopy ..\external\ArmyAnt.js .\ArmyAnt.js  /E /C /Q /Y /I
)

:Copy_CSharp
cd ..\src\ProtobufSource\csharp

:End
cd ..\..\..\test
pause
