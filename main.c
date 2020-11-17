#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int contapalavra(char* palavra);
char** separapalavra(char separacao,char* palavra,int tamanho);
int somaNoIndice(char* lista_relatorio);
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

//-------------------- Cadastro -----------------------/
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
        strcat(descricao_msg, "Breve descricao da receita adicionada: ");
        strcat(msg_final, "\nReceita adicionada com sucesso!");
    }
    else if (cadastro == 2){
        strcat(log, "--------- Cadastro dos gastos -------- \n");
        strcat(valor_msg, "\nDigite o valor desejado para retirar da sua carteira: ");
        strcat(descricao_msg, "Breve descricao do gasto: ");
        strcat(msg_final, "\nGasto adicionado com sucesso!");
        multiplicador = -1;
    }

    char** categoria = malloc(5 * sizeof(char*));
    categoria[0] = "Categorização: Moradia\n";
    categoria[1] = "Categorização: Estudos\n";
    categoria[2] = "Categorização: Transporte\n";
    categoria[3] = "Categorização: Alimentação\n";
    categoria[4] = "Categorização: Trabalho\n";

    strcat(log, pega_string_lista(&op, categoria, 5));

    char filename[50] = "Saldo.txt";
    float saldo = obter_saldo(filename);

    float saldo_momentaneo = 0;
    printf("%s",valor_msg);
    scanf("%f", &saldo_momentaneo);

    float resultado = saldo + (saldo_momentaneo * multiplicador);

    printf(descricao_msg);
    scanf("%c", descricao);
    fgets(descricao,100,stdin);
    char descricao_valor[100];
    sprintf(descricao_valor,"Descrição: %s", descricao);
    strcat(log, descricao_valor);

    char valor_convertido[100];
    if (cadastro == 1){
        sprintf(valor_convertido, "Valor adicionado a conta: +R$%.2f\n", saldo_momentaneo);
    }
    else if (cadastro == 2){
        sprintf(valor_convertido, "Valor debitado da conta: -R$%.2f\n", saldo_momentaneo);
    }

    strcat(log, valor_convertido);

    printf("Digite o dia: ");
    pegar_input(&data[0], 1, 31, "Digite um dia valido: ");
    printf("Digite o mes: ");
    pegar_input(&data[1], 1, 12, "Digite um mês valido: ");
    printf("Digite o ano: ");
    pegar_input(&data[2], 1, 2020, "Digite um ano valido: ");

    char data_convertida[100];
    sprintf(data_convertida, "Data : %d / %d / %d\n", data[0], data[1], data[2]);
    strcat(log, data_convertida);

    puts(msg_final);
    fprintf(cadastro_receita, log);

    fclose(cadastro_receita);

    escreve_saldo("Saldo.txt", resultado);
    return 0;
}

//-------------------- Resetar os dados -----------------------/
int resetar_dados(){
    int saber = 0;
    puts("Você tem certeza que deseja apagar todos os seus dados? Obs.: Apagar seus dados, nao apaga seu saldo. ");
    puts("Digite 1 - Para sim e 2 - Para nao. ");
    scanf("%d", &saber);
    if (saber == 1){
        FILE *apagar = fopen ( "Receita.txt", "r" );
        if ( apagar ) {
            fclose(apagar);
            printf("Removido seus dados.");
            remove("Receita.txt");
            return 1;
        }
        else if (saber == 2){
            puts("Ok! Saindo do programa.");
            return 0;
        }
    }
    return 0;
}

