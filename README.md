# getUsbString: Simple utility to get a USB String Descriptor

Usage:
```
./getUsbString 0x[idVendor] 0x[idProduct] iString
```

Examples:
```
$ sudo ./getUsbString 0x046d 0xc051 2
Read 46 bytes
Raw (hex): 2e 03 55 00 53 00 42 00 2d 00 50 00 53 00 2f 00 32 00 20 00 4f 00 70 00 74 00 69 00 63 00 61 00 6c 00 20 00 4d 00 6f 00 75 00 73 00 65 00
String: USB-PS/2 Optical Mouse
```
```
$ sudo ./getUsbString 0x046d 0xc051 2 2>/dev/null
2e 03 55 00 53 00 42 00 2d 00 50 00 53 00 2f 00 32 00 20 00 4f 00 70 00 74 00 69 00 63 00 61 00 6c 00 20 00 4d 00 6f 00 75 00 73 00 65 00
USB-PS/2 Optical Mouse
```

Requires `libusb-1.0-0-dev` (or equivalent)

Compile with:
```
$ make -f Makefile.no_qt
```
or
```
$ qmake && make
```

Licensed under GPLv2 *or later*
