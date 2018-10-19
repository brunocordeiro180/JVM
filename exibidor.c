#include "exibidor.h"
#include "auxiliar.h"

/*Verifica se o bit na posição id é 1*/
int verifica_bit(int code, int id)
{
    //Realiza o shift a esquerda do bit 0..01 em 'id' posições
    return code & (1 << id);
}
/*De acordo com tabela em https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.1-200-E.1*/
void imprime_flags(int code, FILE* arq)
{

    fprintf(arq, "[");

    if (verifica_bit(code, 0))
        fprintf(arq, "public ");
    else if (verifica_bit(code, 1))
        fprintf(arq, "private ");
    else if (verifica_bit(code, 2))
        fprintf(arq, "protected ");
    if (verifica_bit(code, 3))
        fprintf(arq, "static ");
    if (verifica_bit(code, 4))
        fprintf(arq, "final ");
    if (verifica_bit(code, 5))
        fprintf(arq, "super ");
    if (verifica_bit(code, 6))
        fprintf(arq, "volatile ");
    if (verifica_bit(code, 7))
        fprintf(arq, "transient ");
    if (verifica_bit(code, 8))
        fprintf(arq, "native ");
    if (verifica_bit(code, 9))
        fprintf(arq, "interface ");
    if (verifica_bit(code, 10))
        fprintf(arq, "abstract ");

    fprintf(arq, "]");
}

/*Print do CAFEBABE em hexa*/
void imprime_magic_func(ClassFile* cf, FILE* arq)
{
    fprintf(arq, "-- MAGIC: %x\n", cf->magic);
}

/*Imprime os valores lidos pela função load_versions em "leitor"*/
void imprime_versions(ClassFile* cf, FILE* arq)
{
    fprintf(arq, "-- MINOR VERSION: %d\n", cf->minor_version);
    char *version_jdk =  look_version(cf->major_version);
    fprintf(arq, "MAJOR VERSION: %d - %s\n", cf->major_version, version_jdk);
    free(version_jdk);

    fprintf(arq, "-- CONSTANT POOL COUNT: %d\n", cf->constant_pool_count);

    fprintf(arq, "-- ACCESS_FLAGS: %x ", cf->access_flags);
    imprime_flags(cf->access_flags, arq);
    fprintf(arq, "\n");
    fprintf(arq, "-- THIS_CLASS: %d\n", cf->this_class);
    fprintf(arq, "-- SUPER_CLASS: %d\n", cf->super_class);

    fprintf(arq, "-- INTERFACES_COUNT: %d\n", cf->interfaces_count);
    fprintf(arq, "-- FIELDS_COUNT: %d\n", cf->fields_count);
    fprintf(arq, "-- METHODS_COUNT: %d\n", cf->method_count);
    fprintf(arq, "-- ATTRIBUTES_COUNT: %d\n\n", cf->attributes_count);

}

