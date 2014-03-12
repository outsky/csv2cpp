#include <stdio.h>
#include "convert.h"

signed char to_int8(const char* str)
{
    signed char ret;
    sscanf(str, "%hhd", &ret);
    return ret;
}

signed short to_int16(const char* str)
{
    signed short ret;
    sscanf(str, "%hd", &ret);
    return ret;
}

signed int to_int32(const char* str)
{
    signed int ret;
    sscanf(str, "%d", &ret);
    return ret;
}

signed long long to_int64(const char* str)
{
    signed long long ret;
    sscanf(str, "%Ld", &ret);
    return ret;
}


unsigned char to_uint8(const char* str)
{
    unsigned char ret;
    sscanf(str, "%hhu", &ret);
    return ret;
}

unsigned short to_uint16(const char* str)
{
    unsigned short ret;
    sscanf(str, "%hu", &ret);
    return ret;
}

unsigned int to_uint32(const char* str)
{
    unsigned int ret;
    sscanf(str, "%u", &ret);
    return ret;
}

unsigned long long to_uint64(const char* str)
{
    unsigned long long ret;
    sscanf(str, "%Lu", &ret);
    return ret;
}

