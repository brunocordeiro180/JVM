#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"

#ifndef EXIBIDOR_H
#define EXIBIDOR_H

// Otto ------------------------------------------
int verifica_bit(int code, int id);
void imprime_flags(int code,FILE* arq);
void print_func_magic(ClassFile* cf,FILE* arq);
void imprime_versions(ClassFile* cf,FILE* arq);
void imprime_pool_constantes(ClassFile* cf,FILE* arq);
void imprime_classdata(ClassFile* cf,FILE* arq);
// -----------------------------------------------
void print_interfaces(ClassFile* cf,FILE* arq);
void print_atribute(ClassFile* cf, attribute_info* att,FILE* arq);
void print_fields(ClassFile* cf,FILE* arq);
void print_methodes(ClassFile* cf,FILE* arq);
void print_atributes(ClassFile* cf,FILE* arq);
void print(ClassFile* cf,char* namefile,FILE* arq);
char* look_version(int code);
#endif
