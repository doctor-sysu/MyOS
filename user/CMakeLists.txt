project(myos-user)

enable_language(ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf32 -o <OBJECT> <SOURCE>")

add_executable(letter1
        letter1.cpp

        )

target_compile_options(letter1
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        )
set_target_properties(letter1 PROPERTIES
        LINK_FLAGS "-m32 -nostdlib -nostartfiles -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/linker_l1.ld"
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker_l1.ld
        )
target_link_libraries(letter1
        clib
        cpplib
        )

add_executable(letter2
        letter2.cpp
        )

target_compile_options(letter2
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        )
set_target_properties(letter2 PROPERTIES
        LINK_FLAGS "-m32 -nostdlib -nostartfiles -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/linker_l2.ld"
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker_l2.ld
        )
target_link_libraries(letter2
        clib
        cpplib
        )

add_executable(letter3
        letter3.cpp
        )

target_compile_options(letter3
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        )
set_target_properties(letter3 PROPERTIES
        LINK_FLAGS "-m32 -nostdlib -nostartfiles -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/linker_l3.ld"
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker_l3.ld
        )
target_link_libraries(letter3
        clib
        cpplib
        )

add_executable(letter4
        letter4.cpp
        )

target_compile_options(letter4
        PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-m32 -masm=intel -ffreestanding -fno-stack-protector -fno-exceptions -fno-rtti -mno-red-zone>
        )
set_target_properties(letter4 PROPERTIES
        LINK_FLAGS "-m32 -nostdlib -nostartfiles -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/linker_l4.ld"
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker_l4.ld
        )
target_link_libraries(letter4
        clib
        cpplib
        )