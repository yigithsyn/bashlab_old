#define HELP "-------------- Trapezoidal integration method ----------------\n" \
             "Usage: trapz <y> <=out>                                       \n" \
             "       trapz <x> <y> <=out>                                   \n" \
             "- x  : nodes                                                  \n" \
             "- y  : values                                                 \n" \
             "- out: output variable name [optional]                        \n"

#define DEBUG 1

#include "macros.h"

// #include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "jansson.h"

static struct stat stat_buff;
static json_error_t *json_error;
static json_t *workspace;
static bool valid_workspace = false;
static char res_name[20] = {'\0'};

#include "trapz.h"

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf(HELP);
    return 0;
  }

  /* routine variables */
  json_t *variable = NULL;
  json_t *result = NULL;
  size_t ivar_index;
  json_t *ivar;

  /* load workspace */
  workspace = json_load_file(WORKSPACE, 0, json_error);
  if (stat(WORKSPACE, &stat_buff) == 0)
    if (workspace != NULL && json_typeof(workspace) == JSON_OBJECT)
      valid_workspace = true;

  /* check for variable */

  json_array_foreach(json_object_get(workspace, "variables"), ivar_index, ivar)
  {
    if (strcmp(json_string_value(json_object_get(ivar, "name")), argv[1]) == 0)
    {
      if (DEBUG)
        printf("[DEBUG] Argument '%s' found in workspace.\n", argv[1]);
      variable = ivar;
      break;
    }
  }

  if (variable == NULL)
  {
    printf("Error: %s", BL_ERROR_VARIABLE_NOT_FOUND);
    return 0;
  }

  if (json_typeof(json_object_get(variable, "value")) != JSON_ARRAY)
  {
    printf("Error: %s", BL_ERROR_UNSUPPORTED_ARGUMENT);
    return 0;
  }

  /* operation */
  size_t N = json_array_size(json_object_get(variable, "value"));
  double *y = malloc(N * sizeof(double));
  if (DEBUG)
    printf("[DEBUG] Input array size '%zu'.\n", N);

  json_array_foreach(json_object_get(variable, "value"), ivar_index, ivar)
  {
    y[ivar_index] = json_real_value(ivar);
    if (DEBUG)
      printf("[DEBUG] y[%zu] = %f.\n", ivar_index, json_real_value(ivar));
  }

  result = json_real(trapz1(y, N));
  /* print out */
  if (argc == 2)
    printf("%f\n", json_real_value(result));

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
    json_object_set_new(res, "value", result);
    json_array_append_new(variables, res);

    /* write workspace */
    json_dump_file(workspace, WORKSPACE, JSON_INDENT(2));
  }

  return 0;
}