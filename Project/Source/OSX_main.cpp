
#include "ApplicationPlatform.h"

int main(int argc, char* argv[]) {
    if (!ApplicationPlatform::execution())
    {
        return -1;
    }
    
    return 0;
    
}
