#include "graph.hpp"

//Constrói o grafo a partir do tabuleiro do jogo
Graph::Graph(vector<vector<int>> board, int n){
    _vertexes = n * n;
    _graph.resize(_vertexes);

    if (this == NULL) return;

    position posAux;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(board[i][j] == 0){
                if(inBounds(i, j+1, n) && board[i][j+1] == 0){
                    posAux._predecessor = i * n + j;
                    posAux._next = i * n + (j+1);
                    posAux._direction = 'R';
                    addEdge(i * n + j, posAux);
                }

                if(inBounds(i, j-1, n) && board[i][j-1] == 0){
                    posAux._predecessor = i * n + j;
                    posAux._next = i * n + (j-1);
                    posAux._direction = 'L';
                    addEdge(i * n + j, posAux);
                }     

                if(inBounds(i-1, j, n) && board[i-1][j] == 0){
                    posAux._predecessor = i * n + j;
                    posAux._next = (i-1) * n + j;
                    posAux._direction = 'U';
                    addEdge(i * n + j, posAux);
                }

                if(inBounds(i+1, j, n) && board[i+1][j] == 0){
                    posAux._predecessor = i * n + j;
                    posAux._next = (i+1) * n + j;
                    posAux._direction = 'D';
                    addEdge(i * n + j, posAux);
                }
            }  
        }
    }    
}

//Adiciona uma aresta entre dois vértices do grafo
void Graph::addEdge(int originVertex, position destinyVertex){
    _graph[originVertex]._near.push_back(destinyVertex);
}      

//Retorna a quantidade de vértices do grafo
int Graph::getVertexes(){
    return _vertexes;
}

//Retorna o nó do grafo
vertex Graph::getNode(int position){
   return _graph[position];  
}

//Verifica se vértices adjacentes estão dentro do tabuleiro
bool Graph::inBounds(int i, int j, int n){
    if(i >= 0 && i < n){
        if(j >= 0 && j < n){
            return true;
        }
    }

    return false;
}

//Printa o gráfico, mostrando cada vértice e suas adjacências
//(Usado meramente para testes)
void Graph::printGraph(){
    for (int i = 0; i < _vertexes; i++){
        int actualEdges = _graph[i]._near.size();
        
        cout << i;
        for (int j = 0; j < actualEdges; j++){
            cout << "->" << _graph[i]._near[j]._next;
        }

        cout << endl;
    }
}

//Reseta as cores de todos os vértices. Fundamental para a função do BFS.
void Graph::resetGraph(){
    for(int i = 0; i < _vertexes; i++) {
        _graph[i]._color = 0;
        _graph[i].distance = 0;
    }
}

//Função de busca em largura
vector<char> Graph::BreadthFirstSearch(int pacmanPosition, int ghostPosition, bool &foundByGhost){
    int tamPath = 0;
    int current;
    bool end = false;
    position posAux;
    vector<position> path; //Vetor para guardar todos os vertices percorridos
    queue<int> nextVertex;

    //Verifica se o fantasma passou pelo pacman
    if(ghostPosition == pacmanPosition) {
        end = true;
        foundByGhost = true;
    }

    //Pinta o vertice atual de cinza
    _graph[pacmanPosition]._color = 1;

    //Seta a distancia como 0 (posicao do Pacman)
    _graph[pacmanPosition].distance = 0;
    
    //Insere na fila o vértice atual
    nextVertex.push(pacmanPosition);

    // Enquanto não achar o fantasma e enquanto a fila não estiver vazia
    while(!end && !nextVertex.empty()){
        
        //Retira um vértice da fila
        current = nextVertex.front();
        nextVertex.pop();
        
        //Pinta o vértice de preto
        _graph[current]._color = 2;

        //Verifica cada vizinho do vertice preto
        for(int i = 0; i < _graph[current]._near.size(); i++){
            
            // Guarda o numero do vertice do vizinho
            int neighbor = _graph[current]._near[i]._next;

            //Se vizinho eh branco
            if (_graph[neighbor]._color == 0){

                //Atualiza a distancia do vizinho que vai ser visitado:
                int neighborPredecessor = _graph[current]._near[i]._predecessor;
                _graph[neighbor].distance = _graph[neighborPredecessor].distance + 1;

                //Insere no vetor de caminhos
                path.push_back(_graph[current]._near[i]);
                tamPath++;
                
                //Pinta o vizinho de cinza
                _graph[neighbor]._color = 1;

                //Verifica se o Pacman chegou no fantasma
                if(neighbor == ghostPosition) {
                    end = true;
                    break;
                }

                //Insere vizinho na fila
                nextVertex.push(_graph[current]._near[i]._next);

            }
        }

    }

    //Melhor caminho
    vector<char> bestPath;

    if(!foundByGhost && end) {
        // Guarda a menor distancia (a que chegou no fantasma)
        int distance = _graph[ghostPosition].distance;
        bestPath.resize(distance);

        // Indice para bestPath
        int j = distance - 1;

        //Guarda predecessor do fantasma
        int predecessor = path[tamPath - 1]._predecessor;
        
        //Insere movimento ao chegar no fantasma
        bestPath[j] = path[tamPath - 1]._direction;

        j--;

        //Insere os movimentos para chegar no fantasma (vetor de frente para tras):
        for(int i = tamPath - 2; i >= 0; i--) {
            if(path[i]._next == predecessor) {
                bestPath[j] = path[i]._direction;
                predecessor = path[i]._predecessor;
                j--;
            }
        }
    }

    //Reseta as cores dos vértices para a próxima busca
    resetGraph();

    return bestPath;
}

