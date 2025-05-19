/**
 * @file grafo.c
 * @brief Implementação das funções para manipulação de grafos de antenas.
 */

#include "grafo.h"
#include <string.h>

/**
 * @brief Cria um novo grafo vazio.
 * @return Ponteiro para o grafo criado.
 */
Grafo* criar_grafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->vertices = NULL;
    grafo->num_vertices = 0;
    return grafo;
}

/**
 * @brief Liberta toda a memória ocupada pelo grafo.
 * @param grafo Ponteiro para o grafo a destruir.
 */
void destruir_grafo(Grafo* grafo) {
    Antena* atual = grafo->vertices;
    while (atual) {
        Antena* prox = atual->prox;
        Aresta* aresta = atual->arestas;
        while (aresta) {
            Aresta* prox_aresta = aresta->prox;
            free(aresta);
            aresta = prox_aresta;
        }
        free(atual);
        atual = prox;
    }
    free(grafo);
}

/**
 * @brief Adiciona uma antena ao grafo.
 * @param grafo Ponteiro para o grafo.
 * @param frequencia Frequência da antena.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return Ponteiro para a antena criada.
 */
Antena* adicionar_antena(Grafo* grafo, char frequencia, int x, int y) {
    Antena* nova = (Antena*)malloc(sizeof(Antena));
    nova->frequencia = frequencia;
    nova->x = x;
    nova->y = y;
    nova->visitado = false;
    nova->arestas = NULL;
    nova->prox = grafo->vertices;
    grafo->vertices = nova;
    grafo->num_vertices++;
    return nova;
}

/**
 * @brief Conecta automaticamente antenas com a mesma frequência.
 * @param grafo Ponteiro para o grafo.
 */
void conectar_antenas(Grafo* grafo) {
    Antena* atual = grafo->vertices;
    while (atual) {
        Antena* outra = grafo->vertices;
        while (outra) {
            if (atual != outra && atual->frequencia == outra->frequencia) {
                bool conectado = false;
                Aresta* aresta = atual->arestas;
                while (aresta) {
                    if (aresta->destino == outra) {
                        conectado = true;
                        break;
                    }
                    aresta = aresta->prox;
                }
                if (!conectado) {
                    Aresta* nova_aresta = (Aresta*)malloc(sizeof(Aresta));
                    nova_aresta->destino = outra;
                    nova_aresta->prox = atual->arestas;
                    atual->arestas = nova_aresta;
                }
            }
            outra = outra->prox;
        }
        atual = atual->prox;
    }
}

/**
 * @brief Função auxiliar recursiva para DFS.
 * @param antena Antena atual.
 * @param callback Função chamada em cada visita.
 */
void dfs_util(Antena* antena, void (*callback)(Antena*)) {
    if (!antena || antena->visitado) return;
    antena->visitado = true;
    callback(antena);
    Aresta* aresta = antena->arestas;
    while (aresta) {
        dfs_util(aresta->destino, callback);
        aresta = aresta->prox;
    }
}

/**
 * @brief Executa busca em profundidade (DFS).
 * @param grafo Ponteiro para o grafo.
 * @param inicio Antena inicial.
 * @param callback Função chamada em cada antena visitada.
 */
void dfs(Grafo* grafo, Antena* inicio, void (*callback)(Antena*)) {
    reiniciar_visitas(grafo);
    dfs_util(inicio, callback);
}

/**
 * @brief Executa busca em largura (BFS).
 * @param grafo Ponteiro para o grafo.
 * @param inicio Antena inicial.
 * @param callback Função chamada em cada antena visitada.
 */
void bfs(Grafo* grafo, Antena* inicio, void (*callback)(Antena*)) {
    reiniciar_visitas(grafo);
    Antena** fila = (Antena**)malloc(grafo->num_vertices * sizeof(Antena*));
    int inicio_fila = 0, fim_fila = 0;
    fila[fim_fila++] = inicio;
    inicio->visitado = true;

    while (inicio_fila < fim_fila) {
        Antena* atual = fila[inicio_fila++];
        callback(atual);

        Aresta* aresta = atual->arestas;
        while (aresta) {
            if (!aresta->destino->visitado) {
                fila[fim_fila++] = aresta->destino;
                aresta->destino->visitado = true;
            }
            aresta = aresta->prox;
        }
    }

    free(fila);
}

/**
 * @brief Função auxiliar para encontrar caminhos entre antenas.
 */
void encontrar_caminhos_util(Antena* atual, Antena* destino, Antena** caminho, int* index, void (*callback)(Antena**, int)) {
    caminho[(*index)++] = atual;
    atual->visitado = true;

    if (atual == destino) {
        callback(caminho, *index);
    } else {
        Aresta* aresta = atual->arestas;
        while (aresta) {
            if (!aresta->destino->visitado) {
                encontrar_caminhos_util(aresta->destino, destino, caminho, index, callback);
            }
            aresta = aresta->prox;
        }
    }

    (*index)--;
    atual->visitado = false;
}

/**
 * @brief Encontra todos os caminhos entre duas antenas.
 */
