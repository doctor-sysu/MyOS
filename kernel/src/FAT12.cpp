#include <myos/kernel/IDT.hpp>
#include <clib.h>
#include <myos/kernel/FAT12.hpp>
#include <myos/kernel/MemoryManage.hpp>

namespace myos {
namespace kernel {
namespace FAT12 {

using FAT12::readSector;

uint32_t datasector = 0;

struct Directory_Entry {
    uint8_t DIR_NAME[11];                //文件名8字节，扩展名3字节
    uint8_t DIR_Attr[1];                //文件属性，1字节
    uint8_t unused[10];                  //保留，10字节
    uint8_t DIR_WrtTime[2];             //最后修改时间，2字节
    uint8_t DIR_WrtData[2];             //最后修改日期，2字节
    uint8_t DIR_FstClus[2];             //此条目对应的开始簇号，2字节
    uint8_t DIR_FileSize[4];            //文件大小，4字节
};

struct Directory_Entry Root_Directory[10];

struct FAT_Entry1 {
    /*簇在FAT中的占用1.5个字节、2个字节或4个字节的登记项必须是下列值之一：
    ·一个“0”值，表示这个簇未被分配使用。
    ·一个“001H～FEFH”、“0001H～FFEFH”或“00000001H～FFFFFFEFH”之间的值，表示该簇已经被占用，且该值即是该簇所在文件中下一个簇号的指针。
    ·一个“BAD”(FF7H、FFF7H或FFFFFFF7H), 表明该簇有坏扇区，不能使用。
    一个“EOF”(FF8H～FFFH、FFF8H～FFFFH或FFFFFFF8H～FFFFFFFFH之间的任一值), 表明该簇是文件中的最后一簇。*/
    char Current_Cluster[2];
};

uint32_t ClusterLBA(const uint32_t cluster) {
    return cluster - 2;
}

//read a sector into the offset
void Read_Sector(uint32_t offset, const uint32_t LBA) {
    //use the new way to read sector
    readSector(reinterpret_cast<uint8_t *>(offset), LBA);
}

//加载引导扇区进内存
void Load_Root_Directory(uint32_t RD_In_Memory) {
    uint32_t Root_Directory_Size = ((bpbRootEntries * 32) + (bpbBytesPerSector - 1))    //计算根目录大小
                                   / bpbBytesPerSector;
    uint32_t Root_Directory_Place = bpbNumberOfFATs * bpbSectorsPerFAT//计算根目录的LBA（逻辑块地址）
                                    + bpbReservedSectors;
    datasector = Root_Directory_Place + Root_Directory_Size;

    uint32_t i = 0;
    uint32_t load_address = RD_In_Memory;
    //for (i=0 ; i < Root_Directory_Size ; i++){
    Read_Sector(load_address, Root_Directory_Place);
    uint8_t *ptr = (uint8_t *) load_address;
    //}
    for (; i < 9; i++) {
        uint32_t j = 0;
        for (; j < 11; j++) {
            Root_Directory[i].DIR_NAME[j] = *ptr;
            ptr = ptr + 1;
        }
        Root_Directory[i].DIR_Attr[0] = *ptr;
        ptr = ptr + 1;
        for (j = 0; j < 10; j++) {
            Root_Directory[i].unused[j] = *ptr;
            ptr = ptr + 1;
        }
        Root_Directory[i].DIR_WrtTime[0] = *ptr;
        ptr = ptr + 1;
        Root_Directory[i].DIR_WrtTime[1] = *ptr;
        ptr = ptr + 1;
        Root_Directory[i].DIR_WrtData[0] = *ptr;
        ptr = ptr + 1;
        Root_Directory[i].DIR_WrtData[1] = *ptr;
        ptr = ptr + 1;
        Root_Directory[i].DIR_FstClus[0] = *ptr;
        ptr = ptr + 1;
        Root_Directory[i].DIR_FstClus[1] = *ptr;
        ptr = ptr + 1;
        for (j = 0; j < 4; j++) {
            Root_Directory[i].DIR_FileSize[j] = *ptr;
            ptr = ptr + 1;

        }
    }
}

int Find_File(char *file_name) {
    uint32_t i = 0;
    for (; i < 9; i++) {
        int j = 0;
        char require_file[12];
        for (j = 0; j < 11; j++) require_file[j] = Root_Directory[i].DIR_NAME[j];
        require_file[11] = '\0';
        if (!strcmp(file_name, require_file)) {
            return i;
        }
    }
    return -1;
}

//从扇区中读取FAT进内存
void Load_FAT(uint32_t begin, uint32_t FAT_In_Memory, uintptr_t userCR3) {
    uint32_t i = 0;
    uint32_t memory = FAT_In_Memory;
    uint32_t number_of_FAT_sector = bpbSectorsPerFAT;
    uint32_t FAT_LBA = bpbReservedSectors;
    //mprintf((char*) memory);
    //for (; i<number_of_FAT_sector; i++){
    Read_Sector(memory, FAT_LBA);
    memory += bpbBytesPerSector;
    FAT_LBA++;
    //}

    PageTableEntry *pte = reinterpret_cast<PageTableEntry *>
    (reinterpret_cast<PageDirectoryEntry *>(userCR3)->PageTableAddress << OFFSET);
    uint32_t offset =//static_cast<uint32_t>(0x200000 + 0x20000 * num);
            (pte+USER_START*SIZE_OF_ENTRY)->PageTable << OFFSET;
    uint8_t pageFinish = 0;
    uint32_t pteFinish = 0;
    uint32_t pdeCount = 0;
    uint32_t now = begin;

    while (now >= 0x0002 && now <= 0x0FF6) {
        //把用户程序的当前簇读入内存
        uint32_t LBA = ClusterLBA(now);
        Read_Sector(offset, LBA + datasector);

        offset += bpbBytesPerSector;
        pageFinish++;
        if (pageFinish == 8) {
            pageFinish = 0;
            pteFinish++;
            if (pteFinish == 1024) {
                pteFinish = 0;
                pdeCount++;
            }
            pte = reinterpret_cast<PageTableEntry *>
            (reinterpret_cast<PageDirectoryEntry *>(userCR3 + pdeCount*SIZE_OF_ENTRY)->PageTableAddress << OFFSET);
            offset = (pte + pteFinish*SIZE_OF_ENTRY)->PageTable << OFFSET;
        }

        //获取下一个簇号
        uint32_t index;
        if (now % 2 == 0)
            index = now / 2 * 3;
        else index = (now - 1) / 2 * 3 + 1;

        uint8_t next1 = *((uint8_t *) (FAT_In_Memory) + index);
        uint8_t next2 = *((uint8_t *) (FAT_In_Memory) + index + 1);
        uint32_t next;

        if (now % 2 == 0) {
            uint32_t replace = next2 & 0x0Fu;
            next = next1 + (replace * 256u);
            next &= 0x0FFFu;
        } else {
            uint32_t replace = next1 / 16u;
            next = (next2 * 16u) + replace;
            next &= 0x0FFFu;
        }

        if (next < 0x0002 || next > 0x0FF6) break;
        now = next;
    }
}

//把root directory读入内存,并把在内存的引导扇区放入Root_Directory数组
void Load_RD() {
    asm volatile(
    "cli"
    );
    uint32_t RD_In_Memory = 0x6000;
    Load_Root_Directory(RD_In_Memory);
    asm volatile(
    "sti"
    );
}

//主函数，返回文件信息
bool FAT12(char *file, uintptr_t userCR3) {
    //static char num = 0;
    asm volatile(
    "cli"
    );
    //找到目标文件在根目录中的位置,在root_directory中搜索文件
    //char file_name[13] = "LETTER0 EXE\0";
    //file_name[6] = static_cast<char>(file + 48u);
    int file_In_Directory = Find_File(file);
    if (file_In_Directory == -1) {
        asm volatile(
        "sti"
        );
        return false;
    }
    //先加载FAT表进内存，然后放到FAT数组FAT_List里
    uint32_t FAT_In_Memory = 0x7000;
    uint8_t _begin[2];
    //读取第一个cluster
    _begin[0] = Root_Directory[file_In_Directory].DIR_FstClus[0];
    _begin[1] = Root_Directory[file_In_Directory].DIR_FstClus[1];
    uint32_t begin = _begin[0] + _begin[1] * 256u;
    Load_FAT(begin, FAT_In_Memory, userCR3);    //把FAT表加载入内存

    //然后用簇计算出LBA然后读用户程序的一个簇，并把用户程序读入内存
    //然后跳到下一个簇，依次循环读用户程序，直到遇到0x0FF7或以上的停止读扇区。
    //这样一来，用户程序就在内存里了。
    //num++;
    //return reinterpret_cast<void *>(0x200000 + 0x20000 * (num - 1));        //已经把用户程序加载到偏移量为 0x200000 处
    return true;
}

}
}
}

