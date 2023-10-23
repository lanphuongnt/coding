//C internal library 
#include <iostream>
using std::wcin;
using std::wcout;
using std::wcerr;
using std::endl;
using std::cout;
using std::cin;

#include <string>
using std::string;
using std::wstring;
#include <cstdlib>
using std::exit;
#include "assert.h"
#include "fstream"


//Cryptopp Librari
#include <cryptopp/files.h>
using CryptoPP::FileSource;
using CryptoPP::FileSink;
using CryptoPP::BufferedTransformation;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::Redirector; // string to bytes

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::byte;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

// Block cipher
#include "cryptopp/des.h"
using CryptoPP::DES;
#include "cryptopp/aes.h"
using CryptoPP::AES;

//Mode of operations
#include "cryptopp/modes.h" //ECB, CBC, CBC-CTS, CFB, OFB, CTR
using CryptoPP::ECB_Mode;
using CryptoPP::CBC_Mode;
using CryptoPP::CFB_Mode;
using CryptoPP::OFB_Mode;
using CryptoPP::CTR_Mode;
#include "cryptopp/xts.h"
using CryptoPP::XTS;
#include <cryptopp/ccm.h>
using CryptoPP::CCM;
#include "cryptopp/gcm.h"
using CryptoPP::GCM;
//Ref: more here https://www.cryptopp.com/wiki/AEAD_Comparison


/* Set utf8 support for windows*/ 
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#endif
/* Convert string <--> utf8*/ 
#include <locale>
using std::wstring_convert;
#include <codecvt>
using  std::codecvt_utf8;

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace CryptoPP;

#include "convert.h"
#include "AES_Cipher.h"
#include "userio.h"

AES_Cipher::AES_Cipher(string aMode){
    AES_Cipher::mode = aMode;

    int inputWay = inputWaySelection();

    if (aMode == "XTS"){
        AES_Cipher::keySize = 32;
    }
    else{
        AES_Cipher::keySize = AES::DEFAULT_KEYLENGTH;
    }
    AES_Cipher::key = generateBlock(AES_Cipher::keySize, inputWay, "key");
    if (aMode == "CCM"){
        AES_Cipher::ivSize = 12;
        AES_Cipher::iv = generateBlock(ivSize, inputWay, "iv");
    }
    else if (aMode == "CTR"){
        AES_Cipher::ivSize = AES::BLOCKSIZE;
        AES_Cipher::iv = generateBlock(ivSize, inputWay, "ctr");
    }
    else {
        if (aMode != "ECB"){
            AES_Cipher::ivSize = AES::BLOCKSIZE;
            AES_Cipher::iv = generateBlock(AES::BLOCKSIZE, inputWay, "iv");
        }
    }
}

string AES_Cipher::encrypt(string plain){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    string cipher;
    if (AES_Cipher::mode == "ECB"){ //ok
        ECB_Mode< AES >::Encryption e;
        e.SetKey(AES_Cipher::key, AES_Cipher::keySize);
        StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
    }
    else if (AES_Cipher::mode == "CBC"){//ok
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
    }
    else if (AES_Cipher::mode == "OFB"){//ok
        OFB_Mode< AES >::Encryption e;
        e.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
    }
    else if (AES_Cipher::mode == "CFB"){//ok
        CFB_Mode< AES >::Encryption e;
        e.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
    }
    else if (AES_Cipher::mode == "CTR"){
        CTR_Mode< AES >::Encryption e;
        e.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
    }
    else if (AES_Cipher::mode == "XTS"){// ok
        XTS< AES >::Encryption e;
        e.SetKeyWithIV(AES_Cipher::key, 32, iv );
        StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher), StreamTransformationFilter::NO_PADDING)); // StringSource
    }
    else if (AES_Cipher::mode == "CCM"){
        const int TAG_SIZE = 8;
        CCM< AES, TAG_SIZE >::Encryption e;
        e.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv, 12);
        e.SpecifyDataLengths(0, plain.size(), 0);
        StringSource s(plain, true, new AuthenticatedEncryptionFilter(e, new StringSink(cipher)));
    }
    else if (AES_Cipher::mode == "GCM"){//ok
        GCM< AES >::Encryption e;
        e.SetKeyWithIV( AES_Cipher::key, AES_Cipher::keySize, iv, AES_Cipher::ivSize);
        StringSource s(plain, true, new AuthenticatedEncryptionFilter(e, new StringSink(cipher))); 
    }
    else {
        wcout << "Error!";
        exit(0);
    }
    return cipher;
}
string AES_Cipher::decrypt(string cipher){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    string plain;
    if (AES_Cipher::mode == "ECB"){
        ECB_Mode< AES >::Decryption d;
        d.SetKey(AES_Cipher::key, AES_Cipher::keySize);
        StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(plain)));
    }
    else if (AES_Cipher::mode == "CBC"){
        CBC_Mode< AES >::Decryption d;
        d.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(plain)));
    }
    else if (AES_Cipher::mode == "OFB"){
        OFB_Mode< AES >::Decryption d;
        d.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(plain)));
    }
    else if (AES_Cipher::mode == "CFB"){
        CFB_Mode< AES >::Decryption d;
        d.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(plain)));
    }
    else if (AES_Cipher::mode == "CTR"){
        CTR_Mode< AES >::Decryption d;
        d.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv);
        StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(plain)));
    }
    else if (AES_Cipher::mode == "XTS"){
        XTS< AES >::Decryption d;
        d.SetKeyWithIV(AES_Cipher::key, 32, iv );
        StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(plain), StreamTransformationFilter::NO_PADDING)); // StringSource
    }
    else if (AES_Cipher::mode == "CCM"){
        const int TAG_SIZE = 8;
        CCM< AES, TAG_SIZE >::Decryption d;
        d.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv, 12);
        d.SpecifyDataLengths(0, cipher.size() - TAG_SIZE, 0);
        AuthenticatedDecryptionFilter df(d, new StringSink(plain));
        StringSource s(cipher, true, new Redirector(df));
    }
    else if (AES_Cipher::mode == "GCM"){
        GCM< AES >::Decryption d;
        d.SetKeyWithIV(AES_Cipher::key, AES_Cipher::keySize, iv, AES_Cipher::ivSize);
        StringSource s(cipher, true, new AuthenticatedDecryptionFilter(d, new StringSink(plain)));
    }
    else {
        wcout << "Error!";
        exit(0);
    }
    return plain;
}