#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

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

#endif // ESTRUTURAS_H
