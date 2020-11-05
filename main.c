#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Só roda no VSstudio, não está rodando no repl.it /

//-------------------- Menu cliente -----------------------/
int menu(){
    puts("");
    puts("=============");
    puts("1 - Cadastramento  ");
    puts("2 - Relatorio de movimentacao (ultimos 12 meses)  ");
    puts("3 - Relatorio de movimentacao por categoria  ");
    puts("4 - Resetar Dados  ");
    puts("5 - Mostrar Saldo disponivel   ");
    puts("6 - Mostrar todos os relatorios  ");
    puts("0 - Sair  ");
    printf("opcao => ");
    int op;
    scanf("%d", &op);
    return op;
}

//-------------------- Pega input -----------------------/
void pegar_input(int* input, int start, int end, char* msg){
    scanf("%d", input);
    while (*input < start || *input > end){
        printf("%s", msg);
        scanf("%d", input);
    }
}

//-------------------- Pega str de uma lista -----------------------/
char* pega_string_lista(const int *input, char** lista, int tamanho_lista){
    char * ptr = NULL;

    for(int i = 0; i < tamanho_lista; i++){
        if (*input-1 == i){
            ptr = lista[i];
            break;
        }
    }

    return ptr;
}

//-------------------- obter o saldo -----------------------/
float obter_saldo(char* filename){
    FILE* file;
    file = fopen(filename, "r+");

    if (file == NULL){
        fclose(file);

        file = fopen(filename, "w+");
        fputs("100.00", file);
        fclose(file);

        file = fopen(filename, "r+");
    }

    char linha[100];
    fgets(linha, 100, file);
    float value = 0;
    value = strtof(linha, NULL);

    fclose(file);

    return value;
}

//-------------------- escrever o saldo -----------------------/
void escreve_saldo(char* filename, float valor){
    FILE* file = fopen(filename, "w+");
    fprintf(file, "%.2f", valor);
    fclose(file);
}

int Cadastramento(){
    FILE *cadastro_receita;
    int fflush(FILE *cadastro_receita);
    char descricao[100];
    int data[3];
    int cadastro;
    int op;

    puts("=============");
    puts("\nVoce esta em uma: 1 - receita? ou 2 - gasto");
    pegar_input(&cadastro, 1, 2, "Numero digitado invalido, digite novamente: ");

    puts("\nQual categoria deseja cadastrar? 1 - Moradia, 2 - Estudos, 3 - Transporte, 4 - Alimentacao, 5 - Trabalho: ");
    pegar_input(&op, 1, 5, "Numero digitado invalido, digite novamente: ");

    cadastro_receita = fopen("Receita.txt", "a+");

    char log[1000] = "";
    char valor_msg[100] = "";
    char descricao_msg[100] = "";
    char msg_final[100] = "";
    int multiplicador = 1;

    if (cadastro == 1){
        strcat(log, "--------- Cadastro de receita -------- \n");
        strcat(valor_msg, "\nDigite o valor desejado para adicionar a sua carteira: ");
        strcat(descricao_msg, "Breve descricao da receita adicionada: \n");
        strcat(msg_final, "Receita adicionada com sucesso!");
    }
    else if (cadastro == 2){
        strcat(log, "--------- Cadastro dos gastos -------- \n");
        strcat(valor_msg, "\nDigite o valor desejado para retirar da sua carteira: ");
        strcat(descricao_msg, "Breve descricao do gasto: \n");
        strcat(msg_final, "Gasto adicionado com sucesso!");
        multiplicador = -1;
    }

    char** categoria = malloc(5 * sizeof(char*));
    categoria[0] = "Categorização: Moradia\n";
    categoria[1] = "Categorização: Estudos\n";
    categoria[2] = "Categorização: Transporte\n";
    categoria[3] = "Categorização: Alimentação\n";
    categoria[4] = "Categorização: Trabalho\n";
}

//-------------------- Função main -----------------------/
int main() {
    printf("Sua conta começou, e voce inicialmente possui ou possuia um saldo de R$100,00\n");
    int op = menu();
    if(op == 1)
    {
        Cadastramento();
    }
    else if(op == 2)
    {
        relatorio_mov_12();
    }
    else if(op == 3)
    {

    }
    else if(op == 4)
    {
        resetar_dados();
    }
    else if(op == 5)
    {
        most_saldo();
    }
    else if(op == 6)
    {
       mostrar_todos_rel();
    }
    else if(op == 0)
    {
      return 0;
    }
    return 0;
}