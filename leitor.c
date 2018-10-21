#include "leitor.h"
#include "auxiliar.h"

/*Identifica os primeiros bytes do arquivo, valor esperado é CAFEBABE*/
void carregaAssinatura(ClassFile* cf, FILE* fd)
{
    cf->magic = leU32(fd);
} 

/*Carrega as versões do JAVA que foram utilizadas para gerar o arquivo .class*/
void leVersao(ClassFile* cf, FILE* fd)
{
    cf->minor_version = leU16(fd);
    cf->major_version = leU16(fd);
}

/*Leitura da constant pool*/
void carrega_constant_pool(ClassFile* cf, FILE* fd)
{
    cf->constant_pool_count = leU16(fd);
    
    if (cf->constant_pool_count <= 1)
    {
        cf->constant_pool = NULL;
        return;
    }
    
    cf->constant_pool = (cp_info*) calloc((cf->constant_pool_count - 1), sizeof(cp_info));
    cp_info *cp;
    
    for (cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; ++cp)
    {
        cp->tag = leU8(fd);
        
        if (cp->tag == CLASS){
            cp->info.Class_info.name_index = leU16(fd);
        }
        else if (cp->tag == FIELDREF){
            cp->info.Fieldref_info.class_index = leU16(fd);
            cp->info.Fieldref_info.name_and_type_index = leU16(fd);
        }
        else if (cp->tag == METHOD){
            cp->info.Method_info.class_index = leU16(fd);
            cp->info.Method_info.name_and_type_index = leU16(fd);
        }
        else if (cp->tag == INTERFACE){
            cp->info.Interface_info.class_index = leU16(fd);
            cp->info.Interface_info.name_and_type_index = leU16(fd);
        }
        else if (cp->tag == NAMEANDTYPE){
            cp->info.NameAndType_info.name_index = leU16(fd);
            cp->info.NameAndType_info.descriptor_index = leU16(fd);
        }
        else if (cp->tag == UTF8){
            cp->info.Utf8_info.length = leU16(fd);
            cp->info.Utf8_info.bytes = (uint8_t*)calloc(cp->info.Utf8_info.length+1, sizeof(uint8_t));
            uint8_t* b;
            for (b = cp->info.Utf8_info.bytes ; b < cp->info.Utf8_info.bytes + cp->info.Utf8_info.length ; ++b)
            {
                *b = leU8(fd);
            }
        }
        else if (cp->tag == STRING){
            cp->info.String_info.string_index = leU16(fd);
        }
        else if (cp->tag == INTEGER){
            cp->info.Integer_info.bytes = leU32(fd);
        }
        else if (cp->tag == FLOAT){
            cp->info.Float_info.bytes = leU32(fd);
        }
        else if (cp->tag == LONG){
            cp->info.Long_info.high_bytes = leU32(fd);
            cp->info.Long_info.low_bytes = leU32(fd);
        }
        else if (cp->tag == DOUBLE){
            cp->info.Double_info.high_bytes = leU32(fd);
            cp->info.Double_info.low_bytes = leU32(fd);
        }
        
        if (cp->tag == LONG || cp->tag == DOUBLE)
        {
            ++cp;
            cp->tag = -1;
        }
    }
}

void carrega_class(ClassFile* cf, FILE* fd)
{
    cf->access_flags = leU16(fd);
    cf->this_class = leU16(fd);
    cf->super_class = leU16(fd);
}

void carrega_interfaces(ClassFile* cf, FILE* fd)
{
    cf->interfaces_count = leU16(fd);
    
    if (!cf->interfaces_count)
    {
        cf->interfaces = NULL;
        return;
    }
    
    cf->interfaces = (uint16_t*) calloc(cf->interfaces_count, sizeof(uint16_t));
    uint16_t* bytes;
    
    for (bytes = cf->interfaces; bytes < cf->interfaces + cf->interfaces_count; ++bytes)
    {
        *bytes = leU16(fd);
    }
}

/*Carrega o indice da constante que deve ser de acordo com a tabela:
https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.7.2-300-C.1*/
void carrega_attribute_constant_value(attribute_info* att, FILE* fd)
{
    att->type.ConstantValue.constantvalue_index = leU16(fd);
}

