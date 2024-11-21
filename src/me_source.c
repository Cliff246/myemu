#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "me_myemu.h"
#include "commons.h"
#include "me_constants.h"
#include "me_test.h"


void readdir(char *dir)
{

}

void readfile(char *str)
{
    const char* dir = str + 2;
    char *data = NULL;
    int size = assemble(dir, &data);
    emulate(data, size, CYCLES);
}

void outfile(char *out)
{

}

void testfile(char *str)
{
    const char *dir = "assembly_example/multiply.txt";
    char *data = NULL;
    int size =assemble(dir, &data);
    emulate(data, size, CYCLES);
}


typedef void (*argument_handler_t)(char *);

const argument_handler_t handler[3] = {
    readfile, outfile, testfile       
};

const char *arguments[] = {
    "-R",
    "-O",
    "-T",
};

int runtime(int argc, char* argv[])
{
    if (argc == 0 || argc == 1)
    {
        printf("NO arguments provided\n");
    }
    else
    {

        const int numargs = argc;
        for (int i = 1; i < numargs; i++)
        {
            const int strsize = strlen(argv[i]);
            if (strsize < 2)
            {
                printf("argument not understood %s\n", argv[i]);
            }
            char firstbytes[3] = {argv[i][0], argv[i][1], 0};

            for (int options = 0; options < sizeof(arguments) / sizeof(arguments[0]); options++)
            {

                if (cmpstrings(firstbytes, arguments[options]))
                {
                    handler[options](argv[i]);
                }
            }
        }
    }
}


int main(int argc, char *argv[])
{
    
    //test1();
    //test_asm();
    runtime(argc, argv);
           
}
