#include "include.h"
//estrutura de no da arvore.
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
    char ID[7];
    char nome[50];                                                                                  // 3 digito(idade): 0 = idade < 18 / 1 = 18 > idade < 80 / 2 = idade > 80.
    int idade;
    float peso;                                                                                     // 4-6 digito(pai)/7-9(mae): recebe o id do casal de cima.

    //informacoes sobre suas doencas                                                                // 10 digito(nivel): definine onde que o casal esta na arvore.

    struct Doenca *doenca[1];               //array de ponteiros para apenas 1, ou seja cada pessoa so vai poder ter uma ficha tecnica.

}filho;

typedef struct CONJUGUE{
    //informacoes basicas.
    char ID[7];
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

}casal;

/*

    Inicializo um no como nulo.

    Essa funcao recebe ponteiro de ponteiro devido a necessitar setar o conteudo de uma variavel
    ponteiro como nulo. Se nao for ponteiro de ponteiro, e fizermos um t = NULL, ela estara setando
    a variavel "t" do ESCOPO DA FUNCAO "CRIA".

*/

//funcoes de manipulacao de arquivos
// funcoes para escrever e ler os dados dos integrantes em arquivos

short int cria_pastas           (void);         //funcao para a criacao de pastas, tipo short int pois o short int ja � mais que o necessario para essa funcao.
void escrever_arquivo(casal *C, char opcao);    //funcao para escrever as informacoes em um arquivo binario.
//void ler_arquivo        (pessoa *P);           //funcao para ler os dados da pessoa.

//funcoes complementares.

short int   Adiciona_Pessoa     (casal *C);
void        Adiciona_filho      (casal *C);            //funcao para adicionar as informacoes de um novo integrante,struct pessoa como parametro,passada com &(endere�o)para que seja modificada e salva em arquivo.
void        Adiciona_conjugue   (casal *C);
int  Gera_ID                    (casal *C);            //funcao para adicionar um ID para a pessoa.

short int cria_pastas               (void){

    int status;                                 //variavel de status, determina a condicao da criacao das pastas.

    status = mkdir("./Mulheres");               //cria a pasta reservada aos arquivos das mulheres.
    status = mkdir("./Homens");                 //cria a pasta reservada aos arquivos dos homens.
    status = mkdir("./Registro");

    if (status == 0)                            //a funcao mkdir retorna 0 se teve sucesso.
      printf("Pasta criada com sucesso\n");
     else                                       //normalmente a cada vez que reiniciar o programa e as pastas j� estiverem crias, ele entrar� nesse else.
      printf("Falha ao criar pasta\n");         //eh possivel resolver isso de uma forma mais adequada, mas teria que incluir mais bibliotecas.

    return 0;
}

short int Adiciona_Pessoa       (casal *C){

    char opcao;

    do{
        printf("Quem voce deseja adicionar: filho(1) / conjugue(2)?\n");
        fgets(&opcao, sizeof(opcao), stdin);


            //Adiciona_filho(C);

        if(opcao == '2')
            Adiciona_conjugue(C);
        else
            printf("|ERRO! Opcao invalida.");

    }while((opcao != '1') && (opcao != '2'));

    escrever_arquivo(C, opcao);

    return 0;
}

