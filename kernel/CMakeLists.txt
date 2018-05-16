project(myos-kernel)

enable_language(ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf32 -o <OBJECT> <SOURCE>")

add_executable(kernel
        src/main.cpp
        src/cpu.asm
        src/8259A.asm
        src/IDT.cpp
        include/myos/kernel/IDT.hpp
        include/cstdint
        include/cstddef
        src/interrupt.asm
        src/interrupt.cpp
        src/start.asm
        src/multiboot.asm
        include/myos/kernel/FAT12.hpp
        src/driver.cpp
        src/FAT12.cpp
        include/myos/kernel/Process.hpp
        src/Process.cpp
        include/myos/kernel/Keyboard.hpp
        src/Keyboard.cpp
        include/myos/kernel/RW_port.hpp
        src/RW_port.cpp
        src/Syscall.cpp
        src/Syscall.asm
        )

target_compile_options(kernel
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        )
target_include_directories(kernel
        PRIVATE include
        )
set_target_properties(kernel PROPERTIES
        LINK_FLAGS "-m32 -nostdlib -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
        )
target_link_libraries(kernel
        clib
        cpplib
        )