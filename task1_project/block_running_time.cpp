#include "mypackages/running_time.h"

using namespace std;

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif


int main(){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    block_running_time();
    return 0;
}