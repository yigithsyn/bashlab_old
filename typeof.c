#define HELP "-------------------- Type of variable ------------------------\n"\
             "Usage: typeof <variable>                                      \n"\
             "variable: variable name                                         "\

// #define DEBUG 1

// #include "macros.h"
 
// #include <stdio.h>
// #include <string.h>
// #include <sys/stat.h>
// #include <stdbool.h>

#include "jansson.h"

// static struct stat stat_buff;
// static json_error_t *json_error;
// static json_t *workspace;
// static bool valid_workspace = false;

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf(HELP);
    return 0;
  }
  json_t *new = NULL;
  return typeof(new);
}