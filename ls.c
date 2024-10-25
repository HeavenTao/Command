#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct LineInfo {
  long long size;
  unsigned char type;
  char power[10];
  char *name;
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

  if(file.st_mode&S_IWUSR){
    lineInfo->power[1]='w';
  }else{
    lineInfo->power[1]='-';
  }

  if(file.st_mode&S_IXUSR){
    lineInfo->power[2]='x';
  }else{
    lineInfo->power[2]='-';
  }

  if(file.st_mode&S_IRGRP){
    lineInfo->power[3]='r';
  }else{
    lineInfo->power[3]='-';
  }
  
  if(file.st_mode&S_IWGRP){
    lineInfo->power[4]='w';
  }else{
    lineInfo->power[4]='-';
  }

  if(file.st_mode&S_IXGRP){
    lineInfo->power[5]='x';
  }else{
    lineInfo->power[5]='-';
  }

  if(file.st_mode&S_IROTH){
    lineInfo->power[6]='r';
  }else{
    lineInfo->power[6]='-';
  }

  if(file.st_mode&S_IWOTH){
    lineInfo->power[7]='w';
  }else{
    lineInfo->power[7]='-';
  }

  if(file.st_mode&S_IXOTH){
    lineInfo->power[8]='x';
  }else{
    lineInfo->power[8]='-';
  }

  lineInfo->power[9]='\0';

  lineInfo->size=file.st_size;
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
  char *path = "/home/ht/temp/learnc/";
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
      head = (Node *)malloc(sizeof(Node *));
      data = (LineInfo *)malloc(sizeof(LineInfo *));
      head->next = NULL;
      head->prev = NULL;
      head->lineInfo = data;
      data->size=0;
      data->name = strdup(entry->d_name);
      getFileType(fullPath,data);

      cur = head;
    } else {
      Node *temp = (Node *)malloc(sizeof(Node *));
      data = (LineInfo *)malloc(sizeof(LineInfo *));
      temp->next = NULL;
      temp->prev = NULL;
      temp->lineInfo = data;
      data->size=0;
      data->name = strdup(entry->d_name);
      getFileType(fullPath,data);

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

int main() {

  Node *head = getDirList();
  Node *cur = head;

  sortDirList(head);

  int maxLen = getMaxLength(head);

  while (cur != NULL) {
    char *type=cur->lineInfo->type==1?"\x1b[33m.":"\x1b[31md";
    printf("%s%s %s\n",type,cur->lineInfo->power,cur->lineInfo->name);
    cur = cur->next;
  }

  return 0;
}
