cmake_minimum_required(VERSION 3.10)

project(ziv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main
    main.cpp
    bytecode_executor/bytecode_executor.cpp
    bytecode_executor/handle_concat_instruction.cpp
    bytecode_executor/handle_print_instruction.cpp
    bytecode_executor/handle_variable_declaration.cpp
    bytecode_translator/bytecode_translator.cpp
    bytecode_translator/globals.cpp
    bytecode_translator/handle_function_declaration.cpp
    bytecode_translator/handle_print_statement.cpp
    bytecode_translator/handle_variable_declaration.cpp
    bytecode_translator/tokenize_statement.cpp
    bytecode_translator/translate_line.cpp
    helper/helper.cpp
    memory/memory_management.cpp
)

target_include_directories(main PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/bytecode_executor"
    "${PROJECT_SOURCE_DIR}/bytecode_translator"
    "${PROJECT_SOURCE_DIR}/helper"
    "${PROJECT_SOURCE_DIR}/memory"
)
