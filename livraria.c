/*
 * Sistema de Gestao da Livraria Pitucho
 * Disciplina: LPG - UDESC - Bacharelado em Ciencias da Computacao
 * Professor: Douglas Dutra
 * Integrantes: Julia Izabel Turmena Roza e Vitor Hugo Laert
 *
 * Regras do trabalho seguidas:
 *  - apenas tipos primitivos, vetores e matrizes (sem struct, sem malloc/calloc/free)
 *  - dados relacionados em vetores/matrizes paralelos, indexados pela mesma posicao
 *  - todas as dimensoes definidas com #define
 *  - texto lido com fgets (removendo o \n) e tratado com <string.h>
 *
 * Obs.: os textos do programa estao sem acento de proposito, para nao
 * aparecerem caracteres estranhos em terminais que nao usam UTF-8.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* ------------------------------ CONSTANTES ------------------------------ */
#define MAX_LIVROS        100
#define TAM_TITULO        60
#define TAM_AUTOR         40
#define TAM_CATEGORIA     20
#define MAX_CLIENTES      50
#define TAM_NOME          40
#define TAM_AUX           100   /* tamanho de buffers auxiliares */

/* colunas da matriz numerica dos livros (dados) */
#define COLUNAS           6
#define COL_CODIGO        0
#define COL_ANO           1
#define COL_ESTOQUE       2
#define COL_PRECO         3     /* em centavos */
#define COL_VENDIDOS      4
#define COL_LOCAL         5     /* 1 = autor local, 0 = nao */

/* colunas da matriz numerica dos clientes (compras) */
#define COLUNAS_CLIENTE   2
#define COL_C_COMPRAS     0     /* quantidade de compras feitas */
#define COL_C_GASTO       1     /* total gasto em centavos */

#define LIMITE_ESTOQUE_BAIXO 2
#define ESTOQUE_MAXIMO       999
#define PRECO_MAXIMO_REAIS   1000

/* ------------------------------ PROTOTIPOS ------------------------------ */
/* entrada de dados */
void limparResto(const char *buf);
void lerTexto(const char *msg, char *dest, int tam);
int  lerInteiro(const char *msg, int min, int max);
int  lerPreco(const char *msg);

/* auxiliares */
int  compararSemCase(const char *a, const char *b);
void formatarPreco(long long centavos, char *saida);
int  proximoCodigo(int dados[][COLUNAS], int total);

/* cadastro e exibicao */
int  adicionarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                    char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                    int *total, const char *titulo, const char *autor,
                    const char *categoria, int ano, int estoque, int preco,
                    int vendidos, int autorLocal);
int  adicionarCliente(char clientes[][TAM_NOME], int compras[][COLUNAS_CLIENTE],
                      int *totalClientes, const char *nome);
void carregarDadosIniciais(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                           char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                           int *totalLivros, char clientes[][TAM_NOME],
                           int compras[][COLUNAS_CLIENTE], int *totalClientes);
void cadastrarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                    char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                    int *total);
void cadastrarCliente(char clientes[][TAM_NOME], int compras[][COLUNAS_CLIENTE],
                      int *totalClientes);
void mostrarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                  char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int i);
void listarAcervo(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                  char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total);

/* busca */
int  buscarLivro(char lista[][TAM_TITULO], int total, const char *procurado);
int  buscarCliente(char lista[][TAM_NOME], int total, const char *procurado);
int  sugerirTitulos(char lista[][TAM_TITULO], int total, const char *procurado);
void menuBuscar(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total);

/* ordenacao */
int  deveTrocar(char titulos[][TAM_TITULO], int dados[][COLUNAS], int i, int j, int criterio);
void trocarLivros(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                  char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int i, int j);
void ordenarAcervo(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                   char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                   int total, int criterio);
void menuListar(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total);

/* alteracao de celulas, vendas e estoque */
int  alterarCelula(int matriz[][COLUNAS], int totalLinhas, int linha, int coluna, int novoValor);
void registrarVenda(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                    char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total,
                    char clientes[][TAM_NOME], int compras[][COLUNAS_CLIENTE],
                    int totalClientes, long long *faturamento, int *itensVendidos);
void editarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                 char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total);

