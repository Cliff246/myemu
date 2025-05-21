#include "modules.h"
#include "myemu.h"

//very fucked on windows... don't use... probably won't even compile

p_module_t open_module(const char *path)
{
	p_module_t new_module = (p_module_t)calloc(1, sizeof(module_t));
	if(!new_module)
	{
		//TODO lol
		errno = ENOMEM;
		return NULL;
		
	}
	const int flags = RTLD_NOW; 

	void *handle = dlopen(path, flags);
	if(handle)
	{
		new_module->handle = handle;
		new_module->path = strdup(path);
		new_module->flags = flags;
		return new_module;
	}
	else
	{
		free(new_module);
		fprintf(stderr, "dlopen failed at %s\n", dlerror());
		errno = ENOENT; 
		return NULL;	
	}
}



void close_module(p_module_t module)
{	
	if(module == NULL)
		return;
	free(module->path);
	dlclose(module->handle);
}



p_devdesc_t get_device_from_module(p_module_t module)
{
	if(!module || !module->handle)
	{
		errno = EINVAL;
		return NULL;
	}


	DEVICE_EXPORT const p_devdesc_t (*get_desc)(void) = dlsym(module->handle, "get_device_descriptor");
	if(get_desc == NULL)
	{
		fprintf(stderr, "dlsym failed: %s\n", dlerror());
		errno = ENOSYS;
		return NULL;
	}
	else
	{
		return (p_devdesc_t)get_desc();
	}
}

