#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_BUILDINGS 10

typedef struct Node
{
    int dest;
    int cost;
    struct Node *next;
} Node;

typedef struct Graph
{
    Node *head[NUM_BUILDINGS + 1];
} Graph;

typedef struct MinHeapNode
{
    int v;
    int dist;
} MinHeapNode;

typedef struct MinHeap
{
    int size;
    int capacity;
    int *pos;
    MinHeapNode **array;
} MinHeap;

Node *createNode(int dest, int cost)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->cost = cost;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph()
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    for (int i = 0; i <= NUM_BUILDINGS; ++i)
    {
        graph->head[i] = NULL;
    }
    return graph;
}

// 새로운 노드를 헤드 위치 생성하고 헤드를 뒤로 미는 이유: 이런식으로 생성하게 된다면 노드를 추가할 때 시간은 O(1)이 된다. 탐색 시간이 길어지지만 노드를 추가할 일이 더 많다.
void addEdge(Graph *graph, int src, int dest, int cost)
{
    Node *newNode = createNode(dest, cost);
    newNode->next = graph->head[src];
    graph->head[src] = newNode;
}

MinHeapNode *newMinHeapNode(int v, int dist)
{
    MinHeapNode *minHeapNode = (MinHeapNode *)malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

MinHeap *createMinHeap(int capacity)
{
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode **)malloc(capacity * sizeof(MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b)
{
    MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap *minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx)
    {
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap *minHeap)
{
    return minHeap->size == 0;
}

MinHeapNode *extractMin(MinHeap *minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    MinHeapNode *root = minHeap->array[0];

    MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap *minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return 1;
    return 0;
}

void printArr(int dist[], int n, int prev[])
{
    printf("Vertex Distance from Source\n");
    for (int i = 1; i <= n; ++i)
    {
        printf("%d \t\t %d\n", i, dist[i]);
        if (dist[i] != INT_MAX)
        {
            printf("Path: ");
            int j = i;
            while (prev[j] != -1)
            {
                printf("%d <- ", j);
                j = prev[j];
            }
            printf("2\n");
        }
    }
}

void dijkstra(Graph *graph, int src)
{
    int dist[NUM_BUILDINGS + 1];
    int prev[NUM_BUILDINGS + 1];

    MinHeap *minHeap = createMinHeap(NUM_BUILDINGS);

    for (int v = 1; v <= NUM_BUILDINGS; ++v)
    {
        dist[v] = INT_MAX;
        prev[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = NUM_BUILDINGS;

    while (!isEmpty(minHeap))
    {
        MinHeapNode *minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        Node *pCrawl = graph->head[u];
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->cost + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->cost;
                prev[v] = u;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    printArr(dist, NUM_BUILDINGS, prev);
}

void generate_random_edges(Graph *graph, int num_edges)
{
    int count = 0;
    while (count < num_edges)
    {
        int u = rand() % NUM_BUILDINGS + 1;
        int v = rand() % NUM_BUILDINGS + 1;
        int cost = rand() % 10 + 1;

        if (u != v)
        {
            Node *temp = graph->head[u];
            int exists = 0;
            while (temp != NULL)
            {
                if (temp->dest == v)
                {
                    exists = 1;
                    break;
                }
                temp = temp->next;
            }
            if (!exists)
            {
                addEdge(graph, u, v, cost);
                count++;
            }
        }
    }
}

void printGraph(Graph *graph)
{
    for (int v = 1; v <= NUM_BUILDINGS; ++v)
    {
        Node *pCrawl = graph->head[v];
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d(%d) ", pCrawl->dest, pCrawl->cost);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

int main()
{
    Graph *graph = createGraph();
    int e;
    printf("Enter the number of edges (at least %d): ", NUM_BUILDINGS + 1);
    scanf("%d", &e);

    if (e < NUM_BUILDINGS + 1)
    {
        printf("Number of edges must be at least %d.\n", NUM_BUILDINGS + 1);
        return -1;
    }

    generate_random_edges(graph, e);
    printGraph(graph);

    int start = 2; // 팔달관
    dijkstra(graph, start);

    // Specific edge cost change for extra credit
    printf("\nChanging the cost of a specific edge to negative and recalculating shortest paths...\n");
    int u, v;
    printf("Enter the start and end vertices of the edge to change cost: ");
    scanf("%d %d", &u, &v);

    Node *temp = graph->head[u];
    while (temp != NULL)
    {
        if (temp->dest == v)
        {
            temp->cost = -temp->cost;
            break;
        }
        temp = temp->next;
    }

    dijkstra(graph, start);

    return 0;
}
