/*
objetivo:
Arvore genealogia que detecta possiveis eventuais doencas que os filhos possam ter.

criadores:
Adalberto, Diogo e Matheus.
*/

/*https://github.com/Adalbert-s/Projeto_Arvore_Genealogica-*/

/*Fontes de Referencia:

Estruturas de dado Arvore:                          https://www.freecodecamp.org/portuguese/news/tudo-o-que-voce-precisa-saber-sobre-estruturas-de-dados-em-arvore/amp/
Varredura infixa e pos fixa de uma arvore binaria:  https://www.ime.usp.br/~pf/algoritmos/aulas/bint.html
Estruturas de grafos:                               https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/graphdatastructs.html
Percuso em arvores:                                 http://www.ic.uff.br/~boeres/slides_ed/ed_ArvoresPercursos.pdf

Manipulacao de arquivos e ideias de projeto:        https://chat.openai.com/
Metodo de pesquisa em busca de informacoes do projeto: Bing AI */

/*dados de entrada : nome, idade, peso, ficha de saude, caracteristicas fisicas e parentesco*/

/*estruturas: funções, estruturas de dados(vetores, structs, grafos(arvores, listas)), alocação dinâmica, busca, ordenação, manipulaçao de arquivos binarios*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define vars "IF"

//estrutura das doencas que essa pessoa pode ter.
typedef struct Ficha_Tecnica{

    int Saudavel;                                       //usuario saudavel(nao possui nenhuma doenca)
    //doencas hereditarias
    int Sindrome_de_Down;                               //Sindrome de Down                          //atualmente classificas com IDs de 1000 - 1005 para nao se confundir com os outros.
    int Sindrome_de_Turner;                             //Sindrome de Turner                        //Em breve penso em criar structs com mais informacoes sobre cada doenca.
    int Hemofilia;                                      //Hemofilia
    //doencas nao hereditarias.
    int Diabetes;                                       //Diabetes Tipo 2
    int Doenca_Cardiovascular;                          //Doenca Cardiovascular
    int Doenca_de_Huntington;                           //Doenca de Huntington

}ficha;                                                                                            //tabela de IDs:        doencas: 1000 - 1005
//{1000, 1001, 1002, 1003, 1004, 1005};   //iniciando a estrutura com valores(IDs).

//estrutura com as informacoes de cada um dos 2.                                                    pessoas: formado por 10 digitos(exemplo: 000.000.000.0):
typedef struct FILHO{                                                                               // 1 digito(sexo): 1 = homem / 0 = mulher.

    //informacoes basicas.                                                                          // 2 digito(estado): 1 = casado / 0 = solteiro.
    char ID     [10];
    char nome   [50];                                                                                  // 3 digito(idade): 0 = idade < 18 / 1 = 18 > idade < 80 / 2 = idade > 80.
    char idade  [10];
    char peso   [10];                                                                                     // 4-6 digito(pai)/7-9(mae): recebe o id do casal de cima.

    //informacoes sobre suas doencas                                                                // 10 digito(nivel): definine onde que o casal esta na arvore.

    struct Ficha_Tecnica ficha;               //array de ponteiros para apenas 1, ou seja cada pessoa so vai poder ter uma ficha tecnica.

}filho;

typedef struct CONJUGUE{
    //informacoes basicas.
    char ID     [10];
    char nome   [50];
    char idade  [10];
    char peso   [10];

    struct Ficha_Tecnica ficha;

}conjugue;

//estrutura com as informacoes do casal.
typedef struct{

    char ID[8];                            //ID do casal vai ter 7 digitos(3 de cada pessoa, 1 de descendencia(ex: 000.000.0)).
    struct FILHO filho;                   //descendecia: filho pertencente a arvore(1), conjugue(0). //(idade,descedencia(pai),nivel).
    struct CONJUGUE conjugue;

}casal;                                    //*pessoa[2] (estrutura casal recebe 2 pessoas(struct pessoa)).

//estrutura da arvore.

typedef struct no {

    char ID[8];
    casal casal;

    struct no *ant;
    struct no *prox;

    int tam;

}ListaLDE;

typedef struct
{
  char id[3];
  unsigned short q_registros;
}TCabecalho;

//funcoes de manipulacao de arquivos
// funcoes para escrever e ler os dados dos integrantes em arquivos

short int   cria_pastas         (void);                     //funcao para a criacao de pastas, tipo short int pois o short int ja é mais que o necessario para essa funcao.
int         escrever_arquivo    (casal *C);     //funcao para escrever as informacoes em um arquivo binario.
short int   verificaPasta       (const char* nomePasta);
short int   existe_pessoa       (const char *nome_arquivo_registro);
//void        ler_arquivo         (const char* nomePasta, char nome_arquivo_registro);            //funcao para ler os dados da pessoa.

//funcoes de manipulacao

void cria(ListaLDE *inicio ,ListaLDE *ponteiro, casal *C);
int insere(ListaLDE *l, casal *C, const char* caminho);
int retira(ListaLDE *l, char ID);

//funcoes de visualizacao

int estaVazia(ListaLDE l);
int getTamanho(ListaLDE l);
int mostra(ListaLDE);
int verifica(ListaLDE l, casal *C, char* ID);
void lerArquivoBinario(const char* nome_arquivo);

//funcoes complementares.

short int   Adiciona_Pessoa     ();
void        Adiciona_filho      (casal *C);            //funcao para adicionar as informacoes de um novo integrante,struct pessoa como parametro,passada com &(endereço)para que seja modificada e salva em arquivo.
void        Adiciona_conjugue   (casal *C);
short int   Adiciona_par        (casal *C);
void        Adiciona_filho      (casal *C);
int         Gera_ID             (casal *C);            //funcao para adicionar um ID para a pessoa.
//void PreencheFicha(ficha *c);

int main(void){

    ListaLDE *inicio = NULL, *ponteiro;

    casal *C = NULL;
    TCabecalho cab;

    cria_pastas();
/*=========================================================================================================================*/

    FILE *file;
    char arquivo[50] = "Dados.bin";

    file = fopen(arquivo, "w+");

    if(!file){

        casal  C;

        file=fopen(arquivo,"wb+");

        if(Adiciona_Pessoa() == 1){

            Adiciona_filho(&C);
            if(Adiciona_par(&C)){
                Adiciona_conjugue(&C);
            }

        }else if(Adiciona_Pessoa() == 2){

            Adiciona_conjugue(&C);

        }else if(Adiciona_Pessoa() == 3){

            Adiciona_filho(&C);

        }
        if(!inicio){
            ponteiro = inicio= malloc(sizeof(ListaLDE));
            if(!inicio)   /* Erro */
            return(-1);
            inicio->casal = C;
            inicio->prox = NULL;
        }
        else
        {
            ponteiro=ponteiro->prox = malloc(sizeof(ListaLDE));
            if(!ponteiro)   /* Erro */
                return(-1);
            ponteiro->casal = C;
            ponteiro->prox =NULL;
      }
}
       // cria(&inicio,&ponteiro, &C);
        printf("Salvando pessoa...\n");


