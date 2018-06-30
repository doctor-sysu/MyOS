#pragma once
#include"Scancode.hpp"

namespace myos{
namespace kernel{

#define videomem_base 0xb8000

class Terminals
{
private:
    int cur_video_page;
    int scr_x;
    int scr_y;

    const static int videomem_size = 4000;
    const static int max_video_pages = 3;
    int curx[max_video_pages];
    int cury[max_video_pages];


public:
    Terminals();
    void switch_video_page(int pageid);
    void disp_str(int pageid, char* str, int length);
};

}
}


