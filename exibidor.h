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
void imprime_magic_func(ClassFile* cf,FILE* arq);
void imprime_versions(ClassFile* cf,FILE* arq);
void imprime_pool_constantes(ClassFile* cf,FILE* arq);
void imprime_classdata(ClassFile* cf,FILE* arq);
// -----------------------------------------------
void imprime_interfaces(ClassFile* cf,FILE* arq);
void imprime_atributo(ClassFile* cf, attribute_info* att,FILE* arq);
void imprime_campos(ClassFile* cf,FILE* arq);
void imprime_metodos(ClassFile* cf,FILE* arq);
void imprime_atributos(ClassFile* cf,FILE* arq);
void print(ClassFile* cf,char* namefile,FILE* arq);
char* descobre_versao(int code);
#endif

