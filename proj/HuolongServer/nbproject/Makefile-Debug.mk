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
	${OBJECTDIR}/_ext/ce19c10a/HuolongServer.o


# C Compiler Flags
CFLAGS=-m64

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/libhuolongserver_d.a

../../bin/libhuolongserver_d.a: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${RM} ../../bin/libhuolongserver_d.a
	${AR} -rv ../../bin/libhuolongserver_d.a ${OBJECTFILES} 
	$(RANLIB) ../../bin/libhuolongserver_d.a

${OBJECTDIR}/_ext/ce19c10a/HuolongServer.o: nbproject/Makefile-${CND_CONF}.mk ../../src/SubApplications/HuolongServer/HuolongServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/ce19c10a
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DDEBUG -DOS_Linux -D_64BIT -D_DEBUG -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../src/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/ce19c10a/HuolongServer.o ../../src/SubApplications/HuolongServer/HuolongServer.cpp

# Subprojects
.build-subprojects:
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/libhuolongserver_d.a

# Subprojects
.clean-subprojects:
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
