#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

int run_interactive(int argc, char **argv)
{
    DIR *dir = opendir(".cimtemp");
    if (! dir)
    {
        if (mkdir(".cimtemp", 0700) < 0)
        {
            printf("Unable to create a directory in: '%s'\n", ".cimtemp");
            return 1;
        }
    }
    closedir(dir);

    FILE *filecommands = fopen(".cimtemp/cimtempfilecommands", "w");
    if (!filecommands) {
       printf("Unable to open the file '%s'.\n", ".cimtemp/cimtempfilecommands");
       return 1;
    }
    fclose(filecommands);

    const char header[] =
        "#include <stdio.h>\n"
        "void main()\n"
        "{\n";
    const char bottom[] =
        "\n}\n";

    printf("Interactive mode enabled\n\n");

    const char command[] = "gcc -o .cimtemp/cimtempbin .cimtemp/cimtempfile.c";

    while (1) {
        printf("C > ");
        char current_command[79];
        strcpy(current_command, "");
        fgets(current_command, 79, stdin);

        if ((strlen(current_command)>0) && (current_command[strlen (current_command) - 1] == '\n'))
            current_command[strlen (current_command) - 1] = '\0';

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

        system(command);

        FILE *fp;
        char output[1035];

        fp = popen(".cimtemp/cimtempbin", "r");
        if (fp == NULL) {
            printf("Failed to run command\n");
            return 1;
        }

        while (fgets(output, sizeof(output) - 1, fp) != NULL) {
            printf("%s", output);
        }

        FILE *filecommands = fopen(".cimtemp/cimtempfilecommands", "a");

        regex_t preg;
        reg_errcode_t err;
        const char regex[] = "printf";
        if ((err = regcomp(&preg, regex, REG_EXTENDED|REG_NOSUB)) != REG_NOERROR)
            return err;

        if ((err = regexec(&preg, current_command, 0 , NULL, 0 )) != REG_NOERROR)
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

    return 0;
}
