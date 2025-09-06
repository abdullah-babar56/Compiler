cmake_minimum_required(VERSION 3.10) 
project(Compiler LANGUAGES CXX)

Set C++standard to C++17 for regex support

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

Include directories

include_directories(${CMAKE_SOURCE_DIR}/include)

Source files for the main executable

set(SOURCE_FILES src/main.cpp src/Patterns.cpp src/Lexer.cpp src/TokenUtils.cpp )

Create the main executable

add_executable(lexer ${SOURCE_FILES})

Ensure the compiler links against the standard library (should be automatic, but explicit for clarity)

target_link_libraries(lexer PRIVATE stdc++)

Optionally enable testing

enable_testing()

set(TEST_SOURCE_FILES

src/Patterns.cpp

src/Lexer.cpp

src/TokenUtils.cpp

)

add_executable(test_lexer tests/test_lexer.cpp ${TEST_SOURCE_FILES})

add_executable(test_tokens tests/test_tokens.cpp src/TokenUtils.cpp)

add_test(NAME LexerTests COMMAND test_lexer)

add_test(NAME TokenTests COMMAND test_tokens)

Generate compile_commands.json for IDE support (e.g., VS Code IntelliSense)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)