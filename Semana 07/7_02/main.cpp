/*@ <authors>
 *
 * 7_02
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
 // #include <queue> // <- Se quita

using namespace std;

#include "DigrafoValorado.h"  // Plantilla de grafo dirigida valorada
#include "IndexPQ.h" // <-- Incluimos la plantilla
using Valor = int;
static constexpr Valor INF_VAL = std::numeric_limits<Valor>::max();

class RutasDijkstra {
public:
    // Recibe el grafo, el origen y el vector de tiempos de carga
    RutasDijkstra(DigrafoValorado<Valor> const& g, int origen, const vector<Valor>& tiempos_carga) :
        origen_(origen),
        dist_to_(g.V(), INF_VAL)
    {
        dijkstra(g, tiempos_carga);
    }

    Valor distTo(int v) const {
        return dist_to_[v];
    }

private:
    int origen_;
    vector<Valor> dist_to_;

    void dijkstra(DigrafoValorado<Valor> const& g, const vector<Valor>& tiempos_carga) {
        // Inicializamos con el tiempo de carga de la página de origen.
        dist_to_[origen_] = tiempos_carga[origen_];

        IndexPQ<Valor> pq(g.V());
        // La prioridad inicial es el tiempo de carga del origen
        pq.push(origen_, dist_to_[origen_]); //

        while (!pq.empty()) {
            int v = pq.top().elem; //
            pq.pop(); //

            for (auto arista : g.ady(v)) {
                int w = arista.hasta();
                // peso_arista = click(v,w) + carga(w)
                Valor peso_arista = arista.valor();

                if (dist_to_[w] > dist_to_[v] + peso_arista) {
                    dist_to_[w] = dist_to_[v] + peso_arista;
                    pq.update(w, dist_to_[w]); //
                }
            }
        }
    }
};

/*@ <answer>

 La solución se basa en el algoritmo de **Dijkstra** implementado con una **Cola de Prioridad Indexada (`IndexPQ.h`)**. La clave es modelar el peso de la arista $v \to w$ como: $W_{v,w} = \text{Tiempo\_click}(v, w) + \text{Tiempo\_carga}(w)$.

 Se inicializa la distancia del origen con su propio tiempo de carga. La función `update` de la `IndexPQ` se usa para actualizar las distancias eficientemente.

 El coste de la solución es **$\mathcal{O}((M+N)\log N)$** por caso, (M = enlaces, N = páginas).

 @ </answer> */


 // ================================================================
 // Escribe el código completo de tu solución aquí debajo
 // ================================================================
 //@ <answer>

bool resuelveCaso() {
    int N; // Número de páginas
    if (!(cin >> N) || N == 0) return false;

    vector<Valor> tiempos_carga(N);
    for (int i = 0; i < N; ++i) {
        cin >> tiempos_carga[i];
    }

    int M; // Número de enlaces
    cin >> M;
    DigrafoValorado<Valor> G(N);

    for (int i = 0; i < M; ++i) {
        int v_in, w_in;
        Valor t_click;
        cin >> v_in >> w_in >> t_click;

        int v = v_in - 1;
        int w = w_in - 1;

        Valor peso_arista = t_click + tiempos_carga[w];
        G.ponArista({ v, w, peso_arista });
    }

    RutasDijkstra solver(G, 0, tiempos_carga);

    Valor tiempo_minimo = solver.distTo(N - 1);

    if (tiempo_minimo == INF_VAL) {
        cout << "IMPOSIBLE\n";
    }
    else {
        cout << tiempo_minimo << "\n";
    }

    return true;
}

//@ </answer>
//  Lo que se escriba debajo de esta línea ya no forma parte de la solución.

int main() {
    // ... (main sin cambios) ...
#ifndef DOMJUDGE
    ifstream in("casos.txt");
    if (!in.is_open())
        cout << "Error: no se ha podido abrir el archivo de entrada." << std::endl;
    auto cinbuf = cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

#ifndef DOMJUDGE
    cin.rdbuf(cinbuf);
#endif
    return 0;
}