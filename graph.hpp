#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

typedef struct _position{
    int _predecessor; // Vértice predecessor do movimento
    int _next; // Vértice para onde o Pacman foi no movimento
    char _direction; // Movimento (U, D)
}position;

// Color: 0 = branco, 1 = cinza, 2 = preto
typedef struct _vertex{
    vector<position> _near;
    int distance;
    int _color = 0;
}vertex;


class Graph{
    private:
        int _vertexes;
        vector<vertex> _graph;
    public:
        Graph(vector<vector<int>> board, int vertexes);
        void addEdge(int originVertex, position destinyVertex);
        int getEdges();
        int getVertexes();
        vertex getNode(int position);
        bool inBounds(int i, int j, int n);
        void printGraph();
        void resetGraph();
        vector<char> BreadthFirstSearch(int pacmanPosition, int ghostPosition, bool &foundByGhost);
};


#endif