// ------------------------ Relatório dos últimos 12 meses ------------------------/
void relatorio_ano(FILE* arquivo){
    char lista_relatorio[6][100];
    FILE* relatorio = fopen("relatorio.html", "w");
    fputs("<html>", relatorio);
    fputs("<style>",relatorio);
    fputs("table, th, td {border: 1px solid black}",relatorio);
    fputs("</style>",relatorio);
    fputs("<title >Relatorio do Ano</title>", relatorio);
    fputs("<body>", relatorio);
    fputs("<h1>Relatorio dos últimos 12 meses</h1>", relatorio);
    fputs("<table style: border:1px solid black>",relatorio);
    fputs("<tr>",relatorio);
    fputs("<th>Moradia</th>",relatorio);
    fputs("<th>Estudos</th>",relatorio);
    fputs("<th>Transporte</th>",relatorio);
    fputs("<th>Alimentação</th>",relatorio);
    fputs("<th>Trabalho</th>",relatorio);
    fputs("<th>Datas</th>",relatorio);
    fputs("</tr>",relatorio);
    //float indices[5] = {0,0,0,0,0};
    float* indices = malloc(5 * sizeof(float));
    indices[0] = 0; //Moradia
    indices[1] = 0; //Transporte
    indices[2] = 0; //Trabalho
    indices[3] = 0; //Estudos
    indices[4] = 0; //Alimentação
    
    while(fgets(lista_relatorio[0],100,arquivo) != NULL){
        indices[0] = 0; //Moradia
        indices[1] = 0; //Transporte
        indices[2] = 0; //Trabalho
        indices[3] = 0; //Estudos
        indices[4] = 0; //Alimentação
        fgets(lista_relatorio[1],100,arquivo);
        fgets(lista_relatorio[2],100,arquivo);
        fgets(lista_relatorio[3],100,arquivo);
        fgets(lista_relatorio[4],100,arquivo);
        int caracteres = contapalavra(lista_relatorio[4]);
        char **data = separapalavra(' ',lista_relatorio[4],caracteres);
        float valor = strtof(separapalavra(' ', lista_relatorio[3], contapalavra(lista_relatorio[3]))[contapalavra(lista_relatorio[3]) - 2], NULL);
        char** cadastro = separapalavra(' ', lista_relatorio[0], contapalavra(lista_relatorio[0]));
        // VERIFICAÇÃO DO ANO //
        int year = atoi(data[caracteres-1]);
        int month = atoi(data[caracteres-3]);
        struct tm *data_hora_atual;
        time_t segundos;
        time(&segundos);
        data_hora_atual = localtime(&segundos);
        int mes_atual = data_hora_atual->tm_mon+1;
        int ano_atual = data_hora_atual->tm_year+1900;
        // --------------------- verificação dos últimos 12 meses --------------------------
        if (year < ano_atual - 1 || year > ano_atual)
        {
            continue;
        }else{
            if(year == ano_atual - 1){

                if(month < mes_atual){
                    continue; 
                }
                else{
                    int multiplicador = 1;
                    if(strcmp(cadastro[contapalavra(lista_relatorio[0])-3], "gastos") == 0){
                        multiplicador = -1;
                    }
                    valor *= multiplicador;
                    int pos = somaNoIndice(lista_relatorio[1]);
                    indices[pos] = valor;
                }

            }else if(year == ano_atual){
                if(month > mes_atual){
                    continue;
                }else{
                    int multiplicador = 1;
                    if(strcmp(cadastro[contapalavra(lista_relatorio[0])-3], "gastos") == 0){
                        multiplicador = -1;
                    }
                    valor *= multiplicador;
                    
                    int pos = somaNoIndice(lista_relatorio[1]);
                    indices[pos] = valor;
                }
            }
        }
        int k = 0;
        fputs("<tr>",relatorio);
        for(int i = 0; i < 5; i ++){
            fprintf(relatorio,"<td>%.2f</td>", indices[i]);
        }
        fprintf(relatorio,"<td>%s</td>", lista_relatorio[4]);
        fputs("</tr>",relatorio);
    }

    fputs("</table>",relatorio);
    fputs("</body>", relatorio);
    fputs("</html>", relatorio);
    fclose(relatorio);
}

