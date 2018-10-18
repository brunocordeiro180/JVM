#include "leitor.h"
#include "exibidor.h"
#include "auxiliar.h"

int main(int argc, char* argv[])
{
    char filename_in[1024];
    FILE *file_in = NULL;
    FILE *file_out = NULL;
    ClassFile *classfile = NULL;

    openFiles(argc, argv, filename_in, &file_in, &file_out);
    load(file_in, &classfile);
    print(classfile, filename_in, file_out);

    free_mem(classfile);
    fclose(file_out);
    fclose(file_in);
    return 0;
}
