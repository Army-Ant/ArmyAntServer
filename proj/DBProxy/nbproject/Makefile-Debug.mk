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
	${OBJECTDIR}/_ext/fbe89dad/ArmyAntDBProxy.o


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
LDLIBSOPTIONS=-lpthread -lmysqlcppconn ../../external/ArmyAntLib/bin/libArmyAnt_d_64.so ../DatabaseProxyCore/../../bin/libdatabaseproxycore_d.a ../ServerUtilities/../../bin/libserverutilities_d.a -lprotobuf -lboost_system

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/dbproxyd

../../bin/dbproxyd: ../../external/ArmyAntLib/bin/libArmyAnt_d_64.so

../../bin/dbproxyd: ../DatabaseProxyCore/../../bin/libdatabaseproxycore_d.a

../../bin/dbproxyd: ../ServerUtilities/../../bin/libserverutilities_d.a

../../bin/dbproxyd: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${LINK.cc} -o ../../bin/dbproxyd ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/fbe89dad/ArmyAntDBProxy.o: nbproject/Makefile-${CND_CONF}.mk ../../src/DBProxyEntry/ArmyAntDBProxy.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/fbe89dad
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -DOS_Linux -D_64BIT -D_DEBUG -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.12/include -I../../src/ProtobufSource/cpp -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/fbe89dad/ArmyAntDBProxy.o ../../src/DBProxyEntry/ArmyAntDBProxy.cpp

# Subprojects
.build-subprojects:
	cd ../DatabaseProxyCore && ${MAKE}  -f Makefile CONF=Debug
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/dbproxyd

# Subprojects
.clean-subprojects:
	cd ../DatabaseProxyCore && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
