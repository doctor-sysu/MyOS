project(myos-cpplib)

add_library(cpplib
        include/Queue.hpp
        src/Queue.cpp
        include/System.hpp
        src/System.cpp
        )

target_compile_options(cpplib
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        PRIVATE $<$<COMPILE_LANGUAGE:C>:-m32 -masm=intel -ffreestanding -fno-stack-protector -mno-red-zone>
        )
target_include_directories(cpplib
        PUBLIC include
        PRIVATE src
        )