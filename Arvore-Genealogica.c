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

typedef struct CaracteristicasFisicas{

    int Cor_do_Cabelo;
    int Tipo_de_Cabelo;

    int Cor_da_Pele;

    int Cor_dos_Olhos;
}caracfis;

//estrutura com as informacoes de cada um dos 2.                                                    pessoas: formado por 10 digitos(exemplo: 000.000.000.0):
typedef struct FILHO{                                                                               // 1 digito(sexo): 1 = homem / 0 = mulher.

    //informacoes basicas.                                                                          // 2 digito(estado): 1 = casado / 0 = solteiro.
    char ID     [10];
    char nome   [50];                                                                                  // 3 digito(idade): 0 = idade < 18 / 1 = 18 > idade < 80 / 2 = idade > 80.
    char idade  [10];
    char peso   [10];                                                                                     // 4-6 digito(pai)/7-9(mae): recebe o id do casal de cima.

    //informacoes sobre suas doencas                                                                // 10 digito(nivel): definine onde que o casal esta na arvore.

    struct FichaTecnica ficha;               //array de ponteiros para apenas 1, ou seja cada pessoa so vai poder ter uma ficha tecnica.
    struct CaracteristicasFisicas caracfis;

}filho;

typedef struct CONJUGUE{
    //informacoes basicas.
    char ID     [10];
    char nome   [50];
    char idade  [10];
    char peso   [10];

    struct FichaTecnica ficha;
    struct CaracteristicasFisicas caracfis;

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
void PreencheCaracteristicas(void *struct_ptr, int tipo);

int main(void){
    int opcao;                                              // Variavel para Escolha das opçoes do menu

    do{                                                     // Menu de interação criado para usabilidade dos usuarios 
    printf("-----|ARVORE GENEALOGICA.|-----\n");
    printf("Digite a opcao desejada. \n");
    printf("1. Cadastrar Pessoa.\n");
    printf("2. Ver pessoas cadastradas.\n");
    printf("0. Sair.\n");
    scanf("%d",&opcao);

    switch(opcao){
        case 1:
            cadastrarClientes();
            break;
        case 2:
            recuperarClientes();
            break;
        case 0:
            printf("Saindo...\n");
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
    }
} while (opcao != 0);                                           // Condição de parada utlizada caso a pessoa escolha Sair
    return 0;


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
            break;                      // Sai do loop se a opção for 1 (filho)
        }
        else if(opcao == 2){
            printf("Digite o ID da pessoa com que essa é casada: \n");
            fgets(nome, sizeof(nome), stdin);                                   // Lê o ID da pessoa com que esta é casada a partir da entrada padrão
            printf("Procurando pessoa no sistema...\n");

            if(existe_pessoa(nome)){                                            // Verifica se a pessoa com o ID fornecido já existe no sistema
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

    return opcao;                                                            // Retorna a opção escolhida pelo usuário
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
    if(C != NULL && C->filho.idade != NULL && C->filho.idade >= 18){                                     // Verifica se o casal existe e se o filho tem idade maior ou igual a 18
        printf("Essa pessoa e casada? \n");
        fgets(opcao, sizeof(opcao), stdin);

        if(opcao[0] == 's' || opcao[0] == 'S'){
            printf("Por favor adicione essa pessoa: \n");
            return 1;                                                   // Retorna 1 para indicar que a pessoa deve ser adicionada
        }
        else{
            char desconhecido[20] = "desconhecido";                                 // Define os valores "desconhecido" para o ID, nome, idade e peso do cônjuge
            strcpy(C->conjugue.ID, desconhecido);
            strcpy(C->conjugue.nome, desconhecido);
            strcpy(C->conjugue.idade, desconhecido);
            strcpy(C->conjugue.peso, desconhecido);
        }
    }
    return 0;                                                           // Retorna 0 para indicar que nenhuma ação adicional é necessária
}

int escrever_arquivo(casal *C) {
    FILE *file;
    char nome_arquivo_registro[50] = "dados";


    file = fopen(nome_arquivo_registro, "w+");                      // Abre o arquivo para escrita e leitura (cria o arquivo se não existir)

    if (file == NULL) {                                                 // Verifica se houve um erro ao abrir o arquivo
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
        return 0;                                                           // Retorna 0 para indicar que houve um erro ao abrir o arquivo
    }

    printf("Digite o ID do filho: ");
    fgets(C->filho.ID, sizeof(C->filho.ID) - 1, stdin);                     //utiliza a função fgets para ler uma linha de entrada a partir do fluxo de entrada padrão (stdin) e armazena essa linha no campo ID da estrutura filho.   //O tamanho máximo da string lida é limitado pelo valor sizeof(C->filho.ID) - 1 para evitar o estouro de buffer.
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

    printf("\nPreenchimento da Ficha Tecnica\n");
    do {
        c = 0;
        printf("\nA pessoa é saudável? ");
        fgets(r, sizeof(r), stdin);
        r[strcspn(r, "\n")] = '\0'; // Remover o caractere '\n' do final da string
        for (unsigned int R = 0; R < strlen(r); R++) {
            r[R] = toupper(r[R]);
        }
        if (strcmp(r, "SIM") == 0) {
            ficha_ptr->Saudavel = 1;
            c = 1;
            return;
        } else if (strcmp(r, "NAO") == 0) {
            c = 1;
        }
    } while (c == 0);

    do {
        c = 0;
        printf("\nA pessoa tem Síndrome de Down? ");
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
        printf("\nA pessoa tem Síndrome de Turner? ");
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
        printf("\nA pessoa tem Hemofilia? ");
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
        printf("\nA pessoa tem Diabetes? ");
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
        printf("\nA pessoa tem Doença Cardiovascular? ");
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
        printf("\nA pessoa tem Doença de Huntington? ");
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

void PreencheCaracteristicas(void *struct_ptr, int tipo) {

    casal *casal_ptr = struct_ptr;
    ficha *carac_ptr;
    int c1, c2, c3, t1, r1;

    if (tipo == 1) {
        carac_ptr = &(casal_ptr->filho.ficha);
    } else if (tipo == 2) {
        carac_ptr = &(casal_ptr->conjugue.ficha);
    } else {
        printf("Tipo inválido\n");
        return;
    }

    do{
        printf("\nPreenchimento das Caracteristicas Fisicas\n");
        printf("Qual a cor de cabelo da pessoa?\nCastanho(1)\nLoiro(2)\nPreto(3)\nRuivo(4)\nOutro(5)");
        scanf("%d", &c1);
        r1 = 0;
        switch(c1){
            case 1:
                r1 = 1;
                break;
            case 2:
                r1 = 1;
                break;
            case 3:
                r1 = 1;
                break;
            case 4:
                r1 = 1;
                break;
            case 5:
                r1 = 1;
                break;
            default: printf("\nCor invalida...\n");
        }
    }while(r1 == 0);
/*TA AQUI--------------------------------------*/
   // carac_ptr->Cor_do_Cabelo = r1;
    
/*---------------------------------------------*/
    do{
        printf("Qual o tipo de cabelo da pessoa?\nCacheado(1)\nLiso(2)\nOndulado(3)\nOutro(4)");
        scanf("%d", &t1);
        r1 = 0;
        switch(t1){
            case 1:
                r1 = 1;
                break;
            case 2:
                r1 = 1;
                break;
            case 3:
                r1 = 1;
                break;
            case 4:
                r1 = 1;
                break;
            default: printf("\nTipo invalido...\n");
        }
    }while(r1 == 0);

    do{
        printf("Qual a cor da pele da pessoa?\nBranca(1)\nParda(2)\nPreta(3)\nOutra(4)");
        scanf("%d", &c2);
        r1 = 0;
        switch(c2){
            case 1:
                r1 = 1;
                break;
            case 2:
                r1 = 1;
                break;
            case 3:
                r1 = 1;
                break;
            case 4:
                r1 = 1;
                break;
            default: printf("\nCor invalida...\n");
        }
    }while(r1 == 0);

    do{
        printf("Qual a cor dos olhos da pessoa?\nAzul(1)\nCastanho(2)\nPreto(3)\nVerde(4)\nOutro(5)");
        scanf("%d", &c3);
        r1 = 0;
        switch(c3){
            case 1:
                r1 = 1;
                break;
            case 2:
                r1 = 1;
                break;
            case 3:
                r1 = 1;
                break;
            case 4:
                r1 = 1;
                break;
            case 5:
                r1 = 1;
                break;
            default: printf("\nCor invalida...\n");
        }
    }while(r1 == 0);

}
/*coisa ainda para finalizar:

{funcao de caracteristicas fisicas:

-vai ser parecida, senão igual a da ficha tecnica, só tirar o if da pessoa saldavel e continuar as perguntas da mesma forma(com 0s, 1s ou outro valor)
coloca uns 3 tipos de cor de cabelo, sendo o 3 como "outros" assim como olhos, pele. n precisa colocar altura pq vou colocar na struct de ficha tecnica
} Diogo

-criar a função para mostrar,tanto a da ficha tecnica, quanto a de caracteristicas, essa função vai seguir a logica de apenas mostrar as coisas que a pessoa tem,
com ifs fazendo uma logica de que se for 1 mostra ela e se for 0, n mostra, a parte de salvar em arquivo deixa comigo, só lembra de fazer com que a função não deixe
nenhum campo vazio, se n dá pau no arquivo, tem uma logica parecida na função "adiciona par", só olhar e vê que eu preencho com a palavra "desconhecido".

funçao que cria ids:

- função simples que vai colocar ids nas pessoas, só criar uma função void, com os parametros da struct casal(ela n está no main, está na função preenche filha)
(lembra de colocar a struct com "&" na chamada da função), o sistema de id vai ser simples, primeira pessoa vai ser o 1  a segunda 2 ..., e preenche de 0s o resto
do char[0].(provavelmente vai precisar usar arquivo, então se não for ser eu que fazer, só faz o principio dela, deixa ela salvando os ids normalmente, que dps eu
acho uma forma de salvar isso no arquivo.

funcao de menu:

- função só para mostra o menu, o main não tem nd declarado, então poderia usar uma função menu tranquilo e só ficar ela lá, o menu vai ter as opcoes de cadrastar
pessoa, puxando a função de cadastrar(já feita), e a funcão de mostrar as pessoas salvas(puxando a outra função que tá lá), ao mostrar a pessoa, perguntar se ela
deseja ver as informações da pessoa(perguntar se é o filho ou o conjugue) e mostrar a ficha tecnica dele, (basicamente printf de c->filho.ficha ou do conjugue)
e por fim uma pergunta se ela deseja voltar para o menu 1(o que vai ter as opcoes de cadrastar, ver, sair do programa)

corrigir bugs(deixa comigo essa parte):

arrumar a leitura das informações
warning da função adiciona par

e por fim preciso implementar o ponteiro anterior da lista e a função de pastas
e como vou ser um dos ultimos a terminar vou tentar criar funções sobre saude, que vai dar as chances do filho do casal ter isso ou aquilo...

(nao esquecem de criar as tarefas no projeto, atribuir como issues e colocar vcs como responsaveis, e dps finalizar issues trazendo ela para o ultimo campo,
pq se não o claudio n vê que foram vcs que fizeram e só fica eu como que fez mais de 10 tarefas lá)


e por fim testa as funções pq mais tarde eu vou só finalizar meus bugs, adicionar alguma coisa ali ou aqui, e não vai dar para testar tudo.
*/