/*Imprime os valores referente a constant pool, as tags definidas na tabela
foram definidos no exibidor.h */
void imprime_pool_constantes(ClassFile* cf, FILE* arq)
{
    int i = 1;
    long long Long;
    fprintf(arq, "- CONSTANT POOL COUNT: %d\n", cf->constant_pool_count);
    fprintf(arq, "- CONSTANT_POOL:\n");
    cp_info* cp;
    for (cp = cf->constant_pool; cp < cf->constant_pool + cf->constant_pool_count - 1; ++cp)
    {
        fprintf(arq, "[%d]\n", i++);
        switch (cp->tag)
        {
        case CLASS:
            fprintf(arq, " ---CP_INFO: CLASS\n");
            fprintf(arq, " ---NAME_INDEX: %d: %s\n", cp->info.Class_info.name_index, (char*)cf->constant_pool[cp->info.Class_info.name_index - 1].info.Utf8_info.bytes);
            break;
        case FIELDREF:
            fprintf(arq, " ---CP_INFO: FIELDREF\n");
            fprintf(arq, " ---CLASS_INDEX: %d: %s\n", cp->info.Fieldref_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(arq, " ---NAMEANDTYPE_INDEX: %d: %s%s\n", cp->info.Fieldref_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case METHOD:
            fprintf(arq, " ---CP_INFO: METHOD\n");
            fprintf(arq, " ---CLASS_INDEX: %d: %s\n", cp->info.Method_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(arq, " ---NAMEANDTYPE_INDEX: %d: %s%s\n", cp->info.Method_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case INTERFACE:
            fprintf(arq, " ---CP_INFO: INTERFACE\n");
            fprintf(arq, " ---CLASS_INDEX: %d: %s\n", cp->info.Interface_info.class_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(arq, " ---NAMEANDTYPE_INDEX: %d: %s%s\n", cp->info.Interface_info.name_and_type_index, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes, (char*)cf->constant_pool[cf->constant_pool[cp->info.Interface_info.name_and_type_index - 1].info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case NAMEANDTYPE:
            fprintf(arq, " ---CP_INFO: NAMEANDTYPE\n");
            fprintf(arq, " ---NAME_INDEX: %d: %s\n", cp->info.NameAndType_info.name_index, (char*)cf->constant_pool[cp->info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
            fprintf(arq, " ---DESCRIPTOR_INDEX: %d: %s\n", cp->info.NameAndType_info.descriptor_index, (char*)cf->constant_pool[cp->info.NameAndType_info.descriptor_index - 1].info.Utf8_info.bytes);
            break;
        case UTF8:
            fprintf(arq, " ---CP_INFO: UTF8\n");
            fprintf(arq, " ---LENGTH: %d\n", cp->info.Utf8_info.length);
            fprintf(arq, " ---VALUE: %s\n", (char*)cp->info.Utf8_info.bytes);
            break;
        case STRING:
            fprintf(arq, " ---CP_INFO: STRING\n");
            fprintf(arq, " ---STRING_INDEX: %d: %s\n", cp->info.String_info.string_index, (char*)cf->constant_pool[cp->info.String_info.string_index - 1].info.Utf8_info.bytes);
            break;
        case INTEGER:
            fprintf(arq, " ---CP_INFO: INTEGER\n");
            fprintf(arq, " ---BYTES: %x\n", cp->info.Integer_info.bytes);
            fprintf(arq, " ---VALUE: %u\n", cp->info.Integer_info.bytes);
            break;
        case FLOAT:
            fprintf(arq, " ---CP_INFO: FLOAT\n");
            fprintf(arq, " ---BYTES: %x\n", cp->info.Float_info.bytes);
            u4tofloat.U4 = cp->info.Float_info.bytes;
            fprintf(arq, " ---VALUE: %f\n", u4tofloat.Float);
            break;
        case LONG:
            fprintf(arq, " ---CP_INFO: LONG\n");
            fprintf(arq, " ---HIGH: %x\n", cp->info.Long_info.high_bytes);
            fprintf(arq, " ---LOW: %x\n", cp->info.Long_info.low_bytes);
            Long = ((long long) cp->info.Long_info.high_bytes << 32) | (cp->info.Long_info.low_bytes);
            fprintf(arq, " ---VALUE: %lld\n", Long);
            break;
        case DOUBLE:
            fprintf(arq, " ---CP_INFO: DOUBLE\n");
            fprintf(arq, " ---HIGH: %x\n", cp->info.Double_info.high_bytes);
            fprintf(arq, " ---LOW: %x\n", cp->info.Double_info.low_bytes);
            Long = ((long long) cp->info.Double_info.high_bytes << 32) | (cp->info.Double_info.low_bytes);
            fprintf(arq, " ---VALUE: %lld\n", Long);
            break;
        }
    }
 
}

void imprime_classdata(ClassFile* cf, FILE* arq)
{
    fprintf(arq, "- ACCESS_FLAGS: %x ", cf->access_flags);
    imprime_flags(cf->access_flags, arq);
    fprintf(arq, "\n");
    fprintf(arq, "- THIS_CLASS: %d\n", cf->this_class);
    fprintf(arq, "\n");
    fprintf(arq, "- SUPER_CLASS: %d\n\n", cf->super_class);
    fprintf(arq, "\n");
}



void print_interfaces(ClassFile* cf, FILE* arq)
{
    fprintf(arq, "INTERFACES_COUNT: %d\n", cf->interfaces_count);
    fprintf(arq, "\n");
    fprintf(arq, "INTERFACES:\n");
    fprintf(arq, "\n");
    uint16_t* interface_aux;
    for (interface_aux = cf->interfaces; interface_aux < cf->interfaces + cf->interfaces_count; ++interface_aux)
    {
        fprintf(arq, " ---INTERFACE: %d\n\n", *interface_aux);
    }
    if (!cf->interfaces_count)
    {
        fprintf(arq, "\n");
        return;
    }
}

//TODO: Quebrar em mais funções
/*Imprime um atributo de qualquer tipo*/
void print_atribute(ClassFile* cf, attribute_info* att, FILE* arq)
{

//Comeca do 4, https://cs.au.dk/~mis/dOvs/jvmspec/ref-newarray.html
    char* newarray_value[] = {NULL, NULL, NULL, NULL, "T_BOOLEAN", "T_CHAR", "T_FLOAT", "T_DOUBLE", "T_BYTE", "T_SHORT","T_INT", "T_LONG"};
//10 instrucoes por linha
    char* instruction_name[] =
    {
        "nop", "aconst_null", "iconst_m1", "iconst_0", "iconst_1", "iconst_2", "iconst_3", "iconst_4", "iconst_5", "lconst_0", //9
        "lconst_1", "fconst_0", "fconst_1", "fconst_2", "dconst_0", "dconst_1", "bipush", "sipush", "ldc", "ldc_w",
        "ldc2_w", "iload", "lload", "fload","dload", "aload", "iload_0", "iload_1", "iload_2", "iload_3",
        "lload_0", "lload_1", "lload_2", "lload_3", "fload_0", "fload_1", "fload_2", "fload_3", "dload_0", "dload_1",
        "dload_2", "dload_3", "aload_0", "aload_1", "aload_2", "aload_3", "iaload", "laload", "faload", "daload",
        "aaload", "baload", "caload", "saload", "istore", "lstore", "fstore", "dstore", "astore", "istore_0",
        "istore_1", "istore_2", "istore_3", "lstore_0", "lstore_1", "lstore_2", "lstore_3", "fstore_0", "fstore_1", "fstore_2",
        "fstore_3", "dstore_0", "dstore_1", "dstore_2", "dstore_3", "astore_0", "astore_1", "astore_2", "astore_3", "iastore",
        "lastore", "fastore", "dastore", "aastore", "bastore", "castore", "sastore", "pop", "pop2", "dup",
        "dup_x1", "dup_x2", "dup2", "dup2_x1", "dup2_x2", "swap", "iadd", "ladd", "fadd", "dadd",
        "isub", "lsub", "fsub", "dsub", "imul", "lmul", "fmul", "dmul", "idiv", "ldiv", //109
        "fdiv", "ddiv", "irem", "lrem", "frem", "drem", "ineg", "lneg", "fneg", "dneg",
        "ishl", "lshl", "ishr", "lshr", "iushr", "lushr", "iand", "land", "ior", "lor",
        "ixor", "lxor", "iinc", "i2l", "i2f", "i2d", "l2i", "l2f", "l2d", "f2i",
        "f2l", "f2d", "d2i", "d2l", "d2f", "i2b", "i2c", "i2s", "lcmp", "fcmpl",
        "fcmpg", "dcmpl", "dcmpg", "ifeq", "ifne", "iflt", "ifge","ifgt", "ifle", "if_icmpeq", //159
        "if_icmpne", "if_icmplt", "if_icmpge", "if_icmpgt", "if_icmple", "if_acmpeq", "if_acmpne", "goto", "jsr", "ret",
        "tableswitch", "lookupswitch", "ireturn", "lreturn", "freturn", "dreturn", "areturn", "return", "getstatic", "putstatic",
        "getfield", "putfield", "invokevirtual", "invokespecial", "invokestatic", "invokeinterface", "invokedynamic", "new", "newarray", "anewarray",
        "arraylength", "athrow", "checkcast", "instanceof", "monitorenter", "monitorexit", "wide", "multianewarray", "ifnull", "ifnonnull",
        "goto_w", "jsr_w", "breakpoint", NULL, NULL, NULL, NULL, NULL, NULL, NULL, //209
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, "impdep1", "impdep2" //255
    };
//10 instrucoes por linha
    enum instrucoes_code
    {
        nop, aconst_null, iconst_m1, iconst_0, iconst_1, iconst_2, iconst_3, iconst_4, iconst_5, lconst_0, //9
        lconst_1, fconst_0, fconst_1, fconst_2, dconst_0, dconst_1, bipush, sipush, ldc, ldc_w,
        ldc2_w, iload, lload, fload,dload, aload, iload_0, iload_1, iload_2, iload_3,
        lload_0, lload_1, lload_2, lload_3, fload_0, fload_1, fload_2, fload_3, dload_0, dload_1,
        dload_2, dload_3, aload_0, aload_1, aload_2, aload_3, iaload, laload, faload, daload,//59
        aaload, baload, caload, saload, istore, lstore, fstore, dstore, astore, istore_0,
        istore_1, istore_2, istore_3, lstore_0, lstore_1, lstore_2, lstore_3, fstore_0, fstore_1, fstore_2,
        fstore_3, dstore_0, dstore_1, dstore_2, dstore_3, astore_0, astore_1, astore_2, astore_3, iastore,
        lastore, fastore, dastore, aastore, bastore, castore, sastore, pop, pop2, dup,
        dup_x1, dup_x2, dup2, dup2_x1, dup2_x2, swap, iadd, ladd, fadd, dadd,
        isub, lsub, fsub, dsub, imul, lmul, fmul, dmul, idiv, ldiv, //109
        fdiv, ddiv, irem, lrem, frem, drem, ineg, lneg, fneg, dneg,
        ishl, lshl, ishr, lshr, iushr, lushr, iand, land, ior, lor,
        ixor, lxor, iinc, i2l, i2f, i2d, l2i, l2f, l2d, f2i,
        f2l, f2d, d2i, d2l, d2f, i2b, i2c, i2s, lcmp, fcmpl,
        fcmpg, dcmpl, dcmpg, ifeq, ifne, iflt, ifge,ifgt, ifle, if_icmpeq, //150 ao 159
        if_icmpne, if_icmplt, if_icmpge, if_icmpgt, if_icmple, if_acmpeq, if_acmpne, goto2, jsr, ret,
        tableswitch, lookupswitch, ireturn, lreturn, freturn, dreturn, areturn, return2, getstatic, putstatic,
        getfield, putfield, invokevirtual, invokespecial, invokestatic, invokeinterface, invokedynamic, new, newarray, anewarray,
        arraylength, athrow, checkcast, instanceof, monitorenter, monitorexit, wide, multianewarray, ifnull, ifnonnull,
        goto_w, jsr_w, breakpoint,  //200 ao 203
        impdep1 = 254, impdep2 = 255
    };

    long long Long;
    char* type;
    type = (char*)malloc(sizeof(char) * cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.length+1);
    strcpy(type, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    int i = verificaTipo(type);
    fprintf(arq, "\tATTRIBUTE_NAME_INDEX: %d : %s\n", att->attribute_name_index, (char*)cf->constant_pool[att->attribute_name_index - 1].info.Utf8_info.bytes);
    fprintf(arq, "\tATTRIBUTE_LENGTH: %u\n\n", att->attribute_length);
    switch (i)
    {
    case CONSTANTVALUE:
        fprintf(arq, "\tTYPE: CONSTANT_VALUE\n");
        fprintf(arq, "\tCONSTANTVALUE_INDEX: %d\n\n", att->type.ConstantValue.constantvalue_index);
        switch (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].tag)
        {
        case INTEGER:
            fprintf(arq, "\tCP_INFO: INTEGER\n");
            fprintf(arq, "\tBYTES: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
            fprintf(arq, "\tVALUE: %u\n\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Integer_info.bytes);
            break;
        case FLOAT:
            fprintf(arq, "\tCP_INFO: FLOAT\n");
            fprintf(arq, "\tBYTES: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Float_info.bytes);
            u4tofloat.U4 = cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Float_info.bytes;
            fprintf(arq, "\tVALUE: %f\n\n", u4tofloat.Float);
            break;
        case LONG:
            fprintf(arq, "\tCP_INFO: LONG\n");
            fprintf(arq, "\tHIGH: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.high_bytes);
            fprintf(arq, "\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.low_bytes);
            Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Long_info.low_bytes);
            fprintf(arq, "\tVALUE: %lld\n\n", Long);
            break;
        case DOUBLE:
            fprintf(arq, "\tCP_INFO: DOUBLE\n");
            fprintf(arq, "\tHIGH: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.high_bytes);
            fprintf(arq, "\tLOW: %x\n", cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.low_bytes);
            Long = ((long long) cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.high_bytes << 32) | (cf->constant_pool[att->type.ConstantValue.constantvalue_index - 1].info.Double_info.low_bytes);
            fprintf(arq, "\tVALUE: %lld\n\n", Long);
            break;
        }
        break;
    case CODE:
        fprintf(arq, "\tTYPE: CODE\n");
        fprintf(arq, "\tMAX_STACK: %d\n", att->type.Code_attribute.max_stack);
        fprintf(arq, "\tMAX_LOCALS: %d\n", att->type.Code_attribute.max_locals);
        fprintf(arq, "\tCODE_LENGTH: %u\n", att->type.Code_attribute.code_length);
        fprintf(arq, "\tCODE:\n");
        uint8_t* code;
        cp_info* cp;
        for (code = att->type.Code_attribute.code; code < att->type.Code_attribute.code + att->type.Code_attribute.code_length; ++code)
        {
            fprintf(arq, "\t\t%d | ", (int) (code - (att->type.Code_attribute.code))); //printa a instrucao sem o code em hexa

            fprintf(arq, "%s ", instruction_name[*code]); //printa a instrucao
            uint8_t u1_aux;
            uint16_t u2_aux;
            uint32_t u4_aux;

            switch (*code)
            {
            case aload:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case anewarray:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case astore:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case bipush:
                fprintf(arq, "%d", *(++code)); //byte
                break;
            case checkcast:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case dload:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case dstore:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case fload:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case fstore:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case getfield:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                cp = cf->constant_pool +  u2_aux - 1;
                fprintf(arq, "#%d <%s.%s>", u2_aux,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                break;
            case getstatic:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case goto2: //REVER - MOSTRANDO VALOR INESPERADO
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case goto_w:
                u4_aux = *(++code); //byte1 de branch
                u4_aux = u4_aux << 8; //shift de branch
                u4_aux += *(++code); //byte2 de branch
                u4_aux = u4_aux << 8; //shift de branch
                u4_aux += *(++code); //byte3 de branch
                u4_aux = u4_aux << 8; //shift de branch
                u4_aux += *(++code); //byte4 de branch
                fprintf(arq, "%d", u4_aux); //print branchbyte

                break;
            case if_acmpeq:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case if_acmpne:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case if_icmple:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case if_icmpgt:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case if_icmpge:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case if_icmplt:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case if_icmpne:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case if_icmpeq:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case ifeq:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case ifne:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case ifgt:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case iflt:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case ifle:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case ifge:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case ifnonnull:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case ifnull:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case iinc:
                u1_aux = *(++code); //byte de index
                fprintf(arq, "%d by %d", u1_aux, *(++code)); //print branchbyte
                break;
            case iload:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case instanceof:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case invokedynamic:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                ++code; //leitura de 0 - caracteristica do invokedynamic
                ++code; //leitura de 0 - caracteristica do invokedynamic
                break;
            case invokeinterface:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                fprintf(arq, " count %d", *(++code)); //count
                ++code; //leitura de 0 - caracteristica do invokedynamic
                break;
            case invokespecial:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                cp = cf->constant_pool + u2_aux - 1;
                fprintf(arq, "#%d <%s.%s>", u2_aux,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                break;
            case invokestatic:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case invokevirtual:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                cp = cf->constant_pool + u2_aux - 1;
                fprintf(arq, "#%d <%s.%s>", u2_aux,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Method_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                break;
            case istore:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case jsr:
                u2_aux = *(++code); //byte1 de branch
                u2_aux = u2_aux << 8; //shift de branch
                u2_aux += *(++code); //byte2 de branch
                fprintf(arq, "%d", u2_aux); //print branchbyte
                break;
            case jsr_w:
                u4_aux = *(++code); //byte1 de branch
                u4_aux = u4_aux << 8; //shift de branch
                u4_aux += *(++code); //byte2 de branch
                u4_aux = u4_aux << 8; //shift de branch
                u4_aux += *(++code); //byte3 de branch
                u4_aux = u4_aux << 8; //shift de branch
                u4_aux += *(++code); //byte4 de branch
                fprintf(arq, "%d", u4_aux); //print branchbyte
                break;
            case ldc:
                u1_aux = *(++code); //index
                cp = cf->constant_pool + u2_aux - 1;
                fprintf(arq, "#%d", u2_aux); //CONTEUDO PODE SER STRING, FLOAT...
                break;
            case ldc_w:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case ldc2_w:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case lload:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case lstore:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case multianewarray:
                u2_aux = *(++code); //byte1 de index
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                fprintf(arq, " dim %d", *(++code)); //dimensions
                break;
            case new:
                u2_aux = *(++code);
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                cp = cf->constant_pool +  u2_aux - 1;
                fprintf(arq, "#%d, <%s>", u2_aux, (char*)cf->constant_pool[cp->info.Class_info.name_index - 1].info.Utf8_info.bytes);
                break;
            case newarray:
                fprintf(arq, "%s", newarray_value[*(++code)]);
                break;
            case putfield:
                u2_aux = *(++code);
                u2_aux = u2_aux << 8;
                u2_aux += *(++code);

                cp = cf->constant_pool +  u2_aux - 1;
                fprintf(arq, "#%d <%s.%s>", u2_aux,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.class_index - 1].info.Class_info.name_index - 1].info.Utf8_info.bytes,
                        (char*)cf->constant_pool[cf->constant_pool[cp->info.Fieldref_info.name_and_type_index - 1].info.NameAndType_info.name_index - 1].info.Utf8_info.bytes);
                break;
            case putstatic:
                u2_aux = *(++code);
                u2_aux = u2_aux << 8; //shift de index
                u2_aux += *(++code); //byte2 de index
                fprintf(arq, "#%d", u2_aux); //print indexbyte
                break;
            case ret:
                fprintf(arq, "#%d", *(++code)); //index
                break;
            case sipush:
                u2_aux = *(++code); //byte1
                u2_aux = u2_aux << 8; //shift
                u2_aux += *(++code); //byte2
                fprintf(arq, "%d", u2_aux); //print
                break;
            case wide:
                if ((*(++code)) == iinc)   //caso o proximo byte seja iinc
                {
                    u2_aux = *(++code);
                    u2_aux = u2_aux << 8; //shift de index
                    u2_aux += *(++code); //byte2 de index
                    fprintf(arq, "#%d", u2_aux); //print indexbyte

                    u2_aux = *(++code); //byte1 de constante
                    u2_aux = u2_aux << 8; //shift de constante
                    u2_aux += *(++code); //byte2 de constante
                    fprintf(arq, " const %d", u2_aux); //print constante
                }
                else
                {
                    u2_aux = *(++code); //byte1 de index
                    u2_aux = u2_aux << 8; //shift de index
                    u2_aux += *(++code); //byte2 de index
                    fprintf(arq, "#%d", u2_aux); //print indexbyte
                }
                break;

            //TODO:
            case lookupswitch:
                break;
            //TODO:
            case tableswitch: //REVER - ESTUDAR SOBRE
                break;
            }


            fprintf(arq, "\n");
        }
        fprintf(arq, "--- EXCEPTION_TABLE_LENGTH: %d\n", att->type.Code_attribute.exception_table_length);
        exception_table_info* exp_aux;
        for (exp_aux = att->type.Code_attribute.exception_table; exp_aux < att->type.Code_attribute.exception_table + att->type.Code_attribute.exception_table_length; ++exp_aux)
        {
            fprintf(arq, "--- EXCEPTION:\n");
            fprintf(arq, "\t--- START_PC: %d\n", exp_aux->start_pc);
            fprintf(arq, "\t--- END_PC: %d\n", exp_aux->end_pc);
            fprintf(arq, "\t--- HANDLER_PC: %d\n", exp_aux->handler_pc);
            fprintf(arq, "\t--- CATCH_TYPE: %d\n", exp_aux->catch_type);
        }
        fprintf(arq, "--- ATTRIBUTES_COUNT: %d\n", att->type.Code_attribute.attributes_count);
        attribute_info* aux_att;
        for (aux_att = att->type.Code_attribute.attributes; aux_att < att->type.Code_attribute.attributes + att->type.Code_attribute.attributes_count; ++aux_att)
        {
            print_atribute(cf, aux_att, arq);
        }
        break;
    case EXCEPTIONS:
        fprintf(arq, "--- TYPE: EXCEPTIONS\n");
        fprintf(arq, "--- NUMBER_OF_EXCEPTIONS: %d\n", att->type.Exceptions.number_of_exceptions);
        uint16_t* expt_aux;
        for (expt_aux = att->type.Exceptions.exception_index_table; expt_aux < att->type.Exceptions.exception_index_table + att->type.Exceptions.number_of_exceptions; ++expt_aux)
        {
            fprintf(arq, "--- EXCEPTION:\n");
            fprintf(arq, "\t--- CLASS: %d\n", *expt_aux);
        }
        break;
    case INNERCLASSES:
        fprintf(arq, "--- TYPE: INNER CLASSES:\n");
        fprintf(arq, "--- NUMBER_OF_CLASSES: %d\n", att->type.InnerClasses.number_of_classes);
        classtype_info* classtype_aux;
        for (classtype_aux = att->type.InnerClasses.classes; classtype_aux < att->type.InnerClasses.classes + att->type.InnerClasses.number_of_classes; ++classtype_aux)
        {
            fprintf(arq, "--- INNER CLASS:\n");
            fprintf(arq, "\t--- INNER CLASS: %d\n", classtype_aux->inner_class_info_index);
            fprintf(arq, "\t--- OUTER CLASS: %d\n", classtype_aux->outer_class_info_index);
            fprintf(arq, "\t--- INNER NAME: %d\n", classtype_aux->inner_name_index);
            fprintf(arq, "\t--- INNER CLASS ACCESS FLAGS: %x ", classtype_aux->inner_class_access_flags);
            imprime_flags(classtype_aux->inner_class_access_flags, arq);
            fprintf(arq, "\n\n");
            fprintf(arq, "\n");
        }
        break;
    case OTHER:
        break;
    }
    free(type);
}

void print_fields(ClassFile* cf, FILE* arq)
{
    int var1 = 0;
    int var2 = 0;

    fprintf(arq, "FIELDS_COUNT: %d\n", cf->fields_count);
    if (cf->fields_count == 0)
    {
        fprintf(arq, "\n");
        return;
    }
    fprintf(arq, "FIELDS:\n");
    field_info* aux_field;
    for (aux_field = cf->fields; aux_field < cf->fields + cf->fields_count; ++aux_field)
    {
        fprintf(arq, "\t[%d]\n", var1++);
        fprintf(arq, "\tNAME_INDEX: %d: %s\n", aux_field->name_index, (char*)cf->constant_pool[aux_field->name_index - 1].info.Utf8_info.bytes);
        fprintf(arq, "\tDESCRIPTOR_INDEX: %d: %s\n", aux_field->descriptor_index, (char*)cf->constant_pool[aux_field->descriptor_index - 1].info.Utf8_info.bytes);
        fprintf(arq, "\tACCESS_FLAGS: %x ", aux_field->access_flags);
        imprime_flags(aux_field->access_flags, arq);
        fprintf(arq, "\n");
        fprintf(arq, "\tATTRIBUTE_COUNT: %d\n\n", aux_field->attributes_count);
        attribute_info* aux_att;
        for (aux_att = aux_field->attributes; aux_att < aux_field->attributes + aux_field->attributes_count; ++aux_att)
        {
            fprintf(arq, "[%d] FIELD_ATTRIBUTE:\n", var2++);
            print_atribute(cf, aux_att, arq);
        }
    }
}

void print_methodes(ClassFile* cf, FILE* arq)
{
    int var1 = 0;
    int var2 = 0;

    fprintf(arq, "METHODS_COUNT: %d\n", cf->method_count); // número de estruturas na tabela methods
    if (cf->method_count == 0)
    {
        fprintf(arq, "\n");
        return;
    }
    fprintf(arq, "METHODS:\n");
    method_info* aux_meth;
    for (aux_meth = cf->methods; aux_meth < cf->methods + cf->method_count; ++aux_meth)
    {
        fprintf(arq, "[%d]\n", var1++);
        fprintf(arq, "--- NAME_INDEX: %d: %s\n", aux_meth->name_index, (char*)cf->constant_pool[aux_meth->name_index - 1].info.Utf8_info.bytes);
        fprintf(arq, "--- DESCRIPTOR_INDEX: %d: %s\n", aux_meth->descriptor_index, (char*)cf->constant_pool[aux_meth->descriptor_index - 1].info.Utf8_info.bytes);
        fprintf(arq, "--- ACCESS_FLAGS: %x ", aux_meth->access_flags);
        imprime_flags(aux_meth->access_flags, arq);
        fprintf(arq, "\n");
        fprintf(arq, "--- ATTRIBUTE_COUNT: %d\n", aux_meth->attributes_count);
        attribute_info* aux_att;
        for (aux_att = aux_meth->attributes; aux_att < aux_meth->attributes + aux_meth->attributes_count; ++aux_att)
        {
            fprintf(arq, "--- [%d] METHOD_ATTRIBUTE:\n", var2++);
            print_atribute(cf, aux_att, arq);
        }
    }
}


void print_atributes(ClassFile* cf, FILE* arq)
{
    int att_itera = 0;
    fprintf(arq, "ATTRIBUTES_COUNT: %d\n", cf->attributes_count);
    if (cf->attributes_count == 0)
    {
        fprintf(arq, "\n");
        return;
    }
    fprintf(arq, "ATTRIBUTES:\n");
    attribute_info* aux_att;
    for (aux_att = cf->attributes; aux_att < cf->attributes + cf->attributes_count; ++aux_att)
    {
        fprintf(arq, "[%d] ATTRIBUTE:\n", att_itera++);
        print_atribute(cf, aux_att, arq);
    }
}

/*Função Auxiliar para gravar todas em um Arquivo*/
void print(ClassFile* cf, char* filename_in, FILE* arq)
{
    fprintf(arq, "Nome do arquivo: %s\n\n", filename_in);
    fprintf(arq, "\n");
    imprime_magic_func(cf, arq);
    fprintf(arq, "\n");
    imprime_versions(cf, arq);
    fprintf(arq, "\n");
    imprime_pool_constantes(cf, arq);
    fprintf(arq, "\n");
    print_interfaces(cf, arq);
    fprintf(arq, "\n");
    print_fields(cf, arq);
    fprintf(arq, "\n");
    print_methodes(cf, arq);
    fprintf(arq, "\n");
    print_atributes(cf, arq);
    fprintf(arq, "\n");

    printf("Exibicao concluida!\n");
}

/*A lista utilizada para identificação das major versions pode ser acessada nesse link
https://en.wikipedia.org/wiki/Java_class_file#General_layout
*/

char* look_version(int code)
{

    char* version_jdk;
    switch (code)
    {
    case 45:
        version_jdk = (char*) malloc(sizeof(char) * 8);
        strcpy(version_jdk, "JDK 1.1");
        break;
    case 46:
        version_jdk = (char*) malloc(sizeof(char) * 8);
        strcpy(version_jdk, "JDK 1.2");
        break;
    case 47:
        version_jdk = (char*) malloc(sizeof(char) * 8);
        strcpy(version_jdk, "JDK 1.3");
        break;
    case 48:
        version_jdk = (char*) malloc(sizeof(char) * 8);
        strcpy(version_jdk, "JDK 1.4");
        break;
    case 49:
        version_jdk = (char*) malloc(sizeof(char) * 12);
        strcpy(version_jdk, "Java SE 5.0");
        break;
    case 50:
        version_jdk = (char*) malloc(sizeof(char) * 12);
        strcpy(version_jdk, "Java SE 6.0");
        break;
    case 51:
        version_jdk = (char*) malloc(sizeof(char) * 10);
        strcpy(version_jdk, "Java SE 7");
        break;
    case 52:
        version_jdk = (char*) malloc(sizeof(char) * 10);
        strcpy(version_jdk, "Java SE 8");
        break;
    case 53:
        version_jdk = (char*) malloc(sizeof(char) * 10);
        strcpy(version_jdk, "Java SE 9");
        break;
    case 54:
        version_jdk = (char*) malloc(sizeof(char) * 10);
        strcpy(version_jdk, "Java SE 10");
        break;
    default:
        version_jdk = (char*) malloc(sizeof(char) * 21);
        strcpy(version_jdk, "Java desconhecido");
        break;
    }
    return version_jdk;
}
