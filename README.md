# RaspberryTouchPanelDriver 

It's a driver for multiple touch panels for Raspberry Pi.
It can be used for you, if your panel simulate `eGalaxy`.

To run it you firstly must watch output of next command :

```
sudo xxd -c 25 /dev/hidraw0
```
and touch panel.

In my case output was something like next :

```
aa01 010e 018c bb00 0000 0000 0000 0000 0000 0000 0000 
aa01 0109 0197 bb00 0000 0000 0000 0000 0000 0000 0000
...
aa00 0000 0000 bb00 0000 0000 0000 0000 0000 0000 0000
aa01 0810 01a7 bb00 0000 0000 0000 0000 0000 0000 0000
```

Now I'll explain meaning of fields:
* aa - start marker
* 01 if touched, else 00
* 010e - first x coordinate
* 018c - first y coordinate
* bb - delimeter
* 00 - multitouch flags. Not supported
* 0000 0000 - mutitouch record. Not supported
* last - ending

Different models have different count of mutitouch record and size of ending.
In my case you can see 3 records (0000 0000 0000 0000 0000 0000) and 16-bit ending (0000).
At least one vesrion have 5 records (e.g. 0056 0019 018c 00ef 00b4 02ce 01d7 0182) and 8-bit ending (cc).

When you determined records count and ending - you can start calibration. Run
```
./touchScreen-driver recordCount endingSize calibrate
```
in my case :
```
./touchScreen-driver 3 16 calibrate
```
and click at top left corner and bottom right corner.

Output must be ssems to next:
```
pressed 200 100
...
released
...
pressed 1000 800
...
released
```

in this case - minimal value of x is 200 (0x00c8 in hex), y is 100 (0x0064), maximal value of x is 1000 (0x03e8 in hex), y is 800 (0x0320 in hex).
To start driver we must run (e.g resilution is 800x480):
```
./touchScreen-driver 3 16 use 00c8 03e8 800 0064 0320 480
```

It use xdotool to send X11 events, so previously you need to install it. In Debian-based distributives (e.g. Raspbian) you can make it next way:
```
sudo apt-get install xdotool
```