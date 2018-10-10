:START
cd ..

:GENERATE_SUBMODULE
git submodule update --init --progress
cd external

:GENERATE_BOOST
cd ArmyAntLib
git checkout master
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
cd ..

:GENERATE_ARMYANT_JS
cd ArmyAnt.js
git checkout master
call babel-node_setup.bat
cd ..

:END
