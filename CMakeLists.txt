# SPDX-License-Identifier: GPL-2.0-only
# Copyright © Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.20)
project(IECore VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("${PROJECT_NAME}")

message("\nCompiler Setup")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
message("Using Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("version: ${CMAKE_CXX_COMPILER_VERSION}")
message("CXX standard: ${CMAKE_CXX_STANDARD}")
message("Working environment")
message("System name: ${CMAKE_SYSTEM_NAME}")
message("System version: ${CMAKE_SYSTEM_VERSION}")
message("System processor: ${CMAKE_SYSTEM_PROCESSOR}")

message("\nGathering ${CMAKE_SYSTEM_NAME} specific files")
if (WIN32)
  file(GLOB IMPL_FILE "Implementation/IEWin32.cpp")
elseif(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  file(GLOB IMPL_FILE "Implementation/IEApple.mm")
elseif(LINUX)
  file(GLOB IMPL_FILE "Implementation/IELinux.cpp")
endif()

message("\nGathering source files")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
file(GLOB EXTENSION_FILES "Extensions/*.cpp")
file(GLOB IECore_SOURCE_FILES "Source/*.cpp")
list(APPEND IECore_SOURCE_FILES ${IMPL_FILE} ${EXTENSION_FILES})

message("Creating Library and setting compile time definitions")
add_library(${PROJECT_NAME} SHARED ${IECore_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ".")
file(GLOB HEADER_FILES "IECore.h")
set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER ${HEADER_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE IERESOURCES_DIR="${CMAKE_INSTALL_PREFIX}/IE/Resources")

message("Including Third Party Libraries")
add_subdirectory(ThirdParty/imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_include_directories(${PROJECT_NAME} PUBLIC ThirdParty/stb)

message("Linking ${CMAKE_SYSTEM_NAME} specific libraries")
if(WIN32)
  target_link_libraries(${PROJECT_NAME} PUBLIC dwmapi uxtheme)
elseif(APPLE)
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework Cocoa" "-framework UserNotifications")
elseif(LINUX)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GLIB REQUIRED glib-2.0)
  pkg_check_modules(LIBNOTIFY REQUIRED libnotify)

  target_include_directories(${PROJECT_NAME} PUBLIC 
    ${LIBNOTIFY_INCLUDE_DIRS} 
    ${GLIB_INCLUDE_DIRS})

  target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${LIBNOTIFY_LIBRARIES} 
    ${GLIB_LIBRARIES})
endif()

message("Setting up install")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Resources" DESTINATION "${CMAKE_INSTALL_PREFIX}/IE")
include(InstallRequiredSystemLibraries)

message("Adding extra directories")
add_subdirectory(Examples)

message("------------------------------------------------------------\n")