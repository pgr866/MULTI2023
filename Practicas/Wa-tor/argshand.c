#include <stdio.h>
#include <string.h>
#include "argshand.h"

/*----------------------------------------------------------------------------*/
char *GetArg(char const *str_to_find, int narg, char **arg) {
    int i;
    for(i=0;i<narg;i++)                
        if (    strlen(arg[i]) == strlen(str_to_find)
             && strstr(arg[i],str_to_find)
           ) 
           return arg[i+1];          
    return NULL;           
}


/*----------------------------------------------------------------------------*/
/* Return 1 if the string 'str_to_find' is in the command line 'arg'          */
/*----------------------------------------------------------------------------*/
int ExistArg(char const *str_to_find, int narg, char **arg) {
  int i;
  for(i=0;i<narg;i++)
    if (    strlen(arg[i]) == strlen(str_to_find) 
        && strstr(arg[i],str_to_find)
       )
       return 1;
  return 0;
}

/*----------------------------------------------------------------------------*//*----------------------------------------------------------------------------*/
