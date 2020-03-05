#include <cstdio>
#include <cstring>

#include <functional>
#include <string>
#include <memory>

#include <openssl/des.h>

#include "hexutil.h"
#include "des.h"
#include "fileEnc.h"

static void printHelpMessage();
std::string setParams(std::string hexstr);
void openssl_des(const char* input, long length, const char* key, const char* iv, unsigned char *res, int mode);

struct arg_handler {
    const char *long_flag;
    const char short_flag;
    std::function<int( int, const char ** )> handler;
};

std::string setParams(std::string hexStr) {
    try {
        return hexutil::Hexstr2Binstr(hexStr);
    } catch (std::invalid_argument &e) {
        printf("Ignoring wrong hex input: %s\n", hexStr.c_str());
        return nullptr;
    }
}

int main(int argc, char *argv[]) {
    std::string content, key(8, 0), iv(8, 0);
    bool use_openssl = false;
    bool file_mode = false;
    bool decrypt = false; // false for encrypt, true for decrypt
    std::string rPath, wPath;
    const arg_handler arg_proc[] = {
        {
            "--openssl", 'o',
            [&use_openssl](int, const char **params) -> int {
                use_openssl = true;
                return 0;
            }
        },
        {
                "--decrypt", 'd',
                [&decrypt](int, const char **params) -> int {
                    decrypt = true;
                    return 0;
                }
        },
        {
            "--key", 'k',
            [&key](int, const char **params) -> int {
                std::string k = setParams(params[0]);
                if(k.size() != 8) {
                    printf("Wrong key size: %lu\n", k.size());
                    exit(-1);
                }
                else
                    key = k;
                return 1;
            }
        },
        {
            "--content", 'c',
            [&content](int, const char **params) -> int {
                content = setParams(params[0]);
                return 1;
            }
        },
        {
            "--iv", 'i',
                [&iv](int, const char **params) -> int {
                    std::string k = setParams(params[0]);
                    if(k.size() != 8) {
                        printf("Wrong initial vector size: %lu\n", k.size());
                        exit(-1);
                    }
                    else
                        iv = k;
                    return 1;
                }
        },
        {
            "--file", 'f',
            [&file_mode](int, const char **params) -> int {
                file_mode = true;
                return 0;
            }
        },
        {
            "--read", 'r',
            [&rPath](int, const char **params) -> int {
                rPath = params[0];
                return 1;
            }
        },
        {
            "--write", 'w',
            [&wPath](int, const char **params) -> int {
                wPath = params[0];
                return 1;
            }
        }

    };
    const int num_arguments = sizeof( arg_proc ) / sizeof( arg_proc[0] );
    --argc;
    ++argv;
    if(argc == 0)
        printHelpMessage();

    while( argc ) {
        if(!strcmp( argv[0], "--help" )) {
            printHelpMessage();
            return 0;
        } else {
            bool arg_handled = false;
            for(const auto & arg_handler : arg_proc) {
                char flag[3] = {'-', arg_handler.short_flag, 0};
                if( !strcmp( argv[0], arg_handler.long_flag ) || !strcmp( argv[0], flag )) {
                    argc--;
                    argv++;
                    int args_consumed = arg_handler.handler( argc, ( const char ** )argv );
                    if( args_consumed < 0 ) {
                        printf( "Failed parsing parameter '%s'\n", *( argv - 1 ) );
                        exit( 1 );
                    }
                    argc -= args_consumed;
                    argv += args_consumed;
                    arg_handled = true;
                    break;
                }
            }
            if( !arg_handled ) {
                --argc;
                ++argv;
            }
        }
    }
    if(!file_mode) {
        auto resBuf = std::unique_ptr<unsigned char[]>(new unsigned char[content.size()+8]);
        if(use_openssl) {
            if(decrypt) {
                openssl_des(content.c_str(), content.size(), key.c_str(), iv.c_str(), resBuf.get(), DES_DECRYPT);
            } else {
                openssl_des(content.c_str(), content.size(), key.c_str(), iv.c_str(), resBuf.get(), DES_ENCRYPT);
            }
            printf("%s", hexutil::Binstr2Hexstr(resBuf.get(), content.size()).c_str());
        } else {
            DES des(hexutil::buf_to_int64(key.c_str()), hexutil::buf_to_int64(iv.c_str()));
            if(decrypt)
                des.des_cbc(content.c_str(), content.size(), DES::DECRYPT);
            else
                des.des_cbc(content.c_str(), content.size(), DES::ENCRYPT);
            printf("%s", hexutil::Binstr2Hexstr((const unsigned char *)des.getResult(), des.getResLen()).c_str());
        }
    } else {
        if(use_openssl) {
            printf("Openssl File Encryption & Decryption are not implemented.\n");
            return 1;
        } else {
            DES des(hexutil::buf_to_int64(key.c_str()), hexutil::buf_to_int64(iv.c_str()));
            fileEnc fEnc(&des);
            //Check read & write path
            if(rPath.empty() || wPath.empty()) {
                printf("File path not available.\n");
                return 1;
            }
            bool res = false;
            if(decrypt)
                res = fEnc.doFileEnc(rPath, wPath, DES::DECRYPT);
            else
                res = fEnc.doFileEnc(rPath, wPath, DES::ENCRYPT);
            if(!res)
                printf("Failed.\n");
        }
    }
    return 0;
}

void printHelpMessage() {
    printf( "Command line paramters:\n"
            "--decrypt,-d\n\t\tPerform decryption.\n"
            "--key,-k <Hex>\n\t\tDES Key.\n"
            "--content,-c <Hex>\n\t\tThe content to be encrypted or decrypted.\n"
            "--iv,-i <Hex>\n\t\tInitial vector.");
}

void openssl_des(const char* input, long length, const char* key, const char* iv, unsigned char *res, int mode) {
    DES_cblock tKey = {0, 0, 0, 0, 0, 0, 0, 0};
    DES_cblock tIV = {0, 0, 0, 0, 0, 0, 0, 0};
    memcpy(tKey, key, 8);
    memcpy(tIV, iv, 8);
    DES_key_schedule schKey;
    DES_set_key(&tKey, &schKey);
    DES_ncbc_encrypt((const unsigned char *)input, res, length, &schKey, &tIV, mode);
}