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
	${OBJECTDIR}/_ext/adc16ec7/DBProxyMain.o \
	${OBJECTDIR}/_ext/adc16ec7/mysqlBridge.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../bin/libdatabaseproxycore_d.a

../bin/libdatabaseproxycore_d.a: ${OBJECTFILES}
	${MKDIR} -p ../bin
	${RM} ../bin/libdatabaseproxycore_d.a
	${AR} -rv ../bin/libdatabaseproxycore_d.a ${OBJECTFILES} 
	$(RANLIB) ../bin/libdatabaseproxycore_d.a

${OBJECTDIR}/_ext/adc16ec7/DBProxyMain.o: nbproject/Makefile-${CND_CONF}.mk ../../src/DatabaseProxyCore/DBProxyMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/adc16ec7
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -D_cplusplus -DOS_Linux -D_x86 -D_64BIT -DDEBUG -D_DEBUG -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/adc16ec7/DBProxyMain.o ../../src/DatabaseProxyCore/DBProxyMain.cpp

${OBJECTDIR}/_ext/adc16ec7/mysqlBridge.o: nbproject/Makefile-${CND_CONF}.mk ../../src/DatabaseProxyCore/mysqlBridge.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/adc16ec7
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -D_cplusplus -DOS_Linux -D_x86 -D_64BIT -DDEBUG -D_DEBUG -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/adc16ec7/mysqlBridge.o ../../src/DatabaseProxyCore/mysqlBridge.cpp

# Subprojects
.build-subprojects:
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../bin/libdatabaseproxycore_d.a

# Subprojects
.clean-subprojects:
	cd ../ServerUtilities && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
