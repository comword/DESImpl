//
// Created by henorvell on 10/16/19.
//

#ifndef DESPROJ_FILEENC_H
#define DESPROJ_FILEENC_H

#include "des.h"
#include <string>

class fileEnc {
private:
    DES *des;
public:
    fileEnc(DES *des):des(des){}
    virtual ~fileEnc() {}
    bool doFileEnc(std::string readFilePath, std::string outFilePath, enum DES::MODE mode);
};


#endif //DESPROJ_FILEENC_H
