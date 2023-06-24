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
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>

#define id "IF"
#define NOME_ARQUIVO "dados.bin"

//estrutura das doencas que essa pessoa pode ter.
typedef struct FichaTecnica{

    int Saudavel;                                       //usuario saudavel(nao possui nenhuma doenca)
    //doencas hereditarias
    int Sindrome_de_Down;                               //Sindrome de Down                          
    int Sindrome_de_Turner;                             //Sindrome de Turner                       
    int Hemofilia;                                      //Hemofilia
    //doencas nao hereditarias.
    int Diabetes;                                       //Diabetes Tipo 2
    int Doenca_Cardiovascular;                          //Doenca Cardiovascular
    int Doenca_de_Huntington;                           //Doenca de Huntington

}ficha;                                                                                            


typedef struct CaracteristicasFisicas{

    int Cor_do_Cabelo;
    int Tipo_de_Cabelo;
    int Cor_da_Pele;
    int Cor_dos_Olhos;

    char peso   [10];
    char altura [10];

}caracfis;

//estrutura com as informacoes de cada um dos 2.                                                   
typedef struct FILHO{                                                                              

    //informacoes basicas.                                                                          

    char nome   [50];                                                                                  
    char idade  [10];
                                                                                    
    //informacoes sobre suas doencas                                                                

    struct FichaTecnica ficha;               
    struct CaracteristicasFisicas caracfis;

}filho;

typedef struct CONJUGUE{
    //informacoes basicas.

    char nome   [50];
    char idade  [10];

    struct FichaTecnica ficha;
    struct CaracteristicasFisicas caracfis;

}conjugue;

//estrutura com as informacoes do casal.
typedef struct{

    char ID[8];                            

    struct FILHO filho;                   //descendecia: filho pertencente a arvore(1), conjugue(0). //(idade,descedencia(pai),nivel).
    struct CONJUGUE conjugue;

}casal;                                    //*pessoa[2] (estrutura casal recebe 2 pessoas(struct pessoa)).

//estrutura da arvore.

typedef struct no {

    casal casal;

    struct no *prox;
    struct no *ant;

    int tam;

}ListaLDE;

typedef struct
{
  char ID[3];
  unsigned short q_registros;
}TCabecalho;

//funcoes de manipulacao de arquivos
// funcoes para escrever e ler os dados dos integrantes em arquivos

short int   cria_pastas         (void);                     //funcao para a criacao de pastas, tipo short int pois o short int ja é mais que o necessario para essa funcao.
short int   verificaPasta       (const char* nomePasta);

//funcoes de manipulacao

int         salvar              (ListaLDE *inicio, ListaLDE *aux, TCabecalho cab);
void        cadastrar           ();
void        recuperar           ();

//funcoes complementares.

void        Adiciona_filho      (casal *C);           
void        Adiciona_conjugue   (casal *C);
short int   Adiciona_par        (casal *C);
void        Adiciona_filho      (casal *C);

int         obterResposta       (const char* resposta);

void PreencheFicha              (void *struct_ptr, int tipo);
void PreencheCaracteristicas    (void *struct_ptr, int tipo);

void limparBuffer();
char* lerResposta(int tamanho, FILE* fluxo);

void gerarID(casal *C);
void salvarID(int idPadrao);
int lerID();
void ancestral(ListaLDE *aux, casal *C);

