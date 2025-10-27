/*@ <authors>
 *
 * E86 Nombre Apellidos
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

// Definición de Dijkstra estándar (versión simple) usando IndexPQ
class RutasDijkstra {
public:
    RutasDijkstra(DigrafoValorado<Valor> const& g, int origen) :
        origen_(origen),
        dist_to_(g.V(), INF_VAL)
    {
        dijkstra(g);
    }

    Valor distTo(int v) const {
        return dist_to_[v];
    }

private:
    int origen_;
    vector<Valor> dist_to_;

    void dijkstra(DigrafoValorado<Valor> const& g) {
        dist_to_[origen_] = 0;

        IndexPQ<Valor> pq(g.V());
        pq.push(origen_, 0); //

        while (!pq.empty()) {
            int v = pq.top().elem; //
            pq.pop(); //

            for (auto arista : g.ady(v)) {
                int w = arista.hasta();
                Valor peso = arista.valor();

                if (dist_to_[v] != INF_VAL && dist_to_[w] > dist_to_[v] + peso) {
                    dist_to_[w] = dist_to_[v] + peso;
                    pq.update(w, dist_to_[w]); //
                }
            }
        }
    }
};


/*@ <answer>

 La solución aplica **Dijkstra** sobre el **grafo inverso** ($G^{inv}$), usando una **Cola de Prioridad Indexada (`IndexPQ.h`)**.

 Se calcula $G^{inv}$ con `G.inverso()`. Luego, se ejecuta Dijkstra *desde la salida S* en $G^{inv}$. La distancia resultante `dist_to_[v]` es el tiempo mínimo real para ir de `v` a `S` en el grafo original.

 Finalmente, se cuentan cuántas celdas `v` (incluida la salida) cumplen `dist_to_[v] <= T`.

 El coste es **$\mathcal{O}((P+N)\log N)$** por caso (P = pasadizos, N = celdas).

 @ </answer> */


 // ================================================================
 // Escribe el código completo de tu solución aquí debajo
 // ================================================================
 //@ <answer>

bool resuelveCaso() {
    int N, S_in, T, P;
    if (!(cin >> N >> S_in >> T >> P)) return false;

    int S = S_in - 1;
    DigrafoValorado<Valor> G(N);

    for (int i = 0; i < P; ++i) {
        int v_in, w_in;
        Valor tiempo;
        cin >> v_in >> w_in >> tiempo;
        int v = v_in - 1;
        int w = w_in - 1;
        G.ponArista({ v, w, tiempo });
    }

    // 1. Crear el grafo inverso
    DigrafoValorado<Valor> G_inverso = G.inverso();

    // 2. Ejecutar Dijkstra desde la SALIDA (S) en el grafo inverso
    RutasDijkstra solver(G_inverso, S);

    int ratones_escapados = 0;

    // 3. Contar ratones 
    for (int v = 0; v < N; ++v) {
        Valor dist_a_salida = solver.distTo(v);
        if (dist_a_salida <= T) {
            ratones_escapados++;
        }
    }

    cout << ratones_escapados << "\n";
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