// ----------------- Relatório por categoria do último mes ------------------- //
//pegamos o num_cat como parametro pq lá na main qnd vamos chamar a função, a variavel resposta está como int e vamos precisar desse int para fazer o if 
//da condição das categorias.
int relatorio_cat(FILE* arquivo){
    char lista_relatorio[6][100];
    FILE* relatorio = fopen("relatorioCategorias.html", "w");
    fputs("<html>", relatorio);
    fputs("<style>",relatorio);
    fputs("table, th, td {border: 1px solid black}",relatorio);
    fputs("</style>",relatorio);
    fputs("<title >Relatorio do mes</title>", relatorio);
    fputs("<body>", relatorio);
    fputs("<h1>Relatorio de todas as categorias do ultimo mes</h1>", relatorio);
    fputs("<table style: border:1px solid black>",relatorio);
    fputs("<tr>",relatorio);
    fputs("<th>Moradia</th>",relatorio);
    fputs("<th>Estudos</th>",relatorio);
    fputs("<th>Transporte</th>",relatorio);
    fputs("<th>Alimentação</th>",relatorio);
    fputs("<th>Trabalho</th>",relatorio);
    fputs("<th>Datas</th>",relatorio);
    fputs("</tr>",relatorio);

    //float indices[5] = {0,0,0,0,0};
    float* indices = malloc(5 * sizeof(float));
    indices[0] = 0; //Moradia
    indices[1] = 0; //Transporte
    indices[2] = 0; //Trabalho
    indices[3] = 0; //Estudos
    indices[4] = 0; //Alimentação
    while(fgets(lista_relatorio[0],100,arquivo) != NULL){
        indices[0] = 0; //Moradia
        indices[1] = 0; //Transporte
        indices[2] = 0; //Trabalho
        indices[3] = 0; //Estudos
        indices[4] = 0; //Alimentação
        fgets(lista_relatorio[1],100,arquivo);
        fgets(lista_relatorio[2],100,arquivo);
        fgets(lista_relatorio[3],100,arquivo);
        fgets(lista_relatorio[4],100,arquivo);
        int caracteres = contapalavra(lista_relatorio[4]);
        char **data = separapalavra(' ',lista_relatorio[4],caracteres); 
        // VERIFICAÇÃO DO ANO //
        float valor = strtof(separapalavra(' ', lista_relatorio[3], contapalavra(lista_relatorio[3]))[contapalavra(lista_relatorio[3]) - 2], NULL);
        char** cadastro = separapalavra(' ', lista_relatorio[0], contapalavra(lista_relatorio[0]));

        int year = atoi(data[caracteres-1]);
        int month = atoi(data[caracteres-3]);
        struct tm *data_hora_atual;
        time_t segundos;
        time(&segundos);
        data_hora_atual = localtime(&segundos);
        int mes_atual = data_hora_atual->tm_mon+1;
        int ano_atual = data_hora_atual->tm_year+1900;
        // --------------------- verificação do último mês --------------------------
        if(year == ano_atual){
                if(month == mes_atual - 1){
                int multiplicador = 1;
                if(strcmp(cadastro[contapalavra(lista_relatorio[0])-3], "gastos") == 0){
                    multiplicador = -1;
                }
                valor *= multiplicador;
                int pos = somaNoIndice(lista_relatorio[1]);
                indices[pos] = valor;
                int k = 0;
                fputs("<tr>",relatorio);
                for(int i = 0; i < 5; i ++){
                    fprintf(relatorio,"<td>%.2f</td>", indices[i]);
                  }
                }else{
                    break;
                }
                fprintf(relatorio,"<td>%s</td>", lista_relatorio[4]);
                fputs("</tr>",relatorio);
            }
    }

    fputs("</table>",relatorio);
    fputs("</body>", relatorio);
    fputs("</html>", relatorio);
    fclose(relatorio);
}   
// conta quantas palavras tem dentro de uma lista, para diminuir lixo de memória, vai ler apenas o tanto de palavras que tem, assim não precisa pegar por ex
// lista[100][100].
int contapalavra(char* palavra){
    int contador = 0;
    for(int i=0;i<=(strlen(palavra));i++)
    {
        if(palavra[i]== ' ' || palavra[i]=='\0')
        {
            contador++;    
        }
    }
    return contador;
}
// Função para fazer split (mesma coisa do python)
char** separapalavra(char separacao,char* palavra,int tamanho){
    int j = 0;
    int ctr = 0;
    char** ptr = malloc(sizeof(char*) * 100);
    for (int i = 0; i < 100; i++)
    {
        ptr[i] = malloc(sizeof(char) * 100);
    }
    
    for(int i=0;i<=(strlen(palavra));i++)
    {
        if(palavra[i]== separacao ||palavra[i]=='\0')
        {
            ptr[ctr][j]='\0';
            ctr++;  // próxima palavra
            j=0;    //próxima palavra iniciando no indice 0.
        }
        else
        {
            ptr[ctr][j]=palavra[i];
            j++;
        }
    }
    return ptr;
}

 // verificar qual categoria está chamando e adicionar o valor na variavel indice
int somaNoIndice(char* lista_relatorio){
    int indice = 0;

    if(strcmp(lista_relatorio, "Categorizacao: Moradia\n") == 0){
        indice = 0;
    }
    else if(strcmp(lista_relatorio, "Categorizacao: Estudos\n") == 0){
        indice = 1;
    }
    else if(strcmp(lista_relatorio, "Categorizacao: Transporte\n") == 0){
        indice = 2;
    }
    else if(strcmp(lista_relatorio, "Categorizacao: Alimentacao\n") == 0){
        indice = 3;
    }
    else if(strcmp(lista_relatorio, "Categorizacao: Trabalho\n") == 0){
        indice = 4;
    }

    return indice;
}

//-------------------- Mostrar relatório geral -----------------------/
int mostrar_todos_rel(){
  FILE* cadastros = fopen("Receita.txt", "r");
  char dados[99];
  while(fgets(dados,99,cadastros) != NULL){
    printf("%s", dados);
  }
  fclose(cadastros);
  return 0;
}

int most_saldo(){
    char filename[50] = "Saldo.txt";
    float saldo = obter_saldo(filename);
    printf("Seu saldo atual disponivel é: %.2f", saldo);
    return 0;
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
      relatorio_ano(fopen("Receita.txt","r"));
    }
    else if(op == 3)
    {
      relatorio_cat(fopen("Receita.txt", "r"));
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
