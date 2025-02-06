# Minimum cmake required to compile this project
cmake_minimum_required(VERSION 3.10)

# Project information
project("Chip8 Emulator"
  VERSION 1.0
  DESCRIPTION "An emlator for the chip8"
  LANGUAGES C
)

# Generate compile_commands.json for clang LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)

# Set the C standard used to compile this project
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

add_executable(chip8
  src/chip8.c
  src/chip8_display.c
  src/chip8_keyboard.c
  src/chip8_mem.c
  src/chip8_stack.c
  src/main.c
)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
  # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
  target_link_libraries(chip8 PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(chip8 PRIVATE SDL2::SDL2)

target_include_directories(chip8 PRIVATE "include/")