/* funcoes recursivas */
int       totalEstoqueRec(int dados[][COLUNAS], int n);
long long valorEstoqueRec(int dados[][COLUNAS], int n);
int       listarEstoqueBaixoRec(char titulos[][TAM_TITULO], int dados[][COLUNAS],
                                int total, int indice);

/* relatorios */
void rankingLivros(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                   int dados[][COLUNAS], int total);
void listarAutoresLocais(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                         int dados[][COLUNAS], int total);
void rankingClientes(char clientes[][TAM_NOME], int compras[][COLUNAS_CLIENTE],
                     int totalClientes);
void menuRelatorios(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                    int dados[][COLUNAS], int total, char clientes[][TAM_NOME],
                    int compras[][COLUNAS_CLIENTE], int totalClientes,
                    long long faturamento, int itensVendidos);

/* ============================== ENTRADA ================================= */

/* Se a linha lida nao terminou em '\n', sobrou lixo no teclado: descarta. */
void limparResto(const char *buf)
{
    int c;
    if (strchr(buf, '\n') == NULL) {
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descartando */
        }
    }
}

/* Le uma linha de texto com fgets e remove o '\n' do final. */
void lerTexto(const char *msg, char *dest, int tam)
{
    int len;

    printf("%s", msg);
    if (fgets(dest, tam, stdin) == NULL) {
        printf("\nEntrada encerrada. Saindo do programa.\n");
        exit(0);
    }
    limparResto(dest);
    len = (int)strlen(dest);
    if (len > 0 && dest[len - 1] == '\n') {
        dest[len - 1] = '\0';
    }
}

/* Le um inteiro entre min e max; repete ate o usuario digitar algo valido. */
int lerInteiro(const char *msg, int min, int max)
{
    char buf[32];
    char *fim;
    long v;

    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo do programa.\n");
            exit(0);
        }
        limparResto(buf);

        v = strtol(buf, &fim, 10);
        if (fim == buf) {
            printf("Entrada invalida. Digite um numero.\n");
            continue;
        }
        while (*fim == ' ' || *fim == '\t' || *fim == '\n' || *fim == '\r') {
            fim++;
        }
        if (*fim != '\0') {
            printf("Entrada invalida. Digite apenas um numero.\n");
        } else if (v < min || v > max) {
            printf("Valor fora do intervalo (%d a %d).\n", min, max);
        } else {
            return (int)v;
        }
    }
}

/* Le um preco em reais (aceita 29.90 ou 29,90) e devolve em centavos. */
int lerPreco(const char *msg)
{
    char buf[32];
    char *fim;
    double v;
    int i;

    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            printf("\nEntrada encerrada. Saindo do programa.\n");
            exit(0);
        }
        limparResto(buf);

        for (i = 0; buf[i] != '\0'; i++) {
            if (buf[i] == ',') {
                buf[i] = '.';
            }
        }
        v = strtod(buf, &fim);
        if (fim == buf) {
            printf("Entrada invalida. Digite um valor, por exemplo 29,90.\n");
            continue;
        }
        while (*fim == ' ' || *fim == '\t' || *fim == '\n' || *fim == '\r') {
            fim++;
        }
        if (*fim != '\0') {
            printf("Entrada invalida. Digite um valor, por exemplo 29,90.\n");
        } else if (!(v > 0 && v <= PRECO_MAXIMO_REAIS)) {
            printf("O preco deve ser maior que zero e no maximo R$ %d,00.\n",
                   PRECO_MAXIMO_REAIS);
        } else {
            return (int)(v * 100 + 0.5);
        }
    }
}

/* ============================== AUXILIARES ============================== */

/* Compara duas strings ignorando maiusculas/minusculas (usa strcmp). */
int compararSemCase(const char *a, const char *b)
{
    char ma[TAM_AUX];
    char mb[TAM_AUX];
    int i;

    for (i = 0; a[i] != '\0' && i < TAM_AUX - 1; i++) {
        ma[i] = (char)tolower((unsigned char)a[i]);
    }
    ma[i] = '\0';
    for (i = 0; b[i] != '\0' && i < TAM_AUX - 1; i++) {
        mb[i] = (char)tolower((unsigned char)b[i]);
    }
    mb[i] = '\0';

    return strcmp(ma, mb);
}

/* Converte centavos para o texto "R$ 12,34". */
void formatarPreco(long long centavos, char *saida)
{
    sprintf(saida, "R$ %lld,%02lld", centavos / 100, centavos % 100);
}

