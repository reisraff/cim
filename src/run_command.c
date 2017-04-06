#include <stdio.h>
#include <string.h>
#include "run.h"

int run_command(int argc, char **argv)
{
    if (!strcmp("run", argv[1]))
    {
        return run_start(argc, argv);
    }

    printf("There is no command '%s'\n", argv[1]);

    return 0;
}
