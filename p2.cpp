#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <list>

using namespace std;


struct Pagina {
    int nmp;
    int npv;
    Pagina* next;

    Pagina(int nmp, int npv) {
        this->nmp = nmp;
        this->npv = npv;
        this->next = nullptr;
    }	
};

class TablaHash {
    private:

        vector<Pagina*> tabla;
        int tam;

        int Hashing(int npv) {
            return npv % tam;
        }

    public:

        TablaHash(int tam) {
            this->tam = tam;
            tabla.resize(tam, nullptr);
        }

        void insertItem(int npv, int nmp) {
            int index = Hashing(npv);
            Pagina* nueva_pagina = new Pagina(nmp, npv);
            if (tabla[index] == nullptr) {
                tabla[index] = nueva_pagina;
            } else {
                Pagina* temp = tabla[index];
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = nueva_pagina;
            }
        }

        int findItem(int npv) {
            int index = Hashing(npv);
            Pagina* temp = tabla[index];
            while (temp != nullptr) {
                if (temp->npv == npv) {
                    return temp->nmp;
                }
                temp = temp->next;
            }
            return -1;
        }

        void deleteItem(int npv) {
            int index = Hashing(npv);
            Pagina* temp = tabla[index];
            Pagina* prev = nullptr;
            while (temp != nullptr) {
                if (temp->npv == npv) {
                    if (prev == nullptr) {
                        tabla[index] = temp->next;
                    } else {
                        prev->next = temp->next;
                    }
                    delete temp;
                    return;
                }
                prev = temp;
                temp = temp->next;
            }
        }

        ~TablaHash() {
            for (int i = 0 ; i < tam ; i++) {
                Pagina* temp = tabla[i];
                while (temp != nullptr) {
                    Pagina* next = temp->next;
                    delete temp;
                    temp = next;
                }
            }
        }
};

class Manejador {
    private:
        int nMarcos; // Numero de marcos disponibles
        std::vector<int> marcos; // Almacena las paginas en los marcos actuales
        TablaHash tablaPaginas; // Tabla de paginas 

        //
        queue<int> fifo_queue;
        list<int> lru_queue;


    public:

        Manejador(int nMarcos) {
            this->nMarcos = nMarcos;
            marcos.resize(nMarcos, -1);
            tablaPaginas = TablaHash(nMarcos);
        }

        bool isPageInMemory(int pagina) {
            return tablaPaginas.findItem(pagina) != -1;
        }

        int FIFO(int pagina) {
            // Caso en que la pagina ya está en memoria
            if (isPageInMemory(pagina)) {
                return -1;
            }
            // Caso en que hay espacio en memoria
            if (fifo_queue.size() < nMarcos) {
                fifo_queue.push(pagina);
                int marco = fifo_queue.size() - 1;
                marcos[marco] = pagina;
                tablaPaginas.insertItem(pagina, marco);
                return -1;
            }
            // Caso en que no hay espacio en memoria y se debe reemplazar
            // Se obtiene la victima
            int victima = fifo_queue.front();
            fifo_queue.pop();
            int marco = tablaPaginas.findItem(victima);
            // Se reemplaza la nueva pag.
            fifo_queue.push(pagina); 
            marcos[marco] = pagina;                   // Se actualiza el marco
            tablaPaginas.deleteItem(victima);         // Se elimina la victima
            tablaPaginas.insertItem(pagina, marco);   // Se actualiza el mapeo
            // Se actualiza la tabla de paginas
            return victima; // Página reemplazada
        }
        
