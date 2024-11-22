#include "Manejador.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char *argv[]) {
	/*------------------ Parseo ------------------*/
	int nMarcos = 0;		  // Numero de marcos iniciales.
	string algoritmo = ""; // Algoritmo de reemplazo: Optimo, FIFO, LRU, LRU reloj simple.
	string archivo = "";	  // Archivo.
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "-m") { // Numero de marcos iniciales.
			nMarcos = stoi(argv[i + 1]);
			i += 1;
		} else if (string(argv[i]) == "-a") { // Algoritmo de reemplazo.
			algoritmo = argv[i + 1];
			i += 1;
		} else if (string(argv[i]) == "-f") { // Archivo.
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
			for (int i = 0; i < linea.size(); i++) {
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
	int alg;
	if (algoritmo == "Optimo") {
		alg = 1;
	} else if (algoritmo == "FIFO") {
		alg = 2;
	} else if (algoritmo == "LRU") {
		alg = 3;
	} else if (algoritmo == "LRU_reloj_simple") {
		alg = 4;
	} else {
		cout << "Algoritmo no valido." << endl;
		return 0;
	}
	/*------------------ Simulacion ------------------*/
	int cold_misses = 0;
	int cap_misses = 0;
	Manejador manejador(nMarcos);
	for (int i = 0; i < referencia.size(); i++) {
		int pagina = referencia[i];
		int victima;
		switch (alg) {
		case 1:
			victima = manejador.Optimo(pagina, referencia, i);
			break;
		case 2: 
			victima = manejador.FIFO(pagina); 
			break;
		case 3:
			victima = manejador.LRU(pagina);
			break;
		case 4:
			victima = manejador.LRUrs(pagina);
			break;
		}
		if (victima == -2) {
			cold_misses++;
		} else if (victima != -1) {
			cap_misses++;
		}
	}
	/*------------------ Resultados ------------------*/
	cout << "Algoritmo: " << algoritmo << endl;
	cout << "Marcos: " << nMarcos << endl;
	cout << "Fallos de pagina: " << cold_misses + cap_misses << endl;
	cout << "Por capacidad: " << cap_misses << endl;
	cout << "Por falta de datos: " << cold_misses << endl;
	return 0;
}