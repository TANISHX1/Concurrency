#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

void type(DIR*fdir, char *path){
    struct dirent *entry;
    struct stat status;
    char *path_buffer[1024];
    entry = readdir(fdir);
    snprintf(path_buffer,sizeof(path_buffer),"%s/%s",path,entry->d_name);
     if(stat(path_buffer,&status) == -1){
        fprintf(stderr,"[Error] stat\n");
        exit(1);
     }
    if(S_ISDIR(status.st_mode)){
        printf("DIR\n");
    }
    else if(S_ISREG(status.st_mode)){
        printf("FILE\n");
    }
    else{
        printf("Others\n");
    }
    return 0;
}
int main(int argc , char *argv[]){
    char *path;
    path = argc>1?argv[1]:".";
    DIR *fdir;
    fdir = opendir(path);
    if(fdir ==NULL){
        fprintf(stderr,"[Error] Invalid_path\n");
        exit(1);
    }
    type(fdir,path);
    return 0;

}