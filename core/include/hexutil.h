#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include <vector>

class hexutil
{
public:
    static void int32_to_buf(char *buf, int i, uint32_t value );
    static uint32_t buf_to_int32(const char *a);
    static void int64_to_buf(char *buf, int i, uint64_t value );
    static uint64_t buf_to_int64(const char *a);
    static std::string Binstr2Hexstr(std::string src);
    static std::string Hexstr2Binstr(std::string src);
    static std::string Binstr2Hexstr(const unsigned char *in, long size);
};
#endif //_UTIL_H