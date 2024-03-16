cmake_minimum_required (VERSION 3.10)
project (HELLO)

macro(mojmakro mmX mmY)
 add_executable(${mmX} ${mmY})
endmacro(mojmakro)

mojmakro(test1 test1.cc)
