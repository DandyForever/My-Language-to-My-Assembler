const int varnamesize = 255, NUM_ofvar = 1000;

const char* Expression = nullptr;
int Index = 0;
const int MaxString = 300;

#define CHOOSEOP(name, sym)\
if (!strcmp (oper, #sym))\
    {\
        return name;\
    }\
    else

#define cPUSHn(num) fprintf (file, "PUSH %g\n"  ,  num);
#define cPUSHv(var) fprintf (file, "PUSH [%g]\n",  var);
#define cPUSHr(reg) fprintf (file, "PUSH %s\n"  , #reg);
#define cPUSHo(reg) fprintf (file, "PUSH [%s]\n", #reg);
#define  cPOPo(reg) fprintf (file, "POP [%s]\n" , #reg);
#define  cPOPv(var) fprintf (file, "POP [%g]\n" ,  var);
#define  cPOPr(reg) fprintf (file, "POP %s\n"   , #reg);
#define   cOUT      fprintf (file, "OUT\n"            );
#define    cIN      fprintf (file, "IN\n"             );
#define   cSIN      fprintf (file, "SIN\n"            );
#define   cCOS      fprintf (file, "COS\n"            );
#define  cSQRT      fprintf (file, "SQRT\n"           );
#define   cADD      fprintf (file, "ADD\n"            );
#define   cSUB      fprintf (file, "SUB\n"            );
#define   cMUL      fprintf (file, "MUL\n"            );
#define   cDIV      fprintf (file, "DIV\n"            );
#define   cEND      fprintf (file, "END\n"            );
#define    cJA(lab, num) fprintf (file, "JA :%s%d\n" , #lab, num);
#define    cJB(lab, num) fprintf (file, "JB :%s%d\n"   , #lab, num);
#define    cJE(lab, num) fprintf (file, "JE :%s%d\n"   , #lab, num);
#define   cJAE(lab, num) fprintf (file, "JAE :%s%d\n"  , #lab, num);
#define   cJBE(lab, num) fprintf (file, "JBE :%s%d\n"  , #lab, num);
#define   cJNE(lab, num) fprintf (file, "JNE :%s%d\n"  , #lab, num);
#define   cJMP(lab, num) fprintf (file, "JMP :%s%d\n"  , #lab, num);
#define  cCALL(lab, num) fprintf (file, "CALL :%s%d\n" , #lab, num);
#define   cREV           fprintf (file, "REV\n"                   );
#define   cLAB(lab, num) fprintf (file, ":%s%d\n",       #lab, num);


#define JUMPCMD(name) if (status == IF)\
        {\
            c##name (i, counter)\
            cJMP(e, counter)\
            cLAB(i, counter)\
        }\
        else\
        {\
            c##name (w_, counter)\
            cJMP(pw, counter)\
            cLAB(w_, counter)\
        }

enum types
{
    NUM      = 1,
    VAR      = 2,
    OPERATOR = 3,
    FUNCTION = 4
};

enum operators
{
    ADD   = 1,
    SUB   = 2,
    MUL   = 3,
    DIV   = 4,
    SIN   = 5,
    COS   = 6,
    SQRT  = 7,
    OPER  = 8,
    JA    = 9,
    JB    = 10,
    JE    = 11,
    JNE   = 12,
    JAE   = 13,
    JBE   = 14,
    ASN   = 15,
    IF    = 16,
    ELSE  = 17,
    WHILE = 18,
    SCAN  = 19,
    PRINT = 20,
    CALL  = 21,
    PARAM = 22
};

