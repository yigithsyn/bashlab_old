#define HELP "---------------- Frequency to wavelength convert -------------\n"\
             "Usage: freq2wavelen <freq> <=out>                             \n"\
             "freq: input value (double[]) or variable name                 \n"\
             "out : output variable name                                      "

#define DEBUG 1

#include "macros.h"
 
#include <stdio.h>
#include <string.h>
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
  char res_name[20] = {'\0'};
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
              for (size_t i = 0; i < arg_len; ++i)
                freq[i] = json_real_value(json_array_get(json_object_get(var, "value"), i));
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
        arg_len = 1;
        freq = malloc(sizeof(double));
        freq[0] = atof(argv[1]);
      }
    }
    else
    {
      arg_len = 1;
      freq = malloc(sizeof(double));
      freq[0] = atof(argv[1]);
    }
  }

  /* calculate */
  wlen = malloc(arg_len * sizeof(double));
  for (size_t i = 0; i < arg_len; ++i)
    wlen[i] = (double)(C0) / freq[i];
  if (DEBUG)
    for (size_t i = 0; i < arg_len; ++i)
      printf("[DEBUG] Result[%d]: %f.\n", i, (double)(C0) / freq[i]);

  /* print out */
  if (argc == 2)
    for (size_t i = 0; i < arg_len; ++i)
      printf("%f\n", (double)(C0) / freq[i]);
      
  /* add workspace */
  if (valid_workspace)
  {
    json_t *variables = json_object_get(workspace, "variables");

    if (argc == 2)
      strcpy(res_name, "ans");
    else
      strcpy(res_name, argv[2]);

    /* delete existing */
    size_t index;
    json_t *value;
    json_array_foreach(variables, index, value)
    {
      if (strcmp(json_string_value(json_object_get(value, "name")), res_name) == 0)
        json_array_remove(variables, index);
    }

    /* add new */
    json_t *res = json_object();
    json_object_set_new(res, "name", json_string(res_name));
    json_t *res_val = NULL;
    if (arg_len == 1)
      res_val = json_real(wlen[0]);
    else
    {
      res_val = json_array();
      for (size_t i = 0; i < arg_len; ++i)
        json_array_append_new(res_val, json_real(wlen[i]));
    }
    json_object_set_new(res, "value", res_val);
    json_array_append_new(variables, res);

    /* write workspace */
    json_dump_file(workspace, WORKSPACE, JSON_INDENT(2));
  }

  free(freq);
  free(wlen);
  json_decref(workspace);
  return 0;
}