project(myos-clib)

add_library(clib
        src/ctype.c
        include/ctype.h
        )
target_compile_options(clib
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        PRIVATE $<$<COMPILE_LANGUAGE:C>:-m32 -masm=intel -ffreestanding -fno-stack-protector -mno-red-zone>
        )
target_include_directories(clib
        PUBLIC include
        PRIVATE src
        )