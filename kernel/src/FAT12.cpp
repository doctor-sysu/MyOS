#include <myos/kernel/IDT.hpp>
#include <myos/kernel/FAT12.hpp>
#include "clib.h"

// extern const short Root_Directory_Begin;  //根目录放在第20个扇区，FAT12规定
// extern const short File_Image_Memory;     //read file int32_to memory (8000:0000)
// extern const short Root_Directory_Memory; //read root directory or FAT int32_to memory (0500:0000)

int32_t absolute_sector = 0;
int32_t absolute_head = 0;
int32_t absolute_track = 0;
int32_t datasector = 0;

extern void print();
extern void mprintf(char * pszInfo);
int mgetchar(){
    int eax = 0x0000, result = 0;
    asm volatile(
    "int 0x16 \n"
    "mov %0, eax\n"
    :"=r"(result)
    :"a"(eax)
    );
    //print();
    return result;
}

// int32_t c2i(char ch)
// {
// 	// 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2
// 	if (ch >= '0' && ch <= '9')
// 		return ch - 48;

// 	// 如果是字母，但不是A~F,a~f则返回
// 	if (ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z')
// 		return -1;

// 	// 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10
// 	// 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10
// 	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
// 		return (ch >= 'A' && ch <= 'Z') ? ch - 55 : ch - 87;

// 	return -1;
// }

// void cluter_to_abs(char* cluster)
// {
// 	int32_t len = 8;
// 	int32_t bits;
// 	int32_t num = 0;
// 	for (int32_t i = 0, temp = 0; i<len; i++, temp = 0)
// 	{
// 		temp = c2i(*(cluster + i));
// 		bits = (len - i - 1) * 4;
// 		temp = temp << bits;
// 		num = num | temp;
// 	}

// 	int32_t lba = num - 2;  // 假设每个簇的扇区数为1

// 	//lba to chs
// 	absolute_sector = (lba % SectorsPerTrack) + 1;
// 	absolute_head = (lba / SectorsPerTrack) % NumberOfHeads;
// 	absolute_track = lba / (SectorsPerTrack * NumberOfHeads);
// }

struct Directory_Entry
{
    char DIR_NAME[11];                //文件名8字节，扩展名3字节
    char DIR_Attr[1];                //文件属性，1字节
    char unused[10];                  //保留，10字节
    char DIR_WrtTime[2];             //最后修改时间，2字节
    char DIR_WrtData[2];             //最后修改日期，2字节
    char DIR_FstClus[2];             //此条目对应的开始簇号，2字节
    char DIR_FileSize[4];            //文件大小，4字节
};

struct Directory_Entry Root_Directory[10];
//int32_t Root_EntCnt;

struct FAT_Entry1
{
    /*簇在FAT中的占用1.5个字节、2个字节或4个字节的登记项必须是下列值之一：
    ·一个“0”值，表示这个簇未被分配使用。
    ·一个“001H～FEFH”、“0001H～FFEFH”或“00000001H～FFFFFFEFH”之间的值，表示该簇已经被占用，且该值即是该簇所在文件中下一个簇号的指针。
    ·一个“BAD”(FF7H、FFF7H或FFFFFFF7H), 表明该簇有坏扇区，不能使用。
    一个“EOF”(FF8H～FFFH、FFF8H～FFFFH或FFFFFFF8H～FFFFFFFFH之间的任一值), 表明该簇是文件中的最后一簇。*/
    char Current_Cluster[2];
    //char Next_Cluster[2];
};

typedef struct FAT_Entry1 FAT_Entry;
// FAT_Entry FAT1[50];
// FAT_Entry FAT2[50];
// int32_t FAT_EntCnt;

