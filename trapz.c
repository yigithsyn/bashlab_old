#define HELP "-------------- Trapezoidal integration method ----------------\n" \
             "Usage: trapz <y> <=out>                                       \n" \
             "       trapz <x> <y> <=out>                                   \n" \
             " x  : nodes                                                  \n"  \
             " y  : values                                                 \n"  \
             " out: output variable name [optional]                        \n"

#define DEBUG 1

#include "macros.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "jansson.h"

static struct stat stat_buff;
static bool valid_workspace = false;
static bool has_out_var = false;
static char out_name[20] = {'\0'};

#include "trapz.h"

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf(HELP);
    return 0;
  }

  /* routine variables */
  json_error_t *json_error = NULL;
  json_t *workspace;
  json_t *variables[BL_MAX_ARG_NUM] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  json_t *result = NULL;
  json_t *ivar;
  size_t ivar_index;

  /* load workspace */
  workspace = json_load_file(WORKSPACE, 0, json_error);
  if (stat(WORKSPACE, &stat_buff) == 0)
    if (workspace != NULL && json_typeof(workspace) == JSON_OBJECT)
      valid_workspace = true;

  /* check for variables */
  if (argc > 2 && argv[argc - 1][0] == '=')
    has_out_var = true;


  for (int i = 1; i < argc - has_out_var; ++i)
  {
    json_array_foreach(json_object_get(workspace, "variables"), ivar_index, ivar)
    {
      if (strcmp(json_string_value(json_object_get(ivar, "name")), argv[i]) == 0)
      {
        if (DEBUG)
          printf("[DEBUG] Argument '%s' found in workspace.\n", argv[i]);
        variables[i - 1] = ivar;
        break;
      }
    }
    if (variables[i - 1] == NULL)
    {
      printf("Error: %s: %s", BL_ERROR_VARIABLE_NOT_FOUND, argv[i]);
      return 0;
    }
    if (json_typeof(json_object_get(variables[i - 1], "value")) != JSON_ARRAY)
    {
      printf("Error: %s", BL_ERROR_UNSUPPORTED_ARGUMENT);
      return 0;
    }
  }

  /* operation */
  size_t N = json_array_size(json_object_get(variables[0], "value"));
  double *y = malloc(N * sizeof(double));
  if (DEBUG)
    printf("[DEBUG] Input array size '%zu'.\n", N);

  json_array_foreach(json_object_get(variables[0], "value"), ivar_index, ivar)
  {
    y[ivar_index] = json_real_value(ivar);
    if (DEBUG)
      printf("[DEBUG] y[%zu] = %f.\n", ivar_index, json_real_value(ivar));
  }

  if (argc - 1 - has_out_var == 1)
    result = json_real(trapz1(y, N));
  else
  {
    double *x = y;
    y = malloc(N * sizeof(double));
    json_array_foreach(json_object_get(variables[1], "value"), ivar_index, ivar)
    {
      y[ivar_index] = json_real_value(ivar);
      if (DEBUG)
        printf("[DEBUG] y[%zu] = %f.\n", ivar_index, json_real_value(ivar));
    }
    result = json_real(trapz2(x, y, N));
    free(x);
  }

  free(y);

  if (DEBUG)
    printf("[DEBUG] Result = %f.\n", json_real_value(result));

  /* print out */
  if (!has_out_var)
    printf("%f\n", json_real_value(result));

  /* add workspace */
  if (valid_workspace)
  {
    json_t *variables = json_object_get(workspace, "variables");

    if (!has_out_var)
      strcpy(out_name, "ans");
    else
      strcpy(out_name, argv[argc - 1] + 1);

    /* delete existing */
    size_t index;
    json_t *value;
    json_array_foreach(variables, index, value)
    {
      if (strcmp(json_string_value(json_object_get(value, "name")), out_name) == 0)
        json_array_remove(variables, index);
    }

    /* add new */
    json_t *res = json_object();
    json_object_set_new(res, "name", json_string(out_name));
    json_object_set_new(res, "value", result);
    json_array_append_new(variables, res);

    /* write workspace */
    json_dump_file(workspace, WORKSPACE, JSON_INDENT(2));
  }

  /* free memory */
  for (int i = 0; i < argc - 1 - has_out_var; ++i)
    json_decref(variables[i]);

  json_decref(workspace);
  json_decref(result);
  json_decref(ivar);

  return 0;
}