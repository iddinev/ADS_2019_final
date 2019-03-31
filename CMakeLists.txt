#author Zhivko Petrov


cmake_minimum_required(VERSION 3.5.1)


set(BOTS_ROOT_DIR "${PROJECT_ROOT_DIR}/bots")


#invoke child Cmake file
add_subdirectory(${BOTS_ROOT_DIR}/killer)
add_subdirectory(${BOTS_ROOT_DIR}/not_killer)
