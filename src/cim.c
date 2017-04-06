#include <stdio.h>

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

    printf("%s\n", "To be continued.");

    return 0;
}
