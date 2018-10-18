#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "leitor.h"

#ifndef EXIBIDOR_H
#define EXIBIDOR_H

int bit_is_true(int code, int id);
void print_flags(int code,FILE* arq);
void print_func_magic(ClassFile* cf,FILE* arq);
void print_versions(ClassFile* cf,FILE* arq);
void print_constantpool(ClassFile* cf,FILE* arq);
void print_classdata(ClassFile* cf,FILE* arq);
void print_interfaces(ClassFile* cf,FILE* arq);
void print_atribute(ClassFile* cf, attribute_info* att,FILE* arq);
void print_fields(ClassFile* cf,FILE* arq);
void print_methodes(ClassFile* cf,FILE* arq);
void print_atributes(ClassFile* cf,FILE* arq);
void print(ClassFile* cf,char* namefile,FILE* arq);
char* look_version(int code);
