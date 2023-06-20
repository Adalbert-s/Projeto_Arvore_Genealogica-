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
typedef struct Casal{

    char ID[8];                            //ID do casal vai ter 7 digitos(3 de cada pessoa, 1 de descendencia(ex: 000.000.0)).
    struct FILHO *filho;                   //descendecia: filho pertencente a arvore(1), conjugue(0). //(idade,descedencia(pai),nivel).
    struct CONJUGUE *conjugue;
    struct Casal* herdeiro;

}casal;                                    //*pessoa[2] (estrutura casal recebe 2 pessoas(struct pessoa)).

//estrutura da arvore em grafos.

typedef struct noAdjacente {
    struct no* vertice;
    struct noAdjacente* prox;
} NoAdjacente;

typedef struct ListaNoAdjacente{

    int tam;
    NoAdjacente *inicio;
    NoAdjacente *fim;

} ListaNoAdjacente;


typedef struct no {

    char ID_CASAL[8];
    casal *casal;

    ListaNoAdjacente nosAdjacentes;
    struct no *prox;

}No;

typedef struct {

    int tam;
    No *inicio;
    No *fim;

} Grafo;

//funcoes de manipulacao de arquivos
// funcoes para escrever e ler os dados dos integrantes em arquivos

short int   cria_pastas         (void);                     //funcao para a criacao de pastas, tipo short int pois o short int ja é mais que o necessario para essa funcao.
int         escrever_arquivo    (casal *C, char opcao);     //funcao para escrever as informacoes em um arquivo binario.
short int   verificaPasta       (const char* nomePasta);
short int   existe_pessoa       (const char *nome_arquivo_registro);
//void        ler_arquivo         (const char* nomePasta, char nome_arquivo_registro);            //funcao para ler os dados da pessoa.

//funcoes de manipulacao do grafo.

Grafo*      criarGrafo          ();
short int   adicionarNo         (Grafo* grafo, char ID_CASAL[8], casal *C);
void        criarConexao        (Grafo* grafo, char ID_CASAL1[8], char ID_CASAL2[8]);
void        salvarGrafo         (Grafo* grafo, const char* nomeArquivo);

No*         encontrarNo         (Grafo* grafo, char ID_CASAL[8]);
void        percorrerGrafo      (Grafo* grafo, FILE* arquivo);
void        imprimirGrafo       (Grafo* grafo);

NoAdjacente* criarNoAdjacente   (No* no);
void        adicionarNoAdjacente(ListaNoAdjacente* lista, NoAdjacente* noAdjacente);

//funcoes complementares.

short int   Adiciona_Pessoa     (casal *C);
void        Adiciona_filho      (casal *C);            //funcao para adicionar as informacoes de um novo integrante,struct pessoa como parametro,passada com &(endereço)para que seja modificada e salva em arquivo.
void        Adiciona_conjugue   (casal *C);
void        Adiciona_par        (casal *C);
void        Adiciona_filho      (casal *C);
int         Gera_ID             (casal *C);            //funcao para adicionar um ID para a pessoa.

int main(void){

    Grafo* arvore = criarGrafo();                     //cria o grafo
    casal   C;                                          //cria a struct casal

    cria_pastas();

    // Verifica se a criação do grafo foi bem-sucedida

    if (arvore == NULL) {
        printf("Falha ao criar o grafo.\n");
        return 1;                                       // Termina o programa com código de erro
    }

    Adiciona_Pessoa(&C);
    char ID_CASAL[8] = "101101";
    adicionarNo(arvore, ID_CASAL,&C);

    printf("-----|ARVORE GENEALOGICA.|-----\n");
    salvarGrafo(arvore, "arvore");

    return 0;
}

