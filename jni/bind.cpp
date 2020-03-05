//
// Created by henorvell on 10/16/19.
//
#include "des.h"
#include "hexutil.h"
#include "fileEnc.h"
#include "org_gtdev_webapps_deschat_controllers_DESController.h"

#include <cstring>

DES *des = nullptr;
const char *binKey;
const char *binIv;

std::string setParams(const char *hexStr);

std::string setParams(const char *hexStr) {
    try {
        return hexutil::Hexstr2Binstr(std::string(hexStr));
    } catch (std::invalid_argument &e) {
        printf("Ignoring wrong hex input: %s\n", hexStr);
        return nullptr;
    }
}

extern "C" {
JNIEXPORT jboolean JNICALL Java_org_gtdev_webapps_deschat_controllers_DESController_des_1init
        (JNIEnv *env, jobject, jstring key, jstring iv) {
    const char *keyString = env->GetStringUTFChars(key, 0);
    binKey = setParams(keyString).c_str();
    env->ReleaseStringUTFChars(key, keyString);
    const char *ivString = env->GetStringUTFChars(iv, 0);
    binIv = setParams(ivString).c_str();
    env->ReleaseStringUTFChars(key, ivString);
    if(binKey != nullptr && binIv != nullptr) {
        des = new DES(hexutil::buf_to_int64(binKey), hexutil::buf_to_int64(binIv));
        return true;
    } else
        return false;
}

JNIEXPORT jboolean JNICALL Java_org_gtdev_webapps_deschat_controllers_DESController_des_1get_1inited
        (JNIEnv *, jobject) {
    return des != nullptr;
}

JNIEXPORT jstring JNICALL Java_org_gtdev_webapps_deschat_controllers_DESController_des_1enc
        (JNIEnv *env, jobject, jstring content, jboolean mode) {
    const char *contString = env->GetStringUTFChars(content, 0);
    const char *binCont = setParams(contString).c_str();
    long length = strlen(contString);
    //o_des(binCont, length, binKey, binIv, res, mode);
    //return hexutil::Binstr2Hexstr(res, length).c_str();
    des->des_cbc(binCont, length/2, (enum DES::MODE)mode);
    const char* str = hexutil::Binstr2Hexstr((const unsigned char *)des->getResult(), length/2).c_str();
    fprintf(stderr,"%s\n", str);
    jstring rstr = env->NewStringUTF(str);
    env->ReleaseStringUTFChars(content, contString);
    return rstr;
}

JNIEXPORT void JNICALL Java_org_gtdev_webapps_deschat_controllers_DESController_des_1deInit
        (JNIEnv *, jobject) {
    delete des;
    des = nullptr;
}

JNIEXPORT jboolean JNICALL Java_org_gtdev_webapps_deschat_controllers_DESController_des_1fileEnc
        (JNIEnv *env, jobject, jstring rPath, jstring oPath, jboolean mode) {
    const char *rPathStr = env->GetStringUTFChars(rPath, 0);
    const char *oPathStr = env->GetStringUTFChars(oPath, 0);
    fileEnc fEnc(des);
//    if(rPath.empty() || wPath.empty()) {
//        return false;
//    }
    bool res = false;
    if(mode)
        res = fEnc.doFileEnc(rPathStr, oPathStr, DES::DECRYPT);
    else
        res = fEnc.doFileEnc(rPathStr, oPathStr, DES::ENCRYPT);
    env->ReleaseStringUTFChars(rPath, rPathStr);
    env->ReleaseStringUTFChars(oPath, oPathStr);
    return res;
}

}