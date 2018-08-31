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
	${OBJECTDIR}/_ext/9e673e4/SqlRequest.pb.o \
	${OBJECTDIR}/_ext/35c7a07b/SessionStart.pb.o \
	${OBJECTDIR}/_ext/35c7a07b/SocketHead.pb.o \
	${OBJECTDIR}/_ext/f8bc3adf/EventManager.o \
	${OBJECTDIR}/_ext/f8bc3adf/Logger.o \
	${OBJECTDIR}/_ext/f8bc3adf/MessageQueue.o \
	${OBJECTDIR}/_ext/f8bc3adf/MessageQueueManager.o \
	${OBJECTDIR}/_ext/f8bc3adf/SocketApplication.o \
	${OBJECTDIR}/_ext/f8bc3adf/SocketClientApplication.o \
	${OBJECTDIR}/_ext/f8bc3adf/base.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../bin/libserverutilities.a

../../bin/libserverutilities.a: ${OBJECTFILES}
	${MKDIR} -p ../../bin
	${RM} ../../bin/libserverutilities.a
	${AR} -rv ../../bin/libserverutilities.a ${OBJECTFILES} 
	$(RANLIB) ../../bin/libserverutilities.a

${OBJECTDIR}/_ext/9e673e4/SqlRequest.pb.o: nbproject/Makefile-${CND_CONF}.mk ../../res/ProtobufSource/cpp/ArmyAntMessage/DBProxy/SqlRequest.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/9e673e4
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/9e673e4/SqlRequest.pb.o ../../res/ProtobufSource/cpp/ArmyAntMessage/DBProxy/SqlRequest.pb.cc

${OBJECTDIR}/_ext/35c7a07b/SessionStart.pb.o: nbproject/Makefile-${CND_CONF}.mk ../../res/ProtobufSource/cpp/ArmyAntMessage/System/SessionStart.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/35c7a07b
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/35c7a07b/SessionStart.pb.o ../../res/ProtobufSource/cpp/ArmyAntMessage/System/SessionStart.pb.cc

${OBJECTDIR}/_ext/35c7a07b/SocketHead.pb.o: nbproject/Makefile-${CND_CONF}.mk ../../res/ProtobufSource/cpp/ArmyAntMessage/System/SocketHead.pb.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/35c7a07b
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/35c7a07b/SocketHead.pb.o ../../res/ProtobufSource/cpp/ArmyAntMessage/System/SocketHead.pb.cc

${OBJECTDIR}/_ext/f8bc3adf/EventManager.o: nbproject/Makefile-${CND_CONF}.mk ../../src/Utilities/EventManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/EventManager.o ../../src/Utilities/EventManager.cpp

${OBJECTDIR}/_ext/f8bc3adf/Logger.o: nbproject/Makefile-${CND_CONF}.mk ../../src/Utilities/Logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/Logger.o ../../src/Utilities/Logger.cpp

${OBJECTDIR}/_ext/f8bc3adf/MessageQueue.o: nbproject/Makefile-${CND_CONF}.mk ../../src/Utilities/MessageQueue.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/MessageQueue.o ../../src/Utilities/MessageQueue.cpp

${OBJECTDIR}/_ext/f8bc3adf/MessageQueueManager.o: nbproject/Makefile-${CND_CONF}.mk ../../src/Utilities/MessageQueueManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/MessageQueueManager.o ../../src/Utilities/MessageQueueManager.cpp

${OBJECTDIR}/_ext/f8bc3adf/SocketApplication.o: nbproject/Makefile-${CND_CONF}.mk ../../src/Utilities/SocketApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/SocketApplication.o ../../src/Utilities/SocketApplication.cpp

${OBJECTDIR}/_ext/f8bc3adf/SocketClientApplication.o: nbproject/Makefile-${CND_CONF}.mk ../../src/Utilities/SocketClientApplication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/SocketClientApplication.o ../../src/Utilities/SocketClientApplication.cpp

${OBJECTDIR}/_ext/f8bc3adf/base.o: nbproject/Makefile-${CND_CONF}.mk ../../src/Utilities/base.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f8bc3adf
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -DOS_Linux -D_64BIT -D_cplusplus -D_x86 -I../../inc -I../../external/ArmyAntLib/inc -I../../external/mysql/mysql-connector-c++-8.0.11/include -I../../external/Protobuf/inc -I../../res/ProtobufSource/cpp -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f8bc3adf/base.o ../../src/Utilities/base.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../bin/libserverutilities.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
