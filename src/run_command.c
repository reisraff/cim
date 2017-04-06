#include <stdio.h>
#include <string.h>
#include "run.h"
#include "interactive.h"

int run_command(int argc, char **argv)
{
    if (!strcmp("run", argv[1]))
    {
        return run_start(argc, argv);
    }

    if (!strcmp("interactive", argv[1]))
    {
        return run_interactive(argc, argv);
    }

    printf("There is no command '%s'\n", argv[1]);

    return 0;
}
