#pragma once

#include <cstdint>
#include <cstddef>

namespace  myos {
namespace kernel {
namespace FAT12 {

int FAT12(char file);

uint32_t mgetchar();

void Load_RD();

void printFile_Info();

//judge whether finish reading sector

void readSector(uint8_t *, uint32_t);

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
