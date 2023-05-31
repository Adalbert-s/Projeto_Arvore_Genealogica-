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

//estrutura das doencas que essa pessoa pode ter.
typedef struct Ficha_Tecnica{                                                      

    int saudavel;                                       //usuario saudavel(nao possui nenhuma doenca)
    //doencas hereditarias
    int Sindrome_de_Down;                               //Sindrome de Down                          //atualmente classificas com IDs de 1000 - 1005 para nao se confundir com os outros.
    int Sindrome_de_Turner;                             //Sindrome de Turner                        //Em breve penso em criar structs com mais informacoes sobre cada doenca.
    int Hemofilia;                                      //Hemofilia
    //doencas nao hereditarias.
    int Diabetes;                                       //Diabetes Tipo 2
    int Doenca_Cardiovascular;                          //Doenca Cardiovascular
                                                
}ficha;                                                                                            //tabela de IDs:        doencas: 1000 - 1005
ficha doencas = {1000, 1001, 1002, 1003, 1004, 1005};   //iniciando a estrutura com valores(IDs).

//estrutura com as informacoes de cada um dos 2.                                                    pessoas: formado por 10 digitos(exemplo: 000.000.000.0):   
typedef struct FILHO{                                                                               // 1 digito(sexo): 1 = homem / 0 = mulher.
                                                                                                 
    //informacoes basicas.                                                                          // 2 digito(estado): 1 = casado / 0 = solteiro.
    char ID[8];
    char nome[50];                                                                                  // 3 digito(idade): 0 = idade < 18 / 1 = 18 > idade < 80 / 2 = idade > 80.
    int idade;
    float peso;                                                                                     // 4-6 digito(pai)/7-9(mae): recebe o id do casal de cima.

    //informacoes sobre suas doencas                                                                // 10 digito(nivel): definine onde que o casal esta na arvore.

    struct Doenca *doenca[1];               //array de ponteiros para apenas 1, ou seja cada pessoa so vai poder ter uma ficha tecnica.
    
}filho;

typedef struct CONJUGUE{
    //informacoes basicas.
    char ID[8];
    char nome[50];
    int idade;
    float peso;
}conjugue;

//estrutura com as informacoes do casal.
typedef struct casal{

    char ID[8];                            //ID do casal vai ter 7 digitos(3 de cada pessoa, 1 de descendencia(ex: 000.000.0)).
    struct FILHO *filho;                   //descendecia: filho pertencente a arvore(1), conjugue(0). //(idade,descedencia(pai),nivel).
    struct CONJUGUE *conjugue;

    struct pessoa *prox;

}casal;                                    //*pessoa[2] (estrutura casal recebe 2 pessoas(struct pessoa)).

//estrutura de no da arvore.
typedef struct no {

    struct casal *casal;
    struct no *esq;
    struct no *dir;
} No;

//funcoes de manipulacao de arquivos
// funcoes para escrever e ler os dados dos integrantes em arquivos

short int cria_pastas           (void);         //funcao para a criacao de pastas, tipo short int pois o short int ja é mais que o necessario para essa funcao.
int escrever_arquivo(casal *C, char opcao);    //funcao para escrever as informacoes em um arquivo binario.
//void ler_arquivo        (pessoa *P);           //funcao para ler os dados da pessoa.

//funcoes de manipulacao da arvore.
void cria(No **t);
int insere(No **t, int dado);

//funcoes complementares.

short int   Adiciona_Pessoa     (casal *C);
void        Adiciona_filho      (casal *C);            //funcao para adicionar as informacoes de um novo integrante,struct pessoa como parametro,passada com &(endereço)para que seja modificada e salva em arquivo.
void        Adiciona_conjugue   (casal *C);
int  Gera_ID                    (casal *C);            //funcao para adicionar um ID para a pessoa.

int main(void){

    //criando a arvore genealogica.
    No *arvoreGenealogica;
    casal   C;
    cria(&arvoreGenealogica);
    
    cria_pastas();
    Adiciona_Pessoa(&C);
    
    printf("-----|ARVORE GENEALOGICA.|-----\n");
}

