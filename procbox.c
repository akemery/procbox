#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include <dlfcn.h>

static void* (*real_malloc)(size_t)=NULL;

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
  fprintf(stderr, "process (%d) asked for %ld bytes memory\n", pid, size);
  p = real_malloc(size);
  return p;
}