        int LRU(int pagina) {
            // Caso en que la pagina ya esta en memoria
            if (isPageInMemory(pagina)) {
                // Reubicar la pagina en la cola LRU
                lru_queue.remove(pagina);
                lru_queue.push_back(pagina);
                return -1;
            }
            // Caso en que hay espacio en memoria
            if (lru_queue.size() < nMarcos) {
                lru_queue.push_back(pagina);
                int marco = lru_queue.size() - 1;
                marcos[marco] = pagina;
                tablaPaginas.insertItem(pagina, marco);
                return -1;
            }
            // Caso en que no hay espacio en memoria y se debe reemplazar
            int victima = lru_queue.front();
            lru_queue.pop_front();
            int marco = tablaPaginas.findItem(victima); // Encontramos el marco de la victima
            // Reemplazamos con la nueva pagina
            lru_queue.push_back(pagina); // La nueva pagina se coloca al final
            marcos[marco] = pagina;      // Se actualiza el marco
            tablaPaginas.deleteItem(victima); // Eliminamos la victima de la tabla de paginas
            tablaPaginas.insertItem(pagina, marco); // Insertamos la nueva pagina

            return victima; // Pagina reemplazada
        }

        int LRU_reloj_simple(int pagina) {
            // Caso en que la pagina ya esta en memoria
            if (isPageInMemory(pagina)) {
                return -1;
            }
            // Caso en que hay espacio en memoria
            if (lru_queue.size() < nMarcos) {
                lru_queue.push_back(pagina);
                int marco = lru_queue.size() - 1;
                marcos[marco] = pagina;
                tablaPaginas.insertItem(pagina, marco);
                return -1;
            }
            // Caso en que no hay espacio en memoria y se debe
        }

};



int main(int argc, char *argv[]) {
    /*------------------ Parseo ------------------*/
    int nMarcos = 0;        // Numero de marcos iniciales.
    string algoritmo = "";  // Algoritmo de reemplazo: Optimo, FIFO, LRU, LRU reloj simple.
    string archivo = "";    // Archivo.
    for (int i = 1 ; i < argc ; i++) {
        if (string(argv[i]) == "-m") {  // Numero de marcos iniciales.
            nMarcos = stoi(argv[i + 1]);
            i += 1;
        } else if (string(argv[i]) == "-a") {   // Algoritmo de reemplazo.
            algoritmo = argv[i + 1];
            i += 1;
        } else if (string(argv[i]) == "-f") {   // Archivo.
            archivo = argv[i + 1];
            i += 1;
        }
    }
    /*------------------ Lectura Archivo ------------------*/
    vector<int> referencia;
    string linea;
    ifstream archivo_referencias(archivo);
    if (archivo_referencias.is_open()) {
        while (getline(archivo_referencias, linea)) {
            string numero = "";
            for (int i = 0 ; i < linea.size() ; i++) {
                if (linea[i] == ' ') {
                    referencia.push_back(stoi(numero));
                    numero = "";
                } else {
                    numero += linea[i];
                }
            }
            referencia.push_back(stoi(numero));
        }
        archivo_referencias.close();
    } else {
        cout << "No se pudo abrir el archivo." << endl;
        return 0;
    }
    /*------------------ Seleccion de Algoritmo ------------------*/
    if (algoritmo == "Optimo") {
        // Algoritmo Optimo.
    } else if (algoritmo == "FIFO") {
        // Algoritmo FIFO.
    } else if (algoritmo == "LRU") {
        // Algoritmo LRU.
    } else if (algoritmo == "LRU_reloj_simple") {
        // Algoritmo LRU reloj simple.
    } else {
        cout << "Algoritmo no valido." << endl;
        return 0;
    }
    /*------------------ Simulacion ------------------*/
    int fallos = 0;
    vector<int> marco;
    for (int i = 0 ; i < referencia.size() ; i++) {
        bool fallo = true;
        for (int j = 0 ; j < marco.size() ; j++) {
            if (referencia[i] == marco[j]) {
                fallo = false;
                break;
            }
        }
        if (fallo) {
            fallos++;
            if (marco.size() < nMarcos) {
                marco.push_back(referencia[i]);
            } else {
                // Reemplazo.
            }
        }
    }
    cout << "Fallos de pagina: " << fallos << endl;
    return 0;
}