void encontrar_caminhos(Grafo* grafo, Antena* origem, Antena* destino, void (*callback)(Antena**, int)) {
    reiniciar_visitas(grafo);
    Antena** caminho = (Antena**)malloc(grafo->num_vertices * sizeof(Antena*));
    int index = 0;
    encontrar_caminhos_util(origem, destino, caminho, &index, callback);
    free(caminho);
}

/**
 * @brief Lista interseções entre antenas de diferentes frequências.
 */
void listar_intersecoes(Grafo* grafo, char freqA, char freqB, void (*callback)(Antena*, Antena*)) {
    Antena* a = grafo->vertices;
    while (a) {
        if (a->frequencia == freqA) {
            Antena* b = grafo->vertices;
            while (b) {
                if (b->frequencia == freqB) {
                    if ((a->x == b->x || a->y == b->y) || (abs(a->x - b->x) == abs(a->y - b->y))) {
                        double distA = sqrt(a->x*a->x + a->y*a->y);
                        double distB = sqrt(b->x*b->x + b->y*b->y);
                        if (fabs(distA - 2*distB) < 1e-6 || fabs(distB - 2*distA) < 1e-6) {
                            callback(a, b);
                        }
                    }
                }
                b = b->prox;
            }
        }
        a = a->prox;
    }
}

/**
 * @brief Carrega o grafo de um ficheiro de texto.
 */
Grafo* carregar_grafo_arquivo(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    Grafo* grafo = criar_grafo();
    char linha[1024];
    int y = 0;

    while (fgets(linha, sizeof(linha), file)) {
        for (int x = 0; linha[x] != '\0' && linha[x] != '\n'; x++) {
            if (linha[x] != '.') {
                adicionar_antena(grafo, linha[x], x, y);
            }
        }
        y++;
    }

    fclose(file);
    conectar_antenas(grafo);
    return grafo;
}

/**
 * @brief Gera uma matriz de caracteres representando as antenas.
 */
void gerar_matriz(Grafo* grafo, int linhas, int colunas, char*** matriz) {
    *matriz = (char**)malloc(linhas * sizeof(char*));
    for (int i = 0; i < linhas; i++) {
        (*matriz)[i] = (char*)malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) {
            (*matriz)[i][j] = '.';
        }
    }

    Antena* atual = grafo->vertices;
    while (atual) {
        if (atual->y < linhas && atual->x < colunas) {
            (*matriz)[atual->y][atual->x] = atual->frequencia;
        }
        atual = atual->prox;
    }
}

/**
 * @brief Liberta a memória de uma matriz.
 */
void liberar_matriz(char** matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

/**
 * @brief Imprime a matriz no formato de texto.
 */
void imprimir_matriz(Grafo* grafo, int linhas, int colunas) {
    char** matriz = NULL;
    gerar_matriz(grafo, linhas, colunas, &matriz);
    if (!matriz) return;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c", matriz[i][j]);
        }
        printf("\n");
    }

    liberar_matriz(matriz, linhas);
}

/**
 * @brief Encontra uma antena pelas suas coordenadas.
 */
Antena* encontrar_antena(Grafo* grafo, int x, int y) {
    Antena* atual = grafo->vertices;
    while (atual) {
        if (atual->x == x && atual->y == y) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

/**
 * @brief Reinicia a marcação de "visitado" das antenas.
 */
void reiniciar_visitas(Grafo* grafo) {
    Antena* atual = grafo->vertices;
    while (atual) {
        atual->visitado = false;
        atual = atual->prox;
    }
}

/**
 * @brief Guarda o grafo num ficheiro binário.
 */
bool salvar_grafo_binario(Grafo* grafo, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) return false;

    fwrite(&grafo->num_vertices, sizeof(int), 1, file);
    Antena* atual = grafo->vertices;
    while (atual) {
        fwrite(&atual->frequencia, sizeof(char), 1, file);
        fwrite(&atual->x, sizeof(int), 1, file);
        fwrite(&atual->y, sizeof(int), 1, file);
        atual = atual->prox;
    }

    fclose(file);
    return true;
}

/**
 * @brief Carrega o grafo de um ficheiro binário.
 */
Grafo* carregar_grafo_binario(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;

    Grafo* grafo = criar_grafo();
    int num_vertices;
    if (fread(&num_vertices, sizeof(int), 1, file) != 1) {
        fclose(file);
        destruir_grafo(grafo);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++) {
        char freq;
        int x, y;
        if (fread(&freq, sizeof(char), 1, file) != 1 ||
            fread(&x, sizeof(int), 1, file) != 1 ||
            fread(&y, sizeof(int), 1, file) != 1) {
            fclose(file);
            destruir_grafo(grafo);
            return NULL;
        }
        adicionar_antena(grafo, freq, x, y);
    }

    fclose(file);
    conectar_antenas(grafo);
    return grafo;
}

/**
 * @brief Imprime a matriz no formato binário (em bits).
 */
void imprimir_matriz_em_binario(Grafo* grafo, int linhas, int colunas) {
    char** matriz = NULL;
    gerar_matriz(grafo, linhas, colunas, &matriz);
    if (!matriz) return;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            unsigned char c = matriz[i][j];
            for (int b = 7; b >= 0; b--) {
                printf("%d", (c >> b) & 1);
            }
            printf(" ");
        }
        printf("\n");
    }

    liberar_matriz(matriz, linhas);
}
