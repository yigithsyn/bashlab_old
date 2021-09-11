#define HELP "-------------------- Type of variable ------------------------\n" \
             "Usage: typeof <var_name>                                      \n" \
             "var_name: variable name                                         "

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

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    printf(HELP);
    return 0;
  }

  /* routine variables */
  json_t *variable = NULL;
  char result[20] = {'\0'};

  /* load workspace */
  workspace = json_load_file(WORKSPACE, 0, json_error);
  if (stat(WORKSPACE, &stat_buff) == 0)
  {
    if (workspace != NULL && json_typeof(workspace) == JSON_OBJECT)
      valid_workspace = true;
    else
    {
      printf("Error: %s", BL_ERROR_NO_WORKSPACE);
      return 0;
    }
  }

  /* check for variable */
  size_t var_index;
  json_t *var;
  json_array_foreach(json_object_get(workspace, "variables"), var_index, var)
  {
    if (strcmp(json_string_value(json_object_get(var, "name")), argv[1]) == 0)
    {
      if (DEBUG)
        printf("[DEBUG] Argument '%s' found in workspace.\n", argv[1]);
      variable = var;
      break;
    }
  }

  if (variable == NULL)
  {
    printf("Error: %s", BL_ERROR_VARIABLE_NOT_FOUND);
    return 0;
  }

  /* operation */
  if (json_typeof(json_object_get(variable, "value")) == JSON_OBJECT)
    strcpy(result, "JSON_OBJECT");
  else if (json_typeof(json_object_get(variable, "value")) == JSON_ARRAY)
    strcpy(result, "JSON_ARRAY");
  else if (json_typeof(json_object_get(variable, "value")) == JSON_STRING)
    strcpy(result, "JSON_STRING");
  else if (json_typeof(json_object_get(variable, "value")) == JSON_INTEGER)
    strcpy(result, "JSON_INTEGER");
  else if (json_typeof(json_object_get(variable, "value")) == JSON_REAL)
    strcpy(result, "JSON_REAL");
  else
  {
    printf("Error: %s", BL_ERROR_UNDEFINED_VARIABLE);
    return 0;
  }

  /* print out */
  if (argc == 2)
    printf("%s\n", result);

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
    json_object_set_new(res, "value", json_string(result));
    json_array_append_new(variables, res);

    /* write workspace */
    json_dump_file(workspace, WORKSPACE, JSON_INDENT(2));
  }
}