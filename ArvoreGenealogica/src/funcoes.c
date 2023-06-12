short int cria_pastas               (void){

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

short int Adiciona_Pessoa       (casal *C){

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
            printf("Digite o ID da pessoa com que essa � casada: \n");
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

void Adiciona_filho             (casal *C){

    C->filho = malloc(sizeof(filho)); // Aloca mem�ria para a estrutura filho dentro de casal
    if (C->filho == NULL) {
        // Trate o erro de aloca��o de mem�ria
        printf("Erro ao alocar mem�ria para filho\n");
        return;
    }

    printf("Digite o ID do filho: ");                       //usando fgets para ter mais seguran�a e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma s� vez.
    fgets(C->filho->ID, sizeof(C->filho->ID), stdin);       //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->filho->ID[strcspn(C->filho->ID, "\n")] = '\0';       //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string filho.nome.  //evita possiveis problemas na string
    printf("Digite o nome: ");
    fgets(C->filho->nome, sizeof(C->filho->nome), stdin);
    C->filho->nome[strcspn(C->filho->nome, "\n")] = '\0';

    printf("Digite a idade: ");
    char input[4];                          //por idade ser um inteiro, n�o vou ter uma string de comparacao, ent�o criei uma variavel para armazenar.
    fgets(input, sizeof(input), stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->filho->idade = (int) strtol(input, NULL, 10);// armazenando em filho.idade  //strol(fun��o que transforma string em int), 3 campos(ponteiro para a string, NULL pois vai ser a string inteira, base numerica)

    printf("Digite o peso: ");
    char input_float[6];                    //variavel para armazenar a string que sera lida.
    fgets(input_float, sizeof(input_float), stdin);
    C->filho->peso = strtof(input_float, NULL);    //strtof para transformar em float                      //essa fun��o n�o recebe o argumento de base numerica.

}

void Adiciona_conjugue          (casal *C){

    C->conjugue = malloc(sizeof(conjugue)); // Aloca mem�ria para a estrutura filho dentro de casal
    if (C->conjugue == NULL) {
        // Trate o erro de aloca��o de mem�ria
        printf("Erro ao alocar mem�ria para conjugue\n");
        return;
    }

    printf("Digite o ID: ");                //usando fgets para ter mais seguran�a e nao ocasionar em um overflow, fgets normalmente le uma linha inteira de uma s� vez.
    fgets(C->conjugue->ID, sizeof((C->conjugue->ID)), stdin);   //fgets precisa ter 3 definicoes (ponteiro onde vai armazenar a string lida, tamanho, ponteiro para o fluxo de entrada, normalmente se usa stdin).
    C->conjugue->ID[strcspn(C->conjugue->ID, "\n")] = '\0';     //substitui o caractere de quebra de linha (\n) pelo caractere nulo (\0) na string conjugue.nome.  //evita possiveis problemas na string

    printf("Digite o nome: ");
    fgets(C->conjugue->nome, sizeof(C->conjugue->nome), stdin);
    C->conjugue->nome[strcspn(C->conjugue->nome, "\n")] = '\0';

    printf("Digite a idade: ");
    char input[4];                          //por idade ser um inteiro, n�o vou ter uma string de comparacao, ent�o criei uma variavel para armazenar.
    fgets(input, sizeof(input), stdin);     //char input[4] pois tendo em vista que idade nunca vai ser maior que 3 digitos.        //fgets usando a mesma logica que o de cima.
    C->conjugue->idade = (int) strtol(input, NULL, 10);// armazenando em conjugue.idade  //strol(fun��o que transforma string em int), 3 campos(ponteiro para a string, NULL pois vai ser a string inteira, base numerica)

    printf("Digite o peso: ");
    char input_float[6];                    //variavel para armazenar a string que sera lida.
    fgets(input_float, sizeof(input_float), stdin);
    C->conjugue->peso = strtof(input_float, NULL);    //strtof para transformar em float                      //essa fun��o n�o recebe o argumento de base numerica.

}

void        Adiciona_par        (casal *C){

    char opcao[2];

    if(C != NULL && C->filho != NULL && C->filho->idade >= 18){
        printf("Essa pessoa e casada? \n");
        fgets(opcao, sizeof(opcao), stdin);

        if(opcao[0] == 's' || opcao[0] == 'S'){
            printf("Por favor adicione essa pessoa: \n");
            Adiciona_conjugue(&C);
        }
        else{
            memset(C->conjugue->nome, 0, sizeof(C->conjugue->nome));
            memset(C->conjugue->ID, 0, sizeof(C->conjugue->ID));
            C->conjugue->idade = 0;
            C->conjugue->peso = 0.0;
        }
    }
}

void        Adiciona_filho      (casal *C){

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
            printf("Deseja fazer altera��es nas informa�oes dessa pessoa?\n");

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
            // O caminho existe, mas n�o � uma pasta
            return 0;
        }
    } else {
        // A pasta n�o existe
        return 0;
    }
}

short int   existe_pessoa       (const char *nome_arquivo_registro){

    FILE* arquivo = fopen(nome_arquivo_registro, "rb");

    if (arquivo == NULL) {
        // Arquivo n�o encontrado
        return 1;
    }

    // Arquivo encontrado"
    fclose(arquivo);
    return 0;
}

//void ler_arquivo(const char* nomePasta, char nome_arquivo_registro){}

Grafo* criarGrafo() {

    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));       // Aloca mem�ria para a struct Grafo

    if (grafo == NULL) {                                // Tratamento de erro, falha na aloca��o de mem�ria
        return NULL;
    }
    // Inicializa os campos do grafo

    grafo->tam = 0;        // Tamanho inicial do grafo � zero
    grafo->inicio = NULL;  // O ponteiro "inicio" � inicializado como NULL
    grafo->fim = NULL;     // O ponteiro "fim" � inicializado como NULL
    // Retorna o ponteiro para o grafo criado

    return grafo;
}

