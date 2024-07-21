# Copyright © 2024 mozahzah (Incus Entertainment). All rights reserved.

cmake_minimum_required(VERSION 3.29)
project(IEMidi VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("IECore\n")

if (WIN32)
  file(GLOB IMPL_CPP_FILES "./Implementation/*.cpp")
elseif(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  find_library(COREAUDIO_LIBRARY CoreAudio)
  file(GLOB IMPL_MM_FILES "./Implementation/*.mm")
endif()

file(GLOB EXTENSION_FILES "./Extensions/*.cpp")
file(GLOB SOURCE_FILES "./*.cpp")
list(APPEND SOURCE_FILES ${IMPL_CPP_FILES} ${IMPL_MM_FILES} ${EXTENSION_FILES})
add_library(IECore STATIC ${SOURCE_FILES})
target_include_directories(IECore PUBLIC "./" ThirdParty/stb)
add_subdirectory(ThirdParty/imgui)
target_link_libraries(IECore PUBLIC imgui)

if(WIN32)
    target_link_libraries(IECore PUBLIC dwmapi)
    target_link_libraries(IECore PUBLIC uxtheme)
elseif(APPLE)
    target_link_libraries(IECore PUBLIC ${COREAUDIO_LIBRARY})
    target_link_libraries(IECore PUBLIC "-framework Cocoa")
    target_link_libraries(IECore PUBLIC "-framework UserNotifications")
endif()

set(IECORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "IECore Directory")
message("IECORE_DIR is cached as ${IECORE_DIR}")

message("------------------------------------------------------------\n")