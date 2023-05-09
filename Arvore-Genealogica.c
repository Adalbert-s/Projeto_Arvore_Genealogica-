/*
objetivo:
Arvore genealogia que detecta possiveis eventuais doencas que os filhos possam ter.

criadores:
Adalberto e Diogo.
*/

#include <stdio.h>
#include <stdlib.h>
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
ficha doencas = {1000, 1001, 1002, 1003, 1004, 1005};  //iniciando a estrutura com valores(IDs).

//estrutura com as informacoes de cada um dos 2.                                                    pessoas: formado por 10 digitos(exemplo: 000.000.000.0):   
typedef struct PESSOA{                                                                              // 1 digito(sexo): 1 = homem / 0 = mulher.
                                                                                                 
    //informacoes basicas.                                                                          // 2 digito(estado): 1 = casado / 0 = solteiro.
    char ID[7];
    char nome[50];                                                                                  // 3 digito(idade): 0 = idade < 18 / 1 = 18 > idade < 80 / 2 = idade > 80.
    int idade;
    float peso;                                                                                     // 4-6 digito(pai)/7-9(mae): recebe o id do casal de cima.

    //informacoes sobre suas doencas                                                                // 10 digito(nivel): definine onde que o casal esta na arvore.

    struct Doenca *doenca[1];               //array de ponteiros para apenas 1, ou seja cada pessoa so vai poder ter uma ficha tecnica.
    
}pessoa;

//estrutura com as informacoes do casal.
typedef struct casal{
    char ID[8];                            //ID do casal vai ter 7 digitos(3 de cada pessoa, 1 de descendencia(ex: 000.000.0)).
    struct pessoa *pessoa[2];              //descendecia: filho pertencente a arvore(1), conjuge(0). //(idade,descedencia(pai),nivel).
                                                                                                 
}casal;                                    //*pessoa[2] (estrutura casal recebe 2 pessoas(struct pessoa)).

//estrutura de no da arvore.
typedef struct no {

    struct casal *casal;
    struct no *esq;
    struct no *dir;
} No;

//funcoes de manipulacao de arquivos
// funcoes para escrever e ler os dados dos integrantes em arquivos

void escrever_arquivo   (pessoa *P);            //funcao para escrever as informacoes em um arquivo binario.
//void ler_arquivo        (pessoa *P);        //funcao para ler os dados da pessoa.

//funcoes de manipulacao da arvore.
void cria(No **t);
int insere(No **t, int dado);

//funcoes complementares.
void adiciona_pessoa    (pessoa *P);            //funcao para adicionar as informacoes de um novo integrante.
int  adiciona_ID        (pessoa *P);            //funcao para adicionar um ID para a pessoa.

int main(void){

    //criando a arvore genealogica.
    No *arvoreGenealogica;
    pessoa P;
    cria(&arvoreGenealogica);
    
    adiciona_pessoa(&P);
    escrever_arquivo(&P);
    
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

void escrever_arquivo(pessoa *P){
    printf("lido: %s\n", P->ID);
    printf("lido: %s\n", P->nome);
    printf("lido: %d\n", P->idade);
    printf("lido: %f\n", P->peso);
    FILE *file;
    char nome_arquivo[50];

    sprintf(nome_arquivo, "%s.bin",P->ID);

    file = fopen(nome_arquivo, "wb");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return;
    }
    fwrite(&P, sizeof(pessoa), 1, file);

    fclose(file);
}

void adiciona_pessoa(pessoa *P){            //chama a struct pessoa como parametro, ela é passada com &(endereço)para que seja modificada e salva em arquivo.

    printf("Digite o ID: ");                //usando fgets para ter mais segurança e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma só vez.
    fgets(P->ID, sizeof(P->ID + 1), stdin); //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    P->ID[strcspn(P->ID, "\n")] = '\0';     //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string P.nome.  //evita possiveis problemas na string

    printf("Digite o nome: ");
    fgets(P->nome, sizeof(P->nome), stdin);
    P->nome[strcspn(P->nome, "\n")] = '\0'; 

    printf("Digite a idade: ");
    char input[4];                          //por idade ser um inteiro, não vou ter uma string de comparacao, então criei uma variavel para armazenar. 
    fgets(input, sizeof(input), stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    P->idade = (int) strtol(input, NULL, 10);// armazenando em P.idade  //strol(função que transforma string em int), 3 campos(ponteiro para a string, NULL pois vai ser a string inteira, base numerica)

    printf("Digite o peso: ");
    char input_float[6];                    //variavel para armazenar a string que sera lida.
    fgets(input_float, sizeof(input_float), stdin);
    P->peso = strtof(input_float, NULL);    //strtof para transformar em float                      //essa função não recebe o argumento de base numerica.
}

//pelo amor de deus não muda nada nessa função, gastei muito tempo resolvendo bugs dela e no final era só colocar "+ 1" no "sizeof(P->ID)"
//se precisar saber algo a mais sobre ela me avisa que eu explico