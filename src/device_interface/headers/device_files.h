#ifndef __DEVICE_FILES__
#define __DEVICE_FILES__


typedef enum 
{
    DEVFILE_FILE,
    DEVFILE_PIPE,
    DEVFILE_SOCKET,
	//in memory by mmap'd
	DEVFILE_MEM,      
	//pure in memory
    DEVFILE_VIRTUAL   
} devfile_type_t;

typedef struct device_file devfile_t, *p_devfile_t;
typedef struct device_file_system devfilesys_t, *p_devfilesys_t;


#endif
