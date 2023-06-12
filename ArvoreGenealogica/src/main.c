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

#include "bibliotecas.h"


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