/*=========================================================================================================================*/

    printf("-----|ARVORE GENEALOGICA.|-----\n");
    int cont=0;

     /* Mostra a lista */
    ponteiro=inicio;
    while(ponteiro){

    printf("\nFILHO:   \n\nID: \t%s\nNome:\t%s\nIdade:\t%s\nPeso:\t%s",C->filho.ID,C->filho.nome, C->filho.idade, C->filho.peso);
    printf("\nCONJUGUE:\n\nID: \t%s\nNome:\t%s\nIdade:\t%s\nPeso:\t%s",C->conjugue.ID,C->conjugue.nome, C->conjugue.idade, C->conjugue.peso);

      ponteiro=ponteiro->prox;
      cont++;   /* Conta a quantidade de registros */
    }
    /* Salva a lista em um arquivo */
    strcpy(cab.id, vars);
    cab.q_registros=cont;
    /* Primeiro registro do arquivo */
    fwrite(&cab,sizeof(TCabecalho),1,file);
    ponteiro=inicio;
    while(ponteiro)
    {
      fwrite(&ponteiro->casal,sizeof(ListaLDE),1,file);
      ponteiro=ponteiro->prox;
    }
    /* Passa para um vetor */
    C = malloc(sizeof(ListaLDE)*cont);

    if(!C) /* Erro */
      return(-1);
    /* Passa a lista para o vetor */
    cont=0;
    ponteiro=inicio;
    while(ponteiro)
    {
      C[cont]=ponteiro->casal;
      ponteiro=ponteiro->prox;
    }
    /* Apaga a lista */
    ponteiro=inicio;
    while(ponteiro)
    {
      inicio=ponteiro->prox;
      free(ponteiro);
      ponteiro=inicio;
    }

  /* Recupera os regitros do arquivo */
  if(!C)
  {
    fread(&cab,sizeof(TCabecalho),1,file);
    if(strcmp(cab.id,vars))   /* Erro */
      return(-1);
    C =malloc(sizeof(ListaLDE)*cab.q_registros);

    if(!C)   /* Erro */
      return(-1);
    for(cont=0; cont<cab.q_registros; cont++)
      fread(&C[cont],sizeof(ListaLDE),1,file);
  }
  for(cont=0; cont<cab.q_registros; cont++)
    printf("\nFILHO:   \n\nID: \t%s\nNome:\t%s\nIdade:\t%s\nPeso:\t%s",C[cont].filho.ID,C[cont].filho.nome, C[cont].filho.idade, C[cont].filho.peso);
    printf("\nCONJUGUE:\n\nID: \t%s\nNome:\t%s\nIdade:\t%s\nPeso:\t%s",C[cont].conjugue.ID,C[cont].conjugue.nome, C[cont].conjugue.idade, C[cont].conjugue.peso);
  /* Apaga o vetor */
  free(C);
  /* Fecha o fileuivo */
  fclose(file);

    return 0;
}