int main(void){
    int opcao;                                              // Variavel para Escolha das opçoes do menu

    cria_pastas();

/*=========================================================================================================================*/

    do{                                                     // Menu de interação criado para usabilidade dos usuarios
    printf("-----|ARVORE GENEALOGICA.|-----\n");
    printf("Digite a opcao desejada. \n");
    printf("1. Cadastrar Pessoa.\n");
    printf("2. Ver pessoas cadastradas.\n");
    printf("0. Sair.\n");

    scanf("%d",&opcao);

    switch(opcao){
        case 1:
            cadastrar();
            break;
        case 2:
            recuperar();
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
    }
    } while (!opcao); 

/*=========================================================================================================================*/

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
 
void Adiciona_filho(casal *C){
    printf("Por favor adicione os dados:\n");
    limparBuffer();

    printf("Digite o nome: ");

    char* nomeFilho = lerResposta(sizeof(C->filho.nome), stdin);
    strncpy(C->filho.nome, nomeFilho, sizeof(C->filho.nome) - 1);
    C->filho.nome[sizeof(C->filho.nome) - 1] = '\0';  // Garantir que a string tenha o caractere nulo no final

    printf("Digite a idade: ");                         //por idade ser um inteiro, não vou ter uma string de comparacao, então criei uma variavel para armazenar.
    char* idadeFilho = lerResposta(sizeof(C->filho.idade), stdin);
    strncpy(C->filho.idade, idadeFilho, sizeof(C->filho.idade) - 1);
    C->filho.idade[sizeof(C->filho.idade) - 1] = '\0';  // Garantir que a string tenha o caractere nulo no final

}

void Adiciona_conjugue(casal *C){

    limparBuffer();

    printf("Digite o nome: ");

    char* nomeconjugue = lerResposta(sizeof(C->conjugue.nome), stdin);

    strncpy(C->conjugue.nome, nomeconjugue, sizeof(C->conjugue.nome) - 1);
    C->conjugue.nome[sizeof(C->conjugue.nome) - 1] = '\0';  // Garantir que a string tenha o caractere nulo no final

    printf("Digite a idade: ");

    char* idadeconjugue = lerResposta(sizeof(C->conjugue.idade), stdin);

    strncpy(C->conjugue.idade, idadeconjugue, sizeof(C->conjugue.idade) - 1);
    C->conjugue.idade[sizeof(C->conjugue.idade) - 1] = '\0';  // Garantir que a string tenha o caractere nulo no final

}

short int Adiciona_par(casal *C){

    char opcao[5];
    if (C != NULL && C->filho.idade != NULL && *(C->filho.idade) >= 18){                                     // Verifica se o casal existe e se o filho tem idade maior ou igual a 18
        printf("Essa pessoa e casada? \n");
        fgets(opcao, sizeof(opcao), stdin);

        if(opcao[0] == 's' || opcao[0] == 'S'){
            printf("Por favor adicione essa pessoa: \n");
            return 1;                                                   // Retorna 1 para indicar que a pessoa deve ser adicionada
        }
        else{
            char desconhecido[20] = "desconhecido";                                 // Define os valores "desconhecido" para o ID, nome, idade e peso do cônjuge
            strcpy(C->conjugue.nome, desconhecido);
            strcpy(C->conjugue.idade, desconhecido);
        }
    }
    return 0;                                                           // Retorna 0 para indicar que nenhuma ação adicional é necessária
}

int lerID() {
    int idPadrao = 100000; // Valor padrão caso o arquivo não exista

    FILE* arquivo = fopen("id.txt", "r");
    if (arquivo != NULL) {
        fscanf(arquivo, "%d", &idPadrao);
        fclose(arquivo);
    }

    return idPadrao;
}

void salvarID(int idPadrao) {
    FILE* arquivo = fopen("id.txt", "w");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d", idPadrao);
        fclose(arquivo);
    }
}

