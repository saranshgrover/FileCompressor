#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "debug.h"



int main(int argc, char **argv)
{
    int ret;
    if(validargs(argc, argv))
        USAGE(*argv, EXIT_FAILURE);
    debug("Options: 0x%x", global_options);
    if(global_options & 1)
        USAGE(*argv, EXIT_SUCCESS);
    if(global_options & 0x00000002)
        compress();
    if(global_options & 0x00000004)
        decompress();

    return EXIT_SUCCESS;
}
