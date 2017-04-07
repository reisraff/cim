#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <signal.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "interactive.h"

int run_interactive(int argc, char **argv)
{
    DIR *dir = opendir(".cimtemp");
    if (! dir)
    {
        if (mkdir(".cimtemp", 0700) < 0)
        {
            printf("Unable to create a directory in: '.cimtemp'\n");

            return 1;
        }
    }
    closedir(dir);

    FILE *filecommands = fopen(".cimtemp/cimtempfilecommands", "w");
    if (!filecommands)
    {
       printf("Unable to open the file '.cimtemp/cimtempfilecommands'.\n");

       return 1;
    }
    fclose(filecommands);

    signal(SIGINT, int_handler);

    printf("Interactive mode enabled\n\n");
    printf("type 'quit' to exit\n\n");

    get_interactivity();

    return 0;
}

void int_handler(int sig)
{
     signal(SIGINT, int_handler);
     printf("\nOUCH, did you hit Ctrl-C?\nDo you really want to quit? type 'quit' to exit or press 'enter/return' to continue\n\n");
}

void get_interactivity()
{
    const char header[] =
        "#include <stdio.h>\n"
        "void main()\n"
        "{\n";
    const char bottom[] =
        "\n}\n";

    const char command[] = "gcc -o .cimtemp/cimtempbin .cimtemp/cimtempfile.c";

    while (1)
    {
        char *current_command = readline("C > ");

        if ((strlen(current_command) > 0) && (current_command[strlen(current_command) - 1] == '\n'))
            current_command[strlen (current_command) - 1] = '\0';

        strcpy(current_command , trimwhitespace(current_command));

        if (!strcmp("", current_command))
            continue;
        if (!strcmp("quit", current_command))
            break;

        add_history(current_command);

        FILE *f = fopen(".cimtemp/cimtempfilecommands", "r");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *string = malloc(fsize + 1);
        fread(string, fsize, 1, f);
        fclose(f);

        string[fsize] = 0;
        char commands[strlen(string) + strlen(current_command)];
        strcpy(commands, "");
        strcpy(commands, string);
        strcat(commands, current_command);

        char code[strlen(commands) + strlen(header) + strlen(bottom)];
        strcpy(code, "");
        strcpy(code, header);
        strcat(code, commands);
        strcat(code, bottom);

        FILE *file = fopen(".cimtemp/cimtempfile.c", "w");
        fwrite(code, 1, strlen(code), file);
        fclose (file);

        int out_signal = system(command);
        if (out_signal != 0)
            continue;

        FILE *fp;
        char output[1035];

        fp = popen(".cimtemp/cimtempbin", "r");
        if (fp == NULL)
        {
            printf("Failed to run command\n");

            exit(1);
        }

        while (fgets(output, sizeof(output) - 1, fp) != NULL)
            printf("%s", output);

        FILE *filecommands = fopen(".cimtemp/cimtempfilecommands", "a");

        regex_t preg;
        reg_errcode_t err;
        const char regex[] = "printf";
        if ((err = regcomp(&preg, regex, REG_EXTENDED|REG_NOSUB)) != REG_NOERROR)
            exit(err);

        if ((err = regexec(&preg, current_command, 0 , NULL, 0)) != REG_NOERROR)
        {
            fwrite(current_command, 1, strlen(current_command), filecommands);
            fwrite("\n", 1, 1, filecommands);
        }
        regfree(&preg);

        fclose(filecommands);

        pclose(fp);
        unlink(".cimtemp/cimtempfile.c");
        unlink(".cimtemp/cimtempfile");
        printf("\n");
    }

    unlink(".cimtemp/cimtempfile.c");
    unlink(".cimtemp/cimtempfile");
    unlink(".cimtemp/cimtempfilecommands");
    rmdir(".cimtemp");
}

char *trimwhitespace(char *str)
{
    char *end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end+1) = 0;

    return str;
}
