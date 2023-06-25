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

}ficha;                                                 //tais campos vao ser preenchidos com 1 para sim e 2 para nao

//estrutura destinada a caracteristicas fisicas.

typedef struct CaracteristicasFisicas{

    int Cor_do_Cabelo;                                  //cor de cabelo 5 opcoes disponiveis para ser preenchido ( 1 - 5)
    int Tipo_de_Cabelo;                                 //tipo de cabelo 4 opcoes disponiveis para ser preenchido ( 1 - 4)
    int Cor_da_Pele;                                    //tipo de pele 4 opcoes disponiveis para ser preenchido ( 1 - 4)
    int Cor_dos_Olhos;                                  //cor dos olhos 4 opcoes disponiveis para ser preenchido ( 1 - 4)

    char peso   [10];                                   //peso, atualmente um char
    char altura [15];                                   //altura, tambem como char

}caracfis;

//estrutura com as informacoes de cada um dos 2.

typedef struct FILHO{

    //informacoes basicas.

    char nome   [50];
    char idade  [10];

    //informacoes sobre suas doencas

    struct FichaTecnica ficha;

    //informacoes sobre suas caracteristicas
    struct CaracteristicasFisicas caracfis;

}filho;

typedef struct CONJUGUE{
    //informacoes basicas.

    char nome   [50];
    char idade  [10];

    //informacoes sobre suas doencas
    struct FichaTecnica ficha;

    //informacoes sobre suas caracteristicas
    struct CaracteristicasFisicas caracfis;

}conjugue;

//estrutura com as informacoes do casal.

typedef struct{

    char ID[8];

    struct FILHO filho;                   
    struct CONJUGUE conjugue;

}casal;                                    

//estrutura da Lista

typedef struct no {

    casal casal;

    struct no *prox;
    struct no *ant;

    int tam;

}ListaLDE;

//estrutura de cabecario

typedef struct
{
  char ID[3];
  unsigned short q_registros;

}TCabecalho;

#endif // ESTRUTURAS_H
