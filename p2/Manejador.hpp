#include "TablaHash.hpp"
#include <list>
#include <queue>

class Manejador {

	private:

	// Atributos del manejador.
	int nMarcos;				 // Numero de marcos disponibles.
	std::vector<int> marcos;     // Almacena las paginas en los marcos actuales.
	TablaHash tablaPaginas;	     // Tabla de paginas.
	// Atributos para algoritmos.
	queue<int> fifo_queue;  	// Cola para FIFO.
	list<int> lru_queue;		// Cola para LRU.
	vector<bool> reloj;	    	// Reloj para LRU reloj simple.
	int p;				    	// Puntero del reloj.

	public:
    
	Manejador(int nMarcos) : tablaPaginas(nMarcos) {
		this->nMarcos = nMarcos;
		marcos.resize(nMarcos, -1);
        reloj.resize(nMarcos, false);
        p = 0;
	}

	bool isPageInMemory(int pagina) { 
        return tablaPaginas.findItem(pagina) != -1; 
    }

	int FIFO(int pagina) {
		// Caso en que la pagina ya esta en memoria.
		if (isPageInMemory(pagina)) {
			return -1;
		}
		// Caso en que hay espacio en memoria.
		if (fifo_queue.size() < nMarcos) {
			fifo_queue.push(pagina);
			int marco = fifo_queue.size() - 1;
			marcos[marco] = pagina;
			tablaPaginas.insertItem(pagina, marco);
			return -2;
		}
		// Caso en que no hay espacio en memoria y se debe reemplazar.
		// Se obtiene la victima.
		int victima = fifo_queue.front();
		fifo_queue.pop();
		int marco = tablaPaginas.findItem(victima);
		// Se reemplaza la nueva pag.
		fifo_queue.push(pagina);
		marcos[marco] = pagina;					// Se actualiza el marco.
		tablaPaginas.deleteItem(victima);		// Se elimina la victima.
		tablaPaginas.insertItem(pagina, marco); // Se actualiza el mapeo.
		// Se actualiza la tabla de paginas.
		return victima; // Página reemplazada.
	}

	int LRU(int pagina) {
		// Caso en que la pagina ya esta en memoria.
		if (isPageInMemory(pagina)) {
			// Reubicar la pagina en la cola LRU.
			lru_queue.remove(pagina);
			lru_queue.push_back(pagina);
			return -1;
		}
		// Caso en que hay espacio en memoria.
		if (lru_queue.size() < nMarcos) {
			lru_queue.push_back(pagina);
			int marco = lru_queue.size() - 1;
			marcos[marco] = pagina;
			tablaPaginas.insertItem(pagina, marco);
			return -2;
		}
		// Caso en que no hay espacio en memoria y se debe reemplazar.
		int victima = lru_queue.front();
		lru_queue.pop_front();
		int marco = tablaPaginas.findItem(victima); // Encontramos el marco de la victima.
		// Reemplazamos con la nueva pagina.
		lru_queue.push_back(pagina);			// La nueva pagina se coloca al final.
		marcos[marco] = pagina;				    // Se actualiza el marco.
		tablaPaginas.deleteItem(victima);		// Eliminamos la victima de la tabla de paginas.
		tablaPaginas.insertItem(pagina, marco); // Insertamos la nueva pagina.

		return victima; // Pagina reemplazada.
	}

	int LRUrs(int pagina) {
		// Caso en que la pagina ya esta en memoria.
		if (isPageInMemory(pagina)) {
            reloj[tablaPaginas.findItem(pagina)] = true; // Se marca la pagina con un 1, para una segunda oportunidad.
			return -1;
		}
		// Caso en que hay espacio en memoria.
		for(int i = 0; i < nMarcos; i++){
			if(marcos[i] == -1){
				marcos[i] = pagina;
				tablaPaginas.insertItem(pagina, i);
				reloj[i] = true;
				return -2;
			}
		}
		// Caso en que no hay espacio en memoria y se debe reemplazar.
		while(true) {
			if(!reloj[p]){
				int victima = marcos[p];
				marcos[p] = pagina;
				tablaPaginas.deleteItem(victima);
				tablaPaginas.insertItem(pagina, p);
				p = (p + 1) % nMarcos;
				return victima;
			} else {
				reloj[p] = false;
				p = (p + 1) % nMarcos;
			}
		}
	}

    int Optimo(int pagina, vector<int> paginas, int elindex){ //'elindex' es el index de la Simulación.
        //Caso en que están en memoria.
        if(isPageInMemory(pagina)){
            return -1;
        }
		//Casos en que hay espacio en memoria.
		for(int i = 0; i < nMarcos; i++){
			if(marcos[i] == -1){
				marcos[i] = pagina;
				tablaPaginas.insertItem(pagina, i);
				return -2;
			}
		}
		//Casos en lo que no hay espacio en memoria (Reemplazo).
		int victima = -1;
		int max = -1;
		for(int i = 0; i < nMarcos; i++){
			int index = elindex + 1;
			while(index < paginas.size()){
				if(paginas[index] == marcos[i]){
					if(index > max){
						max = index;
						victima = i;
					}
					break;
				}
				index++;
			}
			if(index == paginas.size()){
				victima = i;
				break;
			}
		}
		int marco = tablaPaginas.findItem(marcos[victima]);
		marcos[victima] = pagina;
		tablaPaginas.deleteItem(marcos[victima]);
		tablaPaginas.insertItem(pagina,marco);
		return victima;
	}
};