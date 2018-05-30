project(myos-KernelProcess)

enable_language(ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf32 -o <OBJECT> <SOURCE>")

add_executable(kerp
        kerp.cpp
        )

target_compile_options(kerp
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        )
set_target_properties(kerp PROPERTIES
        LINK_FLAGS "-m32 -nostdlib -nostartfiles -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
        )
target_link_libraries(kerp
        clib
        cpplib
        )