#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX 512

// Função auxiliar para verificar se um caractere é operador
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Função auxiliar para verificar se é uma função matemática
int isFunction(const char *str) {
    return strcmp(str, "log") == 0 || strcmp(str, "sen") == 0 || strcmp(str, "cos") == 0;
}

// Função auxiliar para realizar operações matemáticas
float performOperation(char op, float a, float b) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

// Função para obter a forma infixa a partir de uma expressão pós-fixada
char *getFormaInFixa(char *Str) {
    static char infixa[MAX];
    char stack[MAX][MAX];
    int top = -1;

    for (int i = 0; i < strlen(Str); i++) {
        if (Str[i] == ' ') continue; // Ignora espaços

        // Se é operando, empilha
        if (!isOperator(Str[i]) && !isalpha(Str[i])) {
            char operand[16];
            int k = 0;
            while (Str[i] != ' ' && Str[i] != '\0') {
                operand[k++] = Str[i++];
            }
            operand[k] = '\0';
            strcpy(stack[++top], operand);
            i--; // Volta um passo após sair do número
        } else if (isalpha(Str[i])) {
            // Verifica se é uma função (log, sen, cos)
            char func[4];
            int j = 0;
            while (Str[i] != ' ' && Str[i] != '\0') {
                func[j++] = Str[i++];
            }
            func[j] = '\0';

            if (isFunction(func)) {
                char op1[MAX];
                strcpy(op1, stack[top--]);
                sprintf(stack[++top], "%s(%s)", func, op1);
            }
            i--; // Corrige índice
        } else {
            // É operador, desempilha dois operandos
            char op2[MAX], op1[MAX];
            strcpy(op2, stack[top--]);
            strcpy(op1, stack[top--]);

            // Concatena na forma "(op1 operador op2)"
            sprintf(stack[++top], "(%s %c %s)", op1, Str[i], op2);
        }
    }

    strcpy(infixa, stack[top]);
    return infixa;
}

// Função para calcular o valor de uma expressão pós-fixada
float getValor(char *Str) {
    float stack[MAX];
    int top = -1;
    char temp[MAX];
    int j = 0;

    for (int i = 0; i < strlen(Str); i++) {
        if (Str[i] == ' ') continue; // Ignora espaços

        if (!isOperator(Str[i]) && !isalpha(Str[i])) {
            // Se for número, converte e empilha
            char number[16];
            int k = 0;
            while (Str[i] != ' ' && Str[i] != '\0') {
                number[k++] = Str[i++];
            }
            number[k] = '\0';
            stack[++top] = atof(number);
            i--; // Ajusta o índice
        } else if (isalpha(Str[i])) {
            // Verifica se é uma função (log, sen, cos)
            char func[4];
            int j = 0;
            while (Str[i] != ' ' && Str[i] != '\0') {
                func[j++] = Str[i++];
            }
            func[j] = '\0';

            float a = stack[top--];
            if (strcmp(func, "log") == 0)
                stack[++top] = log10(a);  // logaritmo de base 10
            else if (strcmp(func, "sen") == 0)
                stack[++top] = sin(a * M_PI / 180.0);  // seno em graus
            else if (strcmp(func, "cos") == 0)
                stack[++top] = cos(a * M_PI / 180.0);  // cosseno em graus
            i--; // Corrige o índice
        } else {
            // Operador: desempilha dois operandos
            float b = stack[top--];
            float a = stack[top--];
            stack[++top] = performOperation(Str[i], a, b);
        }
    }

    return stack[top];
}


//gcc -o calculadora main.c expressao.c -lm
//./calculadora