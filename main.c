#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// ================= ENUM DE TOKENS =================
typedef enum {
    PROGRAMA,
    IDENTIFICADOR,
    NUMERO,
    PONTO_E_VIRGULA,
    ATRIBUICAO,
    INICIO,
    FIM,
    SE,
    ENTAO,
    MAIS,
    MAIOROUIGUAL,
    VIRGULA,
    ABRE_PARENTESES,
    FECHA_PARENTESES,
    PONTO,
    FIM_DE_ARQUIVO,
    DESCONHECIDO
} Token;

// ================= VARIÁVEL GLOBAL =================
FILE *fp;

// ================= ANALISADOR LÉXICO =================
Token analex() {
    char c;
    char buffer[100];
    int i;

    while ((c = fgetc(fp)) != EOF) {

        if (isspace(c)) continue;

        // IDENTIFICADOR OU PALAVRA RESERVADA
        if (isalpha(c)) {
            i = 0;
            buffer[i++] = c;

            while (isalnum(c = fgetc(fp)) || c == '_') {
                buffer[i++] = c;
            }

            buffer[i] = '\0';
            ungetc(c, fp);

            if (strcmp(buffer, "program") == 0) return PROGRAMA;
            else if (strcmp(buffer, "begin") == 0) return INICIO;
            else if (strcmp(buffer, "end") == 0) return FIM;
            else if (strcmp(buffer, "if") == 0) return SE;
            else if (strcmp(buffer, "then") == 0) return ENTAO;
            else return IDENTIFICADOR;
        }

        // NÚMERO
        else if (isdigit(c)) {
            while (isdigit(c = fgetc(fp)));
            ungetc(c, fp);
            return NUMERO;
        }

        // OPERADORES E SÍMBOLOS
        else {
            switch (c) {
                case '+': return MAIS;

                case '>':
                    c = fgetc(fp);
                    if (c == '=') return MAIOROUIGUAL;
                    ungetc(c, fp);
                    break;

                case ';': return PONTO_E_VIRGULA;
                case '.': return PONTO;
                case ',': return VIRGULA;
                case '(': return ABRE_PARENTESES;
                case ')': return FECHA_PARENTESES;

                case ':':
                    c = fgetc(fp);
                    if (c == '=') return ATRIBUICAO;
                    ungetc(c, fp);
                    break;
            }
        }

        return DESCONHECIDO;
    }

    return FIM_DE_ARQUIVO;
}

// ================= BLOCO =================
void compila_bloco() {
    Token token;

    token = analex();
    if (token != INICIO) {
        printf("Erro: esperava BEGIN\n");
        exit(1);
    }

    // Aqui poderia ter comandos (simplificado)

    token = analex();
    if (token != FIM) {
        printf("Erro: esperava END\n");
        exit(1);
    }
}

// ================= PROGRAMA =================
void compila_programa() {
    Token token;

    token = analex();
    if (token != PROGRAMA) {
        printf("Erro: esperava PROGRAM\n");
        exit(1);
    }

    token = analex();
    if (token != IDENTIFICADOR) {
        printf("Erro: esperava identificador\n");
        exit(1);
    }

    token = analex();
    if (token != PONTO_E_VIRGULA) {
        printf("Erro: esperava ;\n");
        exit(1);
    }

    compila_bloco();

    token = analex();
    if (token != PONTO) {
        printf("Erro: esperava .\n");
        exit(1);
    }

    token = analex();
    if (token != FIM_DE_ARQUIVO) {
        printf("Erro: esperava fim de arquivo\n");
        exit(1);
    }

    printf("Programa sintaticamente correto!\n");
}

// ================= MAIN =================
int main() {
    fp = fopen("entrada.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    compila_programa();

    fclose(fp);
    return 0;
}