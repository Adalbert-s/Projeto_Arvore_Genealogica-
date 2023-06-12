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
