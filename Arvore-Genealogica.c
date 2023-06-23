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

    struct FichaTecnica ficha;               //array de ponteiros para apenas 1, ou seja cada pessoa so vai poder ter uma ficha tecnica.

}filho;

typedef struct CONJUGUE{
    //informacoes basicas.
    char ID     [10];
    char nome   [50];
    char idade  [10];
    char peso   [10];

    struct FichaTecnica ficha;

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
  char ID[3];
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

int salvar(ListaLDE *inicio, ListaLDE *ponteiro, TCabecalho cab);
void cadastrarClientes();
void recuperarClientes();

//funcoes complementares.

short int   Adiciona_Pessoa     ();
void        Adiciona_filho      (casal *C);            //funcao para adicionar as informacoes de um novo integrante,struct pessoa como parametro,passada com &(endereço)para que seja modificada e salva em arquivo.
void        Adiciona_conjugue   (casal *C);
short int   Adiciona_par        (casal *C);
void        Adiciona_filho      (casal *C);
int         Gera_ID             (casal *C);            //funcao para adicionar um ID para a pessoa.
void PreencheFicha(void *struct_ptr, int tipo);

int main(void){

    printf("-----|ARVORE GENEALOGICA.|-----\n");
    cria_pastas();
    
/*=========================================================================================================================*/

    cadastrarClientes();
    recuperarClientes();

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
    char nome_arquivo_registro[50] = "dados";


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

void cadastrarClientes() {
    ListaLDE *inicio = NULL, *ponteiro;
    casal C;
    char tecla;
    
    /* Cadastro */
    do {
        int opcao = Adiciona_Pessoa();

        if(opcao== 1){
            Adiciona_filho(&C);

            if(Adiciona_par(&C)){
                Adiciona_conjugue(&C);
        }
        }else if(opcao == 2){
            Adiciona_conjugue(&C);

        }else if(opcao == 3){
            Adiciona_filho(&C);

    }
        fflush(stdin);

        /* Cria uma lista encadeada */
        if (!inicio) {
            ponteiro = inicio = malloc(sizeof(ListaLDE));
            if (!inicio) {
                printf("Erro ao alocar memória.\n");
                return;
            }
            inicio->casal = C;
            inicio->prox = NULL;
        } else {
            ponteiro = ponteiro->prox = malloc(sizeof(ListaLDE));
            if (!ponteiro) {
                printf("Erro ao alocar memória.\n");
                return;
            }
            ponteiro->casal = C;
            ponteiro->prox = NULL;
        }
        
        printf("\nDeseja sair (S/N):\t");
        scanf("%c", &tecla);
        getchar();
    } while (toupper(tecla) != 'S');
    
    /* Mostra a lista */
    ponteiro = inicio;
    while (ponteiro) {
        printf("\nFILHO:");
        printf("\nNome:\t%s\nIdade:\t%s\nPeso:\t%s", ponteiro->casal.filho.nome, ponteiro->casal.filho.idade, ponteiro->casal.filho.peso);
        printf("\nCONJUGUE:");
        printf("\nNome:\t%s\nIdade:\t%s\nPeso:\t%s", ponteiro->casal.conjugue.nome,ponteiro->casal.conjugue.idade, ponteiro->casal.conjugue.peso);
        ponteiro = ponteiro->prox;
    }
    
    /* Salva a lista em um arquivo */
    TCabecalho cab;
    if (salvar(inicio, ponteiro, cab) == -1) {
        printf("Erro ao salvar os dados.\n");
    }
    
    /* Libera a memória */
    ponteiro = inicio;
    while (ponteiro) {
        inicio = ponteiro->prox;
        free(ponteiro);
        ponteiro = inicio;
    }
}

void recuperarClientes() {

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
        printf("\nFILHO:");
        printf("\nNome:\t%s\nIdade:\t%s\nPeso:\t%s", C[cont].filho.nome, C[cont].filho.idade, C[cont].filho.peso);
        printf("\nCONJUGUE:");
        printf("\nNome:\t%s\nIdade:\t%s\nPeso:\t%s", C[cont].conjugue.nome, C[cont].conjugue.idade, C[cont].conjugue.peso);
    }

    free(C);
}

int salvar(ListaLDE *inicio, ListaLDE *ponteiro, TCabecalho cab) {
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
    ponteiro = inicio;
    while (ponteiro) {
        fwrite(&ponteiro->casal, sizeof(casal), 1, arq);
        ponteiro = ponteiro->prox;
    }

    /* Fecha o arquivo */
    fclose(arq);
    return 0;
}

void PreencheFicha(void *struct_ptr, int tipo) {
    int c;
    char r[4];
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

    do {
        c = 0;
        printf("A pessoa é saudável? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0'; // Remover o caractere '\n' do final da string
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Saudavel = 1;
            c = 1;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);

    do {
        c = 0;
        printf("A pessoa tem Síndrome de Down? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0';
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Sindrome_de_Down = 1;
            c = 1;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);

    do {
        c = 0;
        printf("A pessoa tem Síndrome de Turner? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0';
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Sindrome_de_Turner = 1;
            c = 1;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);

    do {
        c = 0;
        printf("A pessoa tem Hemofilia? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0';
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Hemofilia = 1;
            c = 1;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);

    do {
        c = 0;
        printf("A pessoa tem Diabetes? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0';
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Diabetes = 1;
            c = 1;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);

    do {
        c = 0;
        printf("A pessoa tem Doença Cardiovascular? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0';
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Doenca_Cardiovascular = 1;
            c = 1;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);

    do {
        c = 0;
        printf("A pessoa tem Doença de Huntington? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0';
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Doenca_de_Huntington = 1;
            c = 1;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);
}
