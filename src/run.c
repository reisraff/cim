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

    const char path[] = ".cimtemp";
    DIR *dir = opendir(path);
    if (! dir)
    {
        if (mkdir(path, 0700) < 0)
        {
            printf("Unable to create a directory in: '%s'\n", path);
            return 1;
        }
    }
    closedir(dir);

    const char filepath[] = ".cimtemp/cimtempfile.c";
    FILE *file;
    file = fopen(filepath, "w");
    if (!file) {
       printf("Unable to open the file '%s'.\n", filepath);
       return 1;
    }


    const char header[] =
        "#include <stdio.h>\n"
        "void main()\n"
        "{\n";
    const char bottom[] =
        "\n}\n";

    char code[strlen(argv[2]) + strlen(header) + strlen(bottom)];
    strcpy(code, header);
    strcat(code, argv[2]);
    strcat(code, bottom);

    fwrite(code, 1, strlen(code), file);
    fclose (file);

    const char command[] = "gcc -o .cimtemp/cimtempbin .cimtemp/cimtempfile.c";
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

    pclose(fp);
    unlink(".cimtemp/cimtempfile.c");
    unlink(".cimtemp/cimtempfile");
    rmdir(".cimtemp");
    printf("\n");

    return 0;
}
