#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  char line[256];
  int pids[1000];
  int uss[1000];
  int vss[1000];
  int rss[1000];
  int pss[1000];
  char name[50]; 
  char c;
  FILE *file;
  file = fopen("./procrank.txt","r");
  if (file == NULL){
    perror("Error while opening the file!\n");
    exit(EXIT_FAILURE);
  }
  int i = 0;
  while(fgets(line, sizeof(line), file)){
    sscanf(line, "%d %c %s %c %d %c %d %c %d %c %d", &pids[i], &c, name, &c,  &uss[i], &c,  &vss[i], &c,  &rss[i], &c,  &pss[i]); 
    i++;
  }
  fclose(file);
  for (int j = 0; j < i; j++){
    printf("pids[%d] = %d | uss[%d] = %d | vss[%d] = %d | rss[%d] = %d | pss[%d] = %d\n",j, pids[j],j, uss[j],j, vss[j],j, rss[j],j, pss[j]);
  }
}
