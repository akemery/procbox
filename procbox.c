#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

#define LIMIT_SIZE 1048576 //1GB

static void* (*real_malloc)(size_t)=NULL;

// static void process_procrank(pid_t pid, size_t size){
//   char line[256];
//   int pids[1000];
//   int vss[1000];
//   int rss[1000];
//   int uss[1000];
//   int pss[1000];
//   char name[1000];
//   char c;
//   FILE *file;

//   size *= 1024; // Kb is the unit we are working with; size is obtained in bytes 
//   file = fopen("/proc/procrank", "r");
//   if (file == NULL)
//   {
//     perror("Error while opening the file!");
//     exit(EXIT_FAILURE);
//   }
//   int i = 0;
//   while(fgets(line, sizeof(line), file)){
//     sscanf(line, "%d %c %s %c %d %c %d %c %d %c %d", &pids[i], &c, name, &c, &vss[i], &c, &rss[i], &c, &uss[i], &c, &pss[i]);
//     i++;
//   }
//   fclose(file);
//   for (int j = 0; j < i; ++j)
//   {
//     if (pids[j] == pid) //process found!
//     {
//       if ((size + uss[i]) >= LIMIT_SIZE)
//       {
//         fprintf(stderr, "What to do? the process is going to exceed the LIMIT_SIZE with%ld K\n", (LIMIT_SIZE - (size + uss[i])));
//       }
//       else
//       {
//         fprintf(stderr, "All is fine for the moment!\n");
//       }
//     }
//   }
// }

static void sandbox_init(void)
{
  real_malloc = dlsym(RTLD_NEXT, "malloc");
  if (NULL == real_malloc) {
    fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
  }
  printf("Init ok\n");
}

void *malloc(size_t size)
{
  if(real_malloc==NULL) {
    sandbox_init();
  }
 // int argvsize = 5;
  //char **argvs = malloc(argvsize * sizeof(*argvs));
  //char *newenv[] = { NULL };
  //argvs[0] = PYTHON;
  //argvs[argvsize - 1] = NULL;  
  //memcpy(&argvs[1], )
  //char *command = "./procrank.py %d %d";
  //char buf[64];
  void *p = NULL;
  pid_t pid = getpid();
 // snprintf(buf, 64, command, pid, size);
  //process_procrank(pid,size);
  char line[256];
  int pids[1000];
  int vss[1000];
  int rss[1000];
  int uss[1000];
  int pss[1000];
  char name[1000];
  char c;
  FILE *file;

   size *= 1024; // Kb is the unit we are working with; size is obtained in bytes 
   file = fopen("./procrank", "r");
  // if (file == NULL)
  // {
  //   perror("Error while opening the file!");
  //   exit(EXIT_FAILURE);
  // }
  // int i = 0;
  // while(fgets(line, sizeof(line), file)){
  //   sscanf(line, "%d %c %s %c %d %c %d %c %d %c %d", &pids[i], &c, name, &c, &vss[i], &c, &rss[i], &c, &uss[i], &c, &pss[i]);
  //   i++;
  // }
 // fclose(file);
  // for (int j = 0; j < i; ++j)
  // {
  //   if (pids[j] == pid) //process found!
  //   {
  //     if ((size + uss[i]) >= LIMIT_SIZE)
  //     {
  //       fprintf(stderr, "What to do? the process is going to exceed the LIMIT_SIZE with%ld K\n", (LIMIT_SIZE - (size + uss[i])));
  //     }
  //     else
  //     {
  //       fprintf(stderr, "All is fine for the moment!\n");
  //     }
  //   }
  // }
  fprintf(stderr, "process (%d) asked for %ld bytes memory\n", pid, size);
  //system(command);
  p = real_malloc(size);
  return p;
}


