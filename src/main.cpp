#include "Startup.h"

static void do_parameter(int *argc, char **argv)
{

    for(int i = 1; i < *argc; i++){
        if((i+1) == *argc){
            //delete argv[i];
            argv[i] = nullptr;
        }else{
            argv[i] = argv[i+1];
        }
    }
    --(*argc);
}

int main(int argc, char **argv)
{
    Startup startup(argc, argv);
    return startup.exec();
}
