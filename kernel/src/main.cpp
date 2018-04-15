
extern "C" int main() {
    char *video = reinterpret_cast<char *>(0xb8000);
    *(video) = 'A';

    return 0;
}
