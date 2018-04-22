#pragma once

#include <cstdint>
#include <cstddef>

//extern void mprintf(char * pszInfo);
//extern int mgetchar();
namespace  myos {
namespace kernel {
namespace FAT12 {

int FAT12(char file);

uint32_t mgetchar();

void Load_RD();

void printFile_Info();

#define Root_Directory_Begin  0x0020 //根目录放在第20个扇区，FAT12规定
#define File_Image_Memory  0x8000     //read file into memory (8000:0000)
#define Root_Directory_Memory  0x0500 //read root directory or FAT into memory (0500:0000)
#define Directory_Size  0x20          //size of each directory


//judge whether finish reading sector

bool readSector(uint8_t *, uint32_t);

#define bpbBytesPerSector  512
#define bpbRootEntries  224
#define bpbReservedSectors  1
#define bpbSectorsPerTrack  18
#define bpbHeadsPerCylinder  2
#define bpbNumberOfFATs  2
#define bpbSectorsPerFAT  9

}
}
}
