#include "Tree_t.h"
#include "commands.h"
#include <assert.h>

int COUNTJ = 0;

void Make_op (const int status, const int counter, const int oper, FILE* file);
void Make_cmd (const int status, const int counter, const int key, const double value, FILE* file);
void Making_asm (const int status, const Node* current, FILE* file);

void Making_asm (const int status, const Node* current, FILE* file)
{
    int countj = COUNTJ;

    if (current->key == OPERATOR && (int) current->value == CALL)
    {
        cPUSHr(AX)
        cPUSHr(BX)
        cSUB
        cPUSHr(BX)
        cPOPr(DX)
    }

    if (current->left)
    {
        switch ((int) current->value)
        {
            case ELSE:
                Making_asm (ELSE, current->left, file);

                break;

            case IF:
                countj = COUNTJ;
                Making_asm (IF, current->left, file);

                break;

            case WHILE:
                cLAB(w, countj)
                countj = COUNTJ;
                Making_asm (WHILE, current->left, file);

                break;

            case SCAN:
                Making_asm (SCAN, current->left, file);

                break;

            case OPER:
                if (current->left->key == FUNCTION)
                    Making_asm (OPER, current->left, file);

                else
                    Making_asm (-1, current->left, file);

                break;

            case PARAM:
                Making_asm (PARAM, current->left, file);

                break;

            default:
                Making_asm (-1, current->left, file);
        }
    }

    if (current->right)
    {
        switch ((int) current->value)
        {
            case ELSE:
                Making_asm (ELSE, current->right, file);

                break;

            case IF:
                countj = COUNTJ;
                COUNTJ++;

                Making_asm (IF, current->right, file);

                break;

            case WHILE:
                countj = COUNTJ;
                COUNTJ++;

                Making_asm (WHILE, current->right, file);

                break;

            case ASN:
                Making_asm (ASN, current->right, file);

                break;

            case PARAM:
                Making_asm (PARAM, current->right, file);

                break;

            default:
                Making_asm (-1, current->right, file);
        }

        if (current->left->key == FUNCTION && (int) current->left->value >= 0)
        {
            cPUSHr(BX)
            cADD
            cPOPr(BX)
            cPUSHr(AX)
            cADD
            cPOPr(AX)
            cREV
        }
    }

    Make_cmd (status, countj, (int) current->key, current->value, file);

    if (current->key == OPERATOR && ((int) current->value == IF && status != ELSE || (int) current->value == ELSE))
        cLAB(pi, countj)
}

void Make_cmd (const int status, const int counter, const int key, const double value, FILE* file)
{
    switch (key)
    {
    case NUM:
        cPUSHn(value)

        break;

    case VAR:
        if (status == ASN || status == SCAN)
        {
            cPUSHn(value)
            cPUSHr(AX)
            cADD
            cPOPr(CX)
        }

        else if (status == PARAM)
        {
            cPUSHr(BX)
            cPUSHn((double)1)
            cADD
            cPUSHr(AX)
            cPUSHn(value)
            cADD
            cPOPr(CX)
            cPUSHo(CX)
            cPOPo(BX)
            cPOPr(BX)
        }
        else
        {
            cPUSHn(value)
            cPUSHr(AX)
            cADD
            cPOPr(CX)
            cPUSHo(CX)
        }

        break;

    case OPERATOR:
        Make_op (status, counter, (int) value, file);

        break;

    case FUNCTION:
        if ((int) value < 0)
        {
            cLAB (prog, 0)
            cPUSHn(0)
            cPOPr(AX)
            cPUSHn(-value)
            cPOPr(BX)
        }
        else
        {
            if (status == OPER)
                cLAB (f, (int) value)

            else
            {
                cPUSHr(DX)
                cPOPr(AX)
                cPUSHr(DX)
                cPUSHr(BX)
                cSUB
                cCALL (f, (int) value)
            }
        }

        break;

    default:
        printf ("INCORRECT DATA\n");
        assert (0);
    }
}

void Make_op (const int status, const int counter, const int oper, FILE* file)
{
    switch (oper)
    {
    case ADD:
        cADD

        break;

    case SUB:
        cSUB

        break;

    case MUL:
        cMUL

        break;

    case DIV:
        cDIV

        break;

    case SIN:
        cSIN

        break;

    case COS:
        cCOS

        break;

    case SQRT:
        cSQRT

        break;

    case SCAN:
        cIN
        cPOPo(CX)

        break;

    case PRINT:
        cOUT

        break;

    case ASN:
        cPOPo(CX)

        break;
    case JA:
        JUMPCMD(JA)

        break;
    case JB:
        JUMPCMD(JB)

        break;

    case JE:
        JUMPCMD(JE)

        break;

    case JBE:
        JUMPCMD(JBE)

        break;

    case JAE:
        JUMPCMD(JAE)

        break;

    case JNE:
        JUMPCMD(JNE)

        break;

    case IF:
        cJMP(pi, counter)
        cLAB(e, counter)

        break;

    case WHILE:
        cJMP(w, counter)
        cLAB(pw, counter)

        break;
    }
}
