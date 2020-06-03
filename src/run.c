#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int run_start(int argc, char **argv)
{
    if (argc < 3)
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

    FILE *file = fopen(".cimtemp/cimtempfile.c", "w");
    if (!file)
    {
       printf("Unable to open the file '.cimtemp/cimtempfile.c'.\n");

       return 1;
    }
    fwrite(header, 1, strlen(header), file);
    fwrite(argv[2], 1, strlen(argv[2]), file);
    fwrite(bottom, 1, strlen(bottom), file);
    fclose (file);

    int out_signal = system("gcc -o .cimtemp/cimtempbin .cimtemp/cimtempfile.c");
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
    unlink(".cimtemp/cimtempbin");
    rmdir(".cimtemp");

    printf("\n");

    return 0;
}
