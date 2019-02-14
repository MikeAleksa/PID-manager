cmake_minimum_required(VERSION 3.13)
project(pid_manager C)

set(CMAKE_C_STANDARD 11)

add_executable(pid_manager main.c pid_manager.c pid_manager.h)