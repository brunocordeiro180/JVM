#include "leitor.h"

#ifndef AUXILIAR_H
#define AUXILIAR_H

uint8_t leU8(FILE* fd);

uint16_t leU16(FILE* fd);

uint32_t leU32(FILE* fd);

int verificaTipo(char* tipo);

void liberaConstantPool(ClassFile *classfile);

void liberaMemoria(ClassFile* cf);

FILE* abreArquivo(char * filename, int mode);

void abreArquivos(char *filename_in, FILE **arq_in, FILE **arq_out, int argc, char *argv[]);

#endif
