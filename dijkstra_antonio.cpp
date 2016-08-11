#include <stdio.h>		// Para o printf e o scanf
#include <limits>		// Para o limite INFINITO
#include <queue> 		// Para a biblioteca de vector, priority queue e iterator
#include <iostream>		// Para as funcoes cout e cin
#include <fstream>

using namespace std;
 

// Classe Nó, que contém um identificador de origem e o peso desse nó. 
 
class no {
    public: 
		// Variáveis publicas
		unsigned long origem;
		unsigned long peso;
		
		// Construtor da classe nó
		no(unsigned long _origem, unsigned long _peso) : origem(_origem),peso(_peso) {}
};
 
 
/*
*	Estrutura contendo um par de nó, tal que o primeiro unsigned long é a distância e o segundo long é o nó
*/
typedef pair<unsigned long, unsigned long> par_no;

// Definição de Infinito sendo este o numero maximo do unsigned long
const unsigned long INFINITO = numeric_limits<unsigned long>::max();
 
/*	
	Função Dijkstra, recebe como paramêtro:
		- unsigned origem: 								nó de origem
		- const vector< vector<no> > &lista:			lista de adjacência
		- vector<unsigned long> &distancias_minimas:	um vetor com todas as distâncias do nó de origem aos outros nós
	retorno:
		distancias_minimas contém as distâncias calculadas do nó de origem a todos os outros nós
*/

void Dijkstra(unsigned long origem, const vector< vector<no> > &lista, vector<unsigned long> &distancias_minimas)
{
	// Distância do nó para ele mesmo 0
	distancias_minimas[origem] = 0;
		
    priority_queue<par_no, vector<par_no>, greater<par_no> > fila_prioridade;
    fila_prioridade.push( make_pair(distancias_minimas[origem], origem) );
 
    while (!fila_prioridade.empty()) 
    {
		// Pega o primeiro elemento da fila de prioridade que contém o menor peso
        unsigned long dist = fila_prioridade.top().first;
        unsigned long u = fila_prioridade.top().second;        
		fila_prioridade.pop();		
		
		// Se o peso desse nó for maior que o peso encontrado até esse nó, descarto ele pois ele não é um bom candidato
		if (dist > distancias_minimas[u])
			continue;
			
		// Percorro todas as adjacências desse nó
		const vector<no> &nos = lista[u];
		// Esse for pode ser melhorado
		// Porque eu posso armazenar as adjacências já calculada para esse cara ao invés de ficar percorrendo tudo denovo
		// Perfomance ++
		for ( vector<no>::const_iterator iter = nos.begin(); iter != nos.end(); iter++)
		{
			// Pego a adjacencia do cara origem e destino
			unsigned long v = iter->origem;
			unsigned long peso = iter->peso;
			// Somo com a distância anterior
			unsigned long dist_ = dist + peso;
			
			// Distância menor que o já encontrado ?
			if (dist_ < distancias_minimas[v]) {
				// Substituo e coloco esse candidato na fila de prioridade
				distancias_minimas[v] = dist_;
				fila_prioridade.push( make_pair(distancias_minimas[v], v));									
			}
	 
		}
	}
}
 
 
int main(int argc, char *argv[])
{
	//clock_t t;
	//t = clock();
	
	unsigned long tamanho, vertices;
	unsigned long from, to;
	unsigned long cost;
	
	// Primeira entrada é o tamanho total de nós e o numero de ligações
	scanf("%lu %lu\n", &tamanho, &vertices);	
	vector< vector<no> > lista(tamanho);	
	// Recebo as ligações com os respectivos pesos
	for(unsigned int i = 0; i  < vertices; i++) { 		
		scanf("%lu %lu %lu\n", &from, &to, &cost);
		lista[from].push_back(no(to, cost));
	} 
	
	vector< vector<unsigned long> > distancias(tamanho);

	unsigned long origem, destino;
	char q;
	
	// Leio todas as perguntas e vou calculando o caminho
	while(scanf("%c %lu %lu\n", &q, &origem, &destino) != EOF) {
		
		if( origem < 0 || origem > tamanho || destino < 0 || destino > tamanho) {
			cout << "-1" << endl;
			//cout << origem << "->" << destino << ",cost=-1" <<  endl;
		}
		else {
			// Esse é o macete de guardar os caminhos já calculados...se tiver mais perguntas ele só vai na lista dos calculados e mostra pro cara.
			// Velocidade bem melhor..mas em compensação a memoria e um pouco maior.
			if( distancias[origem].size() == 0) {
				try {
					// Inicializo o vetor com todo mundo infinito
					distancias[origem].resize(tamanho, INFINITO);
					Dijkstra(origem, lista, distancias[origem]);
				}
				catch (const std::bad_alloc&) {
					cerr << "Não foi possível alocar mais memória para " << origem << "->" << destino << endl;
					//return -1;
				}
			}
			// Se ainda continuar com o valor 0 é porque deu bad alloc e não tem como mais allocar memoria...
			if( distancias[origem].size() != 0) 
			{
				
				if( distancias[origem][destino] == INFINITO) 
					//cout << origem << "->" << destino << ",cost=-1" <<  endl;
					cout << "-1" << endl;
				else
					cout << distancias[origem][destino] << endl;
					//cout << origem << "->" << destino << ",cost=" << distancias[origem][destino] <<  endl;
			}
		}
	}

	//t = clock() - t;
	//printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
  
	return 0;
}