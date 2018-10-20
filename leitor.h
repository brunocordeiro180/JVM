#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>

#ifndef LEITOR_H
#define LEITOR_H

#define CONTINUED 0
#define CODE 1
#define UTF8 1
#define EXCEPTIONS 2
#define INNERCLASSES 3
#define INTEGER 3
#define FLOAT 4
#define OTHER 4
#define CONSTANTVALUE 5
#define LONG 5
#define DOUBLE 6
#define CLASS 7
#define STRING 8
#define FIELDREF 9
#define METHOD 10
#define INTERFACE 11
#define NAMEANDTYPE 12

typedef struct
{
    uint16_t  inner_class_info_index;
    uint16_t  outer_class_info_index;
    uint16_t  inner_name_index;
    uint16_t  inner_class_access_flags;
} classtype_info;

typedef struct
{
    uint16_t  start_pc;
    uint16_t  end_pc;
    uint16_t  handler_pc;
    uint16_t  catch_type;
} exception_table_info;

union
{
    uint32_t U4;
    float Float;
} u4tofloat;

union
{
    long Long;
    double Double;
} longtodouble;

typedef struct _attribute_info
{
    uint16_t  attribute_name_index;
    uint32_t  attribute_length;
    union
    {
        struct
        {
            uint16_t constantvalue_index;
        } ConstantValue;
        struct
        {
            uint16_t                      max_stack;
            uint16_t                      max_locals;
            uint32_t                      code_length;
            uint8_t*                     code;
            uint16_t                      exception_table_length;
            exception_table_info*   exception_table;
            uint16_t  attributes_count;
            struct _attribute_info* attributes;
        } Code_attribute;
        struct
        {
            uint16_t  number_of_exceptions;
            uint16_t* exception_index_table;
        } Exceptions;
        struct
        {
            uint16_t              number_of_classes;
            classtype_info* classes;
        } InnerClasses;
        struct
        {
            uint8_t* bytes;
        } Other;
    } type;
} attribute_info;

typedef struct
{
    uint16_t              access_flags;
    uint16_t              name_index;
    uint16_t              descriptor_index;
    uint16_t              attributes_count;
    attribute_info* attributes;
}   field_info, method_info;

typedef struct
{
    uint8_t  tag;
    union
    {
        struct
        {
            uint16_t  name_index;
        } Class_info;
        struct
        {
            uint16_t  class_index;
            uint16_t  name_and_type_index;
        } Fieldref_info, Method_info,   Interface_info;
        struct
        {
            uint16_t  name_index;
            uint16_t  descriptor_index;
        } NameAndType_info;
        struct
        {
            uint16_t  length;
            uint8_t* bytes;
        } Utf8_info;
        struct
        {
            uint16_t  string_index;
        } String_info;
        struct
        {
            uint32_t  bytes;
        } Integer_info;
        struct
        {
            uint32_t  bytes;
        } Float_info;
        struct
        {
            uint32_t  high_bytes;
            uint32_t  low_bytes;
        } Long_info, Double_info;
    } info;
} cp_info;

typedef struct
{
    uint32_t              magic;
    uint16_t              minor_version;
    uint16_t              major_version;
    uint16_t              constant_pool_count;
    cp_info*        constant_pool;
    uint16_t              access_flags;
    uint16_t              this_class;
    uint16_t              super_class;
    uint16_t              interfaces_count;
    uint16_t*             interfaces;
    uint16_t              fields_count;
    field_info*     fields;
    uint16_t              method_count;
    method_info*    methods;
    uint16_t              attributes_count;
    attribute_info* attributes;
} ClassFile;

void carrega_versoes(ClassFile* classfile,FILE* fd);
void carrega_constant_pool(ClassFile* classfile,FILE* fd);
void carregaMetodos(ClassFile* classfile,FILE* fd);
void carregaAtributoEspecif(attribute_info* att,ClassFile* classfile,FILE* fd);
void carrega_class(ClassFile* classfile,FILE* fd);
void carrega_interfaces(ClassFile* classfile,FILE* fd);
void carregaAtributo(attribute_info* att, FILE* fd);
void carregaCampos(ClassFile* classfile,FILE* fd);
void carregaAtributos(ClassFile* classfile, FILE* fd);
void carrega_attribute_constant_value(attribute_info* att, FILE* fd);
void carregaAtributoInnerClasses(attribute_info* att,FILE* fd);
void carrega_attribute_code(attribute_info* att, ClassFile* classfile,FILE* fd);
void carrega_attribute_exception(attribute_info* att,FILE* fd);
void load(FILE* fd, ClassFile** classfile);

#endif
