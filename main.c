#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>

// ----------------------------
// CALLBACKS PARA EXIBIÇÃO
// ----------------------------

void mostrar_antena(Antena* a) {
    printf("Antena %c (%d,%d)\n", a->frequencia, a->x, a->y);
}

void mostrar_caminho(Antena** caminho, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%c(%d,%d)", caminho[i]->frequencia, caminho[i]->x, caminho[i]->y);
        if (i < tamanho - 1) printf(" -> ");
    }
    printf("\n");
}

void mostrar_intersecao(Antena* a, Antena* b) {
    printf("%c(%d,%d) e %c(%d,%d)\n", a->frequencia, a->x, a->y, b->frequencia, b->x, b->y);
}

// ----------------------------
// MENU
// ----------------------------

void menu() {
    printf("\n=== MENU ===\n");
    printf("1. Carregar matriz (TXT)\n");
    printf("2. Mostrar matriz\n");
    printf("3. Mostrar grafo (lista de arestas)\n");
    printf("4. DFS\n");
    printf("5. BFS\n");
    printf("6. Encontrar caminhos entre antenas\n");
    printf("7. Listar intersecoes\n");
    printf("8. Guardar em binario\n");
    printf("9. Carregar de binario\n");
    printf("10. Mostrar matriz em binário\n");
    printf("0. Sair\n");
    printf("Escolha: ");
}

// ----------------------------
// MAIN
// ----------------------------

int main() {
    Grafo* grafo = NULL;
    int opcao;

    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (grafo) destruir_grafo(grafo);
                grafo = carregar_grafo_arquivo("C:\\Users\\joaop\\OneDrive\\Ambiente de Trabalho\\Coisas\\EDA2\\matriz.txt");
                printf(grafo ? "Matriz carregada com sucesso!\n" : "Erro ao carregar a matriz.\n");
                break;
            }

            case 2:
                if (grafo) imprimir_matriz(grafo, 12, 12);
                else printf("Carregue a matriz primeiro.\n");
                break;

            case 3:
                if (grafo) {
                    Antena* a = grafo->vertices;
                    while (a) {
                        printf("Antena %c (%d,%d) -> ", a->frequencia, a->x, a->y);
                        Aresta* ar = a->arestas;
                        while (ar) {
                            printf("%c(%d,%d) ", ar->destino->frequencia, ar->destino->x, ar->destino->y);
                            ar = ar->prox;
                        }
                        printf("\n");
                        a = a->prox;
                    }
                } else {
                    printf("Carregue a matriz primeiro.\n");
                }
                break;

            case 4: {
                if (!grafo) { printf("Carregue a matriz primeiro.\n"); break; }
                int x, y;
                printf("Coordenadas da antena (x y): ");
                scanf("%d %d", &x, &y);
                Antena* inicio = encontrar_antena(grafo, x, y);
                inicio ? dfs(grafo, inicio, mostrar_antena)
                       : printf("Antena não encontrada.\n");
                break;
            }

            case 5: {
                if (!grafo) { printf("Carregue a matriz primeiro.\n"); break; }
                int x, y;
                printf("Coordenadas da antena (x y): ");
                scanf("%d %d", &x, &y);
                Antena* inicio = encontrar_antena(grafo, x, y);
                inicio ? bfs(grafo, inicio, mostrar_antena)
                       : printf("Antena não encontrada.\n");
                break;
            }

            case 6: {
                if (!grafo) { printf("Carregue a matriz primeiro.\n"); break; }
                int x1, y1, x2, y2;
                printf("Origem (x y): ");
                scanf("%d %d", &x1, &y1);
                printf("Destino (x y): ");
                scanf("%d %d", &x2, &y2);
                Antena* o = encontrar_antena(grafo, x1, y1);
                Antena* d = encontrar_antena(grafo, x2, y2);
                (o && d) ? encontrar_caminhos(grafo, o, d, mostrar_caminho)
                         : printf("Antena(s) não encontrada(s).\n");
                break;
            }

            case 7: {
                if (!grafo) { printf("Carregue a matriz primeiro.\n"); break; }
                char f1, f2;
                printf("Frequência A: ");
                scanf(" %c", &f1);
                printf("Frequência B: ");
                scanf(" %c", &f2);
                listar_intersecoes(grafo, f1, f2, mostrar_intersecao);
                break;
            }

            case 8:
                if (grafo)
                    printf(salvar_grafo_binario(grafo, "grafo.bin")
                           ? "Grafo guardado com sucesso.\n"
                           : "Erro ao guardar grafo.\n");
                else printf("Carregue a matriz primeiro.\n");
                break;

            case 9:
                if (grafo) destruir_grafo(grafo);
                grafo = carregar_grafo_binario("grafo.bin");
                printf(grafo ? "Grafo carregado do binário.\n" : "Erro ao carregar grafo.\n");
                break;
            case 10:
                if (grafo) imprimir_matriz_em_binario(grafo, 12, 12);
                else printf("Carregue a matriz primeiro.\n");
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    if (grafo) destruir_grafo(grafo);
    return 0;
}