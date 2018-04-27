#include <myos/kernel/FAT12.hpp>
#include <clib.h>

// standard base address of the primary floppy controller
static const uint16_t floppy_base = 0x03f0;
//the floppy controller interface, not include all the interface
//only include what should be used
uint16_t DIGITAL_OUTPUT_REGISTER = 0x3f2; //base + 2
uint16_t DATA_FIFO = 0x3f5;               //base + 5
uint16_t MAIN_STATUS_REGISTER = 0x3f4;    //base + 4
void *buffer = nullptr;
bool finishReadSector;

namespace myos {
namespace kernel {
namespace FAT12 {


uint8_t in(uint32_t port) { //read data form port
    uint8_t data = 0;
    asm volatile(
    "in %0, dx"
    :"=r"(data)
    :"d"(port)
    );
    return data;
}

void out(uint32_t port, uint8_t data) {  //write data to port
    asm volatile(
    "out dx, al"
    ::"d"(port), "a"(data)
    );
}

void LBA_To_CHS(const uint32_t LAB, uint32_t *absolute_sector,
                uint32_t *absolute_head, uint32_t *absolute_track) {
    *absolute_sector = (LAB % (2 * bpbSectorsPerTrack)) % bpbSectorsPerTrack + 1;
    *absolute_head = (LAB % (2 * bpbSectorsPerTrack)) / bpbSectorsPerTrack;
    *absolute_track = LAB / (2 * bpbSectorsPerTrack);
}

uint8_t getMSR() {
    return in(MAIN_STATUS_REGISTER);
}

void wait() {
    bool ready;
    do {
        uint8_t MSR = getMSR();
        ready = (MSR & 0x80) == 0x80; //RQM == 1 then can send command to FIFO
    } while (!ready);
}

//the way to send command to FIFO
void send_cmd_ToFIFO(uint8_t command) {
    wait();
    out(DATA_FIFO, command);
}

void writeDOR(uint8_t data) {
    out(DIGITAL_OUTPUT_REGISTER, data);
}

void setupDMA() {   //this function is to set how the DMA run
    out(0x0A, 0x06);    //mask channel 2
    out(0x0C, 0xFF);    //reset flip-flop
    uint16_t addressL = 0x00;  //set the beginning of DMA buffer
    uint16_t addressH = 0x05;  //to 0x500
    //the floppy will be read in this place
    buffer = reinterpret_cast<void *>(addressH << 8 + addressL);
    out(0x04, static_cast<uint8_t>(addressL));
    out(0x04, static_cast<uint8_t>(addressH));

    out(0x0C, 0xFF);    //reset flip-flop again
    out(0x05, 0xFF);    //size that DMA read 512 bytes
    out(0x05, 0x01);
    out(0x81, 0);       //set the high 8 bits of the address to 0
    out(0x0B, 0x46);    //01:0:0:01:10 = single/inc/no-auto/to-mem/chan2
    out(0x0A, 0x02);    //unmask channel
}

uint8_t readData() {
    wait();
    return in(DATA_FIFO);
}

void memCopy(void *dest, const void *buf, uint16_t len) {
    uint32_t i = 0;
    if ((uintptr_t) dest % sizeof(long) == 0 &&
        (uintptr_t) buf % sizeof(long) == 0 &&
        len % sizeof(long) == 0) {
        long *d = static_cast<long *>(dest);
        const long *s = static_cast<const long *>(buf);
        for (i = 0; i < len / sizeof(long); i++) {
            d[i] = s[i];
        }
    } else {
        char *d = static_cast<char *>(dest);
        const char *s = static_cast<const char *>(buf);
        for (i = 0; i < len; i++) {
            d[i] = s[i];
        }
    }
}

bool readSector(uint8_t *dest, uint32_t LBA) {
    setupDMA();

    //get absolute address
    uint32_t track, head, sector;
    LBA_To_CHS(LBA, &sector, &head, &track);

    finishReadSector = false;
    // reset controller
    writeDOR(0);
    for (int i = 0; i < 50000; ++i) {
        asm volatile("nop");
    }
    //open drive 0, enable controller, normal operation
    writeDOR(0b00011100);
    asm volatile(
    "sti"
    );
    while (!finishReadSector);    // wait for the IRQ handler to run
    asm volatile(
    "cli"
    );
    // SENSE_INTERRUPT : ack IRQ6, get status of last command
    send_cmd_ToFIFO(8);
    //get some message from FIFO, but we don't care, assume it doesn't go wrong
    readData();
    readData();

    finishReadSector = false;
    // send command to read sector
    send_cmd_ToFIFO(0b01000110);  // set above for current direction
    send_cmd_ToFIFO(static_cast<uint8_t>(head << 2));    // 0:0:0:0:0:HD:US1:US0 = head and drive
    send_cmd_ToFIFO(static_cast<uint8_t>(track));  // track
    send_cmd_ToFIFO(static_cast<uint8_t>(head));    // first head (should match with above)
    send_cmd_ToFIFO(static_cast<uint8_t>(sector));    // first sector, strangely counts from 1
    send_cmd_ToFIFO(2);    // bytes/sector, 128*2^x (x=2 -> 512)
    send_cmd_ToFIFO(1);   // number of tracks to operate on
    send_cmd_ToFIFO(0x1a); // GAP3 length, 27 is default for 3.5"
    send_cmd_ToFIFO(0xff); // data length (0xff if B/S != 0)

    // wait for IRQ6, means the sector has been read
    asm volatile(
    "sti"
    );
    while (!finishReadSector);
    asm volatile(
    "cli"
    );

    // get some return from FIFO, if not, it will go wrong
    for (int i = 0; i != 7; ++i) {
        readData();
    }
    //copy a sector to expected memory
    memCopy(dest, buffer, 512);

    return true;
}

}
}
}