/* Proximo codigo livre: maior codigo existente + 1 (comeca em 101). */
int proximoCodigo(int dados[][COLUNAS], int total)
{
    int i;
    int maior = 100;

    for (i = 0; i < total; i++) {
        if (dados[i][COL_CODIGO] > maior) {
            maior = dados[i][COL_CODIGO];
        }
    }
    return maior + 1;
}

/* ========================= CADASTRO E EXIBICAO ========================== */

/* Coloca um livro na proxima posicao livre de todos os vetores paralelos. */
int adicionarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                   char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                   int *total, const char *titulo, const char *autor,
                   const char *categoria, int ano, int estoque, int preco,
                   int vendidos, int autorLocal)
{
    int pos;

    if (*total >= MAX_LIVROS) {
        return 0;
    }
    pos = *total;

    strcpy(titulos[pos], titulo);
    strcpy(autores[pos], autor);
    strcpy(categorias[pos], categoria);

    dados[pos][COL_CODIGO]   = proximoCodigo(dados, *total);
    dados[pos][COL_ANO]      = ano;
    dados[pos][COL_ESTOQUE]  = estoque;
    dados[pos][COL_PRECO]    = preco;
    dados[pos][COL_VENDIDOS] = vendidos;
    dados[pos][COL_LOCAL]    = autorLocal;

    (*total)++;
    return 1;
}

int adicionarCliente(char clientes[][TAM_NOME], int compras[][COLUNAS_CLIENTE],
                     int *totalClientes, const char *nome)
{
    int pos;

    if (*totalClientes >= MAX_CLIENTES) {
        return 0;
    }
    pos = *totalClientes;
    strcpy(clientes[pos], nome);
    compras[pos][COL_C_COMPRAS] = 0;
    compras[pos][COL_C_GASTO]   = 0;
    (*totalClientes)++;
    return 1;
}

/* Dados de exemplo para o sistema ja abrir com alguma coisa na livraria. */
void carregarDadosIniciais(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                           char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                           int *totalLivros, char clientes[][TAM_NOME],
                           int compras[][COLUNAS_CLIENTE], int *totalClientes)
{
    /*                  titulo                          autor                   categoria      ano  est  preco vend local */
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Dom Casmurro", "Machado de Assis", "Romance", 1899, 8, 3490, 12, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Capitaes da Areia", "Jorge Amado", "Romance", 1937, 5, 4290, 9, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "O Pequeno Principe", "Antoine de Saint-Exupery", "Infantil", 1943, 15, 2990, 20, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Vidas Secas", "Graciliano Ramos", "Romance", 1938, 2, 3790, 7, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Memorias Postumas de Bras Cubas", "Machado de Assis", "Romance", 1881, 6, 3990, 4, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Codigo Limpo", "Robert C. Martin", "Tecnico", 2008, 4, 9990, 6, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Harry Potter e a Pedra Filosofal", "J. K. Rowling", "Fantasia", 1997, 10, 5490, 18, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "A Hora da Estrela", "Clarice Lispector", "Romance", 1977, 1, 3290, 3, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Sapiens", "Yuval Noah Harari", "Nao ficcao", 2011, 7, 6990, 11, 0);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "O Alquimista", "Paulo Coelho", "Romance", 1988, 9, 3990, 14, 0);
    /* autores locais (nomes de exemplo, troquem pelos autores da cidade) */
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Historias da Rua do Mercado", "Helena Prado", "Autor local", 2019, 6, 4500, 8, 1);
    adicionarLivro(titulos, autores, categorias, dados, totalLivros,
                   "Poemas de Fim de Tarde", "Rafael Duarte", "Autor local", 2021, 2, 3200, 5, 1);

    /* clientes de exemplo */
    adicionarCliente(clientes, compras, totalClientes, "Maria Souza");
    adicionarCliente(clientes, compras, totalClientes, "Carlos Pereira");
    adicionarCliente(clientes, compras, totalClientes, "Ana Beatriz Lima");
    compras[0][COL_C_COMPRAS] = 5; compras[0][COL_C_GASTO] = 21450;
    compras[1][COL_C_COMPRAS] = 2; compras[1][COL_C_GASTO] = 12980;
    compras[2][COL_C_COMPRAS] = 3; compras[2][COL_C_GASTO] = 15470;
}

void cadastrarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                    char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                    int *total)
{
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    char categoria[TAM_CATEGORIA];
    int ano, estoque, preco, autorLocal;

    printf("\n--- CADASTRAR LIVRO ---\n");

    if (*total >= MAX_LIVROS) {
        printf("O acervo esta cheio (%d livros). Nao e possivel cadastrar mais.\n", MAX_LIVROS);
        return;
    }

    lerTexto("Titulo: ", titulo, TAM_TITULO);
    if (strlen(titulo) == 0) {
        printf("O titulo nao pode ficar vazio. Cadastro cancelado.\n");
        return;
    }
    if (buscarLivro(titulos, *total, titulo) != -1) {
        printf("Ja existe um livro com esse titulo. Para colocar mais exemplares, use a opcao 5 do menu.\n");
        return;
    }

    lerTexto("Autor: ", autor, TAM_AUTOR);
    if (strlen(autor) == 0) {
        printf("O autor nao pode ficar vazio. Cadastro cancelado.\n");
        return;
    }
    lerTexto("Categoria (ex: Romance, Tecnico, Infantil): ", categoria, TAM_CATEGORIA);
    if (strlen(categoria) == 0) {
        printf("A categoria nao pode ficar vazia. Cadastro cancelado.\n");
        return;
    }

    ano        = lerInteiro("Ano de publicacao: ", 1450, 2026);
    estoque    = lerInteiro("Quantidade em estoque: ", 1, ESTOQUE_MAXIMO);
    preco      = lerPreco("Preco (em reais, ex: 39,90): ");
    autorLocal = lerInteiro("Autor local? (1 = sim, 0 = nao): ", 0, 1);

    if (adicionarLivro(titulos, autores, categorias, dados, total,
                       titulo, autor, categoria, ano, estoque, preco, 0, autorLocal)) {
        printf("Livro cadastrado com sucesso! Codigo: %d\n", dados[*total - 1][COL_CODIGO]);
    } else {
        printf("Nao foi possivel cadastrar o livro.\n");
    }
}

void cadastrarCliente(char clientes[][TAM_NOME], int compras[][COLUNAS_CLIENTE],
                      int *totalClientes)
{
    char nome[TAM_NOME];

    printf("\n--- CADASTRAR CLIENTE ---\n");

    if (*totalClientes >= MAX_CLIENTES) {
        printf("Limite de clientes atingido (%d).\n", MAX_CLIENTES);
        return;
    }

    lerTexto("Nome do cliente: ", nome, TAM_NOME);
    if (strlen(nome) == 0) {
        printf("O nome nao pode ficar vazio. Cadastro cancelado.\n");
        return;
    }
    if (buscarCliente(clientes, *totalClientes, nome) != -1) {
        printf("Esse cliente ja esta cadastrado.\n");
        return;
    }

    if (adicionarCliente(clientes, compras, totalClientes, nome)) {
        printf("Cliente cadastrado com sucesso!\n");
    }
}

void mostrarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                  char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int i)
{
    char precoTxt[24];

    formatarPreco(dados[i][COL_PRECO], precoTxt);
    printf("\n  Codigo.........: %d\n", dados[i][COL_CODIGO]);
    printf("  Titulo.........: %s\n", titulos[i]);
    printf("  Autor..........: %s\n", autores[i]);
    printf("  Categoria......: %s\n", categorias[i]);
    printf("  Ano............: %d\n", dados[i][COL_ANO]);
    printf("  Preco..........: %s\n", precoTxt);
    printf("  Estoque........: %d unidade(s)\n", dados[i][COL_ESTOQUE]);
    printf("  Vendidos.......: %d unidade(s)\n", dados[i][COL_VENDIDOS]);
    printf("  Autor local....: %s\n", dados[i][COL_LOCAL] ? "Sim" : "Nao");
}

void listarAcervo(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                  char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total)
{
    int i;
    char precoTxt[24];

    if (total == 0) {
        printf("\nO acervo esta vazio.\n");
        return;
    }

    printf("\n%-5s %-30s %-20s %-12s %-5s %-7s %-11s %-6s %-5s\n",
           "Cod", "Titulo", "Autor", "Categoria", "Ano", "Estoque", "Preco", "Vend.", "Local");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < total; i++) {
        formatarPreco(dados[i][COL_PRECO], precoTxt);
        printf("%-5d %-30.30s %-20.20s %-12.12s %-5d %-7d %-11s %-6d %-5s\n",
               dados[i][COL_CODIGO], titulos[i], autores[i], categorias[i],
               dados[i][COL_ANO], dados[i][COL_ESTOQUE], precoTxt,
               dados[i][COL_VENDIDOS], dados[i][COL_LOCAL] ? "Sim" : "Nao");
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Total de titulos no acervo: %d\n", total);
}

