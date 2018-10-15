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
CND_CONF=Debug
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
LDLIBSOPTIONS=-lpthread ../../external/ArmyAntLib/bin/libArmyAnt_d_64.so ../ServerUtilities/../../bin/libserverutilities_d.a ../SimpleEchoApp/../../bin/libsimpleechoapp_d.a ../HuolongServer/../../bin/libhuolongserver_d.a ../ServerCore/../../bin/libservercore_d.a -lprotobuf

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/armyantserverd

../../bin/armyantserverd: ../../external/ArmyAntLib/bin/libArmyAnt_d_64.so

../../bin/armyantserverd: ../ServerUtilities/../../bin/libserverutilities_d.a

../../bin/armyantserverd: ../SimpleEchoApp/../../bin/libsimpleechoapp_d.a

../../bin/armyantserverd: ../HuolongServer/../../bin/libhuolongserver_d.a

../../bin/armyantserverd: ../ServerCore/../../bin/libservercore_d.a

../../bin/armyantserverd: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/armyantserverd ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/acbbdd84/ArmyAntServer.o: nbproject/Makefile-${CND_CONF}.mk ../../src/ServerEntry/ArmyAntServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/acbbdd84
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -DOS_Linux -D_64BIT -D_DEBUG -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../src/ProtobufSource/cpp -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/acbbdd84/ArmyAntServer.o ../../src/ServerEntry/ArmyAntServer.cpp

# Subprojects
.build-subprojects:
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug
	cd ../SimpleEchoApp && ${MAKE}  -f Makefile CONF=Debug
	cd ../HuolongServer && ${MAKE}  -f Makefile CONF=Debug
	cd ../ServerCore && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/armyantserverd

# Subprojects
.clean-subprojects:
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../SimpleEchoApp && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../HuolongServer && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../ServerCore && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
