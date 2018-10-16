#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/acbbdd84/ArmyAntServer.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=-m64

# Assembler Flags
ASFLAGS=--64

# Link Libraries and Options
LDLIBSOPTIONS=../ServerCore/../../bin/libservercore.a ../HuolongServer/../../bin/libhuolongserver.a ../SimpleEchoApp/../../bin/libsimpleechoapp.a ../ServerUtilities/../../bin/libserverutilities.a ../../external/ArmyAntLib/bin/libArmyAnt_64.so -lprotobuf ../../external/ArmyAntLib/external/boost/stage/lib/libboost_system.a -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/armyantserver

../../bin/armyantserver: ../ServerCore/../../bin/libservercore.a

../../bin/armyantserver: ../HuolongServer/../../bin/libhuolongserver.a

../../bin/armyantserver: ../SimpleEchoApp/../../bin/libsimpleechoapp.a

../../bin/armyantserver: ../ServerUtilities/../../bin/libserverutilities.a

../../bin/armyantserver: ../../external/ArmyAntLib/bin/libArmyAnt_64.so

../../bin/armyantserver: ../../external/ArmyAntLib/external/boost/stage/lib/libboost_system.a

../../bin/armyantserver: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/armyantserver ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/acbbdd84/ArmyAntServer.o: nbproject/Makefile-${CND_CONF}.mk ../../src/ServerEntry/ArmyAntServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/acbbdd84
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../src/ProtobufSource/cpp -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/acbbdd84/ArmyAntServer.o ../../src/ServerEntry/ArmyAntServer.cpp

# Subprojects
.build-subprojects:
	cd ../ServerCore && ${MAKE}  -f Makefile CONF=Release
	cd ../HuolongServer && ${MAKE}  -f Makefile CONF=Release
	cd ../SimpleEchoApp && ${MAKE}  -f Makefile CONF=Release
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/armyantserver

# Subprojects
.clean-subprojects:
	cd ../ServerCore && ${MAKE}  -f Makefile CONF=Release clean
	cd ../HuolongServer && ${MAKE}  -f Makefile CONF=Release clean
	cd ../SimpleEchoApp && ${MAKE}  -f Makefile CONF=Release clean
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
