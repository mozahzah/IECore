# SPDX-License-Identifier: GPL-2.0-only
# Copyright © Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.20)
project(IECore VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("${PROJECT_NAME}\n")

message("Compiler Setup")
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

if (WIN32)
  file(GLOB IMPL_FILE "./Implementation/IEWin32.cpp")
elseif(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  file(GLOB IMPL_FILE "./Implementation/IEApple.mm")
elseif(LINUX)
  file(GLOB IMPL_FILE "./Implementation/IELinux.cpp")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
file(GLOB EXTENSION_FILES "./Extensions/*.cpp")
file(GLOB IECore_SOURCE_FILES "./Source/*.cpp")
list(APPEND IECore_SOURCE_FILES ${IMPL_FILE} ${EXTENSION_FILES})
add_library(${PROJECT_NAME} SHARED ${IECore_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "." ThirdParty/stb)
file(GLOB HEADER_FILES "./IECore.h")
set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER ${HEADER_FILES})
add_subdirectory(ThirdParty/imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/Resources")

set_target_properties(${PROJECT_NAME} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Resources" DESTINATION ".")
include(InstallRequiredSystemLibraries)

get_target_property(DEPENDENCIES ${PROJECT_NAME} LINK_LIBRARIES)
foreach(DEPENDENCY ${DEPENDENCIES})
  if(TARGET ${DEPENDENCY})
    message("Setting up dependency: ${DEPENDENCY}")

    set_target_properties(${DEPENDENCY} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    install(TARGETS ${DEPENDENCY}
        EXPORT "${PROJECT_NAME}Targets"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  endif()
endforeach()

add_subdirectory(Examples)

message("------------------------------------------------------------\n")