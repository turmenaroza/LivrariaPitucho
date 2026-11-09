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

