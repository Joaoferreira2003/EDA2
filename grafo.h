/**
 * @file grafo.h
 * @brief Definições de estruturas e funções para manipulação de grafos de antenas.
 */

#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/**
 * @struct Antena
 * @brief Representa uma antena com frequência e posição no plano.
 */
typedef struct Antena {
    char frequencia;         /**< Frequência da antena. */
    int x, y;                /**< Coordenadas da antena. */
    bool visitado;           /**< Indica se a antena foi visitada (para buscas). */
    struct Aresta* arestas;  /**< Lista de conexões (arestas). */
    struct Antena* prox;     /**< Próxima antena na lista. */
} Antena;

/**
 * @struct Aresta
 * @brief Representa uma conexão entre duas antenas.
 */
typedef struct Aresta {
    Antena* destino;         /**< Antena de destino da conexão. */
    struct Aresta* prox;     /**< Próxima aresta na lista. */
} Aresta;

/**
 * @struct Grafo
 * @brief Representa um grafo contendo antenas e conexões.
 */
typedef struct {
    Antena* vertices;        /**< Lista de antenas (vértices). */
    int num_vertices;        /**< Número de antenas no grafo. */
} Grafo;

// ======== FUNÇÕES BÁSICAS ========

/**
 * @brief Cria um novo grafo vazio.
 * @return Ponteiro para o grafo criado.
 */
Grafo* criar_grafo();

/**
 * @brief Liberta a memória usada por um grafo.
 * @param grafo Ponteiro para o grafo a ser destruído.
 */
void destruir_grafo(Grafo* grafo);

/**
 * @brief Adiciona uma antena ao grafo.
 * @param grafo Ponteiro para o grafo.
 * @param frequencia Letra identificadora da frequência.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return Ponteiro para a antena criada.
 */
Antena* adicionar_antena(Grafo* grafo, char frequencia, int x, int y);

/**
 * @brief Conecta automaticamente antenas com mesma frequência.
 * @param grafo Ponteiro para o grafo.
 */
void conectar_antenas(Grafo* grafo);

// ======== BUSCAS ========

/**
 * @brief Executa busca em profundidade (DFS) a partir de uma antena.
 * @param grafo Ponteiro para o grafo.
 * @param inicio Antena de início.
 * @param callback Função a ser chamada para cada antena visitada.
 */
void dfs(Grafo* grafo, Antena* inicio, void (*callback)(Antena*));

/**
 * @brief Executa busca em largura (BFS) a partir de uma antena.
 * @param grafo Ponteiro para o grafo.
 * @param inicio Antena de início.
 * @param callback Função a ser chamada para cada antena visitada.
 */
void bfs(Grafo* grafo, Antena* inicio, void (*callback)(Antena*));

/**
 * @brief Encontra todos os caminhos possíveis entre duas antenas.
 * @param grafo Ponteiro para o grafo.
 * @param origem Antena de origem.
 * @param destino Antena de destino.
 * @param callback Função chamada para cada caminho encontrado.
 */
void encontrar_caminhos(Grafo* grafo, Antena* origem, Antena* destino, void (*callback)(Antena**, int));

/**
 * @brief Lista pares de antenas com frequências diferentes que se interceptam.
 * @param grafo Ponteiro para o grafo.
 * @param freqA Primeira frequência.
 * @param freqB Segunda frequência.
 * @param callback Função chamada com cada par de intersecção.
 */
void listar_intersecoes(Grafo* grafo, char freqA, char freqB, void (*callback)(Antena*, Antena*));

// ======== FICHEIROS ========

/**
 * @brief Carrega um grafo a partir de um ficheiro de texto.
 * @param filename Caminho do ficheiro.
 * @return Ponteiro para o grafo carregado.
 */
Grafo* carregar_grafo_arquivo(const char* filename);

/**
 * @brief Guarda o grafo num ficheiro binário.
 * @param grafo Ponteiro para o grafo.
 * @param filename Caminho do ficheiro binário.
 * @return true se foi salvo com sucesso.
 */
bool salvar_grafo_binario(Grafo* grafo, const char* filename);

/**
 * @brief Carrega o grafo de um ficheiro binário.
 * @param filename Caminho do ficheiro binário.
 * @return Ponteiro para o grafo carregado.
 */
Grafo* carregar_grafo_binario(const char* filename);

// ======== MATRIZ ========

/**
 * @brief Imprime a matriz de antenas (formato texto).
 * @param grafo Ponteiro para o grafo.
 * @param linhas Número de linhas.
 * @param colunas Número de colunas.
 */
void imprimir_matriz(Grafo* grafo, int linhas, int colunas);

/**
 * @brief Imprime a matriz no formato binário (8 bits por caractere).
 * @param grafo Ponteiro para o grafo.
 * @param linhas Número de linhas.
 * @param colunas Número de colunas.
 */
void imprimir_matriz_em_binario(Grafo* grafo, int linhas, int colunas);

// ======== UTILITÁRIOS ========

/**
 * @brief Encontra uma antena em coordenadas específicas.
 * @param grafo Ponteiro para o grafo.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return Ponteiro para a antena encontrada, ou NULL.
 */
Antena* encontrar_antena(Grafo* grafo, int x, int y);

/**
 * @brief Reinicia o estado de visitado de todas as antenas.
 * @param grafo Ponteiro para o grafo.
 */
void reiniciar_visitas(Grafo* grafo);

#endif // GRAFO_H