/* ================================= BUSCA ================================ */

/* Busca linear: recebe a matriz de titulos e devolve a posicao (ou -1). */
int buscarLivro(char lista[][TAM_TITULO], int total, const char *procurado)
{
    int i;

    for (i = 0; i < total; i++) {
        if (compararSemCase(lista[i], procurado) == 0) {
            return i;
        }
    }
    return -1;
}

int buscarCliente(char lista[][TAM_NOME], int total, const char *procurado)
{
    int i;

    for (i = 0; i < total; i++) {
        if (compararSemCase(lista[i], procurado) == 0) {
            return i;
        }
    }
    return -1;
}

/* Quando nao acha o titulo exato, mostra titulos que contem o texto digitado. */
int sugerirTitulos(char lista[][TAM_TITULO], int total, const char *procurado)
{
    int i, j, achou = 0;
    char minusc[TAM_AUX];
    char titMinusc[TAM_AUX];

    if (strlen(procurado) == 0) {
        return 0;
    }

    for (j = 0; procurado[j] != '\0' && j < TAM_AUX - 1; j++) {
        minusc[j] = (char)tolower((unsigned char)procurado[j]);
    }
    minusc[j] = '\0';

    for (i = 0; i < total; i++) {
        for (j = 0; lista[i][j] != '\0' && j < TAM_AUX - 1; j++) {
            titMinusc[j] = (char)tolower((unsigned char)lista[i][j]);
        }
        titMinusc[j] = '\0';

        if (strstr(titMinusc, minusc) != NULL) {
            if (!achou) {
                printf("Voce quis dizer algum destes?\n");
            }
            printf("  - %s\n", lista[i]);
            achou++;
        }
    }
    return achou;
}

void menuBuscar(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total)
{
    char titulo[TAM_TITULO];
    int pos;

    printf("\n--- BUSCAR LIVRO POR TITULO ---\n");
    lerTexto("Titulo: ", titulo, TAM_TITULO);

    pos = buscarLivro(titulos, total, titulo);
    if (pos == -1) {
        printf("Livro nao encontrado.\n");
        sugerirTitulos(titulos, total, titulo);
    } else {
        mostrarLivro(titulos, autores, categorias, dados, pos);
    }
}

/* ============================== ORDENACAO =============================== */

/* Retorna 1 se o livro i deve vir DEPOIS do livro j no criterio escolhido.
 * criterio 1 = titulo (A-Z), 2 = preco (menor primeiro), 3 = mais vendidos */
int deveTrocar(char titulos[][TAM_TITULO], int dados[][COLUNAS], int i, int j, int criterio)
{
    switch (criterio) {
        case 1:
            return compararSemCase(titulos[i], titulos[j]) > 0;
        case 2:
            return dados[i][COL_PRECO] > dados[j][COL_PRECO];
        case 3:
            return dados[i][COL_VENDIDOS] < dados[j][COL_VENDIDOS];
        default:
            return 0;
    }
}

/* Troca as posicoes i e j em TODOS os vetores/matrizes paralelos. */
void trocarLivros(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                  char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int i, int j)
{
    char tmpTitulo[TAM_TITULO];
    char tmpAutor[TAM_AUTOR];
    char tmpCategoria[TAM_CATEGORIA];
    int tmpNum;
    int c;

    strcpy(tmpTitulo, titulos[i]);
    strcpy(titulos[i], titulos[j]);
    strcpy(titulos[j], tmpTitulo);

    strcpy(tmpAutor, autores[i]);
    strcpy(autores[i], autores[j]);
    strcpy(autores[j], tmpAutor);

    strcpy(tmpCategoria, categorias[i]);
    strcpy(categorias[i], categorias[j]);
    strcpy(categorias[j], tmpCategoria);

    for (c = 0; c < COLUNAS; c++) {
        tmpNum = dados[i][c];
        dados[i][c] = dados[j][c];
        dados[j][c] = tmpNum;
    }
}

