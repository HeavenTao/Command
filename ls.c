#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

typedef struct LineInfo {
  long long size;
  unsigned char type;
  char power[10];
  char *time;
  char *name;
  char *userName;
  char *groupName;
} LineInfo;

typedef struct Node {
  LineInfo *lineInfo;
  struct Node *prev;
  struct Node *next;
} Node;

void getFileType(char *fullPath, LineInfo *lineInfo) {
  struct stat file;
  if (stat(fullPath, &file) == -1) {
    perror("stat");
    return;
  }

  if (S_ISREG(file.st_mode)) {
    lineInfo->type = 1;
  } else if (S_ISDIR(file.st_mode)) {
    lineInfo->type = 2;
  } else {
    lineInfo->type = 3;
  }

  if (file.st_mode & S_IRUSR) {
    lineInfo->power[0] = 'r';
  } else {
    lineInfo->power[0] = '-';
  }

  if (file.st_mode & S_IWUSR) {
    lineInfo->power[1] = 'w';
  } else {
    lineInfo->power[1] = '-';
  }

  if (file.st_mode & S_IXUSR) {
    lineInfo->power[2] = 'x';
  } else {
    lineInfo->power[2] = '-';
  }

  if (file.st_mode & S_IRGRP) {
    lineInfo->power[3] = 'r';
  } else {
    lineInfo->power[3] = '-';
  }

  if (file.st_mode & S_IWGRP) {
    lineInfo->power[4] = 'w';
  } else {
    lineInfo->power[4] = '-';
  }

  if (file.st_mode & S_IXGRP) {
    lineInfo->power[5] = 'x';
  } else {
    lineInfo->power[5] = '-';
  }

  if (file.st_mode & S_IROTH) {
    lineInfo->power[6] = 'r';
  } else {
    lineInfo->power[6] = '-';
  }

  if (file.st_mode & S_IWOTH) {
    lineInfo->power[7] = 'w';
  } else {
    lineInfo->power[7] = '-';
  }

  if (file.st_mode & S_IXOTH) {
    lineInfo->power[8] = 'x';
  } else {
    lineInfo->power[8] = '-';
  }

  lineInfo->power[9] = '\0';

  if (lineInfo->type == 1) {
    lineInfo->size = file.st_size;
  }

  struct tm *time = localtime(&file.st_mtime);
  char *t = malloc(sizeof(char) * 20);
  strftime(t, sizeof(t) * 20, "%Y-%m-%d %H:%M:%S", time);
  lineInfo->time = t;

  struct group *grp = getgrgid(file.st_gid);
  struct passwd *user = getpwuid(file.st_uid);

  lineInfo->userName = user->pw_name;
  lineInfo->groupName = grp->gr_name;
}

void sortDirList(Node *head) {
  Node *cur = NULL;
  cur = head;
  LineInfo *temp = NULL;

  while (head != NULL) {
    while (cur != NULL) {
      if (cur->lineInfo->type > head->lineInfo->type) {
        temp = cur->lineInfo;
        cur->lineInfo = head->lineInfo;
        head->lineInfo = temp;
      }
      cur = cur->next;
    }
    head = head->next;
    if (head != NULL) {
      cur = head->next;
    }
  }

  return;
}

Node *getDirList() {
  char *path = "/home/ht/code/Command/";
  DIR *dir = opendir(path);

  struct dirent *entry;

  Node *head = NULL;
  Node *cur = NULL;
  LineInfo *data = NULL;
  int pathLen = strlen(path);

  while ((entry = readdir(dir)) != NULL) {
    int fileNameLen = strlen(entry->d_name);
    char fullPath[pathLen + fileNameLen];
    strcat(fullPath, path);
    strcat(fullPath, entry->d_name);

    if (head == NULL) {
      head = (Node *)malloc(sizeof(Node));
      data = (LineInfo *)malloc(sizeof(LineInfo));
      data->userName = NULL;
      data->groupName = NULL;
      head->next = NULL;
      head->prev = NULL;
      head->lineInfo = data;
      data->size = 0;
      data->name = strdup(entry->d_name);
      getFileType(fullPath, data);

      cur = head;
    } else {
      Node *temp = (Node *)malloc(sizeof(Node));
      data = (LineInfo *)malloc(sizeof(LineInfo));
      data->userName = NULL;
      data->groupName = NULL;
      temp->next = NULL;
      temp->prev = NULL;
      temp->lineInfo = data;
      data->size = 0;
      data->name = strdup(entry->d_name);
      getFileType(fullPath, data);

      cur->next = temp;
      temp->prev = cur;
      cur = temp;
    }
  }

  closedir(dir);

  return head;
}

int getMaxLength(Node *head) {
  int max = 0;
  int len = 0;
  while (head != NULL) {
    len = strlen(head->lineInfo->name);
    if (len >= max) {
      max = len;
    }
    head = head->next;
  }
  return max;
}

void printHead() {
  printf("\x1b[33;4m%-10s\x1b[0m \x1b[33;4m%-5s\x1b[0m \x1b[33;4m%-5s\x1b[0m "
         "\x1b[33;4m%-5s\x1b[0m "
         "%-20s%s\n",
         "Permissions", "User", "Group", "Time", "Size", "Name");
}

int main() {

  Node *head = getDirList();
  Node *cur = head;

  sortDirList(head);

  int maxLen = getMaxLength(head);

  printHead();

  while (cur != NULL) {
    char *type =
        cur->lineInfo->type == 1 ? "\x1b[33m.\x1b[0m" : "\x1b[31md\x1b[0m";
    printf("%s%-10s %-5s %-5s %-5lld %*s %s\n", type, cur->lineInfo->power,
           cur->lineInfo->userName, cur->lineInfo->groupName,
           cur->lineInfo->size, maxLen * -1, cur->lineInfo->time,
           cur->lineInfo->name);

    /*printf("%s%s %s\n", type, cur->lineInfo->power, cur->lineInfo->name);*/
    cur = cur->next;
  }

  return 0;
}
