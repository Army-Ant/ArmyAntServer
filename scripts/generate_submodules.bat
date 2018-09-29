:START
cd ..

:GENERATE_SUBMODULE
git submodule update --init --progress
cd external

:GENERATE_BOOST
cd ArmyAntLib
cd external
call boost_generate_and_build.bat
cd ../..

:GENERATE_PROTOBUF
cd Protobuf
git submodule update --init --progress --recursive
cd ..

:BUILD_PROTOBUF
call build_protobuf.bat

:GENERATE_CLOSURE_LIBRARY
cd closure-library
git submodule update --init --progress --recursive

:END