short int cria_pastas(void){

    int status;                                 //variavel de status, determina a condicao da criacao das pastas.

    if(!verificaPasta("./Mulheres")){
        status = mkdir("./Mulheres");               //cria a pasta reservada aos arquivos das mulheres.
        if(status == 0){
            printf("Pasta 'Mulheres' criada com sucesso.\n");
        }
        else{
            printf("Falha ao criar a pasta 'Mulheres'.\n");
        }
    }
    else{
        printf("A pasta 'Mulheres' ja existe.\n");
    }

    if(!verificaPasta("./Homens")){

        status = mkdir("./Homens");                 //cria a pasta reservada aos arquivos dos homens.
            if(status == 0){
                printf("Pasta 'Homens' criada com sucesso.\n");
            }
            else{
                printf("Falha ao criar a pasta Homens.\n");
            }
    }
    else{
        printf("A pasta 'Homens' ja existe.\n");
    }

    if(!verificaPasta("./Registro")) {

        status = mkdir("./Registro");
        if (status == 0)
            printf("Pasta 'Registro' criada com sucesso.\n");
        else
            printf("Falha ao criar a pasta 'Registro'.\n");
    } else {
        printf("A pasta 'Registro' ja existe.\n");
    }

    if(!verificaPasta("./Grafo")) {

        status = mkdir("./Grafo");
        if (status == 0)
            printf("Pasta 'Grafo' criada com sucesso.\n");
        else
            printf("Falha ao criar a pasta 'Grafo'.\n");
    } else {
        printf("A pasta 'Grafo' ja existe.\n");
    }

    return 1;
}

short int Adiciona_Pessoa(casal *C){

    char opcao[2];
    char op[2];
    char nome [8];

    do{
        printf("Quem voce deseja adicionar: filho(1) / conjugue(2)?\n");
        fgets(opcao, sizeof(opcao), stdin);

        getchar();
        printf("opcao: %s\n",opcao);

        if(opcao[0] == '1'){
            Adiciona_filho(C);
            Adiciona_par(C);
            break;
        }
        else if(opcao[0] == '2'){
            printf("Digite o ID da pessoa com que essa é casada: \n");
            fgets(nome, sizeof(nome), stdin);
            printf("Procurando pessoa no sistema...\n");

            if(existe_pessoa(nome)){
                printf("Pessoa encontrada com sucesso...\n");
                Adiciona_conjugue(C);
                break;
            }
            else{
                printf("Pessoa nao encontrada, deseja salvar? \n");
                fgets(op, sizeof(op), stdin);
                if(op[0] == 's' || op[0] == 'S'){
                    Adiciona_filho(C);
                    break;
                }
                else
                printf("Certo, Saindo do programa...\n");
                return 0;
            }
            break;
        }
        else
            printf("| ERRO! Opcao invalida.");

    }while(1);

    escrever_arquivo(C, opcao[0]);

    return 1;
}

