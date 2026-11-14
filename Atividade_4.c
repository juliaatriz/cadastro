    /* ====================================================================
    ATIVIDADE 04 - CADASTRO DE LOJA (VERSAO COMENTADA LINHA POR LINHA)

    O que o programa faz:
    - Guarda produtos de uma loja em um arquivo de texto (produtos.txt)
    - Permite criar, listar, inserir, alterar, excluir e buscar produtos
    - Ordena os produtos por nome ou por preco usando Bubble Sort
    - Exporta os dados para um arquivo CSV

    Conceitos usados: struct, vetor, ponteiros, arquivos e ordenacao
    ==================================================================== */

    // ---------------------- BIBLIOTECAS (#include) ----------------------
    #include <stdio.h>   // biblioteca de entrada e saida: printf, scanf, fopen, fclose, fprintf, fscanf
    #include <string.h>  // biblioteca de textos: strcmp (comparar) e strstr (procurar texto dentro de texto)
    #include <ctype.h>   // biblioteca de caracteres: tolower (transforma letra em minuscula)
    #include <stdlib.h>  // biblioteca geral: remove (apaga arquivo) e rename (renomeia arquivo)

    // ---------------------- STRUCT DO PRODUTO ----------------------
    // Uma struct e um "pacote" que junta varias variaveis de tipos diferentes.
    // Aqui, cada produto da loja tem todos esses dados juntos.
    struct produto {                 // comeca a definir a struct chamada Produto
        int id;                      // codigo do produto (numero inteiro)
        char nome[50];               // nome do produto (texto de ate 49 letras + o '\0' no final)
        char categoria[30];          // categoria do produto (texto de ate 29 letras)
        float preco;                 // preco do produto (numero com virgula)
        int quantidade;              // quantidade em estoque (numero inteiro)
        char fornecedor[50];         // nome do fornecedor (texto de ate 49 letras)
    };                               // fim da struct (atencao: precisa do ponto e virgula!)

    // ====================================================================
    // FUNCAO minusculo
    // Serve para transformar um texto todo em letras minusculas.
    // Usamos ela para comparar textos sem ligar para maiuscula/minuscula
    // (assim "Cafe" e "CAFE" sao considerados iguais).
    // Os parametros sao PONTEIROS para char (ponteiro para o texto).
    // origem  = texto original | destino = onde sera guardada a copia minuscula
    // ====================================================================
    void minusculo(char *origem, char *destino) {  // void = a funcao nao devolve nada
        int i = 0;                                 // i e a posicao da letra que estamos olhando (comeca na 0)
        while (origem[i] != '\0') {                // repete enquanto nao chegar no fim do texto ('\0' marca o fim)
            destino[i] = tolower(origem[i]);       // pega a letra da posicao i, deixa minuscula e guarda no destino
            i++;                                   // avanca para a proxima letra (i = i + 1)
        }                                          // fim do while
        destino[i] = '\0';                         // coloca o '\0' no final para "fechar" o texto novo
    }                                              // fim da funcao

    // ====================================================================
    // FUNCAO trocar
    // Troca dois produtos de lugar. Usada no Bubble Sort.
    // Recebe PONTEIROS (enderecos) de dois produtos, por isso a troca
    // acontece de verdade no vetor original (e nao em uma copia).
    // ====================================================================
    void trocar(struct Produto *a, struct Produto *b) {  // a e b guardam os ENDERECOS dos dois produtos
        struct Produto aux;   // variavel auxiliar (um copo vazio para ajudar na troca)
        aux = *a;             // *a significa "o conteudo que esta no endereco a". Guarda o produto a no aux
        *a = *b;              // o produto a passa a ter os dados do produto b
        *b = aux;             // o produto b recebe os dados que estavam guardados no aux (o antigo a)
    }                         // fim da funcao: os dois produtos trocaram de posicao

    // ====================================================================
    // FUNCAO carregar
    // Le todos os produtos do arquivo produtos.txt e coloca no vetor "lista".
    // Devolve quantos produtos foram lidos, ou -1 se o arquivo nao existir.
    // ====================================================================
    int carregar(struct Produto lista[]) {   // recebe o vetor de produtos que sera preenchido
        FILE *arq;                           // arq e um ponteiro para o arquivo que vamos abrir
        int qtd = 0;                         // qtd conta quantos produtos ja foram lidos (comeca em 0)

        arq = fopen("produtos.txt", "r");    // abre o arquivo no modo "r" (read = so leitura)
        if (arq == NULL) {                   // se fopen devolveu NULL, o arquivo nao existe
            return -1;                       // devolve -1 para avisar que deu erro
        }                                    // fim do if

        // O fscanf le dados do arquivo seguindo um formato. Ele devolve quantos campos conseguiu ler.
        // Formato: %d (inteiro) ; %[^;] (texto ate o ponto e virgula) ... %[^\n] (texto ate o fim da linha)
        // O while continua enquanto conseguir ler os 6 campos de uma linha.
        while (fscanf(arq, "%d;%[^;];%[^;];%f;%d;%[^\n]\n",   // le uma linha inteira do arquivo
                    &lista[qtd].id, lista[qtd].nome, lista[qtd].categoria,  // guarda id, nome e categoria
                    &lista[qtd].preco, &lista[qtd].quantidade,              // guarda preco e quantidade
                    lista[qtd].fornecedor) == 6) {                          // guarda fornecedor; 6 = leu tudo certo
            qtd++;                           // leu um produto com sucesso, entao soma 1 no contador
        }                                    // fim do while (acabou o arquivo)

        fclose(arq);                         // fecha o arquivo (sempre fechar depois de usar!)
        return qtd;                          // devolve quantos produtos foram lidos
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO mostrarProduto
    // Mostra UM produto na tela, em formato de linha de tabela.
    // Recebe um ponteiro para o produto; com ponteiro de struct usamos a seta (->)
    // ====================================================================
    void mostrarProduto(struct Produto *p) {   // p e o endereco do produto que vai ser mostrado
        // %-3d = inteiro alinhado a esquerda com 3 espacos | %-20s = texto alinhado a esquerda com 20 espacos
        // %8.2f = numero com 2 casas depois da virgula, ocupando 8 espacos
        printf("| %-3d | %-20s | %-16s | %8.2f | %5d | %-15s |\n",
            p->id, p->nome, p->categoria, p->preco, p->quantidade, p->fornecedor);  // p->id = campo id do produto apontado
    }                                          // fim da funcao

    // ====================================================================
    // FUNCAO cabecalho
    // Mostra o topo da tabela (linhas e titulos das colunas).
    // ====================================================================
    void cabecalho() {   // nao recebe nada e nao devolve nada
        printf("\n-----------------------------------------------------------------------------------------\n");  // linha de tracos (o \n no comeco pula uma linha)
        printf("| ID  | Produto              | Categoria        | Preco    | Qtd   | Fornecedor      |\n");        // titulos das colunas
        printf("-----------------------------------------------------------------------------------------\n");    // outra linha de tracos
    }                    // fim da funcao

    // ====================================================================
    // FUNCAO criar  (opcao 1 do menu)
    // Cria o arquivo produtos.txt ja com os 4 produtos da tabela do enunciado.
    // ====================================================================
    void criar() {                           // inicio da funcao criar
        FILE *arq;                           // ponteiro para o arquivo
        arq = fopen("produtos.txt", "w");    // modo "w" (write): cria o arquivo, e se ja existir APAGA tudo
        if (arq == NULL) {                   // se nao conseguiu criar o arquivo
            printf("Erro ao criar o arquivo!\n");   // avisa o erro
            return;                          // sai da funcao
        }                                    // fim do if

        // fprintf funciona igual ao printf, mas escreve dentro do arquivo (arq) e nao na tela
        // Cada linha segue o formato: ID;Produto;Categoria;Preco;Quantidade;Fornecedor
        fprintf(arq, "1;Camiseta Basica;Vestuario;49.90;30;Moda Brasil\n");        // produto 1
        fprintf(arq, "2;Mouse Optico USB;Informatica;59.99;15;TecnoShop\n");       // produto 2
        fprintf(arq, "3;Shampoo 300ml;Higiene Pessoal;22.50;40;Beleza Pura\n");    // produto 3
        fprintf(arq, "4;Cafe 500g;Alimentos;18.00;25;Graos do Sul\n");             // produto 4

        fclose(arq);                         // fecha o arquivo para salvar tudo
        printf("Arquivo criado com sucesso!\n");   // avisa que deu certo
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO listar  (opcao 2 do menu)
    // Mostra todos os produtos do arquivo em formato de tabela.
    // ====================================================================
    void listar() {                          // inicio da funcao listar
        struct Produto lista[100];           // vetor com espaco para ate 100 produtos
        int qtd, i;                          // qtd = quantos produtos existem | i = contador do for

        qtd = carregar(lista);               // le o arquivo e guarda no vetor; qtd recebe a quantidade
        if (qtd == -1) {                     // -1 significa que o arquivo nao existe
            printf("Arquivo nao existe! Escolha a opcao 1 primeiro.\n");   // orienta o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        cabecalho();                         // mostra o topo da tabela
        for (i = 0; i < qtd; i++) {          // repete do produto 0 ate o ultimo
            mostrarProduto(&lista[i]);       // & pega o ENDERECO do produto i e manda para a funcao
        }                                    // fim do for
        printf("-----------------------------------------------------------------------------------------\n");   // linha final da tabela
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO inserir  (opcao 3 do menu)
    // Cadastra um produto novo.
    // - O ID e gerado sozinho (maior ID + 1)
    // - Nao deixa cadastrar dois produtos com o mesmo nome
    // ====================================================================
    void inserir() {                         // inicio da funcao inserir
        struct Produto lista[100];           // vetor com os produtos que ja existem no arquivo
        struct Produto novo;                 // produto novo que sera cadastrado
        char nomeMin[50], outroMin[50];      // textos para guardar nomes em minuscula (para comparar)
        int qtd, i, maior = 0;               // qtd = quantidade | i = contador | maior = maior ID encontrado
        FILE *arq;                           // ponteiro para o arquivo

        qtd = carregar(lista);               // le os produtos que ja estao no arquivo
        if (qtd == -1) {                     // se o arquivo nao existe
            printf("Arquivo nao existe! Escolha a opcao 1 primeiro.\n");   // avisa o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        // pede o nome do produto
        printf("Nome do produto: ");         // mostra a pergunta na tela
        scanf(" %[^\n]", novo.nome);         // le o texto com espacos; o espaco antes do % limpa o ENTER que sobrou

        // VERIFICA DUPLICATA: compara o nome novo com todos os nomes que ja existem
        minusculo(novo.nome, nomeMin);       // deixa o nome digitado em minuscula e guarda em nomeMin
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos ja cadastrados
            minusculo(lista[i].nome, outroMin);          // deixa o nome do produto i em minuscula
            if (strcmp(nomeMin, outroMin) == 0) {        // strcmp devolve 0 quando os dois textos sao iguais
                printf("Esse produto ja esta cadastrado!\n");   // avisa que e repetido
                return;                      // sai da funcao SEM cadastrar
            }                                // fim do if
        }                                    // fim do for

        // GERA O ID AUTOMATICO: procura o maior ID que existe e soma 1
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            if (lista[i].id > maior) {       // se o ID desse produto e maior que o maior guardado
                maior = lista[i].id;         // entao ele passa a ser o novo maior
            }                                // fim do if
        }                                    // fim do for
        novo.id = maior + 1;                 // o ID do produto novo e o maior mais 1 (nao repete nunca)
        printf("ID gerado: %d\n", novo.id);  // mostra o ID que foi gerado

        // pede o resto dos dados do produto
        printf("Categoria: ");               // pergunta a categoria
        scanf(" %[^\n]", novo.categoria);    // le a categoria (aceita espacos)
        printf("Preco: ");                   // pergunta o preco
        scanf("%f", &novo.preco);            // le um numero com virgula (usa ponto, ex: 15.50); & = onde guardar
        printf("Quantidade: ");              // pergunta a quantidade
        scanf("%d", &novo.quantidade);       // le um numero inteiro
        printf("Fornecedor: ");              // pergunta o fornecedor
        scanf(" %[^\n]", novo.fornecedor);   // le o fornecedor (aceita espacos)

        // grava o produto novo no final do arquivo
        arq = fopen("produtos.txt", "a");    // modo "a" (append): escreve no final SEM apagar o que ja existe
        fprintf(arq, "%d;%s;%s;%.2f;%d;%s\n",   // escreve uma linha no arquivo separando os campos por ;
                novo.id, novo.nome, novo.categoria, novo.preco, novo.quantidade, novo.fornecedor);   // valores de cada campo
        fclose(arq);                         // fecha o arquivo

        printf("Produto cadastrado!\n");     // avisa que deu certo
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO alterar  (opcao 4 do menu)
    // Muda os dados de um produto escolhido pelo ID.
    // Tecnica do arquivo temporario: grava tudo em temp.txt, apaga o
    // original e renomeia o temporario. Isso evita corromper o arquivo.
    // ====================================================================
    void alterar() {                         // inicio da funcao alterar
        struct Produto lista[100];           // vetor com os produtos do arquivo
        int qtd, i, id, achou = 0;           // qtd = quantidade | i = contador | id = ID digitado | achou = 0 (nao) ou 1 (sim)
        int posicao = 0;                     // posicao do produto dentro do vetor
        struct Produto novo;                 // guarda os dados novos digitados
        char nomeMin[50], outroMin[50];      // nomes em minuscula para comparar
        FILE *temp;                          // ponteiro para o arquivo temporario

        qtd = carregar(lista);               // le os produtos do arquivo
        if (qtd == -1) {                     // se o arquivo nao existe
            printf("Arquivo nao existe! Escolha a opcao 1 primeiro.\n");   // avisa o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        printf("Digite o ID do produto que quer alterar: ");   // pergunta qual produto alterar
        scanf("%d", &id);                    // le o ID digitado

        // procura o produto que tem esse ID
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            if (lista[i].id == id) {         // se o ID do produto i e igual ao digitado (== compara)
                achou = 1;                   // marca que achou
                posicao = i;                 // guarda a posicao dele no vetor
            }                                // fim do if
        }                                    // fim do for
        if (achou == 0) {                    // se terminou o for e nao achou
            printf("ID nao encontrado!\n");  // avisa o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        // pede os dados novos
        novo.id = id;                        // o ID continua o mesmo (ID nao muda)
        printf("Novo nome: ");               // pergunta o novo nome
        scanf(" %[^\n]", novo.nome);         // le o novo nome (aceita espacos)

        // confere se o novo nome ja existe em OUTRO produto
        minusculo(novo.nome, nomeMin);       // deixa o nome novo em minuscula
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            minusculo(lista[i].nome, outroMin);   // deixa o nome do produto i em minuscula
            if (strcmp(nomeMin, outroMin) == 0 && lista[i].id != id) {   // nomes iguais E nao e o proprio produto (&& = "e")
                printf("Ja existe outro produto com esse nome!\n");      // avisa que repetiu
                return;                      // sai da funcao sem alterar
            }                                // fim do if
        }                                    // fim do for

        printf("Nova categoria: ");          // pergunta a nova categoria
        scanf(" %[^\n]", novo.categoria);    // le a nova categoria
        printf("Novo preco: ");              // pergunta o novo preco
        scanf("%f", &novo.preco);            // le o novo preco
        printf("Nova quantidade: ");         // pergunta a nova quantidade
        scanf("%d", &novo.quantidade);       // le a nova quantidade
        printf("Novo fornecedor: ");         // pergunta o novo fornecedor
        scanf(" %[^\n]", novo.fornecedor);   // le o novo fornecedor

        lista[posicao] = novo;               // coloca os dados novos no lugar do produto antigo, dentro do vetor

        // grava o vetor inteiro (ja com a alteracao) no arquivo temporario
        temp = fopen("temp.txt", "w");       // cria o arquivo temporario para escrita
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            fprintf(temp, "%d;%s;%s;%.2f;%d;%s\n",   // escreve uma linha no temporario
                    lista[i].id, lista[i].nome, lista[i].categoria,          // id, nome e categoria
                    lista[i].preco, lista[i].quantidade, lista[i].fornecedor);   // preco, quantidade e fornecedor
        }                                    // fim do for
        fclose(temp);                        // fecha o temporario

        remove("produtos.txt");              // apaga o arquivo original (antigo)
        rename("temp.txt", "produtos.txt");  // o temporario ganha o nome do original e passa a ser o oficial

        printf("Produto alterado!\n");       // avisa que deu certo
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO excluir  (opcao 5 do menu)
    // Apaga um produto escolhido pelo ID.
    // Tambem usa o arquivo temporario: copia todos MENOS o excluido.
    // ====================================================================
    void excluir() {                         // inicio da funcao excluir
        struct Produto lista[100];           // vetor com os produtos do arquivo
        int qtd, i, id, achou = 0;           // qtd = quantidade | i = contador | id = ID digitado | achou = 0 ou 1
        FILE *temp;                          // ponteiro para o arquivo temporario

        qtd = carregar(lista);               // le os produtos do arquivo
        if (qtd == -1) {                     // se o arquivo nao existe
            printf("Arquivo nao existe! Escolha a opcao 1 primeiro.\n");   // avisa o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        printf("Digite o ID do produto que quer excluir: ");   // pergunta qual produto excluir
        scanf("%d", &id);                    // le o ID digitado

        temp = fopen("temp.txt", "w");       // cria o arquivo temporario
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            if (lista[i].id == id) {         // se e o produto que queremos excluir
                achou = 1;                   // marca que achou, e NAO grava no temporario (e assim ele "some")
            } else {                         // se NAO e o produto a excluir
                fprintf(temp, "%d;%s;%s;%.2f;%d;%s\n",   // grava o produto no temporario
                        lista[i].id, lista[i].nome, lista[i].categoria,          // id, nome e categoria
                        lista[i].preco, lista[i].quantidade, lista[i].fornecedor);   // preco, quantidade e fornecedor
            }                                // fim do else
        }                                    // fim do for
        fclose(temp);                        // fecha o temporario

        if (achou == 1) {                    // se achou o produto e ele ficou de fora do temporario
            remove("produtos.txt");          // apaga o arquivo original
            rename("temp.txt", "produtos.txt");   // o temporario vira o arquivo oficial
            printf("Produto excluido!\n");   // avisa que deu certo
        } else {                             // se o ID nao existia
            remove("temp.txt");              // joga o temporario fora (nao mudou nada)
            printf("ID nao encontrado!\n");  // avisa o usuario
        }                                    // fim do if/else
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO buscar  (opcoes 6 e 7 do menu)
    // Procura produtos pelo nome ou pela categoria.
    // tipo = 1 -> busca por nome | tipo = 2 -> busca por categoria
    // Nao diferencia maiuscula de minuscula e aceita so um pedaco do texto
    // (ex: "cam" encontra "Camiseta Basica").
    // ====================================================================
    void buscar(int tipo) {                  // recebe o tipo da busca (1 ou 2)
        struct Produto lista[100];           // vetor com os produtos do arquivo
        char busca[50], buscaMin[50], textoMin[50];   // busca = o que foi digitado | buscaMin = digitado em minuscula | textoMin = texto do produto em minuscula
        int qtd, i, achou = 0;               // qtd = quantidade | i = contador | achou = quantos foram encontrados

        qtd = carregar(lista);               // le os produtos do arquivo
        if (qtd == -1) {                     // se o arquivo nao existe
            printf("Arquivo nao existe! Escolha a opcao 1 primeiro.\n");   // avisa o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        if (tipo == 1) {                     // se a busca e por nome
            printf("Digite o nome para buscar: ");        // pergunta o nome
        } else {                             // senao (busca por categoria)
            printf("Digite a categoria para buscar: ");   // pergunta a categoria
        }                                    // fim do if/else
        scanf(" %[^\n]", busca);             // le o texto que o usuario digitou
        minusculo(busca, buscaMin);          // deixa o texto digitado em minuscula

        cabecalho();                         // mostra o topo da tabela
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            // deixa o nome OU a categoria do produto em minuscula, dependendo do tipo
            if (tipo == 1) {                 // busca por nome
                minusculo(lista[i].nome, textoMin);        // pega o nome do produto em minuscula
            } else {                         // busca por categoria
                minusculo(lista[i].categoria, textoMin);   // pega a categoria do produto em minuscula
            }                                // fim do if/else

            // strstr procura um texto dentro do outro; se NAO encontrar, devolve NULL
            if (strstr(textoMin, buscaMin) != NULL) {   // != NULL quer dizer que encontrou
                mostrarProduto(&lista[i]);   // mostra o produto na tabela
                achou++;                     // soma 1 no contador de encontrados
            }                                // fim do if
        }                                    // fim do for
        printf("-----------------------------------------------------------------------------------------\n");   // linha final da tabela

        if (achou == 0) {                    // se nao encontrou nenhum produto
            printf("Nenhum produto encontrado.\n");   // avisa o usuario
        }                                    // fim do if
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO ordenar  (opcoes 8, 9, 10 e 11 do menu)
    // Ordena os produtos usando BUBBLE SORT e mostra a tabela.
    // O arquivo NAO muda, so a copia que esta no vetor (na memoria).
    // opcao: 1 = nome A-Z | 2 = nome Z-A | 3 = preco menor-maior | 4 = preco maior-menor
    // ====================================================================
    void ordenar(int opcao) {                // recebe qual tipo de ordenacao o usuario quer
        struct Produto lista[100];           // vetor com os produtos do arquivo
        char nomeA[50], nomeB[50];           // nomes em minuscula dos dois produtos que serao comparados
        int qtd, i, j;                       // qtd = quantidade | i e j = contadores dos dois for

        qtd = carregar(lista);               // le os produtos do arquivo
        if (qtd == -1) {                     // se o arquivo nao existe
            printf("Arquivo nao existe! Escolha a opcao 1 primeiro.\n");   // avisa o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        // BUBBLE SORT: compara cada produto com o PROXIMO e troca se estiverem fora de ordem.
        // Repetindo isso varias vezes, o vetor vai ficando ordenado (como bolhas subindo).
        for (i = 0; i < qtd - 1; i++) {              // for de fora: controla quantas "rodadas" serao feitas
            for (j = 0; j < qtd - i - 1; j++) {      // for de dentro: compara os vizinhos (a cada rodada, o ultimo ja esta no lugar certo)

                // deixa os nomes em minuscula para comparar sem ligar para maiuscula
                minusculo(lista[j].nome, nomeA);         // nome do produto j em minuscula
                minusculo(lista[j + 1].nome, nomeB);     // nome do produto vizinho (j+1) em minuscula

                if (opcao == 1) {            // opcao 1: nome de A ate Z
                    if (strcmp(nomeA, nomeB) > 0) {      // strcmp > 0 quer dizer que nomeA vem DEPOIS de nomeB no alfabeto
                        trocar(&lista[j], &lista[j + 1]);   // entao troca os dois de lugar
                    }                        // fim do if interno
                }                            // fim do if da opcao 1
                if (opcao == 2) {            // opcao 2: nome de Z ate A
                    if (strcmp(nomeA, nomeB) < 0) {      // strcmp < 0 quer dizer que nomeA vem ANTES de nomeB
                        trocar(&lista[j], &lista[j + 1]);   // troca os dois de lugar
                    }                        // fim do if interno
                }                            // fim do if da opcao 2
                if (opcao == 3) {            // opcao 3: preco do menor para o maior
                    if (lista[j].preco > lista[j + 1].preco) {   // se o preco atual e MAIOR que o do vizinho
                        trocar(&lista[j], &lista[j + 1]);        // troca os dois de lugar
                    }                        // fim do if interno
                }                            // fim do if da opcao 3
                if (opcao == 4) {            // opcao 4: preco do maior para o menor
                    if (lista[j].preco < lista[j + 1].preco) {   // se o preco atual e MENOR que o do vizinho
                        trocar(&lista[j], &lista[j + 1]);        // troca os dois de lugar
                    }                        // fim do if interno
                }                            // fim do if da opcao 4
            }                                // fim do for de dentro
        }                                    // fim do for de fora

        // mostra a tabela com os produtos ja ordenados
        cabecalho();                         // topo da tabela
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            mostrarProduto(&lista[i]);       // mostra cada um
        }                                    // fim do for
        printf("-----------------------------------------------------------------------------------------\n");   // linha final da tabela
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO exportar  (opcao 12 do menu)
    // Cria o arquivo produtos.csv com os dados separados por VIRGULA
    // (esse arquivo pode ser aberto no Excel).
    // ====================================================================
    void exportar() {                        // inicio da funcao exportar
        struct Produto lista[100];           // vetor com os produtos do arquivo
        int qtd, i;                          // qtd = quantidade | i = contador
        FILE *csv;                           // ponteiro para o arquivo CSV

        qtd = carregar(lista);               // le os produtos do arquivo
        if (qtd == -1) {                     // se o arquivo nao existe
            printf("Arquivo nao existe! Escolha a opcao 1 primeiro.\n");   // avisa o usuario
            return;                          // sai da funcao
        }                                    // fim do if

        csv = fopen("produtos.csv", "w");    // cria o arquivo CSV para escrita
        fprintf(csv, "ID,Produto,Categoria,Preco,Quantidade,Fornecedor\n");   // primeira linha: o nome de cada coluna
        for (i = 0; i < qtd; i++) {          // passa por todos os produtos
            fprintf(csv, "%d,%s,%s,%.2f,%d,%s\n",   // escreve uma linha separando os campos por virgula
                    lista[i].id, lista[i].nome, lista[i].categoria,          // id, nome e categoria
                    lista[i].preco, lista[i].quantidade, lista[i].fornecedor);   // preco, quantidade e fornecedor
        }                                    // fim do for
        fclose(csv);                         // fecha o arquivo CSV

        printf("Arquivo produtos.csv criado!\n");   // avisa que deu certo
    }                                        // fim da funcao

    // ====================================================================
    // FUNCAO PRINCIPAL (main)
    // Todo programa em C comeca por aqui.
    // Mostra o menu, le a opcao e chama a funcao certa.
    // ====================================================================
    int main() {                             // inicio do programa
        int opcao;                           // guarda a opcao escolhida pelo usuario

        do {                                 // do...while: executa o menu PELO MENOS uma vez
            printf("\n===== CADASTRO DA LOJA =====\n");   // titulo do menu
            printf("1  - Criar arquivo\n");                // opcao 1
            printf("2  - Listar produtos\n");              // opcao 2
            printf("3  - Inserir produto\n");              // opcao 3
            printf("4  - Alterar produto\n");              // opcao 4
            printf("5  - Excluir produto\n");              // opcao 5
            printf("6  - Buscar por nome\n");              // opcao 6
            printf("7  - Buscar por categoria\n");         // opcao 7
            printf("8  - Ordenar por nome (A-Z)\n");       // opcao 8
            printf("9  - Ordenar por nome (Z-A)\n");       // opcao 9
            printf("10 - Ordenar por preco (menor -> maior)\n");   // opcao 10
            printf("11 - Ordenar por preco (maior -> menor)\n");   // opcao 11
            printf("12 - Exportar para CSV\n");            // opcao 12
            printf("0  - Sair\n");                         // opcao 0
            printf("Escolha: ");                           // pede a escolha do usuario
            scanf("%d", &opcao);                           // le a opcao digitada e guarda em "opcao"

            // switch escolhe qual funcao chamar de acordo com o valor de "opcao"
            switch (opcao) {                 // verifica o valor de opcao
                case 1:  criar();      break;   // se for 1, chama criar() (break sai do switch)
                case 2:  listar();     break;   // se for 2, chama listar()
                case 3:  inserir();    break;   // se for 3, chama inserir()
                case 4:  alterar();    break;   // se for 4, chama alterar()
                case 5:  excluir();    break;   // se for 5, chama excluir()
                case 6:  buscar(1);    break;   // se for 6, busca por nome (tipo 1)
                case 7:  buscar(2);    break;   // se for 7, busca por categoria (tipo 2)
                case 8:  ordenar(1);   break;   // se for 8, ordena por nome A-Z
                case 9:  ordenar(2);   break;   // se for 9, ordena por nome Z-A
                case 10: ordenar(3);   break;   // se for 10, ordena por preco menor-maior
                case 11: ordenar(4);   break;   // se for 11, ordena por preco maior-menor
                case 12: exportar();   break;   // se for 12, exporta para CSV
                case 0:  printf("Saindo...\n"); break;   // se for 0, mostra mensagem de saida
                default: printf("Opcao invalida!\n");    // qualquer outro numero: avisa que e invalido
            }                                // fim do switch
        } while (opcao != 0);                // repete o menu enquanto a opcao for diferente de 0

        return 0;                            // return 0 avisa ao sistema que o programa terminou sem erros
    }                                        // fim do programa