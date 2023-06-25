#include "../include/estruturas.h"

#ifndef DECLARACOES_H
#define DECLARACOES_H

//funcoes de manipulacao de arquivos
// funcoes para escrever e ler os dados dos integrantes em arquivos

short int   cria_pastas         (void);                     //funcao para a criacao de pastas, tipo short int pois o short int ja � mais que o necessario para essa funcao.
short int   verificaPasta       (const char* nomePasta);

//funcoes de manipulacao

int         salvar              (ListaLDE *inicio, ListaLDE *aux, TCabecalho cab);
void        cadastrar           ();
void        recuperar           ();

//funcoes complementares.

void        Adiciona_filho      (casal *C);
void        Adiciona_conjugue   (casal *C);
short int   Adiciona_par        (casal *C);
void        Adiciona_filho      (casal *C);

int         obterResposta       (const char* resposta);

void PreencheFicha              (void *struct_ptr, int tipo);
void PreencheCaracteristicas    (void *struct_ptr, int tipo);

void limparBuffer();
char* lerResposta(int tamanho, FILE* fluxo);

void gerarID(casal *C);
void salvarID(int idPadrao);
int lerID();

void ancestral(ListaLDE *aux, casal *C);
void usage(int menu);

#endif // DECLARACOES_H
