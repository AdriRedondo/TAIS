/*@ <authors>
 *
 * 7_03
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
 // #include <queue> // <- Se quita
using namespace std;

#include "DigrafoValorado.h"  // Propio
#include "IndexPQ.h" // <-- Incluimos la plantilla
using Distancia = int;
using Contador = long long; // Para el número de caminos (garantizado < 2^31)
static constexpr Distancia INF_DIST = std::numeric_limits<Distancia>::max();

class CaminoMinimoContado {
public:
    CaminoMinimoContado(DigrafoValorado<Distancia> const& g, int origen) :
        origen_(origen),
        dist_to_(g.V(), INF_DIST),
        num_caminos_(g.V(), 0)
    {
        dijkstra(g);
    }

    // Devuelve el número de caminos de distancia mínima al vértice v
    Contador numCaminos(int v) const {
        return num_caminos_[v];
    }

private:
    int origen_;
    vector<Distancia> dist_to_;
    vector<Contador> num_caminos_;

    void dijkstra(DigrafoValorado<Distancia> const& g) {
        dist_to_[origen_] = 0;
        num_caminos_[origen_] = 1; // Un camino de longitud 0 al origen

        IndexPQ<Distancia> pq(g.V());
        pq.push(origen_, 0); //

        while (!pq.empty()) {
            int v = pq.top().elem; //
            // La distancia está en dist_to_[v], no la necesitamos de la cola
            pq.pop(); //

            for (auto arista : g.ady(v)) {
                int w = arista.hasta();
                Distancia peso = arista.valor();

                // Caso 1: Se encuentra un camino estrictamente más corto
                if (dist_to_[w] > dist_to_[v] + peso) {
                    dist_to_[w] = dist_to_[v] + peso;
                    num_caminos_[w] = num_caminos_[v]; // Hereda el número de caminos de v
                    pq.update(w, dist_to_[w]); //
                }
                // Caso 2: Se encuentra un camino de igual longitud mínima
                else if (dist_to_[w] == dist_to_[v] + peso) {
                    // Suma el número de caminos de v
                    num_caminos_[w] += num_caminos_[v];
                    // No hay 'update' porque la prioridad (distancia) no ha cambiado
                }
            }
        }
    }
};

/*@ <answer>

 La solución se basa en una variante de **Dijkstra** que usa una **Cola de Prioridad Indexada (`IndexPQ.h`)** y mantiene dos vectores: `dist_to_` y `num_caminos_`.

 La lógica de conteo se aplica en la relajación:
 1. Si `dist_to_[w] > dist_to_[v] + peso`: Se encuentra un camino más corto. Se actualiza `dist_to_[w]`, se reinicia `num_caminos_[w] = num_caminos_[v]` y se llama a `pq.update(w, dist_to_[w])`.
 2. Si `dist_to_[w] == dist_to_[v] + peso`: Se encuentra un camino de igual longitud. Se suma `num_caminos_[w] += num_caminos_[v]`. No se actualiza la cola porque la prioridad no cambia.

 El coste es **$\mathcal{O}((C+N)\log N)$** por caso (C = calles, N = intersecciones).

 @ </answer> */


 // ================================================================
 // Escribe el código completo de tu solución aquí debajo
 // ================================================================
 //@ <answer>

bool resuelveCaso() {
    int N, C;
    if (!(cin >> N >> C)) return false;

    DigrafoValorado<int> G(N);

    for (int i = 0; i < C; ++i) {
        int v_in, w_in, c_in;
        cin >> v_in >> w_in >> c_in;

        int v = v_in - 1;
        int w = w_in - 1;

        G.ponArista({ v, w, c_in });
        G.ponArista({ w, v, c_in });
    }

    CaminoMinimoContado solver(G, 0); // Origen en 0 (casa)

    cout << solver.numCaminos(N - 1) << "\n"; // Destino N-1 (colegio)

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