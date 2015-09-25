#ifndef STRUCT
#define STRUCT

struct MultitouchRecord {
    uint16_t y;
    uint16_t x;
} __attribute__((packed));

struct Record {
    uint8_t header;
    uint8_t state;
    uint16_t x1;
    uint16_t y1;
    uint8_t delimeter;
    uint8_t multitouchFlag;
    MultitouchRecord multitouchPoints[5];
    uint16_t ending;
} __attribute__((packed));

#endif // STRUCT