/*Carrega atributo code, que possui três tabelas de acordo com a tabela:
https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.7.2-300-C.1*/
void carrega_attribute_code(attribute_info* att, ClassFile* cf, FILE* fd)
{
    att->type.Code_attribute.max_stack = leU16(fd);
    att->type.Code_attribute.max_locals = leU16(fd);
    att->type.Code_attribute.code_length = leU32(fd);
    if (att->type.Code_attribute.code_length == 0)
    {
        att->type.Code_attribute.code = NULL;
    }
    else
    {
        att->type.Code_attribute.code = (uint8_t*)calloc(att->type.Code_attribute.code_length, sizeof(uint8_t));
        uint8_t* byte;
        for (byte = att->type.Code_attribute.code; byte < att->type.Code_attribute.code + att->type.Code_attribute.code_length; ++byte)
        {
            *byte = leU8(fd);
        }
    }
    att->type.Code_attribute.exception_table_length = leU16(fd);
    if (att->type.Code_attribute.exception_table_length == 0)
    {
        att->type.Code_attribute.exception_table = NULL;
    }
    else
    {
        att->type.Code_attribute.exception_table = (exception_table_info*)calloc(att->type.Code_attribute.exception_table_length,sizeof(exception_table_info));
        exception_table_info* exp_aux;
        for (exp_aux = att->type.Code_attribute.exception_table; exp_aux < att->type.Code_attribute.exception_table + att->type.Code_attribute.exception_table_length; ++exp_aux)
        {
            exp_aux->start_pc = leU16(fd);
            exp_aux->end_pc = leU16(fd);
            exp_aux->handler_pc = leU16(fd);
            exp_aux->catch_type = leU16(fd);
        }
    }
    att->type.Code_attribute.attributes_count = leU16(fd);
    if (att->type.Code_attribute.attributes_count == 0)
    {
        att->type.Code_attribute.attributes = NULL;
    }
    else
    {
        att->type.Code_attribute.attributes = (attribute_info*)calloc(att->type.Code_attribute.attributes_count,sizeof(attribute_info));
        attribute_info* att_aux;
        for (att_aux = att->type.Code_attribute.attributes; att_aux < att->type.Code_attribute.attributes + att->type.Code_attribute.attributes_count; ++att_aux)
        {
            carregaAtributoEspecif(att_aux, cf, fd);
        }
    }
}

/*Carrega os atributos exception*/
/*Usada pela tabela attributes de method_info*/
void carrega_attribute_exception(attribute_info* att, FILE* fd)
{
    att->type.Exceptions.number_of_exceptions = leU16(fd);
    if (att->type.Exceptions.number_of_exceptions == 0)
    {
        att->type.Exceptions.exception_index_table = NULL;
    }
    else
    {
        att->type.Exceptions.exception_index_table = (uint16_t*) calloc(att->type.Exceptions.number_of_exceptions,sizeof(uint16_t));
        uint16_t* bytes;
        for (bytes = att->type.Exceptions.exception_index_table; bytes < att->type.Exceptions.exception_index_table + att->type.Exceptions.number_of_exceptions; ++bytes)
        {
            *bytes = leU16(fd);
        }
    }
}

/*Carrega o atributo InnerClasses*/
/*Usada pela tabela de atributos de ClassFile*/
void carregaAtributoInnerClasses(attribute_info* att, FILE* fd)
{
    att->type.InnerClasses.number_of_classes = leU16(fd);
    if (att->type.InnerClasses.number_of_classes == 0)
    {
        att->type.InnerClasses.classes = NULL;
        return;
    }
    att->type.InnerClasses.classes = (classtype_info*) calloc(att->type.InnerClasses.number_of_classes,sizeof(classtype_info));
    classtype_info* classtype_aux;
    for (classtype_aux = att->type.InnerClasses.classes; classtype_aux < att->type.InnerClasses.classes + att->type.InnerClasses.number_of_classes; ++classtype_aux)
    {
        classtype_aux->inner_class_info_index = leU16(fd);
        classtype_aux->outer_class_info_index = leU16(fd);
        classtype_aux->inner_name_index = leU16(fd);
        classtype_aux->inner_class_access_flags = leU16(fd);
    }
}

