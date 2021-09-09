
#include "jansson.h"

int typeof(json_t *arg){
  return json_typeof(arg);
}