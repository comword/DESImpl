//
// Created by henorvell on 10/16/19.
//

#include "fileEnc.h"

#include <fstream>

bool fileEnc::doFileEnc(std::string readFilePath, std::string outFilePath, enum DES::MODE mode) {
    using namespace std;
    ifstream rFile(readFilePath, ios::in | ios::binary);
    ofstream oFile(outFilePath, ios::out | ios::binary);
    if(rFile.is_open() && oFile.is_open()) {
        if(mode == DES::MODE::ENCRYPT) {
            //get length of file
            rFile.seekg(0, std::ios::end);
            size_t length = rFile.tellg();
            rFile.seekg(0, std::ios::beg);
            oFile.write(reinterpret_cast<char*>(&length), sizeof(size_t));
            char *buf = new char[length];
            rFile.read(buf, length);
            des->des_cbc(buf, length, mode);
            oFile.write(des->getResult(), des->getResLen());
            return true;
        } else {
            //get length of file
            rFile.seekg(0, std::ios::end);
            size_t length = rFile.tellg();
            rFile.seekg(0, std::ios::beg);

            size_t rl;
            rFile.read(reinterpret_cast<char*>(&rl), sizeof(size_t)); //original file size

            char *buf = new char[length];
            rFile.read(buf, length);
            des->des_cbc(buf, length, mode);

            oFile.write(des->getResult(), rl);
            return true;
        }
    }
    return false;
}