void carregaAtributo(attribute_info* att, FILE* fd)
{
    if (!att->attribute_length)
    {
        att->type.Other.bytes = NULL;
        return;
    }
    att->type.Other.bytes = (uint8_t*) calloc(att->attribute_length,sizeof(uint8_t));
    uint8_t* bytes;
    for (bytes = att->type.Other.bytes; bytes < att->type.Other.bytes + att->attribute_length; ++bytes)
    {
        *bytes = leU8(fd);
    }
}
/*Determina qual o tipo de attribute a ser lido e chama a função apropriada*/
/*Pode ser chamado para ler atributo do ClassFile, Method_info ou Field_info*/
void carregaAtributoEspecif(attribute_info* att, ClassFile* cf, FILE* fd)
{
    char* type;
    att->attribute_name_index = leU16(fd);
    att->attribute_length = leU32(fd);
    type = (char*)calloc(cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.length+1,sizeof(char));
    strcpy(type, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    int typeInt = verificaTipo(type);
    switch (typeInt)
    {
    case CONSTANTVALUE:
        carrega_attribute_constant_value(att, fd);
        break;
    case CODE:
        carrega_attribute_code(att, cf, fd);
        break;
    case EXCEPTIONS:
        carrega_attribute_exception(att, fd);
        break;
    case INNERCLASSES:
        carregaAtributoInnerClasses(att, fd);
        break;
    case OTHER:
        carregaAtributo(att, fd);
        break;
    }
    free(type);
}

void carregaCampos(ClassFile* cf, FILE* fd)
{
// carrega os fields. Dois campos na mesma classe não podem ter o mesmo nome.

    cf->fields_count = leU16(fd);

    if (cf->fields_count == 0)
    {
        cf->fields = NULL;
        return;
    }
    cf->fields = (field_info*)calloc(cf->fields_count,sizeof(field_info));
    field_info* aux_field;
    for (aux_field = cf->fields; aux_field < cf->fields + cf->fields_count; ++aux_field)
    {
        aux_field->access_flags = leU16(fd);
        aux_field->name_index = leU16(fd);
        aux_field->descriptor_index = leU16(fd);
        aux_field->attributes_count = leU16(fd);
        aux_field->attributes = calloc(aux_field->attributes_count,sizeof(attribute_info));
        attribute_info* aux_attribute;
        for (aux_attribute = aux_field->attributes; aux_attribute < aux_field->attributes + aux_field->attributes_count; ++aux_attribute)
        {
            carregaAtributoEspecif(aux_attribute, cf, fd);
        }
    }
}

void carregaMetodos(ClassFile* cf, FILE* fd)
{
    // carrega os métodos
    cf->method_count = leU16(fd);
    if (cf->method_count == 0)
    {
        cf->methods = NULL;
        return;
    }
    cf->methods = (method_info*) calloc(cf->method_count, sizeof(method_info));
    method_info* aux_method;
    for (aux_method = cf->methods; aux_method < cf->methods + cf->method_count; ++aux_method)
    {
        aux_method->access_flags = leU16(fd);
        aux_method->name_index = leU16(fd);
        aux_method->descriptor_index = leU16(fd);
        aux_method->attributes_count = leU16(fd);
        aux_method->attributes = (attribute_info*) calloc(aux_method->attributes_count, sizeof(attribute_info));
        attribute_info* att_aux;
        for (att_aux = aux_method->attributes; att_aux < aux_method->attributes + aux_method->attributes_count; ++att_aux)
        {
            carregaAtributoEspecif(att_aux, cf, fd);
        }
    }
}

void carregaAtributos(ClassFile* cf, FILE* fd)
{
    cf->attributes_count = leU16(fd);
    if (cf->attributes_count == 0)
    {
        cf->attributes = NULL;
        return;
    }
    cf->attributes = (attribute_info*) calloc(cf->attributes_count, sizeof(attribute_info));
    attribute_info* att_aux;
    for (att_aux = cf->attributes; att_aux < cf->attributes + cf->attributes_count; ++att_aux)
    {
        carregaAtributoEspecif(att_aux, cf, fd);
    }
}



