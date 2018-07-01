#include <System.hpp>


void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}

extern "C" int main() {
//    char s[11] = "abcd";
//    printf(s);
//    char p1[13] = "LETTER1 EXE\0";
//    char p2[13] = "LETTER2 EXE\0";
//    char p3[13] = "LETTER3 EXE\0";
//    newProcess(p1);
//    newProcess(p2);
//    newProcess(p3);
    SysC(6);
    SysC(6);
    SysC(6);
    
    while (true){

    }
}





