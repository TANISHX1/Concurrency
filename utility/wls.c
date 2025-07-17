#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#define PATH_LIMIT 1024

int list_entries(DIR *fdir, const char *path, bool advanced)
{
    struct dirent *entry;
    struct stat status;
    char path_buffer[PATH_LIMIT];
    // time buffers
    char created_time[25], modified_time[25];
    char l_accessed_buffer[25];

    char type[10];
    if (advanced == 1)
    {
        printf("inode\t|  Type  |    Creation time      |  last Modified time   |  last access time\t | Device ID | Length(B) |  name \n");
        printf("-----\t   ----       -------------         ------------------      ----------------\t   ---------   ---------    ---- \n");
    }
    else
    {
        printf("Type\t|     Creation time \t|  last Modified time  |  name \n");
        printf("----\t      ------------- \t   ------------------     ---- \n");
    }
    while ((entry = readdir(fdir)) != NULL)
    {

        // To determine the Type of Entry
        snprintf(path_buffer, sizeof(path_buffer), "%s/%s", path, entry->d_name); // full address of particular entry
        if (stat(path_buffer, &status) == -1)
        {
            fprintf(stderr, "[Error] stat\n");
            continue;
        }
        if (S_ISDIR(status.st_mode))
        { // Dir check + inode
            // printf("%-5llu\t | \tDIR\t |",(long long unsigned int )entry->d_ino);
            strcpy(type, "DIR ");
        }
        else if (S_ISREG(status.st_mode))
        { // File check + inode
            // printf("%-5llu\t | \tFILE\t |",(long long unsigned int )entry->d_ino);
            strcpy(type, "FILE");
        }
        else
        { // if syslink , binary etc.. + inode
            // printf("%-5llu\t | \tOTR\t |",(long long unsigned int )entry->d_ino);
            strcpy(type, "OTR ");
        }

        // to find And print Date when file is created and last modified
        struct tm *creation_time, *l_modification_time, *l_access_time;
        creation_time = localtime(&status.st_ctime);
        l_modification_time = localtime(&status.st_mtime);

        strftime(created_time, sizeof(created_time), "%Y-%m-%d %H:%M:%S", creation_time);
        strftime(modified_time, sizeof(modified_time), "%Y-%m-%d %H:%M:%S", l_modification_time);

        if (advanced)
        {
            l_access_time = localtime(&status.st_atime);
            strftime(l_accessed_buffer, sizeof(l_accessed_buffer), "%Y-%m-%d %H:%M:%S", l_access_time);
        }

        if (advanced)
        {
            printf("%llu |  %s\t |  %s  |  %s  |  %s  |  %7lu  |", (long long unsigned int)entry->d_ino, type, created_time, modified_time, l_accessed_buffer, (unsigned long)status.st_dev);
            printf(" %10lu |  %s\n", status.st_size, entry->d_name);
        }
        else
        {
            (printf("%s\t   %s     %s    %s\n", type, created_time, modified_time, entry->d_name));
        }
    }
    printf("%d\n", advanced);
}

void help()
{
    printf("Called help\n");
}

int main(int argc, char *argv[])
{
    struct dirent *entry;
    DIR *fdir;
    const char *path;

    if (argc == 1)
    {
        path = ".";
        fdir = opendir(path);
        if (fdir == NULL)
        {

            fprintf(stderr, "[Error] Invalid_Pah_\n");
            exit(1);
        }

        while ((entry = readdir(fdir)) != NULL)
        {
            if (entry->d_name[0] == '.')
            {
                continue;
            }
            printf("%s\t", entry->d_name);
        }
        printf("\n");
    }

    else if (argc > 1 && argc < 4)
    {

        if (argc == 2)
        {
            if (strncmp(argv[1], "-", 1) == 0)
            {
                if (!strcmp(argv[1], "-A"))
                {
                    path = ".";
                    fdir = opendir(path);
                    if (fdir == NULL)
                    {

                        fprintf(stderr, "[Error] Invalid_Pah_\n");
                        exit(1);
                    }

                    list_entries(fdir, path, true);
                }
                else if (!strcmp(argv[1], "-H"))
                {
                    help();
                    exit(0);
                }
                else
                {
                    fprintf(stderr, "[Error] Invalid Flag Use './wls -H' for Help\n ");
                    exit(1);
                }
            }
            else
            {
                path = argv[1];
                fdir = opendir(path);
                if (fdir == NULL)
                {

                    fprintf(stderr, "[Error] Invalid_Pah_\n");
                    exit(1);
                }

                list_entries(fdir, path, false);
            }
        }

        else if (argc == 3)
        {
            if (strncmp(argv[1], "-", 1) == 0)
            {
                if (!strcmp(argv[1], "-A"))
                {
                    path = argv[2];
                    fdir = opendir(path);
                    if (fdir == NULL)
                    {

                        fprintf(stderr, "[Error] Invalid_Pah_\n");
                        exit(1);
                    }

                    list_entries(fdir, path, true);
                }
            }
            else
            {
                fprintf(stderr, "[Error] Usage: ./wls <option> <path>..\n");
                exit(1);
            }
        }
    }

    else
    {
        fprintf(stderr, "[Error] to_Many_Arguments_in_command_line Usage: <Flag> <Path> \n");
        exit(1);
    }
    closedir(fdir);
    return 0;
}
