/*@ <authors>
 *
 * 7_01
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <algorithm> 
#include <string>
#include <vector>
#include <limits>
#include <sstream>


using namespace std;

#include "DigrafoValorado.h"
#include "IndexPQ.h" 


using Valor = int;
static constexpr Valor INF_VAL = std::numeric_limits<Valor>::max();

class RutasMinimas {
public:
    RutasMinimas(DigrafoValorado<Valor> const& g, int origen) :
        origen_(origen),
        dist_to_(g.V(), INF_VAL),
        parent_(g.V(), -1)
    {
        dijkstra(g);
    }

    // Devuelve la distancia mínima desde el origen hasta v
    Valor distTo(int v) const {
        return dist_to_[v];
    }

    // Devuelve una string con la ruta mínima desde el origen hasta v
    string pathTo(int v, int primer = 1) const {
        if (distTo(v) == INF_VAL) {
            return "NO LLEGA";
        }

        stringstream ss;
        vector<int> path;
        for (int x = v; x != -1; x = parent_[x]) {
            path.push_back(x);
        }
        reverse(path.begin(), path.end());

        ss << distTo(v) << ":";
        for (size_t i = 0; i < path.size(); ++i) {
            ss << " " << (path[i] + primer);
            if (i < path.size() - 1) {
                ss << "->";
            }
        }
        return ss.str();
    }

private:
    int origen_;
    vector<Valor> dist_to_;
    vector<int> parent_; // Vector de predecesores

    void dijkstra(DigrafoValorado<Valor> const& g) {
        dist_to_[origen_] = 0;

        // Cola indexada: Los elementos son los vértices (0 a N-1)
        // La prioridad es la distancia (Valor)
        IndexPQ<Valor> pq(g.V());
        pq.push(origen_, 0); //

        while (!pq.empty()) {
            // Sacamos el vértice con menor distancia
            int v = pq.top().elem; //
            pq.pop(); //

            // Ya no hay chequeo de obsoletos

            for (auto arista : g.ady(v)) {
                int w = arista.hasta();
                Valor peso = arista.valor();

                if (dist_to_[w] > dist_to_[v] + peso) {
                    // Camino mejorado
                    dist_to_[w] = dist_to_[v] + peso;
                    parent_[w] = v; // Registrar predecesor

                    // Actualizamos la prioridad de 'w' en la cola
                    // Si 'w' no estaba, lo inserta. Si estaba, actualiza.
                    pq.update(w, dist_to_[w]); //
                }
            }
        }
    }
};



/*@ <answer>

 La solución se basa en el algoritmo de **Dijkstra**, implementado con una **Cola de Prioridad Indexada (`IndexPQ.h`)**. Esta estructura es más eficiente que una cola de prioridad estándar, ya que permite la operación `update`, evitando entradas duplicadas en la cola.

 Se ejecuta Dijkstra desde el origen P. La clase `RutasMinimas` guarda la distancia mínima (`dist_to_`) y el predecesor (`parent_`) para cada nodo. La ruta se reconstruye iterando hacia atrás por el vector `parent_`.

 El coste de la solución es **O(K * (C log N + N log N))** usando `IndexPQ.h`, que se simplifica a **O(K * (C+N) log N)**. (K = num. pedidos, C = aristas, N = vértices).

 @ </answer> */


 // ================================================================
 // Escribe el código completo de tu solución aquí debajo
 // ================================================================
 //@ <answer>


bool resuelveCaso() {
    int N, C;
    if (!(cin >> N >> C)) return false;

    DigrafoValorado<int> G(N); // Usamos int para el tiempo

    for (int i = 0; i < C; ++i) {
        int v_in, w_in, c_in;
        cin >> v_in >> w_in >> c_in;
        int v = v_in - 1;
        int w = w_in - 1;
        G.ponArista({ v, w, c_in });
        G.ponArista({ w, v, c_in });
    }

    int K;
    cin >> K;

    for (int i = 0; i < K; ++i) {
        int P_in, Q_in;
        cin >> P_in >> Q_in;
        int P = P_in - 1;
        int Q = Q_in - 1;

        RutasMinimas dijkstra_solver(G, P);

        cout << dijkstra_solver.pathTo(Q, 1) << "\n";
    }

    cout << "--\n";
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