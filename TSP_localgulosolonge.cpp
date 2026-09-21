#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <limits>

using namespace std;

struct Coordenada { double x, y; };

vector<vector<int>> carregar_matriz_tsp(const string& nome) {
    vector<Coordenada> cidades;
    ifstream arquivo(nome);
    string linha;
    if (!arquivo.is_open()) return {};
    
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        int id; double x, y;
        if (ss >> id >> x >> y) cidades.push_back({x, y});
    }
    
    int n = cidades.size();
    vector<vector<int>> matrizAdj(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double xd = cidades[i].x - cidades[j].x;
                double yd = cidades[i].y - cidades[j].y;
                matrizAdj[i][j] = (int)(sqrt(xd*xd + yd*yd) + 0.5);
            }
        }
    }
    return matrizAdj;
}

int avaliar_tsp(const vector<int>& caminho, const vector<vector<int>>& matriz) {
    int custo = 0, n = caminho.size();
    for (int i = 0; i < n - 1; i++) {
        if (matriz[caminho[i]][caminho[i+1]] == 0) return 999999;
        custo += matriz[caminho[i]][caminho[i+1]];
    }
    if (matriz[caminho[n-1]][caminho[0]] == 0) return 999999;
    return custo + matriz[caminho[n-1]][caminho[0]];
}


vector<int> vizinhoMaisDistante(const vector<vector<int>>& matrizAdj, int inicio = 0) {
    int numVertices = matrizAdj.size();
    vector<int> rota;
    vector<bool> visitado(numVertices, false);
    
    int atual = inicio;
    int custoTotal = 0;
    
    rota.push_back(atual);
    visitado[atual] = true;
    
    for (int i = 1; i < numVertices; ++i) {
        int proximo = -1;
        int maiorDistancia = -1;
        
        for (int j = 0; j < numVertices; ++j) {
            if (!visitado[j] && matrizAdj[atual][j] > 0 && matrizAdj[atual][j] > maiorDistancia) {
                maiorDistancia = matrizAdj[atual][j];
                proximo = j;
            }
        }
        
        if (proximo != -1) {
            atual = proximo;
            visitado[atual] = true;
            rota.push_back(atual);
        }
        
        
    }
    return rota;
}


int main() {
        

    vector<vector<int>> matriz = carregar_matriz_tsp("cidadestsp.txt");
    if (matriz.empty()) {
        cout << "Erro ao carregar matriz.\n";
        return 1;
    }

    int n = matriz.size();
    
    vector<int> solucao_atual = vizinhoMaisDistante(matriz, 0);
    int custo_guloso = avaliar_tsp(solucao_atual, matriz);
    
    cout << "\nVizinho Mais Proximo\nRota: ";
    for (int i = 0; i < n; i++) cout << solucao_atual[i] + 1 << " ";
    cout << "\nCusto Original: " << custo_guloso << "\n\n";

    int melhor_custo = custo_guloso;
    
    for (int iter = 0; iter < 1000; iter++) {
        bool melhorou = false;
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                vector<int> vizinho = solucao_atual;
                swap(vizinho[i], vizinho[j]);
                int custo_viz = avaliar_tsp(vizinho, matriz);
                
                if (custo_viz < melhor_custo) {
                    solucao_atual = vizinho;
                    melhor_custo = custo_viz;
                    melhorou = true;
                    break; 
                }
            }
            if (melhorou) break;
        }
        if (!melhorou) break;
    }

    cout << "Apos Busca Local\nMelhor rota: ";
    for (int i = 0; i < n; i++) cout << solucao_atual[i] + 1 << " ";
    cout << "\nCusto Refinado: " << melhor_custo << "\n";
    
    return 0;
}
