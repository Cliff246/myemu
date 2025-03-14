#include "asm.h" 






int main(int argc, char *argv[])
{
    if(argc < 2)
        return 1;    
    
    char *directory = argv[1];
    char *bytes = NULL;
    int size = assemble(directory, &bytes);
    const char *default_path = "bytes";
    
    
    char *path = (argc == 3)? argv[2] : (char*)default_path;
    FILE *fptr = fopen(path, "w");
    printf("opened file %s\n", default_path); 
    for(int i = 0; i < size; ++i)
    {
        fputc(bytes[i],fptr);
        printf("put char %c\n", bytes[i]);
    }
    printf("closing\n");
    fclose(fptr);
    printf("end\n");
    return 1;
}
