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
	${OBJECTDIR}/_ext/f8bc3adf/EventManager.o \
	${OBJECTDIR}/_ext/f8bc3adf/Logger.o \
	${OBJECTDIR}/_ext/f8bc3adf/MessageQueue.o \
	${OBJECTDIR}/_ext/f8bc3adf/MessageQueueManager.o \
	${OBJECTDIR}/_ext/f8bc3adf/SocketApplication.o \
	${OBJECTDIR}/_ext/f8bc3adf/SocketClientApplication.o \
	${OBJECTDIR}/_ext/f8bc3adf/base.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserverutilities.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserverutilities.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserverutilities.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserverutilities.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserverutilities.a

${OBJECTDIR}/_ext/f8bc3adf/EventManager.o: ../../src/Utilities/EventManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/EventManager.o ../../src/Utilities/EventManager.cpp

${OBJECTDIR}/_ext/f8bc3adf/Logger.o: ../../src/Utilities/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/Logger.o ../../src/Utilities/Logger.cpp

${OBJECTDIR}/_ext/f8bc3adf/MessageQueue.o: ../../src/Utilities/MessageQueue.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/MessageQueue.o ../../src/Utilities/MessageQueue.cpp

${OBJECTDIR}/_ext/f8bc3adf/MessageQueueManager.o: ../../src/Utilities/MessageQueueManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/MessageQueueManager.o ../../src/Utilities/MessageQueueManager.cpp

${OBJECTDIR}/_ext/f8bc3adf/SocketApplication.o: ../../src/Utilities/SocketApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/SocketApplication.o ../../src/Utilities/SocketApplication.cpp

${OBJECTDIR}/_ext/f8bc3adf/SocketClientApplication.o: ../../src/Utilities/SocketClientApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/SocketClientApplication.o ../../src/Utilities/SocketClientApplication.cpp

${OBJECTDIR}/_ext/f8bc3adf/base.o: ../../src/Utilities/base.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/base.o ../../src/Utilities/base.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libserverutilities.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
