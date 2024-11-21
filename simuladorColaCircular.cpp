#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <fstream>
#include <chrono>

using namespace std;

class Monitor {
private:
    vector<int> cola;
    int numSize;
    int numElementos;
    int frente;
    int final;
    chrono::seconds tiempoEspera;
    mutex mtx;
    condition_variable cv;
    ofstream archivo;
    void duplicarCapacidad() {
        vector<int> colaDuplicada(numSize * 2);
        for (int i = 0; i < numElementos; i++)
        {
            colaDuplicada[i] = cola[frente];
            frente = (frente + 1) % numSize;
        }
        cola = move(colaDuplicada);
        numSize = numSize * 2;
        frente = 0;
        final = numElementos;

    }

    void dividirCola() {
        vector<int> mediaCola(numSize / 2);
        for (int i = 0; i < numElementos; i++)
        {
            mediaCola[i] = cola[frente];
            frente = (frente + 1) % numSize;
        }
        cola = move(mediaCola);
        numSize = numSize / 2;
        frente = 0;
        final = numElementos;
    }

public:
    Monitor(int tamanoInicial, int tiempoEspera) {
        numElementos = 0;
        frente = 0;
        final = 0;
        numSize = tamanoInicial;
        cola.resize(tamanoInicial);
        this->tiempoEspera = chrono::seconds(tiempoEspera);
        archivo.open("log.txt");
        if (archivo.is_open()) {
            cout << "archivo abierto";
            archivo << "capacidad inicial de la cola:" << tamanoInicial << endl;
        } else {
            cout << "error al abrir archivo";
        }
    }
    ~Monitor() {
        if (archivo.is_open()) {
            archivo.close();
        }
    }

    void agregarElemento(int item) {
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, [this] { return numElementos < numSize; });
        if (numElementos == numSize) {
            archivo << "Se duplico la capacidad de la cola y ahora es:" << numSize << endl;
            duplicarCapacidad();
        }
        cola[final] = item;
        final = (final + 1) % numSize;
        numElementos++;
            cv.notify_all();
    }

    int extraerElemento() {
        unique_lock<mutex> lck(mtx);
        if (cv.wait_for(lck, tiempoEspera, [this] { return numElementos > 0; })) {
            if (numElementos <= numSize / 4) {
                archivo << "Se redujo a la mitad la capacidad de la cola y ahora es:" << numSize << endl;
                dividirCola();
            }
            int item = cola[frente];
            frente = (frente + 1) % numSize;
            numElementos--;
            cv.notify_all();

            return item;
        }

        return -1;
    }


};

int main(int argc, char **argv) {

    int p = 0, c = 0, s = 0, t = 0; // num productores, num consumidores, tamano inicial cola, tiempo de espera
    for (int i = 1; i + 1 < argc; i++) {
        if (argv[i] == "-p") {
            p = atoi(argv[i + 1]);
        } else if (argv[i] == "-c") {
            c = atoi(argv[i + 1]);
        } else if (argv[i] == "-s") {
            s = atoi(argv[i + 1]);
        } else if (argv[i] == "-t") {
            t = atoi(argv[i + 1]);
        }
    }
    if (p == 0 || c == 0 || s == 0 || t == 0) {
        cout << "Error en los argumentos" << endl;
        return 1;
    }



    return 0;
}