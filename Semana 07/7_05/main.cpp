/*@ <authors>
 *
 * 7_05
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
using Distancia = long long;
using NumAristas = int;
static constexpr Distancia INF_DIST = std::numeric_limits<Distancia>::max();

// Estado para el priority_queue y para almacenar los resultados
// Esta es la PRIORIDAD que usará IndexPQ.
struct EstadoRuta {
    Distancia dist;
    NumAristas aristas;

    // std::less (el comparador por defecto de IndexPQ) usa operator<
    // Define la prioridad: menor distancia, y a empate, menor num. aristas.
    bool operator<(const EstadoRuta& other) const {
        if (dist != other.dist) return dist < other.dist;
        return aristas < other.aristas;
    }
};

class RutasBicriterio {
public:
    using Resultado = EstadoRuta;

    RutasBicriterio(DigrafoValorado<Distancia> const& g, int origen) :
        origen_(origen),
        dist_y_aristas_(g.V(), { INF_DIST, INF_DIST }) // {dist, aristas}
    {
        dijkstra(g);
    }

    Resultado obtenerResultado(int v) const {
        return dist_y_aristas_[v];
    }

private:
    int origen_;
    vector<Resultado> dist_y_aristas_; // {distancia_minima, num_aristas_minimo}

    void dijkstra(DigrafoValorado<Distancia> const& g) {
        dist_y_aristas_[origen_] = { 0, 0 };

        // La prioridad es el 'EstadoRuta'
        IndexPQ<EstadoRuta> pq(g.V());
        pq.push(origen_, { 0, 0 }); //

        while (!pq.empty()) {
            int v = pq.top().elem; //
            // La prioridad actual (dist, aristas)
            EstadoRuta estado_actual = pq.top().prioridad; //
            pq.pop(); //

            // OJO: A diferencia de los otros, este chequeo SÍ es necesario.
            // ¿Por qué? Porque 'update' no sabe si (10, 3) es "peor" que (10, 2).
            // Si llamamos update(w, {10, 2}) y luego update(w, {10, 3}),
            // la IndexPQ podría (dependiendo de su implementación)
            // hundir el {10, 3}, pero el {10, 2} seguiría siendo el 'top'.
            // Edit: No, si usamos el '<' estricto en la relajación,
            // nunca procesaremos un estado peor.
            // if (estado_actual.dist > dist_y_aristas_[v].dist ||
            //    (estado_actual.dist == dist_y_aristas_[v].dist && 
            //     estado_actual.aristas > dist_y_aristas_[v].aristas))
            //    continue;
            // -> Este 'if' NO es necesario si usamos 'IndexPQ.h' y la relajación correcta.

            for (auto arista : g.ady(v)) {
                int w = arista.hasta();
                Distancia peso = arista.valor();

                // Calculamos el nuevo estado para 'w' viniendo desde 'v'
                EstadoRuta nuevo_estado = { estado_actual.dist + peso, estado_actual.aristas + 1 };

                // Comparamos usando el operator< que definimos
                if (nuevo_estado < dist_y_aristas_[w]) {
                    dist_y_aristas_[w] = nuevo_estado;
                    pq.update(w, nuevo_estado); //
                }
            }
        }
    }
};

/*@ <answer>

 La solución aplica una variante de **Dijkstra** usando una **Cola de Prioridad Indexada (`IndexPQ.h`)**.

 El problema es **bicriterio** (distancia, luego número de aristas). Para manejar esto, la *prioridad* que se almacena en la `IndexPQ` no es un `int`, sino una estructura `EstadoRuta {dist, aristas}`.

 Se define `operator<` para `EstadoRuta` para que la cola priorice la menor distancia y, en caso de empate, el menor número de aristas.

 La función `update` de la cola se usa para actualizar el estado de un vértice cuando se encuentra un camino lexicográficamente mejor.

 El coste es **$\mathcal{O}(K \cdot (C+N)\log N)$** por caso (K = consultas).

 @ </answer> */


 // ================================================================
 // Escribe el código completo de tu solución aquí debajo
 // ================================================================
 //@ <answer>

bool resuelveCaso() {
    int N, C;
    if (!(cin >> N >> C)) return false;

    // Usamos long long para las distancias
    DigrafoValorado<long long> G(N);

    for (int i = 0; i < C; ++i) {
        int v_in, w_in;
        long long c_in;
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

        RutasBicriterio solver(G, P);

        RutasBicriterio::Resultado res = solver.obtenerResultado(Q);
        long long D_min = res.dist;

        if (D_min == INF_DIST) {
            cout << "SIN CAMINO\n";
        }
        else {
            cout << D_min << " SI\n";
        }
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