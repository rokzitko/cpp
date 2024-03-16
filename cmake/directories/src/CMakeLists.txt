set(test1_SRCS test1.cc)
add_executable(test1 ${test1_SRCS})

message(STATUS "binary_dir=${CMAKE_BINARY_DIR}")
message(STATUS "source_dir=${CMAKE_SOURCE_DIR}")
message(STATUS "current_binary_dir=${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "current_source_dir=${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "project_binary_dir=${CMAKE_PROJECT_BINARY_DIR}")
message(STATUS "project_source_dir=${CMAKE_PROJECT_SOURCE_DIR}")