void Adiciona_filho(casal *C){

    C->filho = malloc(sizeof(filho)); // Aloca memória para a estrutura filho dentro de casal
    if (C->filho == NULL) {
        // Trate o erro de alocação de memória
        printf("Erro ao alocar memória para filho\n");
        return;
    }

    printf("Digite o ID do filho: ");                       //usando fgets para ter mais segurança e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma só vez.
    fgets(C->filho->ID, sizeof(C->filho->ID), stdin);       //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
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

void Adiciona_conjugue(casal *C){

    C->conjugue = malloc(sizeof(conjugue)); // Aloca memória para a estrutura filho dentro de casal
    if (C->conjugue == NULL) {
        // Trate o erro de alocação de memória
        printf("Erro ao alocar memória para conjugue\n");
        return;
    }

    printf("Digite o ID: ");                //usando fgets para ter mais segurança e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma só vez.
    fgets(C->conjugue->ID, sizeof((C->conjugue->ID)), stdin);   //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
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
/*
void Adiciona_ficha_tecnica(ficha *C){

    C->ficha = malloc(sizeof(ficha));
    if (C->ficha == NULL) {
        printf("Erro ao alocar memória para a ficha tecnica\n");
        return;
    }

    printf("Digite (1) se for saudavel ou (2) se tiver alguma doenca: ");
    if(
    fgets(C->conjugue->ID, sizeof((C->conjugue->ID)), stdin);   //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
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
*/
void Adiciona_par(casal *C){

    char opcao[2];

    if(C != NULL && C->filho != NULL && C->filho->idade >= 18){
        printf("Essa pessoa e casada? \n");
        fgets(opcao, sizeof(opcao), stdin);

        if(opcao[0] == 's' || opcao[0] == 'S'){
            printf("Por favor adicione essa pessoa: \n");
            Adiciona_conjugue(C);
        }
        else{
            memset(C->conjugue->nome, 0, sizeof(C->conjugue->nome));
            memset(C->conjugue->ID, 0, sizeof(C->conjugue->ID));
            C->conjugue->idade = 0;
            C->conjugue->peso = 0.0;
        }
    }
}

int escrever_arquivo(casal *C, char opcao){

    FILE *file;
    char op[2];
    char nome_arquivo[50];
    char nome_arquivo_registro[50];


    if(opcao == '1'){

        // Salvar em arquivo na pasta "Registro"

        sprintf(nome_arquivo_registro, "./Registro/%s.bin", C->filho->ID);

        if(!existe_pessoa(nome_arquivo_registro)){
            printf("Erro, Pessoa ja cadrastada!\n");
            printf("Deseja fazer alterações nas informaçoes dessa pessoa?\n");

            fgets(op, sizeof(op), stdin);
            if(op[0] == 's' || op[0] == 'S')
               goto continuar;

            else
                return 0;

        }
        else
            printf("A pessoa nao esta registrada.\n");

        continuar:

        file = fopen(nome_arquivo_registro, "wb");

        if (file == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
            return 0;
        }

        fwrite(&C->filho, sizeof(C->filho), 1, file);
        fclose(file);

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
        }
    }
    else if(opcao == '2'){

        // Salvar em arquivo na pasta "Registro"

        sprintf(nome_arquivo_registro, "./Registro/%s.bin", C->conjugue->ID);

        if(!existe_pessoa(nome_arquivo_registro)){
            printf("Erro, Pessoa ja cadrastada!\n");
            return 0;
        }
        else
            printf("A pessoa nao esta registrada.\n");

        file = fopen(nome_arquivo_registro, "wb");

        if (file == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nome_arquivo_registro);
            return 0;
        }

        fwrite(&C->conjugue, sizeof(C->conjugue), 1, file);
        fclose(file);

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
        }
    }
    return 1;
}

short int verificaPasta(const char* nomePasta) {

    struct stat st;
    if (stat(nomePasta, &st) == 0) {
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

short int   existe_pessoa       (const char *nome_arquivo_registro){

    FILE* arquivo = fopen(nome_arquivo_registro, "rb");

    if (arquivo == NULL) {
        // Arquivo não encontrado
        return 1;
    }

    // Arquivo encontrado"
    fclose(arquivo);
    return 0;
}

//void ler_arquivo(const char* nomePasta, char nome_arquivo_registro){}

Grafo* criarGrafo() {

    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));       // Aloca memória para a struct Grafo

    if (grafo == NULL) {                                // Tratamento de erro, falha na alocação de memória
        return NULL;
    }
    // Inicializa os campos do grafo

    grafo->tam = 0;        // Tamanho inicial do grafo é zero
    grafo->inicio = NULL;  // O ponteiro "inicio" é inicializado como NULL
    grafo->fim = NULL;     // O ponteiro "fim" é inicializado como NULL
    // Retorna o ponteiro para o grafo criado

    return grafo;
}

short int adicionarNo(Grafo* grafo, char ID_CASAL[8], casal *C){

    // Cria um novo nó
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Falha ao criar o nó.\n");
        return 1;
    }

    // Preenche os campos do novo nó
    strcpy(novoNo->ID_CASAL, ID_CASAL);
    novoNo->casal = C;
    novoNo->nosAdjacentes.tam = 0;
    novoNo->nosAdjacentes.inicio = NULL;
    novoNo->nosAdjacentes.fim = NULL;
    novoNo->prox = NULL;

    // Verifica se o grafo está vazio
    if (grafo->inicio == NULL) {
        grafo->inicio = novoNo;
        grafo->fim = novoNo;
    } else {
        grafo->fim->prox = novoNo;
        grafo->fim = novoNo;
    }

    // Incrementa o tamanho do grafo
    grafo->tam++;

    return 0;
}

