#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

#include <linux/types.h>
#include <linux/fs.h>

#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <time.h>

#include "modes.h"
#include "exported_inode.h"

static const char *mons[] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};

void get_mode_str(unsigned short mode, char*);

int main( int argc, char *argv[] )
{

    // printf("%8s %11s %6s %9s %13s %10s\n","inode", "mode", "links", "size", "time", "file");

    if( argc != 2 ) {
        printf("Wrong format\nUsage: inodes <path to directory>\n");
        return -1;
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(argv[1]);
    if(!d) {
        printf("No such directory\n");
        return -1;
    }

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            struct exp_inode *inode = malloc(sizeof(struct exp_inode));
            if(syscall(549, dir->d_name, inode)) {
                printf("something went wrong");
                return -1;
            }

            printf("%8lu ", inode->i_ino);
            char s[11];
            get_mode_str(inode->i_mode, s);
            printf("%11s ", s);
            printf("%6d ", inode->i_nlink);
            printf("%9d ", inode->i_size);
            
            int64_t mtime = inode->i_mtime / 1000000000;
            struct tm *tm;
            tm=localtime(&mtime);
            printf("%4s %2d %02d:%02d ", mons[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min);

            printf("%s\n", dir->d_name);

        }
        closedir(d);
    }
    return 0;
}

void get_mode_str(unsigned short mode, char *s) {
    strcpy(s, "----------");

    if(mode & S_IFREG) s[0] = '-';
    else if(mode & S_IFDIR) s[0] = 'd';
    else if(mode & S_IFCHR) s[0] = 'c';
    else if(mode & S_IFBLK) s[0] = 'b';
    else if(mode & S_IFSOCK) s[0] = 's';
    else if(mode & S_IFIFO) s[0] = 'p';
    else if(mode & S_IFLNK) s[0] = 'l';

    if(mode & S_IRUSR) s[1] = 'r';
    if(mode & S_IWUSR) s[2] = 'w';
    if(mode & S_IXUSR) s[3] = 'x';

    if(mode & S_IRGRP) s[4] = 'r';
    if(mode & S_IWGRP) s[5] = 'w';
    if(mode & S_IXGRP) s[6] = 'x';

    if(mode & S_IROTH) s[7] = 'r';
    if(mode & S_IWOTH) s[8] = 'w';
    if(mode & S_IXOTH) s[9] = 'x';
}