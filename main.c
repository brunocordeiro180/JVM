#include "leitor.h"
#include "exibidor.h"
#include "auxiliar.h"

int main(int argc, char* argv[])
{
    char filename_in[1024];
    FILE *fd = NULL;
    FILE *fo = NULL;
    ClassFile *classfile = NULL;

    openFiles(argc, argv, filename_in, &fd, &fo);
    
    load(fd, &classfile);
    
    print(classfile, filename_in, fo);

    free_mem(classfile);
    
    fclose(fo);
    fclose(fd);

    return 0;
}
