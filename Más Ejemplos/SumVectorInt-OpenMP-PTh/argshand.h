/******************************************************************************
			argshand.h  - description
			----------------------
	begin		: Jan 1997
	copywirght	: (C) Obtained from V.Ruiz
	
     ********************************************************************	
     Purpose: Rutines to get comand line parameters
     
     Routines:
     	GetArg(): Look for a string in the arguments. If it is found, return
			the next string in the arguments. 
			Otherwise return NULL.  
	ExistArg(): return 1 if the string to find is in the arguments.
	

*******************************************************************************/
#ifndef __ARGSHAND__
#define __ARGSHAND__

/*----------------------------------------------------------------------------*/
char *GetArg(char const *str_to_find, int narg, char **arg);
int ExistArg(char const *str_to_find, int narg, char **arg);

#endif /*__ARGSHAND__*/
/*----------------------------------------------------------------------------*//*----------------------------------------------------------------------------*/