/* Bubble Sort implementado manualmente. */
void ordenarAcervo(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                   char categorias[][TAM_CATEGORIA], int dados[][COLUNAS],
                   int total, int criterio)
{
    int i, j;

    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - 1 - i; j++) {
            if (deveTrocar(titulos, dados, j, j + 1, criterio)) {
                trocarLivros(titulos, autores, categorias, dados, j, j + 1);
            }
        }
    }
}

void menuListar(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total)
{
    int criterio;

    printf("\n--- LISTAR ACERVO ---\n");
    printf("1 - Ordenar por titulo (A-Z)\n");
    printf("2 - Ordenar por preco (menor para maior)\n");
    printf("3 - Ordenar por mais vendidos\n");
    printf("0 - Listar sem ordenar\n");
    criterio = lerInteiro("Escolha: ", 0, 3);

    if (criterio != 0) {
        ordenarAcervo(titulos, autores, categorias, dados, total, criterio);
    }
    listarAcervo(titulos, autores, categorias, dados, total);
}

/* ======================= ALTERACAO, VENDAS E ESTOQUE ==================== */

/* Altera uma celula da matriz depois de conferir linha e coluna. */
int alterarCelula(int matriz[][COLUNAS], int totalLinhas, int linha, int coluna, int novoValor)
{
    if (linha < 0 || linha >= totalLinhas) {
        printf("Linha invalida.\n");
        return 0;
    }
    if (coluna < 0 || coluna >= COLUNAS) {
        printf("Coluna invalida.\n");
        return 0;
    }
    matriz[linha][coluna] = novoValor;
    return 1;
}

void registrarVenda(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                    char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total,
                    char clientes[][TAM_NOME], int compras[][COLUNAS_CLIENTE],
                    int totalClientes, long long *faturamento, int *itensVendidos)
{
    char titulo[TAM_TITULO];
    char nomeCliente[TAM_NOME];
    char precoTxt[24];
    int pos, posCliente, qtd, confirma;
    long long valor;

    printf("\n--- REGISTRAR VENDA ---\n");
    lerTexto("Titulo do livro: ", titulo, TAM_TITULO);

    pos = buscarLivro(titulos, total, titulo);
    if (pos == -1) {
        printf("Livro nao encontrado.\n");
        sugerirTitulos(titulos, total, titulo);
        return;
    }

    mostrarLivro(titulos, autores, categorias, dados, pos);
    if (dados[pos][COL_ESTOQUE] == 0) {
        printf("\nEste livro esta sem estoque no momento.\n");
        return;
    }

    qtd = lerInteiro("\nQuantidade vendida: ", 1, dados[pos][COL_ESTOQUE]);

    lerTexto("Cliente cadastrado (ENTER para venda sem cadastro): ", nomeCliente, TAM_NOME);
    posCliente = -1;
    if (strlen(nomeCliente) > 0) {
        posCliente = buscarCliente(clientes, totalClientes, nomeCliente);
        if (posCliente == -1) {
            printf("Cliente nao cadastrado. A venda sera registrada sem cliente.\n");
        } else {
            printf("Cliente: %s\n", clientes[posCliente]);
        }
    }

    valor = (long long)qtd * dados[pos][COL_PRECO];
    formatarPreco(valor, precoTxt);
    printf("\nTotal da venda: %s (%d x %s)\n", precoTxt, qtd, titulos[pos]);
    confirma = lerInteiro("Confirmar venda? (1 = sim, 0 = nao): ", 0, 1);

    if (!confirma) {
        printf("Venda cancelada.\n");
        return;
    }

    alterarCelula(dados, total, pos, COL_ESTOQUE, dados[pos][COL_ESTOQUE] - qtd);
    alterarCelula(dados, total, pos, COL_VENDIDOS, dados[pos][COL_VENDIDOS] + qtd);

    if (posCliente != -1) {
        compras[posCliente][COL_C_COMPRAS] += 1;
        compras[posCliente][COL_C_GASTO]   += (int)valor;
    }
    *faturamento += valor;
    *itensVendidos += qtd;

    printf("Venda registrada! Estoque restante: %d\n", dados[pos][COL_ESTOQUE]);
    if (dados[pos][COL_ESTOQUE] <= LIMITE_ESTOQUE_BAIXO) {
        printf("Atencao: estoque baixo. Vale repor este titulo.\n");
    }
}

