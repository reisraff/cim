#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int run_start(int argc, char **argv)
{
    if (!(argc > 2))
    {
        printf("Missing required argument <code>\n");

        return 1;
    }

    DIR *dir = opendir(".cimtemp");
    if (! dir)
    {
        if (mkdir(".cimtemp", 0700) < 0)
        {
            printf("Unable to create a directory: '.cimtemp'\n");

            return 1;
        }
    }
    closedir(dir);

    const char header[] =
        "#include <stdio.h>\n"
        "void main()\n"
        "{\n";
    const char bottom[] =
        "\n}\n";

    char code[strlen(argv[2]) + strlen(header) + strlen(bottom)];
    strcpy(code, "");
    strcpy(code, header);
    strcat(code, argv[2]);
    strcat(code, bottom);

    FILE *file = fopen(".cimtemp/cimtempfile.c", "w");
    if (!file)
    {
       printf("Unable to open the file '.cimtemp/cimtempfile.c'.\n");

       return 1;
    }
    fwrite(code, 1, strlen(code), file);
    fclose (file);

    const char command[] = "gcc -o .cimtemp/cimtempbin .cimtemp/cimtempfile.c";
    int out_signal = system(command);
    if (out_signal != 0)
        return out_signal;

    FILE *fp;
    char output[1035];

    fp = popen(".cimtemp/cimtempbin", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");

        return 1;
    }

    while (fgets(output, sizeof(output) - 1, fp) != NULL)
        printf("%s", output);

    pclose(fp);
    unlink(".cimtemp/cimtempfile.c");
    unlink(".cimtemp/cimtempfile");
    rmdir(".cimtemp");
    printf("\n");

    return 0;
}
