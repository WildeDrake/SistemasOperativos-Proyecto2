#include <vector>

using namespace std;


struct Pagina {
	int nmp;
	int npv;
	Pagina *next;

	Pagina(int nmp, int npv) {
		this->nmp = nmp;
		this->npv = npv;
		this->next = nullptr;
	}
};

class TablaHash {
	private:
	vector<Pagina *> tabla;
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
		Pagina *nueva_pagina = new Pagina(nmp, npv);
		if (tabla[index] == nullptr) {
			tabla[index] = nueva_pagina;
		} else {
			Pagina *temp = tabla[index];
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = nueva_pagina;
		}
	}

	int findItem(int npv) {
		int index = Hashing(npv);
		Pagina *temp = tabla[index];
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
		Pagina *temp = tabla[index];
		Pagina *prev = nullptr;
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
		for (int i = 0; i < tam; i++) {
			Pagina *temp = tabla[i];
			while (temp != nullptr) {
				Pagina *next = temp->next;
				delete temp;
				temp = next;
			}
		}
	}
};