set(test1_SRCS test1.cc)
add_executable(test1 ${test1_SRCS})

INSTALL(TARGETS test1 RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