void criarConexao(Grafo* grafo, char ID_CASAL1[8], char ID_CASAL2[8]) {
    // Encontrar os nós correspondentes aos IDs fornecidos
    No* no1 = encontrarNo(grafo, ID_CASAL1);
    No* no2 = encontrarNo(grafo, ID_CASAL2);

    // Verificar se os nós foram encontrados
    if (no1 == NULL || no2 == NULL) {
        printf("Um ou ambos os nós não foram encontrados.\n");
        return;  // Retornar ou tratar o erro conforme a necessidade do seu programa
    }

    // Criar os nós adjacentes
    NoAdjacente* novoNoAdj1 = criarNoAdjacente(no2);
    NoAdjacente* novoNoAdj2 = criarNoAdjacente(no1);

    // Adicionar os nós adjacentes às respectivas listas de nós adjacentes
    adicionarNoAdjacente(&no1->nosAdjacentes, novoNoAdj1);
    adicionarNoAdjacente(&no2->nosAdjacentes, novoNoAdj2);
}

void salvarGrafo(Grafo* grafo, const char* nomeArquivo) {

    // Criar o caminho completo para o arquivo
    char caminhoCompleto[10]; // Tamanho arbitrário, ajuste conforme necessário
    strcpy(caminhoCompleto, "Grafo/"); // Substitua "Grafo" pelo nome da pasta desejada
    strcat(caminhoCompleto, nomeArquivo);

    // Abrir o arquivo para escrita em modo binário
    FILE* arquivo = fopen(caminhoCompleto, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Criar uma estrutura de dados para salvar o grafo
    Grafo grafoSalvo;
    grafoSalvo.tam = grafo->tam;
    grafoSalvo.inicio = grafo->inicio;
    grafoSalvo.fim = grafo->fim;

    // Escrever a estrutura no arquivo
    fwrite(&grafoSalvo, sizeof(Grafo), 1, arquivo);

    // Fechar o arquivo
    fclose(arquivo);
}

No* encontrarNo(Grafo* grafo, char ID_CASAL[8]) {
    No* atual = grafo->inicio;
    while (atual != NULL) {
        if (strcmp(atual->ID_CASAL, ID_CASAL) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void percorrerGrafo(Grafo* grafo, FILE* arquivo){

    No* atual = grafo->inicio;

    while (atual != NULL) {
        printf("ID_CASAL: %s\n", atual->ID_CASAL);
        printf("Casais adjacentes: ");
        NoAdjacente* adjacente = atual->nosAdjacentes.inicio;
        while (adjacente != NULL) {
            fprintf(arquivo, "%s ", adjacente->vertice->ID_CASAL);
            adjacente = adjacente->prox;
        }
        printf("\n\n");
        atual = atual->prox;
    }
}

void imprimirGrafo(Grafo* grafo) {
    FILE* arquivo = fopen("arquivo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    No* atual = grafo->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "ID_CASAL: %s\n", atual->ID_CASAL);
        fprintf(arquivo, "Casais adjacentes: ");
        NoAdjacente* adjacente = atual->nosAdjacentes.inicio;
        while (adjacente != NULL) {
            fprintf(arquivo, "%s ", adjacente->vertice->ID_CASAL);
            adjacente = adjacente->prox;
        }
        fprintf(arquivo, "\n\n");
        atual = atual->prox;
    }

    fclose(arquivo);
}

NoAdjacente* criarNoAdjacente(No* no) {
    NoAdjacente* novoNoAdj = (NoAdjacente*)malloc(sizeof(NoAdjacente));
    novoNoAdj->vertice = no;
    novoNoAdj->prox = NULL;
    return novoNoAdj;
}

void adicionarNoAdjacente(ListaNoAdjacente* lista, NoAdjacente* noAdjacente){
    if (lista->inicio == NULL) {
        lista->inicio = noAdjacente;
        lista->fim = noAdjacente;
    } else {
        lista->fim->prox = noAdjacente;
        lista->fim = noAdjacente;
    }
    lista->tam++;
}
