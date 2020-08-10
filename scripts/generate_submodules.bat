:START
cd ..

:GENERATE_SUBMODULE
git submodule update --init --progress --recursive
cd external

:GENERATE_BOOST
cd ArmyAntLib
git checkout master
cd external
call boost_generate_and_build.bat
cd ../../..

:BUILD_PROTOBUF
call build_protobuf.bat

:GENERATE_CLOSURE_LIBRARY
cd closure-library
git checkout master
cd ..

:GENERATE_ARMYANT_JS
cd ArmyAnt.js
git checkout master
call babel-node_setup.bat
cd ..

:GENERATE_OTHERS
cd ArmyAntGenerator
git checkout master
cd ..
cd ../res
git checkout master
cd ..

:END
cd script

