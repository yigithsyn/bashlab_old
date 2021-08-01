#define HELP "\n"                                                        \
             "Frequency to wavelength convert\n"                         \
             "Usage: freq2wavelen <freq(s)> \n"                             \
             "                    <freq(s)> <out> \n\n"                     \
             "<freq>: input value [double] or variable name [string] \n" \
             "<out> : output variable name [string] \n"

#include <stdio.h>
// #include <ctype.h>
// #include <string.h>
// #include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

// return true if the file specified
// by the filename exists
bool file_exists(const char *filename)
{
  struct stat buffer;
  return stat(filename, &buffer) == 0 ? true : false;
}

#include "jansson.h"

#define C0 299792458        // Speed of light in m/s
// #define MAX_FREQ_STR_LEN 50 //

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf(HELP);
    return 0;
  }

  struct stat stat_buff;
  double arg = 0, *args = NULL;
  double val = 0, *vals = NULL;

  if (argc >= 2)
  {
    bool var_found = false;
    if (stat("workspace.json", &stat_buff) == 0)
    {
      json_error_t *json_error = NULL;
      json_t *workspace = json_load_file("workspace.json", 0, json_error);
      if (workspace != NULL && json_typeof(workspace) == JSON_OBJECT)
      {
        size_t index;
        json_t *value;
        json_array_foreach(json_object_get(workspace, "variables"), index, value)
        {
          if (strcmp(json_string_value(json_object_get(value, "name")), argv[1]) == 0)
          {
            json_t *variable = json_object_get(value, "value");
            if (variable && (json_typeof(variable) == JSON_REAL || json_typeof(variable) == JSON_ARRAY))
            {
              var_found = true;
              if (json_typeof(variable) == JSON_REAL)
                arg = json_number_value(variable);
              else
              {
                // vector operation here
              }
            }

            break;
          }
        }
      }
      json_decref(workspace);
    }

    if (var_found)
    {
      printf("Variable found in workspace\n");
    }
    if (!var_found)
      arg = atof(argv[1]);
  }

  /* calculate wavelength */
  if (!args)
  {
    printf("%.6e\n", arg);
    double wavelen = (double)(C0) / arg;
    printf("%.6e", wavelen);
  }

  /* print out and add workspace */
  if (stat("workspace.json", &stat_buff) == 0)
  {
    json_error_t *json_error = NULL;
    json_t *workspace = json_load_file("workspace.json", 0, json_error);
    if (workspace != NULL && json_typeof(workspace) == JSON_OBJECT)
    {
    }
  }

  return 0;
}