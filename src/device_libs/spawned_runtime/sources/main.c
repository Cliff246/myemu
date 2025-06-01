
#include "devices.h"
#include "modules_commons.h"

#include "spawned.h"

int main(int argc, char *argv[])
{
	if(argc == 2)
	{
		
		spwn_load_module(argv[1]);
	}	
	return 0;
}
