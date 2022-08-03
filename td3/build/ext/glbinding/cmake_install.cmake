# Install script for directory: /home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/mds3d_td3/ext/glbinding

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/glbinding" TYPE FILE FILES "/home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/build/ext/glbinding/VERSION")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/glbinding" TYPE FILE FILES "/home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/mds3d_td3/ext/glbinding/glbinding-config.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/glbinding" TYPE FILE FILES "/home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/mds3d_td3/ext/glbinding/AUTHORS")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/glbinding" TYPE FILE FILES "/home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/mds3d_td3/ext/glbinding/LICENSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/glbinding" TYPE FILE FILES "/home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/mds3d_td3/ext/glbinding/README.md")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xexamples_datax" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/glbinding" TYPE DIRECTORY FILES "/home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/mds3d_td3/ext/glbinding/data")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/baptiste/Bureau/travail/fac/master/semestre8/monde3d/td3/build/ext/glbinding/source/cmake_install.cmake")

endif()

