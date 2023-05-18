/*
objetivo:
Arvore genealogia que detecta possiveis eventuais doencas que os filhos possam ter.

criadores:
Adalberto e Diogo.
*/

/*Fontes de Referencia:

Estruturas de dado Arvore:  https://www.freecodecamp.org/portuguese/news/tudo-o-que-voce-precisa-saber-sobre-estruturas-de-dados-em-arvore/amp/
Varredura infixa e pos fixa de uma arvore binaria: https://www.ime.usp.br/~pf/algoritmos/aulas/bint.html
Estruturas de grafos: https://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/graphdatastructs.html
Percuso em arvores: http://www.ic.uff.br/~boeres/slides_ed/ed_ArvoresPercursos.pdf

Manipulacao de arquivos e ideias de projeto: https://chat.openai.com/
Metodo de pesquisa em busca de informacoes do projeto: Bing AI */

#include "include.h"
#include "lib.c"


int main(void){

    casal   C;
    cria_pastas();
    //Adiciona_Pessoa(&C);
    Adiciona_filho(&C);

    printf("-----|ARVORE GENEALOGICA.|-----\n");
}
