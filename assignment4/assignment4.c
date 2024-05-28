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
    minHeap->pos = (int *)malloc((capacity + 1) * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode **)malloc((capacity + 1) * sizeof(MinHeapNode *));
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
    left = 2 * idx;
    right = 2 * idx + 1;

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

    MinHeapNode *root = minHeap->array[1];
    MinHeapNode *lastNode = minHeap->array[minHeap->size];
    minHeap->array[1] = lastNode;

    minHeap->pos[root->v] = minHeap->size;
    minHeap->pos[lastNode->v] = 1;

    --minHeap->size;
    minHeapify(minHeap, 1);

    return root;
}

void decreaseKey(MinHeap *minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;

    while (i > 1 && minHeap->array[i]->dist < minHeap->array[i / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = i / 2;
        minHeap->pos[minHeap->array[i / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[i / 2]);
        i = i / 2;
    }
}

int isInMinHeap(MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] <= minHeap->size)
        return 1;
    return 0;
}

void printArr(int dist[], int n, int prev[])
{
    printf("Vertex Distance from Source\n");
    for (int i = 1; i <= n; ++i)
    {
        printf("Path(2->%d): ", i);
        if (dist[i] != INT_MAX)
        {
            int j = i;
            while (prev[j] != -1)
            {
                printf("%d <- ", j);
                j = prev[j];
            }
            printf("2");

            printf(" Distance: %d\n", dist[i]);
        }
        else
        {
            printf("No path\n");
        }
    }
}

void dijkstra(Graph *graph, int src)
{
    int dist[NUM_BUILDINGS + 1];
    int prev[NUM_BUILDINGS + 1];

    MinHeap *minHeap = createMinHeap(NUM_BUILDINGS + 1);

    for (int v = 1; v <= NUM_BUILDINGS; ++v)
    {
        dist[v] = INT_MAX;
        prev[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    dist[src] = 0;
    minHeap->array[src]->dist = 0;
    decreaseKey(minHeap, src, dist[src]);

    minHeap->size = NUM_BUILDINGS;

    while (!isEmpty(minHeap))
    {
        MinHeapNode *minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        free(minHeapNode); // extractMin에서 반환된 노드는 해제해야 함

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
    free(minHeap);
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
