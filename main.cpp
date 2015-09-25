#include <iostream>
#include <string>
#include <functional>
#include <unistd.h>
#include <inttypes.h>
#include "func.h"
#include "x11func.h"
#include "struct.h"
using namespace std;

/*
 * Driver settings
 */
struct
{
    uint16_t xmin;
    uint16_t xmax;
    uint32_t xmaxpixels;
    uint16_t ymin;
    uint16_t ymax;
    uint32_t ymaxpixels;
    int multitouchPoints;
    int endingLength;
} settings;

/*
 * Show calibration information to user
 */
void calibrationHandler(Record* record)
{
    if (record->state == 0x00) {
        cout << "released\n";
    }
    else if (record->state == 0x01) {
        cout << "pressed " << record->x1 << " " << record->y1 << "\n";
    }
}

/*
 * Move cursor and emit button up/down events
 */
void cursorHandler(Record* record)
{
    double xpart = (record->x1 - settings.xmin) / (double)settings.xmax;
    double ypart = (record->y1 - settings.ymin) / (double)settings.ymax;
    if (xpart > 1) {
        xpart = 1;
    }
    if (ypart > 1) {
        ypart = 1;
    }
    uint32_t xpixels = xpart * settings.xmaxpixels;
    uint32_t ypixels = ypart * settings.ymaxpixels;
    if (record->state == 0x01) {
        cursorDown(xpixels, ypixels);
    }
    else if (record->state == 0x00) {
        cursorUp();
    }
}

/*
 * Sends readed data from touchscreen to given handler
 */
void work(function<void(Record*)> handler) {
    while(true) {
        if (!deviceExists()) {
            cerr << "Device " << devicePath << " not found\n";
            usleep(0.5 * 1000000);
            continue;
        }
        while(true) {
            Record readed;
            if (readInfo(&readed, settings.multitouchPoints, settings.endingLength)) {
                handler(&readed);
            }
            usleep(0.5 * 1000000);
        }
    }
}

/*
 * Validate basical device properties
 */
bool deviceConfigValidate(char** argv)
{
    //Multitouch points count
    sscanf(argv[0], "%i", &settings.multitouchPoints);
    if (settings.multitouchPoints > 5) {
        return false;
    }
    //Ending size in bits
    sscanf(argv[1], "%i", &settings.endingLength);
    if (settings.endingLength != 8 && settings.endingLength != 16) {
        return false;
    }
    return true;
}

/*
 * Validate "use" mode arguments
 */
bool validUseArgs(char** argv)
{
    //xmin (uint16, hex)
    if (!(string(argv[0]).size() == 4
            && isxdigit(argv[0][0])
            && isxdigit(argv[0][1])
            && isxdigit(argv[0][2])
            && isxdigit(argv[0][3]) )) {
        return false;
    }
    //xmax (uint16, hex)
    if (!(string(argv[1]).size() == 4
            && isxdigit(argv[1][0])
            && isxdigit(argv[1][1])
            && isxdigit(argv[1][2])
            && isxdigit(argv[1][3]) )) {
        return false;
    }
    //xpixels (uint16, decimal)
    if (!(string(argv[2]).size() == 4
            && isdigit(argv[2][0])
            && isdigit(argv[2][1])
            && isdigit(argv[2][2])
            && isdigit(argv[2][3]) )) {
        return false;
    }
    //ymin (uint16, hex)
    if (!(string(argv[3]).size() == 4
            && isxdigit(argv[3][0])
            && isxdigit(argv[3][1])
            && isxdigit(argv[3][2])
            && isxdigit(argv[3][3]) )) {
        return false;
    }
    //ymax (uint16, hex)
    if (!(string(argv[4]).size() == 4
            && isxdigit(argv[4][0])
            && isxdigit(argv[4][1])
            && isxdigit(argv[4][2])
            && isxdigit(argv[4][3]) )) {
        return false;
    }
    //ypixels (uint16, decimal)
    if (!(string(argv[5]).size() == 4
            && isdigit(argv[5][0])
            && isdigit(argv[5][1])
            && isdigit(argv[5][2])
            && isdigit(argv[5][3]) )) {
        return false;
    }
    return true;
}

/*
 * Parse "use" mode arguments
 */
void parseUseArgs(char** argv)
{
    sscanf( ("0x" + string(argv[0])).c_str(), "%hx", &settings.xmin);
    sscanf( ("0x" + string(argv[1])).c_str(), "%hx", &settings.xmax);
    sscanf( string(argv[2]).c_str(), "%i", &settings.xmaxpixels);
    sscanf( ("0x" + string(argv[3])).c_str(), "%hx", &settings.ymin);
    sscanf( ("0x" + string(argv[4])).c_str(), "%hx", &settings.ymax);
    sscanf( string(argv[5]).c_str(), "%i", &settings.ymaxpixels);
}

int main(int argc, char** argv)
{
    if (!isRoot()) {
        cerr << "Must be root\n";
        return 1;
    }
    if (argc < 3 || !deviceConfigValidate(&argv[1])) {
        cerr << "Wrong device arguments\n Need \n"
             << "- multitouchPointsCount - number of sending multitouch info structures\n"
             << "- endingSize - size of ending (8 or 16 bit)";
        return 2;
    }
    if (argc == 4 && string(argv[3]) == "calibrate") {
        work(calibrationHandler);
    } else if (argc == 10 && string(argv[3]) == "use" && validUseArgs(&argv[4])) {
        parseUseArgs(&argv[4]);
        work(cursorHandler);
    }
    else {
        cerr << "Arguments : \n"
             << "1 - 'multitouchPointsCount endingSize calibrate' \n"
             << "2 - 'multitouchPointsCount endingSize use xmin xmax xmaxpixels ymin ymax ymaxpixels'";
        return 2;
    }
    return 0;
}