void gerarID(casal* C) {
    static int idPadrao = 100000;

    if (idPadrao == 100000) {
        idPadrao = lerID();
    }

    char idString[10]; // Tamanho suficiente para armazenar o ID como uma string
    sprintf(idString, "%d", idPadrao); // Converte o ID numérico em uma string

    strcpy(C->ID, idString); // Copia a string do ID para a estrutura

    idPadrao++;
    salvarID(idPadrao);
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

void cadastrar() {

    ListaLDE *inicio = NULL, *aux;
    casal C;
    char tecla;

    limparBuffer();

    /* Cadastro */
    do{

        Adiciona_filho(&C);
        PreencheFicha(&C,1);
        PreencheCaracteristicas(&C, 1);

        if(Adiciona_par(&C)){
            
            Adiciona_conjugue(&C);
            PreencheFicha(&C,2);
            PreencheCaracteristicas(&C, 2);
        }else{
            C.conjugue.ficha.Saudavel = 0;
            C.conjugue.ficha.Sindrome_de_Down = 0;
            C.conjugue.ficha.Sindrome_de_Turner = 0;
            C.conjugue.ficha.Hemofilia = 0;
            C.conjugue.ficha.Diabetes = 0;
            C.conjugue.ficha.Doenca_Cardiovascular = 0;

            C.conjugue.caracfis.Cor_do_Cabelo = 0;
            C.conjugue.caracfis.Tipo_de_Cabelo = 0;
            C.conjugue.caracfis.Cor_da_Pele = 0;
            C.conjugue.caracfis.Cor_dos_Olhos = 0;

            char desconhecido[20] = "desconhecido";                                 // Define os valores "desconhecido" para o ID, nome, idade e peso do cônjuge
            strcpy(C.conjugue.caracfis.peso, desconhecido);
            strcpy(C.conjugue.caracfis.altura, desconhecido);
        }
                   
        gerarID(&C);

        fflush(stdin);

        /* Cria uma lista encadeada */

        if (!inicio) {
            aux = malloc(sizeof(ListaLDE));
            if (!aux) {
                printf("Erro ao alocar memória.\n");
                return;
            }
            aux->casal = C;
            aux->ant = NULL;
            aux->prox = NULL;
        } else {
                ListaLDE *atras = inicio;
                while (atras->prox != NULL) {
                    atras = atras->prox;
                }
                aux = malloc(sizeof(ListaLDE));
                if (!aux) {
                    printf("Erro ao alocar memória.\n");
                    return;
                }
                aux->casal = C;
                aux->ant = atras;
                aux->prox = NULL;
                atras->prox = aux; // Atualizar o ponteiro 'prox' do último nó
            }

        printf("\nDeseja sair (S/N):\t");
        scanf("%c", &tecla);
        getchar();

    } while (toupper(tecla) != 'S');

/*=======================================================================================================================*/
    /* Mostra a lista */

    aux = inicio;

    while (aux) {
        printf("\n----------|PESSOAS|----------");
        printf("|%-15s   %-15s|\n", "FILHO", "CONJUGE");
        printf("--------------------------------\n");

        printf("|%-15s   %-15s|\n", "Nome:", "Nome:");
        printf("|%-15s   %-15s|\n\n", aux->casal.filho.nome, aux->casal.conjugue.nome);

        printf("\n|%-15s   %-15s|\n", "Idade:", "Idade:");
        printf("|%-15s   %-15s|\n\n", aux->casal.filho.idade, aux->casal.conjugue.idade);

        printf("========================================================================\n");
        printf("\n---------|FICHA TECNICA|---------");
        printf("|%-15s   %-15s|\n", "FILHO", "CONJUGE");
        printf("--------------------------------\n");

        printf("|%-15s   %-15s|\n\n", aux->casal.filho.ficha.Saudavel ? "Sim" : "Não", aux->casal.conjugue.ficha.Saudavel ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", aux->casal.filho.ficha.Diabetes ? "Sim" : "Não", aux->casal.conjugue.ficha.Diabetes ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", aux->casal.filho.ficha.Doenca_Cardiovascular ? "Sim" : "Não", aux->casal.conjugue.ficha.Doenca_Cardiovascular ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", aux->casal.filho.ficha.Hemofilia ? "Sim" : "Não", aux->casal.conjugue.ficha.Hemofilia ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", aux->casal.filho.ficha.Doenca_de_Huntington ? "Sim" : "Não", aux->casal.conjugue.ficha.Doenca_de_Huntington ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", aux->casal.filho.ficha.Sindrome_de_Down ? "Sim" : "Não", aux->casal.conjugue.ficha.Sindrome_de_Down ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", aux->casal.filho.ficha.Sindrome_de_Turner ? "Sim" : "Não", aux->casal.conjugue.ficha.Sindrome_de_Turner ? "Sim" : "Não");

        printf("========================================================================\n");
        printf("\n---------|CARACTERISTICAS FISICAS|---------");
        printf("|%-15s   %-15s|\n", "FILHO", "CONJUGE");
        printf("--------------------------------\n");

        printf("|%-15s   %-15s|\n", "Altura:", "Altura:");
        printf("|%-15s   %-15s|\n\n", aux->casal.filho.caracfis.altura, aux->casal.conjugue.caracfis.altura);

        printf("|%-15s   %-15s|\n", "Peso:", "Peso:");
        printf("|%-15s   %-15s|\n\n", aux->casal.filho.caracfis.peso, aux->casal.conjugue.caracfis.peso);

        printf("|%-15s   %-15s|\n", "Cor de Cabelo:", "Cor de cabelo:");
        printf("|%-15d   %-15d|\n\n", aux->casal.filho.caracfis.Cor_do_Cabelo, aux->casal.conjugue.caracfis.Cor_do_Cabelo);

        printf("|%-15s   %-15s|\n", "Tipo de Cabelo:", "Tipo de Cabelo:");
        printf("|%-15d   %-15d|\n\n", aux->casal.filho.caracfis.Tipo_de_Cabelo, aux->casal.conjugue.caracfis.Tipo_de_Cabelo);
        
        printf("|%-15s   %-15s|\n", "Cor de Pele:", "Cor de Pele:");
        printf("|%-15d   %-15d|\n\n", aux->casal.filho.caracfis.Cor_da_Pele, aux->casal.conjugue.caracfis.Cor_da_Pele);

        printf("|%-15s   %-15s|\n", "Cor dos Olhos:", "Cor dos Olhos:");
        printf("|%-15d   %-15d|\n\n", aux->casal.filho.caracfis.Cor_dos_Olhos, aux->casal.conjugue.caracfis.Cor_dos_Olhos);

        ancestral(aux, &C);
        
        aux = aux->prox;
    }
/*=======================================================================================================================*/

    /* Salva a lista em um arquivo */
    TCabecalho cab;
    if (salvar(inicio, aux, cab) == -1) {
        printf("Erro ao salvar os dados.\n");
    }

    /* Libera a memória */
    aux = inicio;
    while (aux) {
        inicio = aux->prox;
        free(aux);
        aux = inicio;
    }
}

void recuperar() {

    TCabecalho cab;
    FILE* file;
    casal* C = NULL;
    int cont;

    file = fopen(NOME_ARQUIVO, "rb");
    if (!file) {
        printf("Arquivo não encontrado.\n");
        return;
    }

    fread(&cab, sizeof(TCabecalho), 1, file);
    if (strcmp(cab.ID, id) != 0) {
        printf("Arquivo inválido.\n");
        fclose(file);
        return;
    }

    C = malloc(sizeof(casal) * cab.q_registros);
    if (!C) {
        printf("Erro ao alocar memória.\n");
        fclose(file);
        return;
    }

    for (cont = 0; cont < cab.q_registros; cont++) {
        fread(&C[cont], sizeof(casal), 1, file);
    }

    fclose(file);

    for (cont = 0; cont < cab.q_registros; cont++) {
       /* printf("\nFILHO:");
        printf("\nNome:\t%s\nIdade:\t%s", C[cont].filho.nome, C[cont].filho.idade);
        printf("\nCONJUGUE:");
        printf("\nNome:\t%s\nIdade:\t%s", C[cont].conjugue.nome, C[cont].conjugue.idade);*/


                printf("\n----------|PESSOAS|----------\n");
        printf("|%-15s   %-15s|\n", "FILHO", "CONJUGE");
        printf("--------------------------------\n");

        printf("|%-15s   %-15s|\n", "Nome:", "Nome:");
        printf("|%-15s   %-15s|\n\n", C[cont].filho.nome, C[cont].conjugue.nome);

        printf("\n|%-15s   %-15s|\n", "Idade:", "Idade:");
        printf("|%-15s   %-15s|\n\n", C[cont].filho.idade, C[cont].conjugue.idade);

        printf("========================================================================\n");
        printf("\n---------|FICHA TECNICA|---------\n");
        printf("|%-15s   %-15s|\n", "FILHO", "CONJUGE");
        printf("--------------------------------\n");

        printf("|%-15s   %-15s|\n\n", C[cont].filho.ficha.Saudavel ? "Sim" : "Não", C[cont].conjugue.ficha.Saudavel ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", C[cont].filho.ficha.Diabetes ? "Sim" : "Não", C[cont].conjugue.ficha.Diabetes ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", C[cont].filho.ficha.Doenca_Cardiovascular ? "Sim" : "Não", C[cont].conjugue.ficha.Doenca_Cardiovascular ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", C[cont].filho.ficha.Hemofilia ? "Sim" : "Não", C[cont].conjugue.ficha.Hemofilia ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", C[cont].filho.ficha.Doenca_de_Huntington ? "Sim" : "Não", C[cont].conjugue.ficha.Doenca_de_Huntington ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", C[cont].filho.ficha.Sindrome_de_Down ? "Sim" : "Não", C[cont].conjugue.ficha.Sindrome_de_Down ? "Sim" : "Não");

        printf("|%-15s   %-15s|\n\n", C[cont].filho.ficha.Sindrome_de_Turner ? "Sim" : "Não", C[cont].conjugue.ficha.Sindrome_de_Turner ? "Sim" : "Não");

        printf("========================================================================\n");
        printf("\n---------|CARACTERISTICAS FISICAS|---------\n");
        printf("|%-15s   %-15s|\n", "FILHO", "CONJUGE");
        printf("--------------------------------\n");

        printf("|%-15s   %-15s|\n", "Altura:", "Altura:");
        printf("|%-15s   %-15s|\n\n", C[cont].filho.caracfis.altura, C[cont].conjugue.caracfis.altura);

        printf("|%-15s   %-15s|\n", "Peso:", "Peso:");
        printf("|%-15s   %-15s|\n\n", C[cont].filho.caracfis.peso, C[cont].conjugue.caracfis.peso);

        printf("|%-15s   %-15s|\n", "Cor de Cabelo:", "Cor de cabelo:");
        printf("|%-15d   %-15d|\n\n", C[cont].filho.caracfis.Cor_do_Cabelo, C[cont].conjugue.caracfis.Cor_do_Cabelo);

        printf("|%-15s   %-15s|\n", "Tipo de Cabelo:", "Tipo de Cabelo:");
        printf("|%-15d   %-15d|\n\n", C[cont].filho.caracfis.Tipo_de_Cabelo, C[cont].conjugue.caracfis.Tipo_de_Cabelo);
        
        printf("|%-15s   %-15s|\n", "Cor de Pele:", "Cor de Pele:");
        printf("|%-15d   %-15d|\n\n", C[cont].filho.caracfis.Cor_da_Pele, C[cont].conjugue.caracfis.Cor_da_Pele);

        printf("|%-15s   %-15s|\n", "Cor dos Olhos:", "Cor dos Olhos:");
        printf("|%-15d   %-15d|\n\n", C[cont].filho.caracfis.Cor_dos_Olhos, C[cont].conjugue.caracfis.Cor_dos_Olhos);

    }

    free(C);
}

int salvar(ListaLDE *inicio, ListaLDE *aux, TCabecalho cab) {
    FILE *arq;
    ListaLDE *p;

    /* Abre o arquivo */
    arq = fopen(NOME_ARQUIVO, "wb+");
    if (!arq) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    /* Salva a lista em um arquivo */
    strcpy(cab.ID, id);
    cab.q_registros = 0;

    /* Conta a quantidade de registros */
    p = inicio;
    while (p) {
        cab.q_registros++;
        p = p->prox;
    }

    /* Primeiro registro do arquivo */
    fwrite(&cab, sizeof(TCabecalho), 1, arq);

    /* Escreve os registros no arquivo */
    aux = inicio;
    while (aux) {
        fwrite(&aux->casal, sizeof(casal), 1, arq);
        aux = aux->prox;
    }

    /* Fecha o arquivo */
    fclose(arq);
    return 0;
}

void PreencheFicha(void *struct_ptr, int tipo) {
    int c ;
    char r[4];
    int saudavel;

    casal *casal_ptr = struct_ptr;
    ficha *ficha_ptr;

    if (tipo == 1) {
        ficha_ptr = &(casal_ptr->filho.ficha);
    } else if (tipo == 2) {
        ficha_ptr = &(casal_ptr->conjugue.ficha);
    } else {
        printf("Tipo inválido\n");
        return;
    }

    printf("\nPreenchimento da Ficha Tecnica\n");

    do {
        printf("\nA pessoa e saudavel? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0';
        int resposta = obterResposta(r);

        if (resposta) {
            ficha_ptr->Saudavel = 1;
            saudavel = 1;
            break;

        } else {
            ficha_ptr->Saudavel = 0;
            break;
            
        }
    } while (1);

    if(saudavel == 0){
        do {
            c = 0;
            printf("\nA pessoa tem Síndrome de Down? ");
            fgets(r, sizeof(r), stdin);
            r[strcspn(r, "\n")] = '\0';
            int resposta = obterResposta(r);
            if (resposta) {
                ficha_ptr->Sindrome_de_Down = 1;
                c = 1;
            } else {
                ficha_ptr->Sindrome_de_Down = 0;
                c = 1;
            }
        } while (c == 0);

        do {
            c = 0;
            printf("\nA pessoa tem Síndrome de Turner? ");
            fgets(r, sizeof(r), stdin);
            r[strcspn(r, "\n")] = '\0';
            int resposta = obterResposta(r);
            if (resposta) {
                ficha_ptr->Sindrome_de_Turner = 1;
                c = 1;
            } else {
                ficha_ptr->Sindrome_de_Turner = 0;
                c = 1;
            }
        } while (c == 0);

        do {
            c = 0;
            printf("\nA pessoa tem Hemofilia? ");
            fgets(r, sizeof(r), stdin);
            r[strcspn(r, "\n")] = '\0';
            int resposta = obterResposta(r);
            if (resposta) {
                ficha_ptr->Hemofilia = 1;
                c = 1;
            } else {
                ficha_ptr->Hemofilia = 0;
                c = 1;
            }
        } while (c == 0);

        do {
            c = 0;
            printf("\nA pessoa tem Diabetes? ");
            fgets(r, sizeof(r), stdin);
            r[strcspn(r, "\n")] = '\0';
            int resposta = obterResposta(r);
            if (resposta) {
                ficha_ptr->Diabetes = 1;
                c = 1;
            } else {
                ficha_ptr->Diabetes = 0;
                c = 1;
            }
        } while (c == 0);

        do {
            c = 0;
            printf("\nA pessoa tem Doença Cardiovascular? ");
            fgets(r, sizeof(r), stdin);
            r[strcspn(r, "\n")] = '\0';
            int resposta = obterResposta(r);
            if (resposta) {
                ficha_ptr->Doenca_Cardiovascular = 1;
                c = 1;
            } else {
                ficha_ptr->Doenca_Cardiovascular = 0;
                c = 1;
            }
        } while (c == 0);

        do {
            c = 0;
            printf("\nA pessoa tem Doença de Huntington? ");
            fgets(r, sizeof(r), stdin);
            r[strcspn(r, "\n")] = '\0';
            int resposta = obterResposta(r);
            if (resposta) {
                ficha_ptr->Doenca_de_Huntington = 1;
                c = 1;
            } else {
                ficha_ptr->Doenca_de_Huntington = 0;
                c = 1;
            }
        } while (c == 0);
    }
    if(saudavel == 1){
    
    ficha_ptr->Sindrome_de_Down = 0;
    ficha_ptr->Sindrome_de_Turner = 0;
    ficha_ptr->Hemofilia = 0;
    ficha_ptr->Diabetes = 0;
    ficha_ptr->Doenca_Cardiovascular = 0;
    }
}

void PreencheCaracteristicas(void *struct_ptr, int tipo) {           // No início da função, é feita uma verificação do tipo para determinar qual aux de estrutura deve ser utilizado para acessar as características físicas.
                                                              
    casal *casal_ptr = struct_ptr;
    caracfis *carac_ptr;

    int c1, c2, c3, c4;

    if (tipo == 1) {
        carac_ptr = &(casal_ptr->filho.caracfis);
    } else if (tipo == 2) {
        carac_ptr = &(casal_ptr->conjugue.caracfis);
    } else {
        printf("Tipo inválido\n");
        return;
    }  

    do{
        printf("\nPreenchimento das Caracteristicas Fisicas\n");
        printf("Qual a cor de cabelo da pessoa?\nCastanho(1)\nLoiro(2)\nPreto(3)\nRuivo(4)\nOutro(5)");

        scanf("%d", &c1);
        getchar();
        
        if(c1 < 1 || c1 > 5){
            printf("\nTipo invalido...\n");
        }

    } while(c1 < 1 || c1 > 5);

    carac_ptr->Cor_do_Cabelo = c1;

    do{
        printf("Qual o tipo de cabelo da pessoa?\nCacheado(1)\nLiso(2)\nOndulado(3)\nOutro(4)");
        scanf("%d", &c2);
        getchar();
        
        if(c2 < 1 || c2 > 4){
            printf("\nTipo invalido...\n");
        }
     } while(c2 < 1 || c2 > 4);

    carac_ptr->Tipo_de_Cabelo = c2;

    do{
        printf("Qual a cor da pele da pessoa?\nBranca(1)\nParda(2)\nPreta(3)\nOutra(4)");
        scanf("%d", &c3);
        getchar();
    
        if(c3 < 1 || c3 > 4){
            printf("\nTipo invalido...\n");
        }
    } while(c3 < 1 || c3 > 4);

    carac_ptr->Cor_da_Pele = c3;

    do{
        printf("Qual a cor dos olhos da pessoa?\nAzul(1)\nCastanho(2)\nPreto(3)\nVerde(4)\nOutro(5)");
         scanf("%d", &c4);
        getchar();
    
        if(c4 < 1 || c4 > 5){
            printf("\nTipo invalido...\n");
        }
    } while(c3 < 1 || c3 > 5);

    carac_ptr->Cor_dos_Olhos = c4;

    printf("Digite o peso: ");
    char* peso = lerResposta(sizeof(carac_ptr->peso), stdin);
    strncpy(carac_ptr->peso, peso, sizeof(carac_ptr->peso) - 1);
    carac_ptr->peso[sizeof(carac_ptr->peso) - 1] = '\0';  // Garantir que a string tenha o caractere nulo no final

    printf("Digite a altura: ");

    char* altura = lerResposta(sizeof(carac_ptr->peso), stdin);

    strncpy(carac_ptr->altura, altura, sizeof(carac_ptr->altura) - 1);
    carac_ptr->altura[sizeof(carac_ptr->altura) - 1] = '\0';  // Garantir que a string tenha o caractere nulo no final
                
}

void ancestral(ListaLDE *aux, casal *C){

    int avoD = aux->ant->casal.filho.ficha.Diabetes;
    int avD = aux->ant->casal.conjugue.ficha.Diabetes;
    int paiD = C->filho.ficha.Diabetes;
    int maeD = C->conjugue.ficha.Diabetes;

    int porcentagemDiabetes = avoD + avD + paiD + maeD;
    if (porcentagemDiabetes > 2) {
        printf("\nPode ser que seu futuro filho tenha Diabetes");
    }

    int avoDC = aux->ant->casal.filho.ficha.Doenca_Cardiovascular;
    int avDC = aux->ant->casal.conjugue.ficha.Doenca_Cardiovascular;
    int paiDC = C->filho.ficha.Doenca_Cardiovascular;
    int maeDC = C->conjugue.ficha.Doenca_Cardiovascular;

    int porcentagemDoencaCardio = avoDC + avDC + paiDC + maeDC;
    if (porcentagemDoencaCardio > 2) {
        printf("\nPode ser que seu futuro filho tenha alguma doença cardiovascular");
    }

    int avoDH = aux->ant->casal.filho.ficha.Doenca_de_Huntington;
    int avDH = aux->ant->casal.conjugue.ficha.Doenca_de_Huntington;
    int paiDH = C->filho.ficha.Doenca_de_Huntington;
    int maeDH = C->conjugue.ficha.Doenca_de_Huntington;

    int porcentagemHuntington = avoDH + avDH + paiDH + maeDH;
    if (porcentagemHuntington > 2) {
        printf("\nPode ser que seu futuro filho tenha Doenca de Huntington");
    }

    int avoh = aux->ant->casal.filho.ficha.Hemofilia;
    int avh = aux->ant->casal.conjugue.ficha.Hemofilia;
    int paih = C->filho.ficha.Hemofilia;
    int maeh = C->conjugue.ficha.Hemofilia;

    int porcentagemHemofilia = (avoh + avh + paih + maeh);
    if(porcentagemHemofilia > 2){
        printf("\nPode ser que seu futuro filho tenha Hemofilia");
    }

    int avosD = aux->ant->casal.filho.ficha.Sindrome_de_Down;
    int avsD  = aux->ant->casal.conjugue.ficha.Sindrome_de_Down;
    int paisD = C->filho.ficha.Sindrome_de_Down;
    int maesD = C->conjugue.ficha.Sindrome_de_Down;

    int porcentagemdown = (avosD + avsD + paisD + maesD);
    if(porcentagemdown > 2){
        printf("\nPode ser que seu futuro filho tenha Sindrome de Down");
    }

    int avost = aux->ant->casal.filho.ficha.Sindrome_de_Turner;
    int avst = aux->ant->casal.conjugue.ficha.Sindrome_de_Turner;
    int paist = C->filho.ficha.Sindrome_de_Turner;
    int maest = C->conjugue.ficha.Sindrome_de_Turner;

    int porcentagemturner = (avost + avst + paist + maest);
    if(porcentagemturner > 2){
        printf("\nPode ser que seu futuro filho tenha Sindrome de Turner");
    }
    
}

int obterResposta(const char* resposta) {

    char respostaMaiuscula[4];
    strncpy(respostaMaiuscula, resposta, 3);
    respostaMaiuscula[3] = '\0';

    for (int i = 0; i < 3; i++) {
        respostaMaiuscula[i] = toupper(respostaMaiuscula[i]);
    }

    if (strcmp(respostaMaiuscula, "SIM") == 0) {
        return 1;
    } else {
        return 0;
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    printf("\nAperte enter para continuar..\n");
}

char* lerResposta(int tamanho, FILE* fluxo) {
    static char resposta[100]; // Variável estática para manter seu valor entre chamadas
    int valido;
    do {
        valido = 0;
        fgets(resposta, tamanho, fluxo);
        resposta[strcspn(resposta, "\n")] = '\0';
        if (strlen(resposta) > 0 && strspn(resposta, " \t") != strlen(resposta)) {
            valido = 1;
        }
    } while (!valido);
    return resposta;
}
