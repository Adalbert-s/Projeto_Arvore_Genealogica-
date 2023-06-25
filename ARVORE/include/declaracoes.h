#include "../include/estruturas.h"

#ifndef DECLARACOES_H
#define DECLARACOES_H

//funcoes responaveis por criar pastas
short int   cria_pastas         (void);                     //essa funcao e responsavel por criar as pastas
short int   verificaPasta       (const char* nomePasta);    //essa funcao verifica se a pasta existe ou não e retorna um valor

//funcoes de manipulacao de informacoes

int         salvar              (ListaLDE *inicio, ListaLDE *aux, TCabecalho cab);  //funcao responsavel por salvar a lista em arquivo
void        cadastrar           ();                                                 //funcao responsavel por cadrastrar pessoas
void        recuperar           ();                                                 //funcao responsavel por recuperar as informacoes do arquivo

//funcoes complementares.

void        Adiciona_filho      (casal *C);                 //funcao responsavel por adicionar informacoes do filho
void        Adiciona_conjugue   (casal *C);                 //funcao responsavel por adicionar informacoes do conjugue
short int   Adiciona_par        (casal *C);                 //funcao que completa os campos com desconhecido se for o caso
void        PreencheFicha              (void *struct_ptr, int tipo);    //funcao responsavel por capturar informacoes da ficha medica
void        PreencheCaracteristicas    (void *struct_ptr, int tipo);    //funcao responsavel por preencher as informacoes de caracteristica fisica
              
//funcoes responsaveis por captura de buffer

int         obterResposta       (const char* resposta);     //funcao que captura o buffer
void        limparBuffer        ();                         //funcao de limpar o buffer
char*       lerResposta         (int tamanho, FILE* fluxo); //funcao que le a resposta digitada

//funcoes de ID

void gerarID(casal *C);                                     //funcao que gera id
void salvarID(int idPadrao);                                //funcao que salva o id em arquivo
int lerID();                                                //funcao auxiliar 

void ancestral(ListaLDE *aux, casal *C);                    //funcao responsavel por dar mensagens 

//funcao de menu

void usage(int menu);                                       //responsavel por mostrar legendas para auxiliar o usuario

#endif // DECLARACOES_H
