
//printA
static char *start = reinterpret_cast<char *>(0xb8000);
static int x = 0;
static int y = 0;
static int count = 40;
static char letter = 'A' - 1;

//printOuch
static int flag = 5;

extern "C" void printA() {
    --count;
    if (count) {
        return;
    }
    if (x == 0 && y == 0) {
        //clear the screen
        for (int i = 0; i < 2000; ++i)
            *(start + 2 * i) = ' ';
        if (letter == 'Z')
            letter = 'A';
        else
            ++letter;
    }
    char *videomemory = start + 2 * (80 * y + x);
    *(videomemory) = letter;
    if (y == 0 && x < 79)
        ++x;
    else if (x == 79 && y < 24)
        ++y;
    else if (y == 24 && x > 0)
        --x;
    else
        --y;
    count = 40;
    //clear "OUCH!"
    if (flag == 0)
        return;
    else if (flag == 1) {
        char *video = reinterpret_cast<char *>(0xb8000 + 2 * (13 * 80 + 35));
        for (int i = 0; i < 10; ++i) {
            *(video) = ' ';
            video += 2;
        }
        flag = 0;
    } else if (flag > 1)
        --flag;
}

extern "C" void printOuch() {
    char *videomem = reinterpret_cast<char *>(0xb8000 + 2 * (13 * 80 + 35));
    char str[11] = "OUCH!OUCH!";
    for (int i = 0; i < 10; ++i) {
        *(videomem) = str[i];
        videomem += 2;
    }
    flag = 10;
}

//windwill
static int state = 1;
static char *videomem_windwill = reinterpret_cast<char *>(0xb8000 + 2 * (5 * 80 + 20));

//floppy interrupt
extern bool finishReadSector;
extern "C" void read_finished(){
    finishReadSector = true;
}

extern "C" void windmill() {
    if (state == 5) {
        *(videomem_windwill) = '-';
        state = 1;
    } else {
        if (state == 1)
            *(videomem_windwill) = '|';
        else if (state == 2)
            *(videomem_windwill) = '/';
        else if (state == 3)
            *(videomem_windwill) = '-';
        else if (state == 4)
            *(videomem_windwill) = '\\';
        ++state;
    }
}


//print_info
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

extern "C" void print_info() {
    --info_count;
    if (info_count)
        return;
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
    info_count = 40;
}

//every 100 clock interrupts have a flash
int helloworld_count = 200;

extern "C" void print_Hello() {
    if (helloworld_count != 100)
        return;
    char *videomem_Hello = reinterpret_cast<char *>(0xb8000 + 2760);
    for (int i = 0; i < 5; ++i) {
        *(videomem_Hello + 60) = ' ';
        videomem_Hello += 2;
    }
    videomem_Hello -= 10;
    char str[6] = "Hello";
    for (int i = 0; i < 5; ++i) {
        *(videomem_Hello) = str[i];
        videomem_Hello += 2;
    }
}

extern "C" void print_World() {
    --helloworld_count;
    if (helloworld_count)
        return;
    char *videomem_World = reinterpret_cast<char *>(0xb8000 + 2820);
    for(int i = 0; i < 5; ++i)
    {
        *(videomem_World - 60) = ' ';
        videomem_World += 2;
    }
    videomem_World -= 10;  
    char str[6] = "World";
    for (int i = 0; i < 5; ++i) {
        *(videomem_World) = str[i];
        videomem_World += 2;
    }
    helloworld_count = 200;
}
