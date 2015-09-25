#ifndef FUNC
#define FUNC

#include <string>
#include "struct.h"
using namespace std;

const string devicePath = "/dev/hidraw0";

bool is_root();
bool deviceExists();
bool validateRecord(Record* record);
bool readInfo(Record *record, unsigned int pointCount, unsigned int endingLength);

#endif // FUNC

