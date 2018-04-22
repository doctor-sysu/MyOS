

extern "C" int main() {
    char info[40] = "16337062FENGZHENXUAN16337067GUANPEIDONG";
    int info_x = 0;
    int info_y = 0;
    int index = 0;

    char *videomem_info = reinterpret_cast<char *>(0xb8000 + 2 * (80 * (info_y + 1) + info_x + 40));
    *(videomem_info) = info[5];

    //info_count = 40;
    return 0;
}