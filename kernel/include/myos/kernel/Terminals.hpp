#pragma once

namespace myos{
namespace kernel{

class Terminals
{
private:
    int cur_video_page;
    int scr_x;
    int scr_y;

    void* videomem_base = reinterpret_cast<void*> (0xb8000);
    const static int videomem_size = 4000;
    const static int max_video_pages = 3;
    int curx[max_video_pages];
    int cury[max_video_pages];

    void disp_str(char* str, int length);
public:
    Terminals();
    void switch_video_page(int pageid);

};

}
}


