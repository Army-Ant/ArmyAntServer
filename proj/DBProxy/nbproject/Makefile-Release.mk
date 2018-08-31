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
	${OBJECTDIR}/_ext/fbe89dad/ArmyAntDBProxy.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../bin/dbproxy

../bin/dbproxy: ${OBJECTFILES}
	${MKDIR} -p ../bin
	${LINK.cc} -o ../bin/dbproxy ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/fbe89dad/ArmyAntDBProxy.o: nbproject/Makefile-${CND_CONF}.mk ../../src/DBProxyEntry/ArmyAntDBProxy.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/fbe89dad
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fbe89dad/ArmyAntDBProxy.o ../../src/DBProxyEntry/ArmyAntDBProxy.cpp

# Subprojects
.build-subprojects:
	cd ../DatabaseProxyCore && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../bin/dbproxy

# Subprojects
.clean-subprojects:
	cd ../DatabaseProxyCore && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
