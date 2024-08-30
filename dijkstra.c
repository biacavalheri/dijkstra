#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INFINITO INT_MAX

// Função para encontrar o vértice com a menor distância, que ainda não foi incluído no conjunto de vértices processados
int menorDistancia(int dist[], int processado[], int n) {
    int min = INFINITO;
    int indice_min = -1;

    for (int v = 0; v < n; v++) {
        if (dist[v] <= min) {
            min = dist[v];
            indice_min = v;
    }
    }

    return indice_min;
}

// Implementação do algoritmo de Dijkstra
int* dijkstra(int **grafo, int origem, int n) {
    int *dist = (int *)malloc(n * sizeof(int)); // Array para armazenar a menor distância da origem até i
    int *processado = (int *)malloc(n * sizeof(int)); // Array que indica se o vértice foi processado

    if (dist == NULL || processado == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    // Inicializa todas as distâncias como INFINITO e processado[] como falso
    for (int i = 0; i < n; i++) {
        dist[i] = INFINITO;
        processado[i] = 0;
    }

    // A distância do vértice de origem para ele mesmo é sempre 0
    dist[origem] = 0;

    // Encontra o menor caminho para todos os vértices
    for (int count = 0; count < n - 1; count++) {
        int u = menorDistancia(dist, processado, n);

        // Marca o vértice como processado
        processado[u] = 1;

        // Atualiza o valor da distância dos vértices adjacentes do vértice selecionado
        for (int v = 0; v < n; v++) {
            if (!processado[v] && grafo[u][v] && dist[u] != INFINITO && dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
            }
        }
    }

    free(processado);
    return dist;
}

// Função para carregar o grafo de um arquivo
int** carregarGrafo(const char *nomeArquivo, int *n) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    // Contar o número de linhas e colunas no arquivo
    int linhas = 0;
    int colunas = 0;
    char ch;

    while ((ch = fgetc(arquivo)) != EOF) {
        if (ch == '\n') {
            linhas++;
        }
    }
    if (ch == '\0' && ftell(arquivo) != 0) {
        linhas++; // Contar a última linha se o arquivo não terminar com uma nova linha
    }
    rewind(arquivo);

    // Alocar memória para a matriz de adjacência
    int **grafo = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        grafo[i] = (int *)malloc(linhas * sizeof(int));
    }

    // Ler a matriz de adjacência
    *n = linhas;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < linhas; j++) {
            if (fscanf(arquivo, "%d", &grafo[i][j]) != 1) {
                printf("Erro ao ler o arquivo!\n");
                fclose(arquivo);
                exit(1);
            }
        }
    }

    fclose(arquivo);
    return grafo;
}

int main() {
    int **grafo;
    int n;
    int origem;
    int *distancias;

    // Como o código foi feito pensando em reutilizar o grafo gerado no exercício anterior, fizemos essa leitura de arquivo para implementação do algoritmo
    printf("Digite o nome do arquivo com a matriz adjacência do grafo: ");
    char nomeArquivo[100];
    scanf("%s", nomeArquivo);
    grafo = carregarGrafo(nomeArquivo, &n);

    // Exibir o grafo carregado
    printf("Grafo carregado:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", grafo[i][j]);
        }
        printf("\n");
    }
    printf("Digite o nó de origem (1 a %d): ", n);
    scanf("%d", &origem);

    if (origem < 1 || origem > n) {
        printf("Nó de origem inválido!\n");
        exit(1);
    }

    // Ajustar o índice do nó de origem para a base 0
    origem--;

    distancias = dijkstra(grafo, origem, n);
    printf("Distâncias a partir do nó %d:\n", origem + 1);
    for (int i = 0; i < n; i++) {
        if (distancias[i] == INFINITO) {
            printf("Nó %d: Infinito\n", i + 1);
        } else {
            printf("Nó %d: %d\n", i + 1, distancias[i]);
        }
    }
    for (int i = 0; i < n; i++) {
        free(grafo[i]);
    }
    free(grafo);
    free(distancias);

    return 0;
}
