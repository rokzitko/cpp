#add_library(Hello hello.cc)
add_library(Hello SHARED hello.cc)
target_include_directories(Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
