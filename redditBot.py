#!/usr/bin/env python
for iter in range(16):
    with open("littleschoolbus.bmp", "rb") as file:
        data = file.read()
        data = data[iter:]

        bits = ""
        for c in data:
            lsb = str(c & 0x1)
            bits += lsb

        bytess = [chr(int(bits[i:i+8], 2)) for i in range(0, len(bits), 8)]
        lsbstr = "".join(bytess)
        print(lsbstr)
        if "flag" in lsbstr:
            break
