#include <errno.h>
#include "leitor.h"
#include "auxiliar.h"
#define IN 1
#define OUT 2

uint8_t leU8(FILE* fd)
{
    return getc(fd);
}

uint16_t leU16(FILE* fd)
{
    uint16_t retorno = getc(fd);
    retorno = (retorno << 8)|(getc(fd));
    return retorno;
}

uint32_t leU32(FILE* fd)
{
    uint32_t retorno = leU16(fd) << 16;
	retorno |= leU16(fd);
    return retorno;
}

/** \brief Retorna tipo da string
 *
 * \param
 * \param
 * \return Constante correspondente
 *
 */
int verificaTipo(char* tipo)
{
    if(!strcmp(tipo, "Code"))
    {
        return CODE;
    }
    else if(!strcmp(tipo, "ConstantValue"))
    {
        return CONSTANTVALUE;
    }
    else if(!strcmp(tipo, "InnerClasses"))
    {
        return INNERCLASSES;
    }
    else if(!strcmp(tipo, "Exceptions"))
    {
        return EXCEPTIONS;
    }
    else
    {
        return OTHER;
    }
}

/** \brief Desaloca pool de constantes
 *
 * \param classfile
 * \return void.
 *
 */
void liberaConstantPool(ClassFile *classfile)
{
    cp_info *it;
    cp_info *first_cp = classfile->constant_pool;
    cp_info *last_cp = first_cp + classfile->constant_pool_count-1;

    for(it = first_cp; it < last_cp; it++)
    {
        if(it->tag == UTF8)
        {
            free(it->info.Utf8_info.bytes);
        }
    }
    free(classfile->constant_pool);
}

/** \brief Free na memória para terminar programa
 *
 * \param classfile
 * \return void
 *
 */
void liberaMemoria(ClassFile* classfile)
{
    if(classfile)
    {
        liberaConstantPool(classfile);
        if(classfile->fields)
            free(classfile->fields);
        if(classfile->interfaces)
            free(classfile->interfaces);
        if(classfile->methods)
            free(classfile->methods);
        free(classfile);
    }
}

/** \brief Abre o arquivo e faz checagem
 *
 * \param *filename = nome do arquivo a ser aberto.
 * \param mode = modo de abertura do arquivo.
 * \return arquivo aberto ou NULL.
 *
 */
FILE* abreArquivo(char *filename, int mode)
{
    FILE* arq = fopen(filename, (mode==IN?"rb":"w+"));

    if(!arq)
    {
        printf("Erro: Arquivo de %s nao encontrado.\n", (mode==IN?"entrada":"saida"));
    }
    return arq;
}

/** \brief Abre arquivos de entrada e saida.
 *
 * \param argc = numero de argumentos da main.
 * \param *argv = argumentos da main.
 * \param *filename_in = nome do arquivo de entrada.
 * \param **file_in = arquivo de entrada.
 * \param **file_out = arquivo de saida.
 * \return void
 *
 */
void abreArquivos(char *filename_in, FILE **file_in, FILE **file_out, int argc, char *argv[])
{
    char filename_out[1024];

    switch(argc)
    {
    case(1):
        do
        {
            printf("Digite o nome do arquivo de entrada: ");
            scanf("%s", filename_in);
            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", filename_out);
            fflush(stdin);
            (*file_in) = abreArquivo(filename_in, IN);
        }
        while(!(*file_in));
        break;
    case(3):
        strcpy(filename_in, argv[1]);
        strcpy(filename_out, argv[2]);
        (*file_in) = abreArquivo(filename_in, IN);
        while(!(*file_in))
        {
            printf("Digite o nome do arquivo de entrada: ");
            scanf("%s", filename_in);
            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", filename_out);
            fflush(stdin);
            (*file_in) = abreArquivo(filename_in, IN);
        }
        break;
    default:
        printf("Parametros invalidos!\n");
        printf("Sintaxe Linux: ./LeitorExibidor [nome do class] [nome da saida]\n");
        printf("Sintaxe Windows: LeitorExibidor.exe [nome do class] [nome da saida]\n");
        exit(0);
        break;
    }
    if(!(*file_out = abreArquivo(filename_out, OUT)))
    {
        exit(0);
    }

    return;
}
