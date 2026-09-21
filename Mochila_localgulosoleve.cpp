#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

struct ItemMochila { int peso; int valor; };

bool carregar_mochila(const string& nome, vector<ItemMochila>& itens) {
    ifstream arquivo(nome);
    if (!arquivo.is_open()) return false;
    
    itens.clear();
    int peso, valor;
    
    while (arquivo >> peso >> valor) {
        itens.push_back({peso, valor});
    }
    
    return true;
}

int avaliar_mochila(string solucao, vector<ItemMochila>& itens, int cap) {
    int peso = 0, valor = 0;
    for (size_t i = 0; i < solucao.length(); i++) {
        if (solucao[i] == '1') {
            peso += itens[i].peso;
            valor += itens[i].valor;
        }
    }
    return (peso > cap) ? -1 : valor;
}

string gulosoMaisLeves(const vector<ItemMochila>& itens, int cap) {
    int n = itens.size();
    vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[i] = i;

    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return itens[a].peso < itens[b].peso;
    });

    string solucao(n, '0');
    int pesoAtual = 0;
    
    for (int i = 0; i < n; i++) {
        int idx = indices[i];
        if (pesoAtual + itens[idx].peso <= cap) {
            pesoAtual += itens[idx].peso;
            solucao[idx] = '1';
        }
    }
    return solucao;
}

int main() {

    vector<ItemMochila> itens;
    
    if (!carregar_mochila("mochila.txt", itens)) {
        cout << "Erro ao ler arquivo.\n";
        return 1;
    }

    int cap = 50; 
    int n = itens.size();
    
    string solucao_atual = gulosoMaisLeves(itens, cap);
    int melhor_valor = avaliar_mochila(solucao_atual, itens, cap);
    
    cout << "\nGuloso (Itens Mais Leves)\n";
    cout << "Solucao: " << solucao_atual << "\nValor: " << melhor_valor << "\n\n";

    for (int iter = 0; iter < 1000; iter++) {
        bool melhorou = false;
        for (int i = 0; i < n; i++) {
            string vizinho = solucao_atual;
            vizinho[i] = (vizinho[i] == '0') ? '1' : '0'; 
            int valor_viz = avaliar_mochila(vizinho, itens, cap);
            
            if (valor_viz > melhor_valor) {
                solucao_atual = vizinho;
                melhor_valor = valor_viz;
                melhorou = true;
                break;
            }
        }
        if (!melhorou) break;
    }
    cout << "Apos Busca Local\n";
    cout << "Melhor solucao: " << solucao_atual << "\nValor: " << melhor_valor << "\n";
    
    return 0;
}
