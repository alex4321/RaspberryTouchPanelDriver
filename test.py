#!/usr/bin/env python
hex_string = raw_input().replace(' ', '')
bytes = hex_string.decode('hex')
file = open("/dev/hidraw0", "ab")
file.write(bytes)
file.close()
