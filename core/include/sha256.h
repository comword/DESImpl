//
// Created by henorvell on 10/17/19.
// Reference: https://github.com/hlilje/sha-256/blob/master/sha256.cpp

#ifndef DESPROJ_SHA256_H
#define DESPROJ_SHA256_H

#include <iomanip>
#include <sstream>
#include <vector>
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long long ll;

class SHA256 {
private:
    const static WORD K[];

    std::vector<std::vector<WORD>> M; // Message to be hashed
    std::vector<std::vector<WORD>> H; // Hashed message
    std::vector<BYTE> bytes;          // Plain and padded message bytes
    WORD W[64];                       // Message schedule
    ll l = 0;                         // Message length in bits
    int N;                            // Number of blocks in padded message

    // Working variables
    WORD a, b, c, d, e, f, g, h;

    // Temporary words
    WORD T1, T2;
private:

    void store_message_bytes(const std::string &hex_str);
    int calc_padding();
    void pad_message();
    void parse_message();
    void init_hash();
    WORD ROTR(const WORD &n, const WORD &x);
    WORD SHR(const WORD &n, const WORD &x);
    WORD Ch(const WORD &x, const WORD &y, const WORD &z);
    WORD Maj(const WORD &x, const WORD &y, const WORD &z);
    WORD lsigma0(const WORD &x);
    WORD lsigma1(const WORD &x);
    WORD ssigma0(const WORD &x);
    WORD ssigma1(const WORD &x);
    void compute_hash();
    void clear();

public:
};

#endif //DESPROJ_SHA256_H
