//
// Created by henorvell on 9/24/19.
//

#include "des.h"

#include <bitset>
//#include <algorithm>

const char DES::pc1Table[] = {
        57, 49, 41, 33, 25, 17, 9, 1,
        58, 50, 42, 34, 26, 18, 10, 2,
        59, 51, 43, 35, 27, 19, 11, 3,
        60, 52, 44, 36, 63, 55, 47, 39,
        31, 23, 15, 7, 62, 54, 46, 38,
        30, 22, 14, 6, 61, 53, 45, 37,
        29, 21, 13, 5, 28, 20, 12, 4
};

const char DES::pc2Table[] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

void DES::gen_subKeys() {
    for(int i=0;i<16;i++)
        keySet[i] = get_key(i);
}

static int shiftBits[] = {1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28};
#define ROTATE(a,w,n) (((a)>>(n))+(((a)<<(w-(n)))))

uint64_t DES::get_key(int step) {
    using namespace std;
    auto k = bitset<64>(key);
    auto key48 = bitset<48>();
    auto key56 = bitset<56>();
//    auto str = k.to_string();
//    std::reverse(str.begin(), str.end());
//    k = bitset<64>(str);
    //Lookup pc1 table
    for(int i = 0; i < 56; i++)
        key56[i] = k[pc1Table[i] - 1];
    //leftShift the key
    uint32_t l = ROTATE(key56.to_ulong()>>28 & 0xFFFFFFF, 28, shiftBits[step]) & 0xFFFFFFF;
    uint32_t r = ROTATE(key56.to_ulong() & 0xFFFFFFF, 28, shiftBits[step]) & 0xFFFFFFF;
    key56 = bitset<56>(((uint64_t)l<<28)|r);
    //Lookup pc2 table
    for(int i=0; i<48; i++)
        key48[i] = key56[pc2Table[i] - 1];
    return key48.to_ulong();
}