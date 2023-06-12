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


