#include <stdio.h>
#include <regex.h>
#include "run_command.h"

const char presentation_string[] =
    "  ___  ____  __  __ \n"
    " / __)(_  _)(  \\/  )\n"
    "( (__  _)(_  )    ( \n"
    " \\___)(____)(_/\\/\\_)\n"
    "CIM version 0.1.0 2017-04-06 15:19:04\n"
    "\n"
    "Usage:\n"
    "  command [options] [arguments]\n"
    "\n"
    "Options:\n"
    "  --help\n"
    "\n"
    "Available commands:\n"
    "  run            Run a simple piece of code\n"
    "  interactive    Run interactively\n"
    ;

void show_presentation()
{
    printf("%s", presentation_string);
}

int main(int argc, char **argv)
{
    if (!(argc > 1))
    {
        show_presentation();

        return 0;
    }

    int i;
    for (i = 1; i < argc; i++)
    {
        regex_t preg;
        reg_errcode_t err;
        const char regex[] = "--help";
        if ((err = regcomp(&preg, regex, REG_EXTENDED|REG_NOSUB)) != REG_NOERROR)
            return err;

        if ((err = regexec(&preg, argv[i], 0 , NULL, 0 )) != REG_NOERROR)
        {
            continue;
        }
        else
        {
            show_presentation();

            return 0;
        }
        regfree(&preg);
    }

    return run_command(argc, argv);
}
