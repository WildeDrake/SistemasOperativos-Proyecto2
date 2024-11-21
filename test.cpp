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

public:
    Monitor(int tamanoInicial, int tiempoEspera) {
        numElementos = 0;
        frente = 0;
        final = 0;
        numSize = tamanoInicial;
        cola.resize(tamanoInicial);
        this->tiempoEspera = chrono::seconds(tiempoEspera);
    }
    ~Monitor() {}

    void agregarElemento(int item) {
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, [this] { return numElementos < numSize; });
        cola[final] = item;
        final = (final + 1) % numSize;
        numElementos++;
        cv.notify_all();
    }

    int extraerElemento() {
        unique_lock<mutex> lck(mtx);
        if (cv.wait_for(lck, tiempoEspera, [this] {cout << "extr_wait" << endl; return numElementos > 0; })) {
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

    Monitor monitor(10, 5);

    thread t([&monitor] {
        int n =  monitor.extraerElemento();
        cout << "Elemento extraido: " << n << endl;
        });

    thread t1([&monitor] {
        for (int i = 0; i < 5; i++) {
            monitor.agregarElemento(i);
        }
        });

    thread t2([&monitor] {
        for (int i = 0; i < 6; i++) {
        int n =  monitor.extraerElemento();
        cout << "Elemento extraido: " << n << endl;
        }
        });


    t.join();
    t1.join();
    t2.join();

    return 0;
}