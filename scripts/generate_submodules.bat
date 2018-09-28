:GENERATE_SUBMODULE
cd ..
git submodule update --init --progress
cd external

:GENERATE_BOOST
cd ArmyAntLib
cd external
call boost_generate_and_build.bat

:GENERATE_PROTOBUF
cd ../..
cd Protobuf
git submodule update --init --progress --recursive

:BUILD_PROTOBUF
cd ..
call build_protobuf.bat

:END
