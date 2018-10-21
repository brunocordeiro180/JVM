#include "leitor.h"
#include "exibidor.h"
#include "auxiliar.h"

/* Executa leitura e processamento do arquivo .class */
void processaClass(FILE* fd, ClassFile** classfile)
{
    *classfile = (ClassFile*) calloc(1,sizeof(ClassFile));

    carregaAssinatura(*classfile, fd);
    leVersao(*classfile, fd);
    carrega_constant_pool(*classfile, fd);
    carrega_class(*classfile, fd);
    carrega_interfaces(*classfile, fd);
    carregaCampos(*classfile, fd);
    carregaMetodos(*classfile, fd);
    carregaAtributos(*classfile, fd);
}

/* Funcao principal */
int main(int argc, char* argv[])
{
    ClassFile *classfile = NULL;
    char file_in_name[1024];

    FILE *file_in = NULL;
    FILE *file_out = NULL;
    
	abreArquivos(file_in_name, &file_in, &file_out, argc, argv);
    processaClass(file_in, &classfile);
    
    print(classfile, file_in_name, file_out);
    liberaMemoria(classfile); 

    fclose(file_out);
    fclose(file_in);

    return 0;
}
