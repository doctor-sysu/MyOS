#include<System.hpp>
#include <myos/kernel/Terminals.hpp>


void myos::kernel::Terminals::switch_video_page(int pageid) {
    //write back page0 to current process's video page
   memcpy((void*)videomem_base + videomem_size * cur_video_page, videomem_base, videomem_size);

    //write back the poistion
    curx[cur_video_page] = scr_x;
    cury[cur_video_page] = scr_y;

    //load
    memcpy(videomem_base, (void*)videomem_base + pageid * videomem_size, videomem_size);
    scr_x = curx[pageid];
    scr_y = cury[pageid];
    cur_video_page = pageid;

    static char* info = const_cast<char *>("Terminal  ");
    int info_length = 10;
    info[9] = static_cast<char>('0' + cur_video_page);
    disp_str(info, info_length);
}

void myos::kernel::Terminals::disp_str(char *str, int length) {
    for(int i = 0; i < length; ++i) {
        if(str[i] == '\n')
        {
            scr_y = 0;
            ++scr_x;
            continue;
        }
        *reinterpret_cast<char*>(videomem_base + 2 * (scr_x + 80 * scr_y++)) = str[i];
        if(scr_y == 80)
        {
            scr_y = 0;
            ++scr_x;
        }
        if(scr_x == 25)
            scr_x = scr_y = 0;
    }
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
}