short int cria_pastas(void){

    int status;                                 //variavel de status, determina a condicao da criacao das pastas.

    if(!verificaPasta("./Registro")) {

        status = mkdir("./Registro");
        if (status == 0)
            printf("Pasta 'Registro' criada com sucesso.\n");
        else
            printf("Falha ao criar a pasta 'Registro'.\n");
    } else {
        printf("A pasta 'Registro' ja existe.\n");
    }

    if(!verificaPasta("./arquivos")) {

        status = mkdir("./arquivos");
        if (status == 0)
            printf("Pasta 'arquivos' criada com sucesso.\n");
        else
            printf("Falha ao criar a pasta 'arquivos'.\n");
    } else {
        printf("A pasta 'arquivos' ja existe.\n");
    }

    return 1;
}

short int Adiciona_Pessoa(){

    int opcao;
    char op[2];
    char nome [8];

    do{
        printf("Quem voce deseja adicionar: filho(1) / conjugue(2)?\n");
        scanf("%d", &opcao);

        getchar();
        printf("opcao: %d\n",opcao);

        if(opcao == 1){
            break;
        }
        else if(opcao == 2){
            printf("Digite o ID da pessoa com que essa é casada: \n");
            fgets(nome, sizeof(nome), stdin);
            printf("Procurando pessoa no sistema...\n");

            if(existe_pessoa(nome)){
                printf("Pessoa encontrada com sucesso...\n");
                opcao = 2;
                break;
            }
            else{
                printf("Pessoa nao encontrada, deseja salvar? \n");
                fgets(op, sizeof(op), stdin);
                if(op[0] == 's' || op[0] == 'S'){
                    opcao = 3;
                    break;
                }
                else
                printf("Certo, Saindo do programa...\n");
                opcao = 0;
                break;
            }
        }
        else
            printf("| ERRO! Opcao invalida.");

    }while(1);

    return opcao;
}