void Adiciona_filho             (casal *C){

    printf("Digite o ID: ");                //usando fgets para ter mais seguran�a e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma s� vez.
    fgets(C->filho->ID, sizeof((C->filho->ID) + 1), stdin); //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->filho->ID[strcspn(C->filho->ID, "\n")] = '\0';     //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string filho.nome.  //evita possiveis problemas na string

    printf("Digite o nome: ");
    fgets(C->filho->nome, sizeof(C->filho->nome), stdin);
    C->filho->nome[strcspn(C->filho->nome, "\n")] = '\0';

    printf("Digite a idade: ");
    char input[4];                          //por idade ser um inteiro, n�o vou ter uma string de comparacao, ent�o criei uma variavel para armazenar.
    fgets(input, sizeof(input), stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->filho->idade = (int) strtol(input, NULL, 10);// armazenando em filho.idade  //strol(fun��o que transforma string em int), 3 campos(ponteiro para a string, NULL pois vai ser a string inteira, base numerica)

    printf("Digite o peso: ");
    char input_float[6];                    //variavel para armazenar a string que sera lida.
    fgets(input_float, sizeof(input_float), stdin);
    C->filho->peso = strtof(input_float, NULL);    //strtof para transformar em float                      //essa fun��o n�o recebe o argumento de base numerica.

}

void Adiciona_conjugue          (casal *C){

    printf("Digite o ID: ");                //usando fgets para ter mais seguran�a e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma s� vez.
    fgets(C->conjugue->ID, sizeof((C->conjugue->ID) + 1), stdin); //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->conjugue->ID[strcspn(C->conjugue->ID, "\n")] = '\0';     //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string conjugue.nome.  //evita possiveis problemas na string

    printf("Digite o nome: ");
    fgets(C->conjugue->nome, sizeof(C->conjugue->nome), stdin);
    C->conjugue->nome[strcspn(C->conjugue->nome, "\n")] = '\0';

    printf("Digite a idade: ");
    char input[4];                          //por idade ser um inteiro, n�o vou ter uma string de comparacao, ent�o criei uma variavel para armazenar.
    fgets(input, sizeof(input), stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->conjugue->idade = (int) strtol(input, NULL, 10);// armazenando em conjugue.idade  //strol(fun��o que transforma string em int), 3 campos(ponteiro para a string, NULL pois vai ser a string inteira, base numerica)

    printf("Digite o peso: ");
    char input_float[6];                    //variavel para armazenar a string que sera lida.
    fgets(input_float, sizeof(input_float), stdin);
    C->conjugue->peso = strtof(input_float, NULL);    //strtof para transformar em float                      //essa fun��o n�o recebe o argumento de base numerica.

}

void escrever_arquivo(casal *C, char opcao){

    printf("lido: %s\n", C->filho->ID);
    printf("lido: %s\n", C->filho->nome);
    printf("lido: %d\n", C->filho->idade);
    printf("lido: %f\n", C->filho->peso);

    FILE *file;
    char nome_arquivo[50];

    if(opcao == '1'){

        if(C->filho->ID[0] == '1'){

            sprintf(nome_arquivo, "./Homens/%s.bin", C->filho->ID);
            sprintf(nome_arquivo, "./Registro/%s.bin", C->filho->ID);

            file = fopen(nome_arquivo, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
                return;
            }
            fwrite(&C->filho, sizeof(C->filho), 1, file);

            fclose(file);
        }
        else if(C->filho->ID[0] == '0'){

            sprintf(nome_arquivo, "./Mulheres/%s.bin",C->filho->ID);
            sprintf(nome_arquivo, "./Registro/%s.bin", C->filho->ID);

            file = fopen(nome_arquivo, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
                return;
            }
            fwrite(&C->filho, sizeof(C->filho), 1, file);

            fclose(file);
        }
    }
    else if(opcao == '2'){

        if(C->conjugue->ID[0] == '1'){

            sprintf(nome_arquivo, "./Homens/%s.bin",C->conjugue->ID);
            sprintf(nome_arquivo, "./Registro/%s.bin", C->conjugue->ID);

            file = fopen(nome_arquivo, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
                return;
            }
            fwrite(&C->conjugue, sizeof(C->conjugue), 1, file);

            fclose(file);
        }
        else if(C->conjugue->ID[0] == '0'){

            sprintf(nome_arquivo, "./Mulheres/%s.bin",C->conjugue->ID);
            sprintf(nome_arquivo, "./Registro/%s.bin", C->conjugue->ID);

            file = fopen(nome_arquivo, "wb");

            if (file == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
                return;
            }
            fwrite(&C->conjugue, sizeof(C->conjugue), 1, file);

            fclose(file);
        }
    }
}


