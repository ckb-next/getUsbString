/*
    Copyright (C) 2019  Tasos Sahanidis <code@tasossah.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include <iconv.h>

#define EXIT_WITH_ARG_ERR { \
        fprintf(stderr, "Invalid arguments specified.\nUsage: ./getUsbString 0x[idVendor] 0x[idProduct] iString\n"); \
        return 1; \
    }

int main(int argc, char** argv)
{
    if(argc != 4)
        EXIT_WITH_ARG_ERR

    unsigned short ven = 0;
    unsigned short dev = 0;
    unsigned char iString = 0;

    // TODO: For windows support, don't use %hhu
    if(sscanf(argv[1], "0x%hx", &ven) != 1 || sscanf(argv[2], "0x%hx", &dev) != 1 || sscanf(argv[3], "%hhu", &iString) != 1)
        EXIT_WITH_ARG_ERR

    int ret = libusb_init(NULL);
    if(ret != 0)
    {
        fprintf(stderr, "libusb_init failed with %d", ret);
        return 1;
    }

    struct libusb_device_handle* handle = libusb_open_device_with_vid_pid(NULL, ven, dev);
    if(handle == NULL)
    {
        fprintf(stderr, "libusb_open_device_with_vid_pid failed with Ven: %hu, Dev: %hu\n", ven, dev);
        libusb_exit(NULL);
        return 1;
    }

    // Try to get the string descriptor
    unsigned char buffer[256] = {0};
    ret = libusb_get_string_descriptor(handle, iString, 0, buffer, 256);
    if(ret < 0)
        fprintf(stderr, "Failed to get string descriptor %hhu with %d (%s: %s)\n", iString, ret, libusb_error_name(ret), libusb_strerror(ret));
    else
    {
        fprintf(stderr, "Read %d bytes\nRaw (hex): ", ret);
        for(int i = 0; i < ret; i++)
            printf("%02hhx ", buffer[i]);
        printf("\n");

#ifndef NO_ICONV
        if(ret > 4)
        {
            // Try to decode the string, ignoring the first two bytes (buffer[1] should be 0x03 == string descriptor)
            char utf8str[256] = {0};
            char* utf8str_ = (char*)utf8str;
            char* instr = (char*)buffer + 2;
            iconv_t utf8descr = iconv_open("UTF-8", "UTF-16LE");
            if(utf8descr != (iconv_t) -1)
            {
                size_t inbytes = ret - 2;
                size_t outbytes = 256;
                iconv(utf8descr, &instr, &inbytes, &utf8str_, &outbytes);
                iconv_close(utf8descr);
            }
            fprintf(stderr, "String: ");
            printf("%s\n", utf8str);
        }
#endif
    }

    libusb_close(handle);
    libusb_exit(NULL);

    return 0;
}

