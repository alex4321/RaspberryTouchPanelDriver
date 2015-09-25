#include "x11func.h"
#include <string>
#include <stdlib.h>
using namespace std;

void cursorDown(unsigned int x, unsigned int y)
{
    string command = "xdotool mousemove " + to_string(x) + " " + to_string(y) + " mousedown 1";
    system(command.c_str());
}

void cursorUp()
{
    string command = "xdotool mouseup 1";
    system(command.c_str());
}
