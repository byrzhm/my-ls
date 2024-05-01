cmake_minimum_required(VERSION 3.20.0)

project(list LANGUAGES C)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(list ls.c)
