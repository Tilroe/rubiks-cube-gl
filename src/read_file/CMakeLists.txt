add_library(read_file
        read_file.c
        read_file.h
)
target_include_directories(read_file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})