/*

    Inicializo um no como nulo.

    Essa funcao recebe ponteiro de ponteiro devido a necessitar setar o conteudo de uma variavel
    ponteiro como nulo. Se nao for ponteiro de ponteiro, e fizermos um t = NULL, ela estara setando
    a variavel "t" do ESCOPO DA FUNCAO "CRIA".

*/

void cria(No **t){

    *t = NULL;
}

short int cria_pastas               (void){

    int status;                                 //variavel de status, determina a condicao da criacao das pastas.

    status = mkdir("./Mulheres");               //cria a pasta reservada aos arquivos das mulheres.
    status = mkdir("./Homens");                 //cria a pasta reservada aos arquivos dos homens.
    status = mkdir("./Registro");

    if (status == 0)                            //a funcao mkdir retorna 0 se teve sucesso.
      printf("Pasta criada com sucesso\n");
     else                                       //normalmente a cada vez que reiniciar o programa e as pastas já estiverem crias, ele entrará nesse else.
      printf("Falha ao criar pasta\n");         //eh possivel resolver isso de uma forma mais adequada, mas teria que incluir mais bibliotecas.

    return 1;
}

short int Adiciona_Pessoa       (casal *C){

    char opcao[2];

    do{
        printf("Quem voce deseja adicionar: filho(1) / conjugue(2)?\n");
        fgets(opcao, sizeof(opcao), stdin);
        
        getchar();
        printf("opcao: %s\n",opcao);
        
        if(opcao[0] == '1'){
            Adiciona_filho(C);
            break;
        }
        else if(opcao[0] == '2'){
            Adiciona_conjugue(C);
            break;
        }
        else
            printf("|ERRO! Opcao invalida.");

    }while(1);

    escrever_arquivo(C, opcao[0]);

    return 1;
}