short int adicionarNo(Grafo* grafo, char ID_CASAL[8], casal *C){

    // Cria um novo n�
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Falha ao criar o n�.\n");
        return 1;
    }

    // Preenche os campos do novo n�
    strcpy(novoNo->ID_CASAL, ID_CASAL);
    novoNo->casal = C;
    novoNo->nosAdjacentes.tam = 0;
    novoNo->nosAdjacentes.inicio = NULL;
    novoNo->nosAdjacentes.fim = NULL;
    novoNo->prox = NULL;

    // Verifica se o grafo est� vazio
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
    // Encontrar os n�s correspondentes aos IDs fornecidos
    No* no1 = encontrarNo(grafo, ID_CASAL1);
    No* no2 = encontrarNo(grafo, ID_CASAL2);

    // Verificar se os n�s foram encontrados
    if (no1 == NULL || no2 == NULL) {
        printf("Um ou ambos os n�s n�o foram encontrados.\n");
        return;  // Retornar ou tratar o erro conforme a necessidade do seu programa
    }

    // Criar os n�s adjacentes
    NoAdjacente* novoNoAdj1 = criarNoAdjacente(no2);
    NoAdjacente* novoNoAdj2 = criarNoAdjacente(no1);

    // Adicionar os n�s adjacentes �s respectivas listas de n�s adjacentes
    adicionarNoAdjacente(&no1->nosAdjacentes, novoNoAdj1);
    adicionarNoAdjacente(&no2->nosAdjacentes, novoNoAdj2);
}

void salvarGrafo(Grafo* grafo, const char* nomeArquivo) {

    // Criar o caminho completo para o arquivo
    char caminhoCompleto[10]; // Tamanho arbitr�rio, ajuste conforme necess�rio
    strcpy(caminhoCompleto, "Grafo/"); // Substitua "Grafo" pelo nome da pasta desejada
    strcat(caminhoCompleto, nomeArquivo);

    // Abrir o arquivo para escrita em modo bin�rio
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

