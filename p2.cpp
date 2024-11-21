#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;




int main(int argc, char *argv[]) {
    /*------------------ Parseo ------------------*/
    int marcos = 0;         // Numero de marcos iniciales.
    string algoritmo = "";  // Algoritmo de reemplazo: Optimo, FIFO, LRU, LRU reloj simple.
    string archivo = "";    // Archivo. 

    for (int i = 1 ; i < argc ; i++) {
        if (string(argv[i]) == "-m") {  // Numero de marcos iniciales.
            marcos = stoi(argv[i + 1]);
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
    
    /*------------------ a ------------------*/

    return 0;
}
