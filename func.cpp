#include "func.h"
#include <unistd.h>
#include <stdio.h>

bool isRoot()
{
    return getuid() == 0;
}

bool deviceExists()
{
    return access(devicePath.c_str(), 0) == 0;
}

bool validateRecord(Record* record)
{
    if (record->header != 0xaa) {
        return false;
    }
    if (record->state > 0x01) {
        return false;
    }
    if (record->delimeter != 0xbb) {
        return false;
    }
    return true;
}

void revertBytes(uint16_t* number)
{
    uint8_t* parts = (uint8_t*)number;
    uint8_t tmp;
    tmp = parts[1];
    parts[1] = parts[0];
    parts[0] = tmp;
}

bool readInfo(Record *record, unsigned int pointCount, unsigned int endingLength)
{
    FILE* deviceFile = fopen(devicePath.c_str(), "rb");
    int size = 2 * sizeof(uint8_t) + 2 * sizeof(uint16_t) + 2 * sizeof(uint8_t) + 
        pointCount * sizeof(MultitouchRecord) + endingLength / 8;
    fseek(deviceFile, -size, SEEK_END);
    if (deviceFile == NULL) {
        return false;
    }
    if (fread(&record->header, sizeof(uint8_t), 1, deviceFile) != 1) {
        return false;
    }
    if (fread(&record->state, sizeof(uint8_t), 1, deviceFile) != 1) {
        return false;
    }
    if (fread(&record->x1, sizeof(uint16_t), 1, deviceFile) != 1) {
        return false;
    }
    revertBytes(&record->x1);
    if (fread(&record->y1, sizeof(uint16_t), 1, deviceFile) != 1) {
        return false;
    }
    revertBytes(&record->y1);
    if (fread(&record->delimeter, sizeof(uint8_t), 1, deviceFile) != 1) {
        return false;
    }
    if (fread(&record->multitouchFlag, sizeof(uint8_t), 1, deviceFile) != 1) {
        return false;
    }
    for (int i=0; i<pointCount; i++) {
        if (fread(&record->multitouchPoints[i].y, sizeof(uint16_t), 1, deviceFile) != 1) {
            return false;
        }
        revertBytes(&record->multitouchPoints[i].y);
        if (fread(&record->multitouchPoints[i].x, sizeof(uint16_t), 1, deviceFile) != 1) {
            return false;
        }
        revertBytes(&record->multitouchPoints[i].x);
    }
    if (endingLength == 8) {
        uint8_t ending;
        if (fread(&ending, sizeof(uint8_t), 1, deviceFile) != 1) {
            return false;
        }
        record->ending = ending;
    }
    else {
        if (fread(&record->ending, sizeof(uint16_t), 1, deviceFile) != 1) {
            return false;
        }
        revertBytes(&record->ending);
    }
    return validateRecord(record);
}
