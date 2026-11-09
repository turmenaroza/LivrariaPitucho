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

