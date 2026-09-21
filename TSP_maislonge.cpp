#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;


vector<int> extrairDadosLimpos(const string& nomeArquivo) {
    vector<int> dados;
    ifstream arquivo(nomeArquivo);
    string linha;
    if (!arquivo.is_open()) return dados; 
    
    while (getline(arquivo, linha)) {
        size_t pos = linha.find('#');
        if (pos != string::npos) linha = linha.substr(0, pos);
        
        stringstream ss(linha);
        int valor;
        while (ss >> valor) dados.push_back(valor);
    }
    arquivo.close();
    return dados;
}


void vizinhoMaisDistante(const vector<vector<int>>& matrizAdj, int inicio = 0) {
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
        
        if (proximo == -1) {
            cout << "Erro: Nao existe caminho para visitar todas as cidades a partir do no atual.\n";
            return;
        }
        
        atual = proximo;
        visitado[atual] = true;
        rota.push_back(atual);
        custoTotal += maiorDistancia;
    }
    
    if (matrizAdj[atual][inicio] > 0) {
        custoTotal += matrizAdj[atual][inicio];
        rota.push_back(inicio);
    } else {
        cout << "Aviso: Nao ha caminho de regresso direto a origem.\n";
    }
    
    cout << "Vizinho Mais Distante\nRota: ";
    for (size_t i = 0; i < rota.size(); ++i) {
        cout << (rota[i] + 1); 
        if (i < rota.size() - 1) cout << " -> ";
    }
    cout << "\nCusto Total: " << custoTotal << "\n\n";
}

struct Cidade {
    int id;
    int x;
    int y;
};

int main() {
    vector<int> dados = extrairDadosLimpos("cidadestsp.txt");

    if (dados.empty() || dados.size() % 3 != 0) {
        cout << "Erro: Ficheiro vazio ou com formato inválido (esperado: id x y)." << endl;
        return 1;
    }


    int numVertices = dados.size() / 3;
    vector<Cidade> cidades(numVertices);
    
    int index = 0;
    for (int i = 0; i < numVertices; ++i) {
        cidades[i].id = dados[index];
        cidades[i].x = dados[index + 1];
        cidades[i].y = dados[index + 2];
        index += 3;
    }


    vector<vector<int>> matrizAdj(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (i != j) {
                double dx = cidades[i].x - cidades[j].x;
                double dy = cidades[i].y - cidades[j].y;
                matrizAdj[i][j] = round(sqrt(dx * dx + dy * dy));
            }
        }
    }
    
    vizinhoMaisDistante(matrizAdj, 0);

    return 0;
}
