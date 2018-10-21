#include "leitor.h"
#include "exibidor.h"
#include "auxiliar.h"

int main(int argc, char* argv[])
{
    char file_in_name[1024];
    FILE *file_in = NULL;
    FILE *file_out = NULL;
    ClassFile *classfile = NULL;

	abreArquivos(file_in_name, &file_in, &file_out, argc, argv);
    
    load(file_in, &classfile);
    
    print(classfile, file_in_name, file_out);

    liberaMemoria(classfile); 
    fclose(file_out);
    fclose(file_in);

    return 0;
}
