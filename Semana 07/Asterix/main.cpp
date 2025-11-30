/*@ <authors>
 *
 * E49 Adrián Redondo Gómez
 * E07 Pablo Bernal Calleja
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <algorithm>
#include<vector>
#include<limits>
using namespace std;

#include "GrafoValorado.h"
#include "IndexPQ.h"

/*@ <answer>
 
 Para este problema, hacemos una implementación del algoritmo de Dijkstra adaptado para que dado un vector de orígenes (que son las aldeas pertenecientes a un bando),
 calcule un vector con las distancias mínimas a las aldeas tanto de un bando como del contrario y las neutrales, pero solo consideramos las neutrales. Llamamos al algoritmo dos veces,
 una por bando, para construir el mapa de distancias de cada uno de ellos.

 El coste del algoritmo es O( (N + C) log N) siendo N el número de vértices del grafo (número de aldeas) y C el número de aristas del grafo (número de caminos que conectan las aldeas).
 
 @ </answer> */


// ================================================================
// Escribe el código completo de tu solución aquí debajo (después de la marca)
//@ <answer>

template <typename Valor>
class AldeaNeutra {
private:
    const Valor INF = std::numeric_limits<Valor>::max();
    vector<int> origen;
    std::vector<Valor> dist;
    IndexPQ<Valor> pq;
    
public:
    AldeaNeutra(GrafoValorado<Valor> const& g, vector<int> const& origen) : origen(origen),
        dist(g.V(), INF), pq(g.V()) {


        for (auto o:origen)
        {
            if (dist[o] == INF) {
                dist[o] = 0;
                pq.push(o, 0);
            }
        }
        while (!pq.empty()) {
            int v = pq.top().elem; pq.pop();
            for (auto a : g.ady(v)) {
                int w = a.otro(v);
                if (dist[w] > dist[v] + a.valor()) {
                    dist[w] = dist[v] + a.valor();
                    pq.update(w, dist[w]);
                }
            }
                
        }

    }


    vector<Valor> distancia() const { return dist; }

};

bool resuelveCaso() {
  
  // leemos la entrada
  int N, C;
  cin >> N >> C;
  if (!cin)
    return false;

  GrafoValorado<int> g(N);
  for (int i = 0; i < C; i++)
  {
      int ini, fin, a; cin >> ini >> fin >> a;
      g.ponArista({ ini - 1,fin - 1,a });
  }
  // leer el resto del caso y resolverlo
  int n_g; cin >> n_g;
  vector<int> galos(n_g);
  for (int i = 0; i < n_g; i++)
  {
      cin >> galos[i];
      galos[i] -= 1;
  }
  AldeaNeutra<int>distanciasGalos(g, galos);
  
  int n_r; cin >> n_r;
  vector<int> romanos(n_r);
  for (int i = 0; i < n_r; i++)
  {
      cin >> romanos[i];
      romanos[i] -= 1;
  }
  AldeaNeutra<int>distanciasRomanos(g, romanos);
  vector<int> dist_G = distanciasGalos.distancia();
  vector<int> dist_R = distanciasRomanos.distancia();
  int mejor_indice = -1, mejor_distancia = numeric_limits<int>::max();
  
  //teniendo las distancias a todas las aldeas desde cada uno de los bandos comparamos aldea por aldea, siempre que sean neutras, es decir, que su distancia a alguno de los
  //vértices de los bandos no sea 0. Guardaremos la menor distancia (diferencia en valor absoluto de la distancia de la aldea más cercana de cada bando a dicha aldea neutral)

  for (int i = 0; i < dist_G.size(); i++) {
      int diferencia = abs(dist_G[i] - dist_R[i]);
      if (diferencia < mejor_distancia && dist_G[i] != 0 && dist_R[i] != 0) {
          mejor_distancia = diferencia;
          mejor_indice = i;
      }
  }
  cout << mejor_indice + 1 << ' ' << mejor_distancia << '\n';
  return true;
}

//@ </answer>
//  Lo que se escriba debajo de esta línea ya no forma parte de la solución.

int main() {
  // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
  ifstream in("casos.txt");
  if (!in.is_open())
    cout << "Error: no se ha podido abrir el archivo de entrada." << endl;
  auto cinbuf = cin.rdbuf(in.rdbuf());
#endif
  
  // Resolvemos
  while (resuelveCaso());
  
  // para dejar todo como estaba al principio
#ifndef DOMJUDGE
  cin.rdbuf(cinbuf);
#endif
  return 0;
}
