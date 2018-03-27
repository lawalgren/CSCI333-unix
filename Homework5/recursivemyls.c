#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void myls(char *dirname, bool recursive, bool showhidden, int level) {
  DIR *dir = opendir(dirname);
  if (recursive) {
    char format[256] = {}; //add spaces to beginning of directory name for readability
    int i;
    for (i = 0; i < level; i++) {
      if (i == 0)
        strncpy(format, "  ", 256);
      else
        strncat(format, "  ", 256);
    }
    strncat(format, dirname, 256);
    printf("%s:\n", format);
  }
  if (dir == NULL) { 
    perror(NULL);
    exit(1);
  }
  for (;;) {
    struct dirent *entry = readdir(dir);
    bool recursed = false;
    if (entry == NULL) break;
    if (entry->d_name[0] != '.' || showhidden) {
      if (recursive) {
        struct stat sb; //there is an easier way to do this on linux, but lstat is the more POSIX compliant one
        bool success = true;
        char path[256]; //store path to current entry in path 
        strncpy(path,dirname,256);
        if (path[strlen(path) - 1] != '/')
          strncat(path,"/",256);
        strncat(path,entry->d_name,256); 
        if(lstat(path, &sb) == -1) //use lstat so hopefully program won't run forever on loops made with symlinks
          success = false;
        if (S_ISDIR(sb.st_mode) && success && strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) 
        { //if lstat can get info on current entry and it is a directory
          recursed = true;
          myls(path, 1, showhidden, level + 1); //recurse into it
        }
      }
      if (!recursed) { //add spaces to beginning of filename for readability
        char nameformat[256] = {};
        int j;
        for (j = 0; j <= level; j++) {
          if (j == 0)
            strncpy(nameformat, "  ", 256);
          else
            strncat(nameformat, "  ", 256);
        }
        strncat(nameformat, entry->d_name, 256);
        strncat(nameformat, "\0", 256);
        printf("%s\n", nameformat);
      }
    }
  }
  if (closedir(dir) != 0) {
    perror(NULL);
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  bool recursive = false;
  bool showhidden = false;
  int level = 0;
  if (argc == 1) {
    myls(".", recursive, showhidden, 0);
  } else {
    for (int i=1; i<argc; ++i) {
      if (strcmp(argv[i], "-R") == 0)
        recursive = true;
      else if (strcmp(argv[i], "-a") == 0)
        showhidden = true;
      else
        myls(argv[i], recursive, showhidden, 0);
    }
  }

  return 0;
}
