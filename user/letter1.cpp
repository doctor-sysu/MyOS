static char info[40] = "16337062FENGZHENXUAN16337067GUANPEIDONG";
static int info_x = 0;
static int info_y = 0;
static int index = 0;
static int info_count = 40;
/*
 * 1 right down
 * 2 right up
 * 3 left down
 * 4 left up
 */
static int info_state = 1;

extern "C" int main() {
    int count = 0;
    while (info_count) {
        if (count < 100000000){
            count++;
            continue;
        }
        count = 0;

        --info_count;
        if (info_count)
            return 0;
        char *videomem_info = reinterpret_cast<char *>(0xb8000 + 2 * (80 * (info_y + 1) + info_x + 40));
        *(videomem_info) = info[index++];
        if (index == 40)
            index = 0;
        if (info_state == 1) {
            ++info_x;
            ++info_y;
            if (info_x == 37 && info_y == 11)
                info_state = 4;
            else if (info_y == 11)
                info_state = 2;
            else if (info_x == 37)
                info_state = 3;
        } else if (info_state == 2) {
            ++info_x;
            --info_y;
            if (info_x == 37 && info_y == 0)
                info_state = 3;
            else if (info_x == 37)
                info_state = 4;
            else if (info_y == 0)
                info_state = 1;
        } else if (info_state == 3) {
            --info_x;
            ++info_y;
            if (info_x == 0 && info_y == 11)
                info_state = 2;
            else if (info_x == 0)
                info_state = 1;
            else if (info_y == 11)
                info_state = 4;
        } else if (info_state == 4) {
            --info_x;
            --info_y;
            if (info_x == 0 && info_y == 0)
                info_state = 1;
            else if (info_x == 0)
                info_state = 2;
            else if (info_y == 0)
                info_state = 3;
        }
    }
    info_count = 40;
    return 0;
}