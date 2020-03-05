//
// Created by henorvell on 9/16/19.
//

#include "hexutil.h"
#include <sstream>
#include <string>
#include <cstdlib>
#include <iomanip>

void hexutil::int32_to_buf(char *buf, int i, uint32_t value )
{
    buf[i + 3] = ( char ) value ;
    buf[i + 2] = ( char )( value >> 8 );
    buf[i + 1] = ( char )( value >> 16 );
    buf[i] = ( char )( value >> 24 );
}

uint32_t hexutil::buf_to_int32(const char *a)
{
    uint32_t n = 0;
    n = (((int32_t)a[0] << 24) & 0xFF000000)
        | (((int32_t)a[1] << 16) & 0x00FF0000)
        | (((int32_t)a[2] << 8) & 0x0000FF00)
        | (((int32_t)a[3]) & 0x000000FF);
    return n;
}

void hexutil::int64_to_buf(char *buf, int i, uint64_t value )
{
    buf[i + 7] = ( char ) value;
    buf[i + 6] = ( char )( value >> 8 );
    buf[i + 5] = ( char )( value >> 16 );
    buf[i + 4] = ( char )( value >> 24 );
    buf[i + 3] = ( char )( value >> 32 );
    buf[i + 2] = ( char )( value >> 40 );
    buf[i + 1] = ( char )( value >> 48 );
    buf[i + 0] = ( char )( value >> 56 );
}

uint64_t hexutil::buf_to_int64(const char *a)
{
    uint64_t n = 0;
    n = (((int64_t)a[0] << 56) & 0xFF00000000000000U)
        | (((int64_t)a[1] << 48) & 0x00FF000000000000U)
        | (((int64_t)a[2] << 40) & 0x0000FF0000000000U)
        | (((int64_t)a[3] << 32) & 0x000000FF00000000U)
        | ((a[4] << 24) & 0x00000000FF000000U)
        | ((a[5] << 16) & 0x0000000000FF0000U)
        | ((a[6] <<  8) & 0x000000000000FF00U)
        | (a[7]        & 0x00000000000000FFU);
    return n;
}

std::string hexutil::Binstr2Hexstr(std::string src)
{
    std::stringstream oss;
    const char *buf = src.c_str();
    for (size_t i = 0; i < src.size(); i += 1) {
        oss << std::setfill('0') << std::setw(2) << std::hex << (unsigned short)buf[i];
    }
    return oss.str();
}

std::string hexutil::Binstr2Hexstr(const unsigned char *in, long size) {
    std::stringstream oss;
    for (size_t i = 0; i < size; i += 1) {
        oss << std::setfill('0') << std::setw(2) << std::hex << (unsigned short)in[i];
    }
    return oss.str();
}

std::string hexutil::Hexstr2Binstr(std::string src)
{
    std::string res;
    res.reserve(src.size() / 2);
    for (size_t i = 0; i < src.size(); i += 2)
    {
        int temp = std::stoi(src.substr(i, 2), 0, 16);
        res += static_cast<char>(temp);
    }
    return res;
}