void Adiciona_filho             (casal *C){    

    C->filho = malloc(sizeof(filho)); // Aloca memória para a estrutura filho dentro de casal
    if (C->filho == NULL) {
        // Trate o erro de alocação de memória
        printf("Erro ao alocar memória para filho\n");
        return;
    }        

    printf("Digite o ID do filho: ");                       //usando fgets para ter mais segurança e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma só vez.
    fgets(C->filho->ID, sizeof(C->filho->ID), stdin);  //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->filho->ID[strcspn(C->filho->ID, "\n")] = '\0';       //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string filho.nome.  //evita possiveis problemas na string
    printf("Digite o nome: ");
    fgets(C->filho->nome, sizeof(C->filho->nome), stdin);
    C->filho->nome[strcspn(C->filho->nome, "\n")] = '\0'; 

    printf("Digite a idade: ");
    char input[4];                          //por idade ser um inteiro, não vou ter uma string de comparacao, então criei uma variavel para armazenar. 
    fgets(input, sizeof(input), stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->filho->idade = (int) strtol(input, NULL, 10);// armazenando em filho.idade  //strol(função que transforma string em int), 3 campos(ponteiro para a string, NULL pois vai ser a string inteira, base numerica)

    printf("Digite o peso: ");
    char input_float[6];                    //variavel para armazenar a string que sera lida.
    fgets(input_float, sizeof(input_float), stdin);
    C->filho->peso = strtof(input_float, NULL);    //strtof para transformar em float                      //essa função não recebe o argumento de base numerica.

}

void Adiciona_conjugue          (casal *C){

    C->conjugue = malloc(sizeof(conjugue)); // Aloca memória para a estrutura filho dentro de casal
    if (C->conjugue == NULL) {
        // Trate o erro de alocação de memória
        printf("Erro ao alocar memória para conjugue\n");
        return;
    }  

    printf("Digite o ID: ");                //usando fgets para ter mais segurança e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma só vez.
    fgets(C->conjugue->ID, sizeof((C->conjugue->ID)), stdin); //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->conjugue->ID[strcspn(C->conjugue->ID, "\n")] = '\0';     //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string conjugue.nome.  //evita possiveis problemas na string

    printf("Digite o nome: ");
    fgets(C->conjugue->nome, sizeof(C->conjugue->nome), stdin);
    C->conjugue->nome[strcspn(C->conjugue->nome, "\n")] = '\0'; 

    printf("Digite a idade: ");
    char input[4];                          //por idade ser um inteiro, não vou ter uma string de comparacao, então criei uma variavel para armazenar. 
    fgets(input, sizeof(input), stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->conjugue->idade = (int) strtol(input, NULL, 10);// armazenando em conjugue.idade  //strol(função que transforma string em int), 3 campos(ponteiro para a string, NULL pois vai ser a string inteira, base numerica)

    printf("Digite o peso: ");
    char input_float[6];                    //variavel para armazenar a string que sera lida.
    fgets(input_float, sizeof(input_float), stdin);
    C->conjugue->peso = strtof(input_float, NULL);    //strtof para transformar em float                      //essa função não recebe o argumento de base numerica.

}

int escrever_arquivo(casal *C, char opcao){

    FILE *file;
    char nome_arquivo[50];
    char nome_arquivo_registro[50];

    if(opcao == '1'){

        if(C->filho->ID[0] == '1'){

            // Salvar em arquivo na pasta "Homens"
            sprintf(nome_arquivo, "./Homens/%s.bin", C->filho->ID);

            file = fopen(nome_arquivo, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
                return 0;
            }
            fwrite(&C->filho, sizeof(C->filho), 1, file);

            fclose(file);

            // Salvar em arquivo na pasta "Registro"
            sprintf(nome_arquivo_registro, "./Registro/%s.bin", C->filho->ID);

            file = fopen(nome_arquivo_registro, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
                return 0;
            }
            fwrite(&C->filho, sizeof(C->filho), 1, file);
            fclose(file);

        }
        else if(C->filho->ID[0] == '0'){

             // Salvar em arquivo na pasta "Mulheres"

            sprintf(nome_arquivo, "./Mulheres/%s.bin", C->filho->ID);
            file = fopen(nome_arquivo, "wb");
            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
                return 0;
            }
            fwrite(&C->filho, sizeof(C->filho), 1, file);
            fclose(file);

            // Salvar em arquivo na pasta "Registro"

            sprintf(nome_arquivo_registro, "./Registro/%s.bin", C->filho->ID);

            file = fopen(nome_arquivo_registro, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
                return 0;
            }
            fwrite(&C->filho, sizeof(C->filho), 1, file);
            fclose(file);
        }
    }
    else if(opcao == '2'){

        if(C->conjugue->ID[0] == '1'){

        // Salvar em arquivo na pasta "Homens"

        sprintf(nome_arquivo, "./Homens/%s.bin", C->conjugue->ID);

        file = fopen(nome_arquivo, "wb");

        if (file == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
            return 0;
        }

        fwrite(&C->conjugue, sizeof(C->conjugue), 1, file);
        fclose(file);

        // Salvar em arquivo na pasta "Registro"

        sprintf(nome_arquivo_registro, "./Registro/%s.bin", C->conjugue->ID);
        file = fopen(nome_arquivo_registro, "wb");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
            return 0;
        }
        fwrite(&C->conjugue, sizeof(C->conjugue), 1, file);
        fclose(file);

        }
        else if(C->conjugue->ID[0] == '0'){

            // Salvar em arquivo na pasta "Mulheres"
            sprintf(nome_arquivo, "./Mulheres/%s.bin", C->conjugue->ID);

            file = fopen(nome_arquivo, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
                return 0;
            }
            fwrite(&C->conjugue, sizeof(C->conjugue), 1, file);
            fclose(file);

            // Salvar em arquivo na pasta "Registro"

            sprintf(nome_arquivo_registro, "./Registro/%s.bin", C->conjugue->ID);

            file = fopen(nome_arquivo_registro, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
                return 0;
            }
            fwrite(&C->conjugue, sizeof(C->conjugue), 1, file);

            fclose(file);
        }  
    }
    return 1;
}


