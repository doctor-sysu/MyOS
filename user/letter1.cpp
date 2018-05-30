
#include <System.hpp>

void sleep();


extern "C" int main() {
//    char *videomem_Hello = reinterpret_cast<char *>(0xb8000 + 2770);
//    char str[6] = "Hello";
//    for (int i = 0; i < 2; ++i) {
//        *(videomem_Hello) = str[i];
//        videomem_Hello += 2;
//    }
//    //SysC(1);
//    for (int i = 2; i < 5; ++i) {
//        *(videomem_Hello) = str[i];
//        videomem_Hello += 2;
//    }

    for(int i = 0; i < 100000; ++i) {
//        for (int j = 0; j < 5; ++j){
//            *(videomem_Hello) = str[j];
//            videomem_Hello += 2;
//        }
        SysC(2);
        sleep();
//        for (int j = 0; j < 5; j++){
//            *(videomem_Hello) = ' ';
//            videomem_Hello -= 2;
//        }
    }
    //SysC(90);
    while (true){}
}

void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}