//显示用户程序的信息
//void printFile_Info()
//{
//    uint32_t index = 0;
//    for(; index < 4; ++index)
//    {
//        char info[15] = "NAME:";
//        uint32_t j = 0;
//        for (; j < 11; ++j)
//            info[j+5] = Root_Directory[index].DIR_NAME[j];
//        char info1[15] = "   SIZE:";
//        info1[0] = Root_Directory[index].DIR_NAME[10];
//
//        {uint32_t size = 0, mul = 1;
//            for (j = 11; j < 15; ++j){
//                size = size + Root_Directory[index].DIR_FileSize[j-11] * mul;
//                mul *= 256;
//            }
//            char test[12];
//            uint32_t a = size,i=0;
//            while (a){
//                test[i] = a % 10 +'0';
//                a = a/10;
//                i++;
//            }
//            uint32_t k = 0;
//            for (;k<i;k++) info1[14-k] = test[k];
//            for (k=8;k<15-i;k++) info1[k] =' ';
//
//        }
//        char info2[15] = "     LOCATION:";
//
//        char info3[15];
//        {	uint32_t location = 0, mul = 1;
//            for (j = 0; j < 2; ++j){
//                location = location + Root_Directory[index].DIR_FstClus[j] * mul;
//                mul *= 256;
//            }
//            char test[12];
//            uint32_t a = location,i=0;
//            while (a){
//                test[i] = a % 10 +'0';
//                a = a/10;
//                i++;
//            }
//            int k = 0;
//            for (;k<i;k++) info3[k] = test[i-k-1];
//            for (k=i;k<13;k++) info3[k] =' ';
//            info3[13] = '\r';
//            info3[14] = '\n';
//        }
//
//        mprintf(info);
//        mprintf(info1);
//        mprintf(info2);
//        mprintf(info3);
//    }
//    return;
//}