//显示用户程序的信息
void printFile_Info()
{
    int index = 0;
    for(; index < 4; ++index)
    {
        char info[15] = "NAME:";
        int j = 0;
        for (; j < 11; ++j)
            info[j+5] = Root_Directory[index].DIR_NAME[j];
        //mprintf(info);
        char info1[15] = "   SIZE:";
        info1[0] = Root_Directory[index].DIR_NAME[10];
        // info[0] = Root_Directory[index].DIR_NAME[10];
        // info[1] = '\r';
        // info[2] = '\n';
        // info[3] = 'S';
        // info[4] = 'I';
        // info[5] = 'Z';
        // info[6] = 'E';
        // info[7] = ':';
        {uint32_t size = 0, mul = 1;
            for (j = 11; j < 15; ++j){
                size = size + Root_Directory[index].DIR_FileSize[j-11] * mul;
                mul *= 256;
            }
            char test[12];
            int a = size,i=0;
            while (a){
                test[i] = a % 10 +'0';
                a = a/10;
                i++;
            }
            int k = 0;
            for (;k<i;k++) info1[14-k] = test[k];
            for (k=8;k<15-i;k++) info1[k] =' ';

        }
        char info2[15] = "      LOCATION:";

        char info3[15];
        {	uint32_t location = 0, mul = 1;
            for (j = 0; j < 2; ++j){
                location = location + Root_Directory[index].DIR_FstClus[j] * mul;
                mul *= 256;
            }
            char test[12];
            int a = location,i=0;
            while (a){
                test[i] = a % 10 +'0';
                a = a/10;
                i++;
            }
            int k = 0;
            for (;k<i;k++) info3[k] = test[i-k-1];
            for (k=i;k<13;k++) info3[k] =' ';
            info3[13] = '\r';
            info3[14] = '\n';
        }

        mprintf(info);
        mprintf(info1);
        mprintf(info2);
        mprintf(info3);
    }
    return;
}

void LBA_To_CHS(const int32_t LAB){
    absolute_sector = (LAB % (2 * bpbSectorsPerTrack)) % bpbSectorsPerTrack+ 1;
    absolute_head = (LAB % (2*bpbSectorsPerTrack)) / bpbSectorsPerTrack;
    absolute_track = LAB / (2*bpbSectorsPerTrack);
}

int32_t ClusterLBA(const int32_t cluster){
    return cluster -2;
}

void Read_Sector(int32_t offset, const int32_t LBA){
    absolute_sector = 0;
    absolute_head = 0;
    absolute_track = 0;

    LBA_To_CHS (LBA);		//计算CHS
    int32_t eax =0x00000201,
            ecx = absolute_sector + 0x100 * absolute_track,
            edx = 0x100 * absolute_head;
    int32_t ebx = offset;
    asm volatile(				//读扇区
    "int 0x13"
    ::"a"(eax),"b"(ebx),"c"(ecx),"d"(edx)
    :
    );
}

//加载引导扇区进内存
void Load_Root_Directory(int32_t RD_In_Memory){
    int32_t Root_Directory_Size = ((bpbRootEntries*32) + (bpbBytesPerSector-1)) 	//计算根目录大小
                                  / bpbBytesPerSector;
    int32_t Root_Directory_Place = bpbNumberOfFATs * bpbSectorsPerFAT//计算根目录的LBA（逻辑块地址）
                                   +bpbReservedSectors;
    datasector = Root_Directory_Place + Root_Directory_Size;

    int32_t i = 0;
    int32_t load_address = RD_In_Memory;
    //mprintf((char *)load_address);
    //for (i=0 ; i < Root_Directory_Size ; i++){
    Read_Sector(load_address, Root_Directory_Place);
    char* ptr = (char*)load_address;
    //}
    for (; i<9;i++){
        int32_t j = 0;
        for (; j<11; j++) {
            Root_Directory[i].DIR_NAME[j] = *ptr;	ptr = ptr + 1 ;
        }
        //mprintf(Root_Directory[i].DIR_NAME);
        Root_Directory[i].DIR_Attr[0] = *ptr; 	ptr = ptr + 1 ;
        for (j=0; j<10; j++){
            Root_Directory[i].unused[j] = *ptr;		ptr = ptr + 1 ;
        }
        Root_Directory[i].DIR_WrtTime[0] = *ptr; 	ptr = ptr + 1 ;
        Root_Directory[i].DIR_WrtTime[1] = *ptr; 	ptr = ptr + 1 ;
        Root_Directory[i].DIR_WrtData[0] = *ptr; 	ptr = ptr + 1 ;
        Root_Directory[i].DIR_WrtData[1] = *ptr; 	ptr = ptr + 1 ;
        Root_Directory[i].DIR_FstClus[0] = *ptr; 	ptr = ptr + 1 ;
        Root_Directory[i].DIR_FstClus[1] = *ptr; 	ptr = ptr + 1 ;
        for (j=0; j<4; j++){
            Root_Directory[i].DIR_FileSize[j] = *ptr;	ptr = ptr + 1 ;

        }
    }
}

