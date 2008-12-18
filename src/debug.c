#include <stdio.h>
#include <stdlib.h>

#include "debug.h"




void crashIf(int value, char* msg, char* param){
	if( value){
		if( param == NULL)
			fprintf(stderr, "crash: %s\n", msg);
		else
			fprintf(stderr, "crash: %s - %s\n", msg, param);

		exit(1);
	}
}
