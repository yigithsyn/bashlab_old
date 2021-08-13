#define HELP "\n"                                                        \
             "Frequency to wavelength convert\n"                         \
             "Usage: freq2wavelen <freq(s)> \n"                          \
             "                    <freq(s)> <out> \n\n"                  \
             "<freq>: input value [double] or variable name [string] \n" \
             "<out> : output variable name [string] \n"

#define DEBUG 1

#include "macros.h"

#include <stdio.h>
#include <string.h>
// #include <ctype.h>
// #include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "jansson.h"

static struct stat stat_buff;
static json_error_t *json_error;
static json_t *workspace;
static bool valid_workspace = false;

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf(HELP);
    return 0;
  }

  size_t arg_len = 0;
  double *freq = NULL, *wlen = NULL;
  workspace = json_load_file(WORKSPACE, 0, json_error);
  if (stat(WORKSPACE, &stat_buff) == 0)
    if (workspace != NULL && json_typeof(workspace) == JSON_OBJECT)
      valid_workspace = true;

  if (DEBUG)
    if (valid_workspace)
      printf("[DEBUG] Workspace file found.\n");
    else
      printf("[DEBUG] Workspace file not found.\n");

  if (argc >= 2)
  {
    if (valid_workspace)
    {
      size_t var_index;
      json_t *var;
      json_array_foreach(json_object_get(workspace, "variables"), var_index, var)
      {
        if (strcmp(json_string_value(json_object_get(var, "name")), argv[1]) == 0)
        {
          if (DEBUG)
            printf("[DEBUG] Argument '%s' found in workspace.\n", argv[1]);
          if (json_object_get(var, "value") != NULL)
          {
            if (DEBUG)
              printf("[DEBUG] Argument '%s' value found in workspace.\n", argv[1]);
            if (json_typeof(json_object_get(var, "value")) == JSON_REAL)
            {
              arg_len = 1;
              freq = malloc(sizeof(double));
              freq[0] = json_number_value(json_object_get(var, "value"));
              if (DEBUG)
                printf("[DEBUG] Value of '%s' is = %.6e.\n", argv[1], freq[0]);
            }
            else if (json_typeof(json_object_get(var, "value")) == JSON_ARRAY)
            {
              arg_len = json_array_size(json_object_get(var, "value"));
              freq = malloc(arg_len * sizeof(double));
              if (DEBUG)
              {
                printf("[DEBUG] Argument '%s' is array with size of %d.\n", argv[1], arg_len);
                printf("[DEBUG] Argument pointer is allocated dynamiccally with size of %dx%d=%d.\n", sizeof(*freq), arg_len, sizeof(*freq) * arg_len);
              }
            }
            else
            {
              if (DEBUG)
                printf("[DEBUG] Argument value is not supported.\n");
            }
          }
          else
          {
            if (DEBUG)
              printf("[DEBUG] Argument value not found in workspace.\n");
          }
          break;
        }
        if (DEBUG)
          printf("[DEBUG] Argument not found in workspace.\n");
      }
    }
  }

  /* calculate wavelength */
  // if (SIZEOF_ARR(freq) == 1)
  // {
  //   printf("%.6e\n", freq[0]);
  //   double wavelen = (double)(C0) / freq[0];
  //   printf("%.6e", wavelen);
  // }

  /* print out and add workspace */
  // if (stat(WORKSPACE, &stat_buff) == 0)
  // {
  //   json_error_t *json_error = NULL;
  //   json_t *workspace = json_load_file("workspace.json", 0, json_error);
  //   if (workspace != NULL && json_typeof(workspace) == JSON_OBJECT)
  //   {
  //   }
  // }

  
  free(freq);
  free(wlen);
  json_decref(workspace);
  return 0;
}