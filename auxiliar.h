#include "leitor.h"

#ifndef AUXILIAR_H
#define AUXILIAR_H

void free_mem(ClassFile* cf);
int verificaTipo(char* tipo);
FILE* open_file(char *namefile);
void openFiles(int argc, char *argv[], char *filename_in, FILE **arq_in, FILE **arq_out);
uint8_t leU8(FILE* fd);
uint16_t leU16(FILE* fd);
uint32_t leU32(FILE* fd);
#endif
