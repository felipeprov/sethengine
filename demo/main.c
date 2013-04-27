
#include <stdlib.h>
#include <stdio.h>

#include "environment/environment.h"

int main(int argc, char* argv[])
{
	printf("SethEngineC starting\n");

	EnvironmentInit();
	EnvironmentNewWindow("Demo", 600, 800);
	 

	while(EnvironmentGetKey('Q') == 0)
	{
		EnvironmentRenderBegin();
		EnvironmentRenderEnd();
		EnvironmentUpdate();
	}

	return 0;
}