//
// Created by henorvell on 9/16/19.
//

#include "des.h"
#include "hexutil.h"

#include <vector>
#include <bitset>
#include <cstring>

const char DES::sBox[8][4][16] = {
    //s1
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
    //s2
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
    //s3
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
    //s4
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
    //s5
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
    //s6
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
    //s7
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
    //s8
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

const char DES::eTable[] = {
        32,  1,  2,  3,  4,  5,
        4,  5,  6,  7,  8,  9,
        8,  9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32,  1
};

const char DES::pTable[] = {
        16, 7,  20, 21,
        29, 12, 28, 17,
        1,  15, 23, 26,
        5,  18, 31, 10,
        2,  8,  24, 14,
        32, 27, 3,  9,
        19, 13, 30, 6,
        22, 11, 4,  25
};

uint64_t DES::extendTo48(uint32_t a) {
    std::bitset<48> r;
    std::bitset<32> in(a);
    for (int i = 0; i < 48; i++)
        r[i] = in[eTable[i] - 1];
    return r.to_ulong();
}

uint32_t DES::fFunc(uint32_t R, uint64_t K) {
    uint64_t expandR = extendTo48(R);
    expandR ^= K;
    using namespace std;
    bitset<48> er(expandR);
    bitset<32> sr;
    // Search S Box
    // s1 to s8
    int j = 0, k=0;
    for(int i=0;i<48;i=i+6){
        int row = 2*er[i] + er[i+5];
        int col = 8*er[i+1] + 4*er[i+2] + 2*er[i+3] + er[i+4];
        int value = sBox[j][row][col];
        j++;
        for(int x = 8; x>0; x>>= 1){
            sr[k] = ((value & x) == x) ? 1:0;
            k++;
        }
    }
    //p table
    bitset<32> psr;
    for (int i = 0; i < 32; i++) {
        psr[i] = sr[pTable[i] - 1];
    }
    return psr.to_ulong();
}

void DES::encrypt(uint32_t &l, uint32_t &r, enum MODE m) {
    uint64_t in = ((uint64_t)l << 32) | r;
    //Initial permutation
    auto lr = doIP(in);
    if(m) { //encrypt
        for (int i = 0; i < 16; i++) {
            uint64_t mKey = keySet[i];
            auto f = fFunc(r, mKey);
//            printf("Round %d: f(%u, %llu)=%u\n", i+1, r, mKey, f);
            auto t = l;
            l = r;
            r = t ^ f;
        }
    } else { //decrypt
        for (int i = 15; i >= 0; i--) {
            uint64_t mKey = keySet[i];
            auto f = fFunc(r, mKey);
//            printf("Round %d: f(%u, %llu)=%u\n", i+1, r, mKey, f);
            auto t = l;
            l = r;
            r = t ^ f;
        }
    }
    //Final permutation & swap l&r
    auto f = DES::doFP(LR{.L=r, .R=l});
    l = f.L;
    r = f.R;
}

void DES::des_cbc(const char* input, long length, enum MODE m) {
    uint32_t tin0, tin1, tmpIn0, tmpIn1, tmpOut0, tmpOut1, xor0=0, xor1=0, pos=0;
    //padding
    int fillNum = (8-length) % 8;
    if (fillNum<0) fillNum += 8;
    std::vector<char> buf(length+fillNum+1, 0);
    char *pIn = &buf.front();
    memcpy(pIn, input, length);

    long l = length + fillNum;
    len = l;
    out = std::unique_ptr<char[]>(new char[l+1]);
    char iVec[8];
    hexutil::int64_to_buf(iVec, 0, iv);
    auto pIv = (unsigned char *)iVec;
    if(m) { //Encrypt
        //Load initial vector to the out buffer.
        tmpOut0 = hexutil::buf_to_int32((const char *)pIv);
        tmpOut1 = hexutil::buf_to_int32((const char *)pIv+4);
        for(l -= 8; l >= 0; l -= 8) {
            //Load block
            tmpIn0 = hexutil::buf_to_int32((const char *)pIn + pos);
            tmpIn1 = hexutil::buf_to_int32((const char *)pIn + pos + 4);
            //Xor with the previous result
            tmpIn0 ^= tmpOut0;
            tmpIn1 ^= tmpOut1;
            //DES encryption
            encrypt(tmpIn0, tmpIn1, m);
            //Extract the result
            tmpOut0 = tmpIn0;
            tmpOut1 = tmpIn1;
            hexutil::int32_to_buf(out.get(), pos, tmpOut0);
            hexutil::int32_to_buf(out.get(), pos+4, tmpOut1);
            pos+=8;
        }
    } else { //Decrypt
        xor0 = hexutil::buf_to_int32((const char *)pIv);
        xor1 = hexutil::buf_to_int32((const char *)pIv+4);
        for(l -= 8; l >= 0; l -= 8) {
            tin0 = tmpIn0 = hexutil::buf_to_int32((const char *)pIn + pos);
            tin1 = tmpIn1 = hexutil::buf_to_int32((const char *)pIn + pos + 4);
            encrypt(tmpIn0, tmpIn1, m);
            tmpOut0 = tmpIn0 ^ xor0;
            tmpOut1 = tmpIn1 ^ xor1;
            hexutil::int32_to_buf(out.get(), pos, tmpOut0);
            hexutil::int32_to_buf(out.get(), pos+4, tmpOut1);
            xor0 = tin0;
            xor1 = tin1;
            pos+=8;
        }
    }
}

char* DES::getResult() {
    return out.get();
}

size_t DES::getResLen() {
    return len;
}