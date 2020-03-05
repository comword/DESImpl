//
// Created by henorvell on 9/19/19.
//
#include "des.h"

#include <bitset>

const char DES::ip_table[] = {
        58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,
        64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,
        59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,
        63,55,47,39,31,23,15,7
};

const char DES::iip_table[] = {
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
};

DES::LR DES::doIP(uint64_t in) {
    std::bitset<64> bsIn(in);
    std::bitset<32> r, l;
    for (int i = 0; i < 32; i++) {
        r[i] = bsIn[ip_table[i] - 1];
        l[i] = bsIn[ip_table[i+32] - 1];
    }
    DES::LR lr{
        .L = (uint32_t)l.to_ulong(),
        .R = (uint32_t)r.to_ulong()
    };
    return lr;
}

DES::LR DES::doFP(DES::LR in) {
    std::bitset<64> res;
    std::bitset<32> r(in.R), l(in.L);
    for (int i = 0; i < 64; i++) {
        if (iip_table[i] <= 32) {
            res[i] = r[iip_table[i] - 1];
        } else {
            res[i] = l[iip_table[i] - 32 - 1];
        }
    }
    DES::LR lr{
        .L = (uint32_t)l.to_ulong(),
        .R = (uint32_t)r.to_ulong()
    };
    return lr;
    //return res.to_ulong();
}