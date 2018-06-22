project(myos-clib)


enable_language(ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf32 -o <OBJECT> <SOURCE>")

add_library(clib
        src/ctype.c
        include/ctype.h
        include/clib.h
        src/clib.c
        src/crt.c
        src/kliba.asm
        )

target_compile_options(clib
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        PRIVATE $<$<COMPILE_LANGUAGE:C>:-m32 -masm=intel -ffreestanding -fno-stack-protector -mno-red-zone>
        )
target_include_directories(clib
        PUBLIC include
        PRIVATE src
        )