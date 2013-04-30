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
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++.exe
CXX=g++.exe
FC=
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AccessDLL.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 -mno-cygwin -Wl,--add-stdcall-alias -shared -m32
CXXFLAGS=-m32 -mno-cygwin -Wl,--add-stdcall-alias -shared -m32

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpsapi

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk libAccessDll.dll

libAccessDll.dll: ${OBJECTFILES}
	${LINK.cc} -shared -m32 -mno-cygwin -shared -o libAccessDll.dll ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/AccessDLL.o: nbproject/Makefile-${CND_CONF}.mk AccessDLL.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/cygdrive/C/Program\ Files\ \(x86\)/Java/jdk1.6.0_20/include/win32 -I/cygdrive/C/Program\ Files\ \(x86\)/Java/jdk1.6.0_20/include/  -MMD -MP -MF $@.d -o ${OBJECTDIR}/AccessDLL.o AccessDLL.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} libAccessDll.dll

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
