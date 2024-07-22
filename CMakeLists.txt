# Copyright © 2024 mozahzah (Incus Entertainment). All rights reserved.

cmake_minimum_required(VERSION 3.29)
project(IECore VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("${PROJECT_NAME}\n")

if (WIN32)
  file(GLOB IMPL_CPP_FILES "./Implementation/*.cpp")
elseif(APPLE)
  set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)
  find_library(COREAUDIO_LIBRARY CoreAudio)
  file(GLOB IMPL_MM_FILES "./Implementation/*.mm")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
file(GLOB EXTENSION_FILES "./Extensions/*.cpp")
file(GLOB SOURCE_FILES "./*.cpp")
list(APPEND SOURCE_FILES ${IMPL_CPP_FILES} ${IMPL_MM_FILES} ${EXTENSION_FILES})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "." ThirdParty/stb)
add_subdirectory(ThirdParty/imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC dwmapi uxtheme)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${COREAUDIO_LIBRARY} "-framework Cocoa" "-framework UserNotifications")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/Resources")

install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Resources" DESTINATION ".")

get_target_property(DEPENDENCIES ${PROJECT_NAME} LINK_LIBRARIES)
foreach(DEPENDENCY ${DEPENDENCIES})
  if(TARGET ${DEPENDENCY})
    message("Installing dependency: ${DEPENDENCY}")
    install(TARGETS ${DEPENDENCY}
        EXPORT "${PROJECT_NAME}Targets"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
  endif()
endforeach()

message("------------------------------------------------------------\n")