void Adiciona_filho(casal *C){


    printf("Digite o ID do filho: ");                       //usando fgets para ter mais segurança e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma só vez.
    fgets(C->filho.ID, sizeof(C->filho.ID) - 1, stdin);       //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->filho.ID[strcspn(C->filho.ID, "\n")] = '\0';       //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string filho.nome.  //evita possiveis problemas na string

    printf("ID:%s\n", C->filho.ID);

    printf("Digite o nome: ");
    fgets(C->filho.nome, sizeof(C->filho.nome) - 1, stdin);
    C->filho.nome[strcspn(C->filho.nome, "\n")] = '\0';

    printf("Digite a idade: ");                         //por idade ser um inteiro, não vou ter uma string de comparacao, então criei uma variavel para armazenar.
    fgets(C->filho.idade, sizeof(C->filho.idade) - 1, stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->filho.idade[strcspn(C->filho.idade, "\n")] = '\0';

    printf("Digite o peso: ");
    fgets(C->filho.peso, sizeof(C->filho.peso) - 1, stdin);
    C->filho.peso[strcspn(C->filho.peso, "\n")] = '\0';    //strtof para transformar em float                      //essa função não recebe o argumento de base numerica.

}

void Adiciona_conjugue(casal *C){

    printf("Digite o ID: ");                //usando fgets para ter mais segurança e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma só vez.
    fgets(C->conjugue.ID, sizeof(C->conjugue.ID)- 1, stdin);   //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->conjugue.ID[strcspn(C->conjugue.ID, "\n")] = '\0';     //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string conjugue.nome.  //evita possiveis problemas na string

    printf("Digite o nome: ");
    fgets(C->conjugue.nome, sizeof(C->conjugue.nome) - 1, stdin);
    C->conjugue.nome[strcspn(C->conjugue.nome, "\n")] = '\0';

    printf("Digite a idade: ");
    fgets(C->conjugue.idade, sizeof(C->conjugue.idade) - 1, stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->conjugue.idade[strcspn(C->conjugue.idade, "\n")] = '\0';

    printf("Digite o peso: ");
    fgets(C->conjugue.peso, sizeof(C->conjugue.peso) - 1, stdin);
    C->conjugue.peso[strcspn(C->conjugue.peso, "\n")] = '\0';

}

short int Adiciona_par(casal *C){

    char opcao[5];
    if(C != NULL && C->filho.idade != NULL && C->filho.idade >= 18){
        printf("Essa pessoa e casada? \n");
        fgets(opcao, sizeof(opcao), stdin);

        if(opcao[0] == 's' || opcao[0] == 'S'){
            printf("Por favor adicione essa pessoa: \n");
            return 1;
        }
        else{
            char desconhecido[20] = "desconhecido";
            strcpy(C->conjugue.ID, desconhecido);
            strcpy(C->conjugue.nome, desconhecido);
            strcpy(C->conjugue.idade, desconhecido);
            strcpy(C->conjugue.peso, desconhecido);
        }
    }
    return 0;
}

int escrever_arquivo(casal *C) {
    FILE *file;
    char nome_arquivo_registro[50];


    file = fopen(nome_arquivo_registro, "w+");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
        return 0;
    }

    printf("Digite o ID do filho: ");
    fgets(C->filho.ID, sizeof(C->filho.ID) - 1, stdin);
    C->filho.ID[strcspn(C->filho.ID, "\n")] = '\0';

    printf("ID:%s\n", C->filho.ID);

    printf("Digite o nome: ");
    fgets(C->filho.nome, sizeof(C->filho.nome) - 1, stdin);
    C->filho.nome[strcspn(C->filho.nome, "\n")] = '\0';

    printf("Digite a idade: ");
    fgets(C->filho.idade, sizeof(C->filho.idade) - 1, stdin);
    C->filho.idade[strcspn(C->filho.idade, "\n")] = '\0';

    printf("Digite o peso: ");
    fgets(C->filho.peso, sizeof(C->filho.peso) - 1, stdin);
    C->filho.peso[strcspn(C->filho.peso, "\n")] = '\0';


    printf("Digite o ID: ");
    fgets(C->conjugue.ID, sizeof(C->conjugue.ID)- 1, stdin);
    C->conjugue.ID[strcspn(C->conjugue.ID, "\n")] = '\0';

    printf("Digite o nome: ");
    fgets(C->conjugue.nome, sizeof(C->conjugue.nome) - 1, stdin);
    C->conjugue.nome[strcspn(C->conjugue.nome, "\n")] = '\0';

    printf("Digite a idade: ");
    fgets(C->conjugue.idade, sizeof(C->conjugue.idade) - 1, stdin);
    C->conjugue.idade[strcspn(C->conjugue.idade, "\n")] = '\0';

    printf("Digite o peso: ");
    fgets(C->conjugue.peso, sizeof(C->conjugue.peso) - 1, stdin);
    C->conjugue.peso[strcspn(C->conjugue.peso, "\n")] = '\0';



    /*if (existe_pessoa(nome_arquivo_registro)) {
        printf("Erro, Casal ja cadrastada!\n");
        printf("Deseja fazer alteracoes nas informacoes desse casal?\n");
        getchar();
        fgets(op, sizeof(op), stdin);
        if (op[0] == 's' || op[0] == 'S') {
            goto continuar;
        } else {
            return 0;
        }
    } else {
        printf("A pessoa nao esta registrada.\n");
    }

    continuar:*/
    /*printf("DADOS DO CASAL: \n");
    printf("FILHO: \n");
    printf("ID:%s\n", C->filho.ID);
    printf("nome:%s\n", C->filho.nome);
    printf("idade:%i\n", C->filho.idade);
    printf("Peso:%i\n", C->filho.peso);

    printf("CONJUGUE: \n");
    printf("ID:%s\n", C->conjugue.ID);
    printf("nome:%s\n", C->conjugue.nome);
    printf("idade:%i\n", C->conjugue.idade);
    printf("Peso:%i\n", C->conjugue.peso);*/


/*fprintf(file, "%s\n", C->ID);
fprintf(file, "%s %s %s %f\n", C->filho.ID, C->filho.nome, C->filho.idade, C->filho.peso);
fprintf(file, "%s %s %s %f\n", C->conjugue.ID, C->conjugue.nome, C->conjugue.idade, C->conjugue.peso);*/


    fwrite(&C, sizeof(casal), 1, file);
    fclose(file);

    return 1;
}


short int verificaPasta(const char* nomePasta) {

    struct stat st;
    if(stat(nomePasta, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            // A pasta existe
            return 1;
        } else {
            // O caminho existe, mas não é uma pasta
            return 0;
        }
    } else {
        // A pasta não existe
        return 0;
    }
}

short int existe_pessoa(const char *nome_arquivo_registro){

    FILE* arquivo = fopen(nome_arquivo_registro, "r");

    if (arquivo == NULL) {
        // Arquivo não encontrado
        return 1;
    }

    // Arquivo encontrado"
    fclose(arquivo);
    return 0;
}

/*void PreencheFicha(ficha){
    int c;
    char r[4];
    printf("-----------Preenchendo ficha tecnica-----------");
    do{
        c = 0;
        printf("A pessoa tem Sindrome de Down? ");
        gets(r);
        for (int R = 0; R< strlen(r); R++){
            r[R] = toupper(r[R]);
        }
        if(r == 'SIM'){
            C->filho.ficha.Sindrome_de_Down = 1;
            c = 1;
        } if(r == 'NAO'){
            C->filho.ficha.Saudavel = 1;
            c = 1;
        }
    }while(c == 0);

    do{
        c = 0;
        printf("A pessoa tem Sindrome de Turner? ");
        gets(r);
        for (int R = 0; R< strlen(r); R++){
            r[R] = toupper(r[R]);
        }
        if(r == 'SIM'){
            C->filho.ficha.Sindrome_de_Turner = 1;
            c = 1;
        } if(r == 'NAO'){
            C->filho.ficha.Saudavel = 1;
            c = 1;
        }
    }while(c == 0);

    do{
        c = 0;
        printf("A pessoa tem Sindrome de Turner? ");
        gets(r);
        for (int R = 0; R< strlen(r); R++){
            r[R] = toupper(r[R]);
        }
        if(r == 'SIM'){
            C->filho.ficha.Sindrome_de_Turner = 1;
            c = 1;
        } if(r == 'NAO'){
            C->filho.ficha.Saudavel = 1;
            c = 1;
        }
    }while(c == 0);

    do{
        c = 0;
        printf("A pessoa tem Diabetes? ");
        gets(r);
        for (int R = 0; R< strlen(r); R++){
            r[R] = toupper(r[R]);
        }
        if(r == 'SIM'){
            C->filho.ficha.Diabetes = 1;
            c = 1;
        } if(r == 'NAO'){
            C->filho.ficha.Saudavel = 1;
            c = 1;
        }
    }while(c == 0);

    do{
        c = 0;
        printf("A pessoa tem Doenca Cardiovascular? ");
        gets(r);
        for (int R = 0; R< strlen(r); R++){
            r[R] = toupper(r[R]);
        }
        if(r == 'SIM'){
            C->filho.ficha.Doenca_Cardiovascular = 1;
            c = 1;
        } if(r == 'NAO'){
            C->filho.ficha.Saudavel = 1;
            c = 1;
        }
    }while(c == 0);

    do{
        c = 0;
        printf("A pessoa tem Doenca de Huntington? ");
        gets(r);
        for (int R = 0; R< strlen(r); R++){
            r[R] = toupper(r[R]);
        }
        if(r == 'SIM'){
            C->filho.ficha.Doenca_de_Huntington = 1;
            c = 1;
        } if(r == 'NAO'){
            C->filho.ficha.Saudavel = 1;
            c = 1;
        }
    }while(c == 0);
}*/
