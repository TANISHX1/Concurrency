#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "style.h"
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
    char *entry_ccode;
    if (advanced == 1)
    {
        printf("%sinode\t|  Type  |    Creation time      |  last Modified time   |  last access time\t | Device ID | Length(B)  |  name \n%s", FG_BWHITE, RESET);
        printf("-----\t   ----       -------------         ------------------      ----------------\t   ---------   ---------     ---- \n");
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
            fprintf(stderr, "[%sError%s] stat\n", FG_BRED, RESET);
            continue;
        }
        if (S_ISDIR(status.st_mode))
        { // Dir check + inode
            // printf("%-5llu\t | \tDIR\t |",(long long unsigned int )entry->d_ino);
            strcpy(type, "DIR ");
            entry_ccode = FG_BBLUE;
        }
        else if (S_ISREG(status.st_mode))
        { // File check + inode
            // printf("%-5llu\t | \tFILE\t |",(long long unsigned int )entry->d_ino);
            strcpy(type, "FILE");
            entry_ccode = FG_BGREEN;
        }
        else
        { // if syslink , binary etc.. + inode
            // printf("%-5llu\t | \tOTR\t |",(long long unsigned int )entry->d_ino);
            strcpy(type, "OTR ");
            entry_ccode = FG_BWHITE;
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
            printf("%s%llu |  %s\t |  %s  |  %s  |  %s  |  %7lu  |%s", entry_ccode, (long long unsigned int)entry->d_ino, type, created_time, modified_time, l_accessed_buffer, (unsigned long)status.st_dev, RESET);
            printf(" %s%10lu |  %s%s\n", entry_ccode, status.st_size, entry->d_name, RESET);
        }
        else
        {
            (printf("%s%s\t   %s     %s    %s%s\n", entry_ccode, type, created_time, modified_time, entry->d_name, RESET));
        }
    }
    printf("%d\n", advanced);
}

void help()
{
    printf("%s=== WLS - Custom Directory Listing Utility ===%s\n\n", FG_CYAN, RESET);

    printf("%sNAME%s\n", FG_YELLOW, RESET);
    printf("    wls - list directory contents (custom implementation of ls)\n\n");

    printf("%sSYNOPSIS%s\n", FG_YELLOW, RESET);
    printf("    %s./wls%s [OPTION] [DIRECTORY]\n\n", FG_GREEN, RESET);

    printf("%sDESCRIPTION%s\n", FG_YELLOW, RESET);
    printf("    List information about files and directories in the specified directory.\n");
    printf("    If no directory is specified, lists contents of current directory.\n");
    printf("    Hidden files (starting with '.') are excluded by default.\n\n");

    printf("%sOPTIONS%s\n", FG_YELLOW, RESET);
    printf("    %s-A%s          Advanced mode - show detailed file information including:\n", FG_GREEN, RESET);
    printf("                 • Inode number\n");
    printf("                 • File type (DIR/FILE/OTR)\n");
    printf("                 • Creation time\n");
    printf("                 • Last modification time\n");
    printf("                 • Last access time\n");
    printf("                 • Device ID\n");
    printf("                 • File size in bytes\n");
    printf("                 • File/directory name\n\n");

    printf("    %s-H%s          Show this help message and exit\n\n", FG_GREEN, RESET);

    printf("%sUSAGE EXAMPLES%s\n", FG_YELLOW, RESET);
    printf("    %s./wls%s                    List current directory (simple format)\n", FG_GREEN, RESET);
    printf("    %s./wls /home/user%s         List specified directory (simple format)\n", FG_GREEN, RESET);
    printf("    %s./wls -A%s                 List current directory (advanced format)\n", FG_GREEN, RESET);
    printf("    %s./wls -A /var/log%s        List specified directory (advanced format)\n", FG_GREEN, RESET);
    printf("    %s./wls -H%s                 Show this help message\n\n", FG_GREEN, RESET);

    printf("%sOUTPUT FORMATS%s\n", FG_YELLOW, RESET);
    printf("    %sSimple Mode:%s\n", FG_MAGENTA, RESET);
    printf("    • Just file/directory names in tab-separated format\n");
    printf("    • Hidden files are not shown\n\n");

    printf("    %sAdvanced Mode (-A):%s\n", FG_MAGENTA, RESET);
    printf("    • Tabular format with detailed file information\n");
    printf("    • Columns: Inode | Type | Creation | Modified | Accessed | Device | Size | Name\n\n");

    printf("%sFILE TYPES%s\n", FG_YELLOW, RESET);
    printf("    %sDIR %s         Directory\n", FG_BLUE, RESET);
    printf("    %sFILE%s         Regular file\n", FG_GREEN, RESET);
    printf("    %sOTR %s         Other (symbolic links, special files, etc.)\n", FG_YELLOW, RESET);
    printf("\n");

    printf("%sDATE FORMAT%s\n", FG_YELLOW, RESET);
    printf("    All timestamps are displayed in format: YYYY-MM-DD HH:MM:SS\n\n");

    printf("%sNOTES%s\n", FG_YELLOW, RESET);
    printf("    • Hidden files (starting with '.') are not displayed\n");
    printf("    • Creation time actually shows inode change time (ctime)\n");
    printf("    • File sizes are displayed in bytes\n");
    printf("    • Maximum path length supported: %d characters\n", PATH_LIMIT);
    printf("    • Requires read permissions on the target directory\n\n");

    printf("%sERROR HANDLING%s\n", FG_YELLOW, RESET);
    printf("    The program will exit with an error message if:\n");
    printf("    • Invalid directory path is provided\n");
    printf("    • Permission denied to read directory\n");
    printf("    • Invalid command line arguments\n");
    printf("    • Too many arguments provided\n\n");

    printf("%sEXIT STATUS%s\n", FG_YELLOW, RESET);
    printf("    %s0%s    Success\n", FG_GREEN, RESET);
    printf("    %s1%s    Error (invalid path, permissions, or arguments)\n\n", FG_BRED, RESET);

    printf("%sAUTHOR%s\n", FG_YELLOW, RESET);
    printf("   TANISH_X\n\n");
    // printf("Called help\n");
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

            fprintf(stderr, "[%sError%s] Invalid_Path_\nUse './wls -H' for Help\n", FG_BRED, RESET);
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

                        fprintf(stderr, "[%sError%s] Invalid_Path_\nUse './wls -H' for Help\n", FG_BRED, RESET);
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
                    fprintf(stderr, "[%sError%s] Invalid Flag Use\n'./wls -H' for Help\n ", FG_BRED, RESET);
                    exit(1);
                }
            }
            else
            {
                path = argv[1];
                fdir = opendir(path);
                if (fdir == NULL)
                {

                    fprintf(stderr, "[%sError%s] Invalid_Path_\nUse './wls -H' for Help\n", FG_BRED, RESET);
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

                        fprintf(stderr, "[%sError%s] Invalid_tPah_\nUse './wls -H' for Help\n", FG_BRED, RESET);
                        exit(1);
                    }

                    list_entries(fdir, path, true);
                }
            }
            else
            {
                fprintf(stderr, "[%sError%s] Usage: ./wls <option> <path>..\nUse './wls -H' for Help\n", FG_BRED, RESET);
                exit(1);
            }
        }
    }

    else
    {
        fprintf(stderr, "[%sError%s] to_Many_Arguments_in_command_line Usage: <Flag> <Path>\nUse './wls -H' for Help \n", FG_BRED, RESET);
        exit(1);
    }
    closedir(fdir);
    return 0;
}