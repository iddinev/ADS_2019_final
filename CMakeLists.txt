#author Zhivko Petrov


cmake_minimum_required(VERSION 3.5.1)


#define fizzlinghot project
set(_PROJECT not_killer)
project(${_PROJECT})


set(_BOT_ROOT_DIR "${BOTS_ROOT_DIR}/not_killer")


#file(GLOB...) allows for wildcard additions:
file(GLOB _SOURCES "${_BOT_ROOT_DIR}/*.cpp"
                   "${_BOT_ROOT_DIR}/*.c")


#Generate the shared library from the sources
#adding -shared flag
add_library(${_PROJECT} SHARED
            ${_SOURCES})


#adding -std=c++14 flag
set_property(TARGET ${_PROJECT} PROPERTY CXX_STANDARD 14)


#needed for -fPic flag
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


#adding -fPic flag
set_property(TARGET ${_PROJECT}
        PROPERTY POSITION_INDEPENDENT_CODE ON)


target_link_libraries(
        ${_PROJECT}
        ${UTILS_LIB_NAME}            # -lutils.a
        ${SDL2_LIBRARY}              # -lSDL2 flag
        ${CMAKE_DL_LIBS}             # -ldl flag
        ${GRAPHICS_MGR}              # graphics manager flag
        ${R_DYNAMIC_FLAG}            # export of static symbols
        m)                           # -lm flag


#Additional include directories - add game root folder as a relative directory
target_include_directories(${_PROJECT} PRIVATE ${_BOT_ROOT_DIR})


#add project dependancies
add_dependencies(${_PROJECT} ${ROOT_PROJECT})



