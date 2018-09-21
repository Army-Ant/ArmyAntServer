:GENERATE_SUBMODULE
cd ..
git submodule update --init --progress
cd external

:GENERATE_BOOST
cd ArmyAntLib
cd external
start boost_generate_and_build.bat

:GENERATE_PROTOBUF
cd ../..
cd Protobuf
git submodule update --init --progress --recursive

:BUILD_PROTOBUF
cd cmake
mkdir build_win
cd build_win
mkdir debug
mkdir release
cd debug
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../.. ../..
nmake
nmake install
cd ../release
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../.. ../..
nmake
nmake install

:END
