#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h> 

#define LIMIT_SIZE 1048576 //1GB

bool is_open = false;

static void* (*real_malloc)(size_t)=NULL;
static ssize_t (*real_write)(int fd, const void *buf, size_t count);
static ssize_t (*real_read)(int fd, void *buf, size_t count);
typedef int (*real_open_type)(const char *pathname, int flags);


ssize_t write(int fd, const void *buf, size_t count)
{
  fprintf(stderr, "The programm is trying to make write call\n");
  real_write = dlsym(RTLD_NEXT,"write");
  if (is_open)
  {
    return 0;
  }
  else
  {
    return real_write(fd, buf, count);
  }
  
}

ssize_t read(int fd, void *buf, size_t count)
{
  fprintf(stderr, "Read syscall is made\n");
  real_read = dlsym(RTLD_NEXT, "read");
  if (is_open)
  {
    return 0;
  }
  else
  {
    return real_read(fd, buf, count);
  }
  
}


int open(const char *pathname, int flags, ...)
{
  //This is our custom open function

  real_open_type real_open;
  real_open = (real_open_type)dlsym(RTLD_NEXT,"open");
  fprintf(stderr, "The program is trying to make an open call\n" );
  if (is_open)
  {
    return 0;
  }
  else
  {
    return real_open(pathname, flags);
  }
  //printf("You can't make this call anymore\n");
  //return 0 ;//real_open(pathname,flags);
}

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

  void *p = NULL;
  pid_t pid = getpid();
  int pids[1000];
  int vss[1000];
  int rss[1000];
  int uss[1000];
  int pss[1000];
  char name[1000];
  char *line = (char *) calloc(100,sizeof(char)); 
  int read_return = 5;

   size *= 1024; // Kb is the unit we are working with; size is obtained in bytes 
   int file = open("/proc/procrank", O_RDONLY);
   if (file == -1)
   {
     perror("Error while opening the file!");
     exit(EXIT_FAILURE);
   }
   int line_number = 0;
   while (read_return != 0 )
   {

    int i=0, j=0, k=0, l=0;
    char store[256][256];
    int line_size = strlen(line);

    read_return = read(file, line, 60);

    for (i = 0, j = 0, k = 0; i < line_size;)
    {
      if (line[i] != '|')
      {
        store[j][k] = line[i];
        k++;
        i++;
      }
      else
      {
        i++;
        j++;
        k = 0;
      }
    }
   // fprintf(stderr, "store0:%d store1%s\n", );
    pids[line_number] = atoi(store[0]);
    uss[line_number] = atoi(store[3]);
    vss[line_number] = atoi(store[1]);
    pss[line_number] = atoi(store[4]);
    rss[line_number] = atoi(store[2]);
    line_number++;

   }
   //fprintf(stderr, "%s\n %d bytes\n", line, read_return);
  close(file);
   for (int j = 0; j < line_number; ++j)
   {

    //fprintf(stderr, "pid[%d]= %d et pid = %d\n" ,pids[j],pid );
   // fprintf(stderr, " %d et pid = %d\n" ,pids[j],pid );
     if (pids[j] == pid) //process found!
     {
       if ((size + uss[j]) >= LIMIT_SIZE)
       {
         fprintf(stderr, "The process is going to exceed the LIMIT_SIZE with%ld K\n", (LIMIT_SIZE - (size + uss[j])));
         is_open = true;
       }
       else
       {
         fprintf(stderr, "%ld %d %d %d %d\n\n", size*1024,vss[j],uss[j],rss[j],pss[j]);
       }
     }
   else
   {
    if (size > LIMIT_SIZE)
    {
    //  fprintf(stderr, "The process is asking for too memory space. Allowed:%d, requesting:%ld\n",LIMIT_SIZE, size );
    }
   }
  }
   //  clock_t begin = clock();
   // clock_t end = clock();
   //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  fprintf(stderr, "process (%d) asked for %ldK memory\n", pid, size*1024);
  //fprintf(stderr, "tim:%f\n", time_spent);
  //system(command);
  p = real_malloc(size);
      fprintf(stderr, "ppp%p\n", p);
  return p;
}


