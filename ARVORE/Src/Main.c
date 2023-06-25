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

/*dados de entrada : nome, idade, peso, ficha de saude, caracteristicas fisicas e caracteristicas fisicas*/

/*estruturas: funcoes, estruturas de dados(vetores, structs, listas)), alocacao dinamica, manipulacao de arquivos binarios*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "../include/declaracoes.h"
#include "../include/estruturas.h"
#include "../Lib/Funcoes.c"

int main(void){

    int opcao;  
    setlocale(LC_ALL, "Portuguese");                                            

    cria_pastas();                                          //chama a funcao para criar as pastas

/*=========================================================================================================================*/
    usage(1);                                               //chama a funcao de legenda
    
    do{                                                     // Menu de interacao criado para usabilidade dos usuarios
    printf("-----|ARVORE GENEALOGICA.|-----\n");
    printf("Digite a opcao desejada. \n");
    printf("1. Cadastrar Pessoa.\n");
    printf("2. Ver pessoas cadastradas.\n");
    printf("3.Ajuda\n");
    printf("0. Sair.\n");

    scanf("%d",&opcao);

    switch(opcao){                                          //switch para o redirecionamento da opcao desejada
        case 1:
            cadastrar();
            break;
        case 2:
            recuperar();
            break;
        case 3:
            usage(1);
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            usage(1);
            break;
    }
    } while (!opcao);

/*=========================================================================================================================*/

    return 0;
}
