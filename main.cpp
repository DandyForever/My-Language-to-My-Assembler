#include <iostream>
#include "asm code creater.h"

void Readf (char* string, FILE* file);
int ChooseOperator (const char* oper);
int ChooseKey (FILE* loading, double* value);
void LoadNode (FILE* loading, Node* parent, const int depth, Tree_t* data);
void LoadFromFile (const char* file_name, Tree_t* data);


int main()
{
    Tree_t data;
    LoadFromFile ("../tree.txt", &data);
    FILE* file = fopen ("../prog.txt", "w");
    assert (file);
    cJMP(prog, 0);
    Making_asm (-1, data.First (), file);
    cEND
    //data.Dump ();
    return 0;
}

void LoadFromFile (const char* file_name, Tree_t* data)
{
    FILE* loading = fopen (file_name, "rb");
    assert (loading);

    double value = 0;
    int key = ChooseKey (loading, &value);

    data->PushFirst (key, value);

    LoadNode (loading, data->First (), 0, data);

    fclose (loading);
}

void LoadNode (FILE* loading, Node* parent, const int depth, Tree_t* data)
{
    if (getc (loading) == '\t')
    {
        double value = 0;
        int key = ChooseKey (loading, &value);

        data->PushLeft (parent, key, value);

        for (int i = 0; i < depth + 1; i++)
            getc (loading);

        LoadNode (loading, parent->left, depth + 1, data);

        for (int i = 0; i < depth; i++)
            getc (loading);

        if (getc (loading) == '\t')
        {
            double value = 0;
            int key = ChooseKey (loading, &value);

            data->PushRight (parent, key, value);

            for (int i = 0; i < depth + 1; i++)
                getc (loading);

            LoadNode (loading, parent->right, depth + 1, data);

            for (int i = 0; i < depth + 1; i++)
                getc (loading);
        }
    }

    getc (loading);
    getc (loading);

}

void Readf (char* string, FILE* file)
{
    int length = 0;

    char sym = '0';
    while (sym != '\'')
    {
        sym = getc (file);
        string[length] = sym;
        length++;
    }
    string[length - 1] = '\0';

    getc (file);
    getc (file);
}

int ChooseKey (FILE* loading, double* value)
{
    getc (loading);

    char key = getc (loading);

    getc (loading);

    switch (key)
    {
    case 'N':
        fscanf (loading, "%lf", value);
        getc (loading);
        getc (loading);
        getc (loading);

        return NUM;
        break;

    case 'V':
        fscanf (loading, "%lf", value);
        getc (loading);
        getc (loading);
        getc (loading);

        return VAR;
        break;

    case 'F':
        fscanf (loading, "%lf", value);
        getc (loading);
        getc (loading);
        getc (loading);

        return FUNCTION;
        break;

    case 'O':
        char* str = new char [MaxString];
        assert (str);

        Readf (str, loading);

        *value = ChooseOperator (str);

        return OPERATOR;
        break;
    }
}

int ChooseOperator (const char* oper)
{
    CHOOSEOP(JA   , >)
    CHOOSEOP(JE   , ==)
    CHOOSEOP(JB   , <)
    CHOOSEOP(JNE  , <>)
    CHOOSEOP(JAE  , >=)
    CHOOSEOP(JBE  , <=)
    CHOOSEOP(OPER , operator)
    CHOOSEOP(IF   , if)
    CHOOSEOP(ELSE , else)
    CHOOSEOP(WHILE, while)
    CHOOSEOP(SCAN , scan)
    CHOOSEOP(PRINT, print)
    CHOOSEOP(ASN  , =)
    CHOOSEOP(ADD  , +)
    CHOOSEOP(SUB  , -)
    CHOOSEOP(MUL  , *)
    CHOOSEOP(DIV  , /)
    CHOOSEOP(SIN  , sin)
    CHOOSEOP(COS  , cos)
    CHOOSEOP(SQRT , sqrt)
    CHOOSEOP(CALL , call)
    CHOOSEOP(PARAM, param)
    assert (0);
}
