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

#include "convert.h"
#include "userio.h"
#include "AES_Cipher.h"
#include "running_task.h"

void full_option()
{
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif

    string mode, userSelection, plaintext, ciphertext;
    modeSelection(mode);
    userSelection = selectEncryptOrDecrypt();
    AES_Cipher aes(mode);
    if (userSelection == "encrypt"){
        inputPlaintextSelection(plaintext);
        ciphertext = aes.encrypt(plaintext);
        outputSelection(ciphertext, false, true);
    }
    else {
        ciphertext = inputCiphertextSelection();
        plaintext = aes.decrypt(ciphertext);
        outputSelection(plaintext, true, false);
    }
    wcout << "SUMMARISE:\n"
            << "MODE : " << string_to_wstring(mode) << '\n'
            << "KEY : " << string_to_wstring(hexencode(aes.key, aes.keySize)) << '\n';
    if (mode != "ECB"){
        wcout << "IV/CTR : " << string_to_wstring(hexencode(aes.iv, aes.ivSize)) << '\n';
    }
    wcout << "LENGTH OF PLAINTEXT : " << plaintext.size() << '\n'
          << "LENGTH OF CIPHERTEXT : " << ciphertext.size() << '\n';
}
