#include <iostream>
#include <vector>
#include "graph.hpp"

using namespace std;

/*
    Trabalho de Grafos
    João Pedro Buzzo Silva - NUSP 1042591
    Rafael Comitre Garcia Conrado - NUSP 13671806
    Victor Hugo Trigolo Amaral - NUSP 12688487

*/


// Struct auxiliar usada para contar os passos totais dados pelo pacman,
// assim como contar quantos passos foram dados em cada direção
typedef struct _steps{
    int total = 0;
    int up = 0;
    int down = 0;
    int right = 0;
    int left = 0;
} Steps;

// Lê a posição inicial do pacman ou do fantasma
void readPosition(int &position, int n);

// Atualiza a posição anterior do personagem
void updatePosition(int move, int &ghostBegin, int n, Graph graph);

// Atualiza a struct Steps com a quantidade de passos dada baseado no
// vetor de movimentos
void updateSteps(Steps &steps, vector<char> movements);

// Imprime a quantidade de passos dadas em cada direçao
void printSteps(Steps steps, vector<char> bestPath, bool found);


int main(){
    int n, m;
    int tile;
    char move;
    int pacmanBegin, ghostBegin;
    vector<char> bestPath;
    vector<char> movements;
    vector<vector<int>> board;
    Steps steps;
    bool foundByGhost = false;

    //Leitura do tamanho N do tabuleiro e alocação 
    cin >> n;
    board.resize(n);

    //Construção do tabuleiro
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> tile;
            board[i].push_back(tile);
        }
    }

    // Grafo é construído a partir do tabuleiro lido
    Graph graph = Graph(board, n);

    // Lê as posições iniciais do pacman e do fantasma
    readPosition(pacmanBegin, n);
    readPosition(ghostBegin, n);
    
    // Lê a quantidade de movimentos a serem feitos pelo fantasma
    cin >> m;
    
    // Para cada movimento do fantasma, faz-se uma busca do melhor caminho até ele.
    // O pacman então se move 1 posição e o fantasma se move 1 posição. Após o movimento
    // do fantasma, deve ser feita outra busca (caso o fantasma esteja dentro de uma parede
    // e não seja possível chegar a ele, por exemplo).
    for(int i = 0; i < m; i++) {
        
        //Procura o caminho até o fantasma
        bestPath = graph.BreadthFirstSearch(pacmanBegin, ghostBegin, foundByGhost);

        //Verifica se o vetor de melhor caminho não é vazio. 
        if(bestPath.size() > 0) {
            
            //Copia o primeiro passo para o vetor de movimentos do Pacman.
            movements.push_back(bestPath[0]);
        }

        //Leitura do movimento do fantasma
        cin >> move;

        //Se o vetor de movimentos não for vazio, atualiza a posição do
        //Pacman no grafo
        if(movements.size() > 0) {
            updatePosition(movements[i], pacmanBegin, n, graph);
        }

        //Atualiza a posição do fantasma no grafo
        updatePosition(move, ghostBegin, n, graph);

    }

    //Realiza uma última busca em profundidade depois que o fantasma parou de andar
    bestPath = graph.BreadthFirstSearch(pacmanBegin, ghostBegin, foundByGhost);
    
    //Atualiza o vetor de movimentos com os movimentos restantes
    for(int i = 0; i < bestPath.size(); i++){
        movements.push_back(bestPath[i]);
    }

    // Atualiza os passos
    updateSteps(steps, movements);
    // Imprime os passos no formato requisitado
    printSteps(steps, bestPath, foundByGhost);

    return 0;
}

void readPosition(int &position, int n) {
    int coordX, coordY;
    cin >> coordY;
    cin >> coordX;

    // A coordenada bidimensional é linearizada para tratar as posições,
    // uma vez que o grafo é linear
    position = coordX + coordY * n;
}

void updatePosition(int move, int &begin, int n, Graph graph) {
    
    // Atualiza a posição baseado no movimento realizado, pensando na linearização
    // feita em readPosition
    switch(move) {
        case 'U':
            if(begin - n >= 0) {
                begin = begin - n; // posição acima na matriz nxn
            }
            break;

        case 'D':
            if(begin + n < graph.getVertexes()) {
                begin = begin + n; // posição abaixo na matriz nxn
            }
            break;

        case 'R':
            if((begin + 1) % n != 0 && begin != 0) {
                begin = begin + 1; // posição imediatamente à direita na matriz nxn
            }
            break;

        case 'L':
            if(begin % n != 0 && begin != graph.getVertexes() - 1) {
                begin = begin - 1; // posição imediatamente à esquerda na matriz nxn
            }
            break;
    }
}

void updateSteps(Steps &steps, vector<char> movements){
    
    // Para cada movimento feito, atualiza a quantidade de passos
    // e incrementa na direção correta
    for(int i = 0; i < movements.size(); i++) {
        steps.total++;

        switch (movements[i]){
           case 'U':
            steps.up++;
            break;

        case 'D':
            steps.down++;
            break;

        case 'R':
            steps.right++;
            break;

        case 'L':
            steps.left++;
            break;

        }
    }
}

void printSteps(Steps steps, vector<char> bestPath, bool found){
    
    // Se o caminho não pode ser encontrado e o fantasma não passou
    // pelo pacman, então não foi possível encontrar um caminho
    if(bestPath.size() == 0 && !found) {
        cout << "Não foi possível achar um caminho";
    } else {
        cout << "Número de passos: " << steps.total << "\n";
        cout << "Movimentos para cima: " << steps.up << "\n";
        cout << "Movimentos para baixo: " << steps.down << "\n";
        cout << "Movimentos para esquerda: " << steps.left << "\n";
        cout << "Movimentos para direita: " << steps.right << "\n";
    }
}