int Find_File(char* file_name){
    int i = 0;
    for (; i < 9 ; i++){
        int j = 0;
        char require_file[12];
        for (j = 0; j<11; j++) require_file[j] = Root_Directory[i].DIR_NAME[j];
        require_file[11] = '\0';
        if (!strcmp(file_name,require_file)){
            //mprintf(require_file);
            return i;
        }
    }
}

//从扇区中读取FAT进内存
void Load_FAT(int32_t begin, int32_t FAT_In_Memory){
    int32_t i = 0;
    int32_t memory = FAT_In_Memory;
    int32_t number_of_FAT_sector = bpbSectorsPerFAT;
    int32_t FAT_LBA = bpbReservedSectors ;
    //mprintf((char*) memory);
    //for (; i<number_of_FAT_sector; i++){
    Read_Sector(memory, FAT_LBA);
    memory += bpbBytesPerSector;
    FAT_LBA++;
    //}
    int32_t offset = 0xA100;

    int32_t now = begin;

    while(now >= 0x0002 && now <= 0x0FF6){
        // {	char test[12];
        // 	int a = now,i=0;
        // 	while (a){
        // 		test[i] = a % 16 +'0';
        // 			if (test[i] >'9') test[i] = test[i]-'9'+'A'-1;
        // 		a = a/16;
        // 		i++;
        // 	}
        // 	mprintf(test);
        // }
        //把用户程序的当前簇读入内存
        int32_t LBA = ClusterLBA(now);
        Read_Sector(offset, LBA + datasector);

        offset += bpbBytesPerSector;

        //获取下一个簇号
        int32_t index;
        if (now % 2 == 0)
            index = now / 2 * 3;
        else  index = (now - 1) / 2 * 3 + 1;

        uchar_t next1 = *((char*)(FAT_In_Memory) + index);
        uchar_t next2 = *((char*)(FAT_In_Memory) + index +1);
        int32_t next;

        if (now % 2 == 0){
            int replace = next2 & 0x0F;
            next = next1 + (replace *256);
            next &= 0x0FFF;
        }
        else{
            int replace = next1 / 16;
            next = (next2 * 16) + replace ;
            next &= 0x0FFF;
        }

        if (next < 0x0002 && next > 0x0FF6) break;
        now = next;
    }
}

//把root directory读入内存,并把在内存的引导扇区放入Root_Directory数组
void Load_RD(){
    int32_t RD_In_Memory = 0x6000;
    Load_Root_Directory(RD_In_Memory);
}
//主函数，返回文件信息
int FAT12(char file)
{
    //找到目标文件在根目录中的位置,在root_directory中搜索文件
    char file_name[12] = "LETTER0 COM\0";
    file_name[6] = file;
    int file_In_Directory = Find_File(file_name);
    //print();
    //先加载FAT表进内存，然后放到FAT数组FAT_List里
    int32_t FAT_In_Memory = 0x7000;
    char _begin[2];
    //读取第一个cluster
    _begin[0] = Root_Directory[file_In_Directory].DIR_FstClus[0];
    _begin[1] = Root_Directory[file_In_Directory].DIR_FstClus[1];
    int32_t begin = _begin[0] + _begin[1] *256;
    Load_FAT(begin, FAT_In_Memory);	//把FAT表加载入内存

    //然后用簇计算出LBA然后读用户程序的一个簇，并把用户程序读入内存
    //然后跳到下一个簇，依次循环读用户程序，直到遇到0x0FF7或以上的停止读扇区。
    //这样一来，用户程序就在内存里了。
    return 0xA100;		//已经把用户程序加载到偏移量为 0xA100 处
}

// int32_t index = 0;
// for (; index < Root_EntCnt; ++index)
// 	if (strcmp(filename, Root_Directory[index].DIR_NAME) == 0)
// 		break;
// if (index == Root_EntCnt)
// 	return NULL;

