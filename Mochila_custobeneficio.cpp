#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Item {
    int id;
    int peso;
    int valor;
    double custoBeneficio; 
};

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

bool compararCustoBeneficio(const Item& a, const Item& b) {
    return a.custoBeneficio > b.custoBeneficio;
}

int main() {
    vector<int> dados = extrairDadosLimpos("mochila.txt");

    if (dados.empty() || dados.size() % 2 != 0) {
        cout << "Erro: Ficheiro vazio ou com formato invalido (esperado: peso valor)." << endl;
        return 1;
    }

    int numItens = dados.size() / 2;
    vector<Item> itens(numItens);
    
    int index = 0;
    for (int i = 0; i < numItens; ++i) {
        itens[i].id = i + 1; 
        itens[i].peso = dados[index];
        itens[i].valor = dados[index + 1];
        itens[i].custoBeneficio = (itens[i].peso > 0) ? (double)itens[i].valor / itens[i].peso : 0;
        index += 2;
    }

    int capacidade = 50; 
    
    sort(itens.begin(), itens.end(), compararCustoBeneficio);
    
    int pesoAtual = 0;
    int valorTotal = 0;
    vector<int> itensEscolhidos;
    
    for (const auto& item : itens) {
        if (pesoAtual + item.peso <= capacidade) {
            pesoAtual += item.peso;
            valorTotal += item.valor;
            itensEscolhidos.push_back(item.id);
        }
    }
    
    cout << "Melhor Custo-Beneficio\n";
    cout << "Capacidade maxima: " << capacidade << "\n";
    cout << "Itens escolhidos (ID): ";
    for (int id : itensEscolhidos) cout << id << " ";
    cout << "\nPeso Total: " << pesoAtual << " / " << capacidade;
    cout << "\nValor Total: " << valorTotal << "\n";

    return 0;
}
