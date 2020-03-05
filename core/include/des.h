//
// Created by henorvell on 9/16/19.
//

#ifndef DESPROJ_DES_H
#define DESPROJ_DES_H

#include <cstdint>
#include <cstdlib>
#include <memory>

class DES {
private:
    const static char sBox[8][4][16];
    const static char ip_table[];
    const static char iip_table[];
    const static char pc1Table[];
    const static char pc2Table[];
    const static char eTable[];
    const static char pTable[];
private:
    uint64_t key, iv = 0;
    uint64_t keySet[16] = {0};
    std::unique_ptr<char[]> out;
    size_t len=0;
public:
    DES() {}
    DES(uint64_t key, uint64_t iv):key(key), iv(iv){ gen_subKeys();}
    virtual ~DES() { }
    struct LR {
        uint32_t L;
        uint32_t R;
    };
    enum MODE : bool {
        DECRYPT = false,
        ENCRYPT
    };
public:
    void setKey_Iv(uint64_t key, uint64_t iv) {
        this->key = key;
        this->iv = iv;
        gen_subKeys();
    }
    void des_cbc(const char* input, long length, enum MODE);
    void gen_subKeys();
    char* getResult();
    size_t getResLen();

private:
    static DES::LR doIP(uint64_t in);
    static DES::LR doFP(DES::LR in);
    void encrypt(uint32_t &l, uint32_t &r, enum MODE m);
    uint64_t get_key(int step);
    static uint64_t extendTo48(uint32_t a);
    uint32_t fFunc(uint32_t R, uint64_t K);
};

#endif //DESPROJ_DES_H
