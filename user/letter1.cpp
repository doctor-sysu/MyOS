
#include <System.hpp>

extern "C" int main() {
    char *videomem_Hello = reinterpret_cast<char *>(0xb8000 + 2760);
    char str[6] = "Hello";
    for (int i = 0; i < 2; ++i) {
        *(videomem_Hello) = str[i];
        videomem_Hello += 2;
    }
    //SysC(90);
    for (int i = 2; i < 5; ++i) {
        *(videomem_Hello) = str[i];
        videomem_Hello += 2;
    }
    SysC(90);
    return 0;
}