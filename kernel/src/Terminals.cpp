#include<System.hpp>
#include <myos/kernel/Terminals.hpp>

myos::kernel::Terminals terminal;

void myos::kernel::Terminals::switch_video_page(int pageid) {
    //write back page0 to current process's video page
   memcpy((void*)(videomem_base + videomem_size * cur_video_page), (void*)(videomem_base), videomem_size);

    //write back the poistion
    curx[cur_video_page] = scr_x;
    cury[cur_video_page] = scr_y;

    //load
    memcpy((void*)videomem_base, (void*)videomem_base + pageid * videomem_size, videomem_size);

    cur_video_page = pageid;
    scr_x = 0;
    scr_y = 24;

    static char* info = const_cast<char *>("Terminal  ");
    int info_length = 10;
    info[9] = static_cast<char>('0' + cur_video_page);
    disp_str(cur_video_page, info, info_length);

    scr_x = curx[pageid];
    scr_y = cury[pageid];
}

void myos::kernel::Terminals::disp_str(int pageid, char *str, int length) {
    scr_x = curx[pageid];
    scr_y = cury[pageid];
    for(int i = 0; i < length; ++i) {
        if(str[i] == '\n')
        {
            scr_y = 0;
            ++scr_x;
            continue;
        }
        *reinterpret_cast<char*>(videomem_base + pageid * videomem_size + 2 * (scr_x++ + 80 * scr_y)) = str[i];
        if(scr_y == 80)
        {
            scr_y = 0;
            ++scr_x;
        }
        if(scr_x == 25)
            scr_x = scr_y = 0;
    }
    scr_x = curx[cur_video_page];
    scr_y = cury[cur_video_page];
}

myos::kernel::Terminals::Terminals() {
    cur_video_page = 0;
    scr_x = 0;
    scr_y = 0;
    for(int i = 0; i < max_video_pages; ++i)
    {
        curx[i] = 0;
        cury[i] = 0;
    }
    for(int i = 1; i < 4; ++i) {
        switch_video_page(i);
        char info[7] = "page  ";
        info[5] = static_cast<char>('0' + i);
        disp_str(i, info, 6);
    }
    switch_video_page(0);
}
