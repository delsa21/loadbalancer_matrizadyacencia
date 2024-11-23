#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <queue>

using namespace std;

// Clase para representar un balanceador de carga
class LoadBalancer {
private:
    int numServers;                         // Número de servidores
    int maxRequestsPerServer;               // Máximo de peticiones por servidor
    vector<vector<int>> costMatrix;         // Matriz de adyacencia para los costos de conexión
    vector<int> serverLoad;                 // Carga actual de cada servidor

public:
    // Constructor para inicializar la configuración
    LoadBalancer(int servers, int maxRequests)
        : numServers(servers), maxRequestsPerServer(maxRequests) {
        costMatrix.resize(numServers, vector<int>(numServers, 0));
        serverLoad.resize(numServers, 0);
        generateCostMatrix();
    }

    // Generar la matriz de costos aleatorios
    void generateCostMatrix() {
        srand(time(0)); // Semilla para números aleatorios
        for (int i = 0; i < numServers; i++) {
            for (int j = 0; j < numServers; j++) {
                if (i == j) {
                    costMatrix[i][j] = 0; // Costo de conexión consigo mismo es 0
                } else {
                    costMatrix[i][j] = rand() % 20 + 1; // Costos aleatorios entre 1 y 20
                }
            }
        }
    }

    // Imprimir la matriz de costos
    void printCostMatrix() {
        cout << "\nMatriz de costos de conexión:\n";
        for (const auto& row : costMatrix) {
            for (int cost : row) {
                cout << cost << "\t";
            }
            cout << endl;
        }
    }

    // Distribuir una petición al servidor óptimo
    void distributeRequest() {
        int bestServer = -1;
        int minCost = numeric_limits<int>::max();

        // Buscar el servidor con menor costo y que no esté sobrecargado
        for (int i = 0; i < numServers; i++) {
            if (serverLoad[i] < maxRequestsPerServer) {
                int cost = calculateTotalCost(i);
                if (cost < minCost) {
                    minCost = cost;
                    bestServer = i;
                }
            }
        }

        if (bestServer == -1) {
            cout << "No hay servidores disponibles. Todos están sobrecargados.\n";
        } else {
            serverLoad[bestServer]++;
            cout << "Petición asignada al servidor " << bestServer
                 << " (Carga actual: " << serverLoad[bestServer] << "/"
                 << maxRequestsPerServer << ").\n";
        }
    }

    // Calcular el costo total de conexión a un servidor
    int calculateTotalCost(int server) {
        int totalCost = 0;
        for (int i = 0; i < numServers; i++) {
            totalCost += costMatrix[server][i];
        }
        return totalCost;
    }

    // Mostrar el estado actual de los servidores
    void printServerStatus() {
        cout << "\nEstado de los servidores:\n";
        for (int i = 0; i < numServers; i++) {
            cout << "Servidor " << i << ": " << serverLoad[i] << "/"
                 << maxRequestsPerServer << " peticiones.\n";
        }
    }
};

int main() {
    int numServers, maxRequests, numRequests;

    // Configuración inicial
    cout << "Ingrese el número de servidores: ";
    cin >> numServers;
    cout << "Ingrese el máximo de peticiones por servidor: ";
    cin >> maxRequests;

    LoadBalancer balancer(numServers, maxRequests);

    balancer.printCostMatrix();

    // Simulación de solicitudes
    cout << "\nIngrese el número de peticiones a procesar: ";
    cin >> numRequests;

    for (int i = 0; i < numRequests; i++) {
        cout << "\nProcesando petición " << i + 1 << ":\n";
        balancer.distributeRequest();
        balancer.printServerStatus();
    }

    return 0;
}
