cmake_minimum_required (VERSION 2.6)
project (ThroughputMesServer)

add_executable(ThroughputMesServer server_main.c)

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(ThroughputMesServer PRIVATE -Wall -Wextra)
endif()
if ( MSVC )
    target_compile_options(ThroughputMesServer PRIVATE /W4)
endif()
  

