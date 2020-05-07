# Install script for directory: /home/acewzj/Linux_Net_Prog/muduo-master/muduo/base

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/acewzj/Linux_Net_Prog/build/release-install-cpp11")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/acewzj/Linux_Net_Prog/build/release-cpp11/lib/libmuduo_base.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/LogFile.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Timestamp.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/BlockingQueue.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/ThreadLocal.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Mutex.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/WeakCallback.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/AsyncLogging.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/StringPiece.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/CurrentThread.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Date.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/ThreadLocalSingleton.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Thread.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/BoundedBlockingQueue.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/FileUtil.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Logging.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/ThreadPool.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/noncopyable.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Singleton.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/ProcessInfo.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/TimeZone.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Condition.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/copyable.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/GzipFile.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Atomic.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/CountDownLatch.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/LogStream.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Exception.h"
    "/home/acewzj/Linux_Net_Prog/muduo-master/muduo/base/Types.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/acewzj/Linux_Net_Prog/build/release-cpp11/muduo/base/tests/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