void editarLivro(char titulos[][TAM_TITULO], char autores[][TAM_AUTOR],
                 char categorias[][TAM_CATEGORIA], int dados[][COLUNAS], int total)
{
    char titulo[TAM_TITULO];
    int pos, opcao, qtd, novo;

    printf("\n--- REPOR ESTOQUE / EDITAR LIVRO ---\n");
    lerTexto("Titulo do livro: ", titulo, TAM_TITULO);

    pos = buscarLivro(titulos, total, titulo);
    if (pos == -1) {
        printf("Livro nao encontrado.\n");
        sugerirTitulos(titulos, total, titulo);
        return;
    }
    mostrarLivro(titulos, autores, categorias, dados, pos);

    printf("\n1 - Repor estoque (somar unidades)\n");
    printf("2 - Corrigir preco\n");
    printf("3 - Definir estoque exato (contagem)\n");
    printf("4 - Marcar/desmarcar como autor local\n");
    printf("5 - Tirar de linha (zerar estoque)\n");
    printf("0 - Voltar\n");
    opcao = lerInteiro("Escolha: ", 0, 5);

    switch (opcao) {
        case 1:
            qtd = lerInteiro("Quantidade a adicionar: ", 1, ESTOQUE_MAXIMO);
            novo = dados[pos][COL_ESTOQUE] + qtd;
            if (novo > ESTOQUE_MAXIMO) {
                printf("O estoque nao pode passar de %d unidades.\n", ESTOQUE_MAXIMO);
            } else if (alterarCelula(dados, total, pos, COL_ESTOQUE, novo)) {
                printf("Estoque atualizado: %d unidades.\n", novo);
            }
            break;
        case 2:
            novo = lerPreco("Novo preco (em reais): ");
            if (alterarCelula(dados, total, pos, COL_PRECO, novo)) {
                printf("Preco atualizado.\n");
            }
            break;
        case 3:
            novo = lerInteiro("Estoque correto: ", 0, ESTOQUE_MAXIMO);
            if (alterarCelula(dados, total, pos, COL_ESTOQUE, novo)) {
                printf("Estoque atualizado: %d unidades.\n", novo);
            }
            break;
        case 4:
            novo = !dados[pos][COL_LOCAL];
            if (alterarCelula(dados, total, pos, COL_LOCAL, novo)) {
                printf("Agora este livro %s de autor local.\n", novo ? "e" : "NAO e");
            }
            break;
        case 5:
            if (alterarCelula(dados, total, pos, COL_ESTOQUE, 0)) {
                printf("Livro tirado de linha (estoque zerado).\n");
            }
            break;
        case 0:
            printf("Nenhuma alteracao feita.\n");
            break;
        default:
            printf("Opcao invalida.\n");
    }
}

/* =========================== FUNCOES RECURSIVAS ========================= */

/* Soma recursiva do estoque dos primeiros n livros. */
int totalEstoqueRec(int dados[][COLUNAS], int n)
{
    if (n == 0) {
        return 0;
    }
    return dados[n - 1][COL_ESTOQUE] + totalEstoqueRec(dados, n - 1);
}

/* Soma recursiva do valor (preco x estoque) dos primeiros n livros, em centavos. */
long long valorEstoqueRec(int dados[][COLUNAS], int n)
{
    if (n == 0) {
        return 0;
    }
    return (long long)dados[n - 1][COL_PRECO] * dados[n - 1][COL_ESTOQUE]
           + valorEstoqueRec(dados, n - 1);
}

/* Lista recursivamente os livros com estoque baixo e devolve quantos achou. */
int listarEstoqueBaixoRec(char titulos[][TAM_TITULO], int dados[][COLUNAS],
                          int total, int indice)
{
    int achou = 0;

    if (indice >= total) {
        return 0;
    }
    if (dados[indice][COL_ESTOQUE] <= LIMITE_ESTOQUE_BAIXO) {
        printf("  - %-34.34s (cod %d) estoque: %d\n", titulos[indice],
               dados[indice][COL_CODIGO], dados[indice][COL_ESTOQUE]);
        achou = 1;
    }
    return achou + listarEstoqueBaixoRec(titulos, dados, total, indice + 1);
}


