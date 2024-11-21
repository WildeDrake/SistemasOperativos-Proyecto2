#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;



 // 1) La tabla de páginas debe ser implementada como tabla hash y chaining.

struct Pagina {
    int nmp;
    int npv;

    Pagina(int nmp, int npv) {
        this->nmp = nmp;
        this->npv = npv;
    }	
};

struct TablaHash {
    vector<vector<Pagina>> tabla;
    int tam;

    TablaHash(int tam) {
        this->tam = tam;
        tabla.resize(tam);
    }

    int Hashing(int x){
        return x % tam; // Funcion de hashing
    }

    void insertItem(int npv){
        int nmp = Hashing(npv);
        /*if(hay colisión){
            //hacer algo de reemplazo (LRU, )
        }*/
        tabla[nmp].push_back(Pagina(nmp, npv));
    }

    void deleteItem(int npv){
        int nmp = Hashing(npv);
        for(int i = 0 ; i < tam ; i++) {
            if(tabla[nmp][i].npv == npv) {
                tabla[nmp].erase(tabla[nmp].begin() + i);
                break;
            }
        }
    }


};



int main(int argc, char *argv[]) {
    /*------------------ Parseo ------------------*/
    int nMarcos = 0;         // Numero de marcos iniciales.
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
