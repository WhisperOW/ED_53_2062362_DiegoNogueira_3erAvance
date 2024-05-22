#include <Windows.h>
#include <iostream>
#include <CommCtrl.h>
#include "Resource.h"
#include <string>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;
int i{}; string a{};
string agria;
int clave = 0;
int indice = 0;
char nombre[100] = {};

string linea, val = "1";
ifstream archivo1;

char tmp[100] = {};
char temp[100] = {};
bool status = 0;
bool status1 = true;
//
//char validador1[] = "123", validador2[] = "123";
//char validar1[sizeof(validador1)], validar2[sizeof(validador1)];
//
char validador1[100], validador2[100];
char validar1[sizeof(validador1)], validar2[sizeof(validador1)];

//Para las fotos me imagino
char fotoDirU[MAX_PATH];
//enum int { menor, adulto, mayor };
//enum int { credito, debito, transferencia, efectivo };

struct user {
	char usuario[100];
	char contra[100];
	char correo[100];
	char nombres[100];
	char apellidoP[100];
	char apellidoM[100];
	SYSTEMTIME nacimiento;
	int admin;
	bool genero; // 0 para mujer, 1 para hombre
	char foto[MAX_PATH];
	user* der = NULL;
	user* izq = NULL;
} usuP;

user* raiz = NULL;

struct piloto
{
	char usuario[100] = {};
	char horasVuelo[100] = {};
	char nombre[100] = {};
	char apellidoP[100] = {};
	char apellidoM[100] = {};
	char correo[100] = {};
	bool ocupado = false;
	piloto* sig = NULL;
	piloto* ant = NULL;
}piloP;	piloto* inicio_pilo = NULL, * final_pilo = NULL, * auxP = NULL, * pivoteP = new piloto();

struct vuelo
{
	int clave;
	char origen[100];
	char destino[100];
	char piloto[100];
	//Se guardar  todo, tanto como el dia y la hora elegida para el vuelo
	SYSTEMTIME fecha;
	//pueden variar la cantidad de asientos en cada avion, pero que su multiplo sea de 100
	int turista;
	int ejecutivo;
	int asientosTotales = turista + ejecutivo;

	float menor = 0, mayor = 0;
	char usuario[100];    //Quien genero el vuelo
	/* los asientos de menores y de mayores compenden el 25%, por lo que
	creo que es mejor que en maquina si llega al porcentaje, directamente
	no deje elegir uno, por ejemplo, si son 100 asientos y ya
	se eligieron los 15 para los de 3ra edad, entonces ya no puede
	elegir ese en vez de generar un valor propio para el asiento aqu  */
	SYSTEMTIME fechaUsuario;	//Fecha en que el usuario genero el vuelo
	bool tipoAvion; //0 fuselaje estrecho, 1 fuselaje ancho
	int estado = 1; //0, cancelado, 1 en proceso, 2 ya paso
	vuelo* sig;
	vuelo* ant;
}vP;	vuelo* inicio_vuelo = NULL, * final_vuelo = NULL, * auxV = NULL, * pivoteV = new vuelo();

//				PARA EL MANIFESTO
//En una ventana especifica se mostraran las claves de vuelo, y seleccionado el vuelo muestre toda la informacion de este.

struct pasajero
{
	char nombre[100];
	char apellidoP[100];
	char apellidoM[100];
	SYSTEMTIME nacimiento;
	bool conBoleto;
	bool genero;	//0 mujer, 1 hombre
	char nacionalidad[100];
	char usuario[100];	//Quien guardo al pasajero
	SYSTEMTIME registro;	//Fecha y hora de registro del pasajero
	pasajero* sig = NULL;
	pasajero* ant = NULL;
}pasaP;
pasajero* inicio_pas = NULL, * final_pas = NULL, * auxPas = NULL, * pivotePas = new pasajero();

struct boleto
{
	char usuario[100];
	//char cod[6];
	int claveAsiento;
	int claveVuelo;	//la clave que tiene el vuelo
	int generacion;	//0 menor, 1 adulto, 2 mayor
	bool tipo;	//0 turista, 1 ejecutivo
	int pago;	//0 credito, 1 debito, 2 transferencia, 3 efectivo
	char pasajero[100];	//Nombre del pasajero
	bool pase = 1;	//0 no abordo, 1 si abordo
	SYSTEMTIME abordo;	//Fecha y hora del pase de abordo
	int precio;

	boleto* sig = NULL;
	boleto* ant = NULL;
}bP;
boleto* inicio_b = NULL, * final_b = NULL, * auxB = NULL;


//bool genero;	0 para mujer, 1 para hombre
//bool admin; 0 por usuario normal, 1 para administrador
//Funciones

void leerVuelos()
{
	string linea;
	ifstream archivo;
	archivo.open("vuelos.bin", ios::binary);
	if (archivo.fail()) {
		return;
	}
	inicio_vuelo = new vuelo();
	auxV = inicio_vuelo;
	while (!getline(archivo, linea).eof()) {
		//auxU = new user();
		auxV->clave = stoi(linea);

		getline(archivo, linea);
		strcpy_s(auxV->origen, linea.c_str());
		getline(archivo, linea);
		strcpy_s(auxV->destino, linea.c_str());
		getline(archivo, linea);
		strcpy_s(auxV->piloto, linea.c_str());

		getline(archivo, linea);
		sscanf_s(linea.c_str(), "%02hd/%02hd/%04hd %02hd:%02hd:%02hd",
			&auxV->fecha.wDay, &auxV->fecha.wMonth, &auxV->fecha.wYear,
			&auxV->fecha.wHour, &auxV->fecha.wMinute, &auxV->fecha.wSecond);
		getline(archivo, linea);
		auxV->turista = stoi(linea);
		getline(archivo, linea);
		auxV->ejecutivo = stoi(linea);
		getline(archivo, linea);
		auxV->asientosTotales = stoi(linea);
		getline(archivo, linea);
		auxV->menor = stof(linea);
		getline(archivo, linea);
		auxV->mayor = stof(linea);
		getline(archivo, linea);
		strcpy_s(auxV->usuario, linea.c_str());
		sscanf_s(linea.c_str(), "%02hd/%02hd/%04hd %02hd:%02hd:%02hd",
			&auxV->fechaUsuario.wDay, &auxV->fechaUsuario.wMonth, &auxV->fechaUsuario.wYear,
			&auxV->fechaUsuario.wHour, &auxV->fechaUsuario.wMinute, &auxV->fechaUsuario.wSecond);
		getline(archivo, linea);
		if (linea == "0") auxV->tipoAvion = 0;
		if (linea == "1") auxV->tipoAvion = 1;
		getline(archivo, linea);
		if (linea == "1") auxV->estado = 1;
		if (linea == "0") auxV->estado = 0;
		auxV->sig = new vuelo();
		auxV->sig->ant = auxV;
		auxV = auxV->sig;
	}
	if (strcmp(auxV->origen, "") == false) {
		auxV->ant->sig = NULL;
	}
	archivo.close();
}


void agregarUsu(user*& raiz, user* nuevoUsuario);
//void agregarUsu(user*&, user*&, char[], char[], char[], char[], char[], char[], SYSTEMTIME, bool, int, char[]);
void eliminarUsu(user*& raiz, char usuario[]);
void limpiarUsu(HWND);
user* buscarUsuario(user* raiz, const char* usuario);
void leerArbolAdmin(ifstream& archivo, user& raiz);
void guardarUsuarios();
void leerUsuarios();

void agregarPiloto(piloto*&, piloto*&, char[], char[], char[], char[], char[], char[], bool);
void eliminarPiloto(piloto*&, char[]);
void limpiarPiloto(HWND);
void guardarPiloto();


//void leerPiloto(piloto*&, piloto*&);

void agregarVuelooo(vuelo*&, vuelo*&, int, char[], char[], char[], SYSTEMTIME, int, int, int, float, char[], SYSTEMTIME, bool, int);

void eliminarVuelooo(vuelo*&, char[]);

void limpiarVuelooo(HWND);
void guardarVuelooo();
//void leerVuelos(user*&, user*&);

void agregarPasajero(pasajero*&, pasajero*&, char[], char[], char[], SYSTEMTIME, bool, bool, char[], char[], SYSTEMTIME);
void eliminarPasajero(pasajero*&, char[]);
void limpiarPasajero(HWND);
void guardarPasajero();
void leerPasajero();

void agregarBoleto(boleto*&, boleto*&, /*char[],*/ char[], int, int, int, bool, int, char[], bool, SYSTEMTIME, int);
void eliminarBoleto(boleto*&, char[]);
void limpiarBoleto(HWND);
void guardarBoleto();

void loadUser();
void saveUser();
void savePilot();
void loadPilot();
void loadVuelo();
void saveVuelo();
void loadPasajero();
void savePasajero();
void loadBoleto();
void saveBoleto();

//NO QUITAR
bool contieneNumero(const char* cadena) {
	while (*cadena) {
		if (isdigit(*cadena)) {
			return true;
		}
		cadena++;
	}
	return false;
}

bool contieneCorreo(const char* cadena) {
	if (strstr(cadena, "@") != NULL && strstr(cadena, ".com") != NULL)
		return true;
	return false;
}

bool noTieneNum(const char* cadena) {
	while (*cadena) {
		if (!isdigit(*cadena))
			return true;
		cadena++;
	}
	return false;
}

bool seisMeses(SYSTEMTIME fecha) {
	SYSTEMTIME hoy;
	GetLocalTime(&hoy);
	SYSTEMTIME seisMesesDespues = hoy;
	seisMesesDespues.wMonth += 6;
	if (seisMesesDespues.wMonth > 12) {
		seisMesesDespues.wYear += 1;
		seisMesesDespues.wMonth -= 12;
	}
	if (fecha.wYear > seisMesesDespues.wYear ||
		(fecha.wYear == seisMesesDespues.wYear && fecha.wMonth > seisMesesDespues.wMonth) ||
		(fecha.wYear == seisMesesDespues.wYear && fecha.wMonth == seisMesesDespues.wMonth && fecha.wDay >= seisMesesDespues.wDay)) {
	}
	else {
		if (fecha.wYear == hoy.wYear && fecha.wMonth == hoy.wMonth && fecha.wDay == hoy.wDay) {
			return true;
			//std::cout << "La variable 'a' es mayor a 6 meses a partir de hoy y es igual a hoy" << std::endl;
		}
		else return false;
		//std::cout << "La variable 'a' no es mayor a 6 meses a partir de hoy." << std::endl;
	}
}

bool existe = false;
#pragma region quickSort
piloto* partition(piloto* inicio, piloto* final) {
	char nombre[100] = {};
	strcpy(nombre, final->nombre);
	piloto* i = inicio->ant;
	for (piloto* j = inicio; j != final && j != NULL; j = j->sig) {
		if (j->nombre[0] <= nombre[0]) {
			i = (i == NULL) ? inicio : i->sig;
			swap(i, j);
		}
	}
	if (i == NULL) i = inicio; else i = i->sig;
	swap(i, final);
	return i;
}
void swap(piloto* a, piloto* b) {
	piloto aux;
	//aux = a
	strcpy(aux.nombre, a->nombre);
	strcpy(aux.apellidoM, a->apellidoM);
	strcpy(aux.apellidoP, a->apellidoP);
	strcpy(aux.correo, a->correo);
	strcpy(aux.horasVuelo, a->horasVuelo);

	//a = b
	strcpy(a->nombre, b->nombre);
	strcpy(a->apellidoM, b->apellidoM);
	strcpy(a->apellidoP, b->apellidoP);
	strcpy(a->correo, b->correo);
	strcpy(a->horasVuelo, b->horasVuelo);

	//b = aux
	strcpy(b->nombre, aux.nombre);
	strcpy(b->apellidoM, aux.apellidoM);
	strcpy(b->apellidoP, aux.apellidoP);
	strcpy(b->correo, aux.correo);
	strcpy(b->horasVuelo, aux.horasVuelo);
}
void quickSort(piloto* inicio, piloto* final) {
	if (final != NULL && inicio != final && inicio != final->sig) {
		piloto* p = partition(inicio, final);
		quickSort(inicio, p->ant);
		quickSort(p->sig, final);
	}
}
#pragma endregion

bool pilotoOcupado(char* nombre) {
	auxP = inicio_pilo;
	while (auxP != NULL) {
		if (strcmp(auxP->nombre, nombre) == true) {
			if (auxP->ocupado == true) {
				existe = true;
				return true;
			}
			else {
				existe = true;
				return false;
			}
		}
		else {
			auxP = auxP->sig;
		}
	}
	existe = false;
	return false;
}

#pragma region heapSort
bool esAnterior(SYSTEMTIME& fecha, SYSTEMTIME& hoy) {
	// Comparar las fechas
	if (fecha.wYear < hoy.wYear) {
		return true;
	}
	else if (fecha.wYear == hoy.wYear) {
		if (fecha.wMonth < hoy.wMonth) {
			return true;
		}
		else if (fecha.wMonth == hoy.wMonth) {
			if (fecha.wDay < hoy.wDay) {
				return true;
			}
		}
	}
	return false;
}
bool esHoy(SYSTEMTIME& fecha, SYSTEMTIME& hoy) {
	if (fecha.wYear == hoy.wYear) {
		if (fecha.wMonth == hoy.wMonth) {
			if (fecha.wDay == hoy.wDay) {
				return true;
			}
		}
	}
	return false;
}

// Funci n para intercambiar dos nodos
void swap(vuelo* a, vuelo* b) {
	if (b != NULL) {
		int temp = a->clave;
		a->clave = b->clave;
		b->clave = temp;

		temp = a->turista;
		a->turista = b->turista;
		b->turista = temp;

		temp = a->ejecutivo;
		a->ejecutivo = b->ejecutivo;
		b->ejecutivo = temp;

		a->asientosTotales = a->turista + a->ejecutivo;
		b->asientosTotales = b->turista + b->ejecutivo;

		char tempc[100] = {};
		strcpy_s(tempc, a->origen);
		strcpy_s(a->origen, b->origen);
		strcpy_s(b->origen, tempc);

		strcpy_s(tempc, a->destino);
		strcpy_s(a->destino, b->destino);
		strcpy_s(b->destino, tempc);

		strcpy_s(tempc, a->piloto);
		strcpy_s(a->piloto, b->piloto);
		strcpy_s(b->piloto, tempc);

		strcpy_s(tempc, a->usuario);
		strcpy_s(a->usuario, b->usuario);
		strcpy_s(b->usuario, tempc);

		SYSTEMTIME temps{};
		temps = a->fecha;
		a->fecha = b->fecha;
		b->fecha = temps;

		temps = a->fechaUsuario;
		a->fechaUsuario = b->fechaUsuario;
		b->fechaUsuario = temps;

		bool tempb = a->tipoAvion;
		a->tipoAvion = b->tipoAvion;
		b->tipoAvion = tempb;
	}
}
// Funci n para hacer Heapify hacia abajo
void heapifyDown(vuelo* root, vuelo* end) {
	vuelo* largest = root;
	vuelo* left = root->sig;
	vuelo* right = left ? left->sig : nullptr;

	if (left != nullptr && left->clave > largest->clave)
		largest = left;
	if (right != nullptr && right->clave > largest->clave)
		largest = right;

	if (largest != root) {
		swap(root, largest);
		heapifyDown(largest, end);
	}
}


// Funci n para construir el heap
void buildHeap(vuelo* head, vuelo* end) {
	vuelo* lastParent = head;
	while (lastParent != NULL && lastParent->sig != end)
		lastParent = lastParent->sig;

	while (lastParent != head) {
		heapifyDown(lastParent, end);
		lastParent = lastParent->ant;
	}
}

// Funci n para ordenar la lista utilizando HeapSort
void heapSort(vuelo* head) {
	vuelo* end = nullptr;
	vuelo* current = head;

	while (current->sig != nullptr)
		current = current->sig;
	end = current;

	buildHeap(head, end);

	// Extraer elementos uno por uno del heap
	while (end != head) {
		swap(head, end);
		end = end->ant;
		heapifyDown(head, end);
	}
}

// Funci n para agregarVueloar un elemento al final de la lista
void agregarVuelo(vuelo*& head) {
	vuelo* nuevo = new vuelo;
	nuevo->asientosTotales = head->asientosTotales;
	nuevo->clave = head->clave;
	strcpy_s(nuevo->destino, head->destino);
	nuevo->ejecutivo = head->ejecutivo;
	nuevo->fecha = head->fecha;
	nuevo->fechaUsuario = head->fechaUsuario;
	strcpy_s(nuevo->origen, head->origen);
	strcpy_s(nuevo->piloto, head->piloto);
	nuevo->tipoAvion = head->tipoAvion;
	nuevo->turista = head->turista;
	strcpy_s(nuevo->usuario, head->usuario);
	nuevo->estado = 1;
	nuevo->sig = nullptr;

	if (inicio_vuelo == nullptr) {
		nuevo->ant = nullptr;
		inicio_vuelo = nuevo;
	}
	else {
		vuelo* temp = inicio_vuelo;
		while (temp->sig != nullptr)
			temp = temp->sig;
		temp->sig = nuevo;
		nuevo->ant = temp;
	}
}
#pragma endregion

void agregarPilot(piloto*& inicio, piloto*& final, piloto*& aux) {
	piloto* nuevo = new piloto();
	strcpy(nuevo->nombre, aux->nombre);
	strcpy(nuevo->apellidoM, aux->apellidoM);
	strcpy(nuevo->apellidoP, aux->apellidoP);
	strcpy(nuevo->correo, aux->correo);
	strcpy(nuevo->horasVuelo, aux->horasVuelo);
	nuevo->ocupado = aux->ocupado;

	piloto* aux1 = inicio;
	piloto* aux2 = NULL;
	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}
	if (inicio == aux1)	inicio = nuevo;
	else aux2->sig = nuevo;

	nuevo->sig = aux1;
	nuevo->ant = aux2;
	final = nuevo;
}

void eliminarPilot(piloto*& inicio, char nombre[]) {
	if (inicio->sig != NULL) {
		piloto* aux = inicio;
		piloto* ant = NULL;
		while (aux != NULL && strcmp(aux->nombre, nombre) != 0) {
			ant = aux;
			aux = aux->sig;
		}
		if (ant == NULL) {
			inicio = inicio->sig;
			if (aux->sig != NULL) aux->sig->ant = NULL;
			delete aux;
		}
		else {
			if (aux->sig != NULL) {
				ant->sig = aux->sig;
				aux->sig->ant = ant;
			}
			else ant->sig = NULL;
			delete aux;
		}
	}
	else {
		delete inicio;
		inicio_pilo = NULL;
	}
}

void modifiPilo(piloto*& inicio, piloto*& aux, piloto*& pivote) {
	aux = inicio;
	while (aux != NULL) {
		if (strcmp(aux->nombre, pivote->nombre) == 0) {
			if (aux->ant != NULL) {
				if (aux->sig != NULL) {
					aux->ant->sig = pivote;
					aux->sig->ant = pivote;
					delete aux;
				}
				else {
					aux->ant->sig = pivote;
					delete aux;
				}
			}
			else if (aux->sig != NULL) {
				aux->sig->ant = pivote;
				delete aux;
			}
			else {
				inicio = pivote;
			}
			break;
		}
		aux = aux->sig;
	}
}

bool buscarPilo(piloto*& inicio, piloto*& final, piloto*& pivote) {
	piloto* aux = inicio;
	while (aux != NULL) {
		if (strcmp(aux->nombre, pivote->nombre) == 0) return true;
		aux = aux->sig;
	}
	return false;
}


void preorden(user* raiz, ofstream& archivo) {
	if (raiz != NULL) {
		archivo << raiz->usuario << endl;
		archivo << raiz->contra << endl;
		archivo << raiz->correo << endl;
		archivo << raiz->nombres << endl;
		archivo << raiz->apellidoP << endl;
		archivo << raiz->apellidoM << endl;
		char timeString[100];
		sprintf_s(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			raiz->nacimiento.wDay, raiz->nacimiento.wMonth, raiz->nacimiento.wYear,
			raiz->nacimiento.wHour, raiz->nacimiento.wMinute, raiz->nacimiento.wSecond);
		archivo << timeString << endl;
		archivo << raiz->genero << endl;
		archivo << raiz->admin << endl;
		archivo << raiz->foto << endl;

		preorden(raiz->izq, archivo);
		preorden(raiz->der, archivo);
	}
}

void guardarAdmin1(user* raiz, ofstream& archivo) {
	if (raiz != NULL) {
		archivo << 1 << endl;
		archivo << raiz->usuario << endl;
		archivo << raiz->contra << endl;
		archivo << raiz->correo << endl;
		archivo << raiz->nombres << endl;
		archivo << raiz->apellidoP << endl;
		archivo << raiz->apellidoM << endl;
		char timeString[100];
		sprintf_s(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			raiz->nacimiento.wDay, raiz->nacimiento.wMonth, raiz->nacimiento.wYear,
			raiz->nacimiento.wHour, raiz->nacimiento.wMinute, raiz->nacimiento.wSecond);
		archivo << timeString << endl;
		archivo << raiz->genero << endl;
		archivo << raiz->admin << endl;
		archivo << raiz->foto << endl;

		guardarAdmin1(raiz->izq, archivo);
		guardarAdmin1(raiz->der, archivo);
	}
}

void guardarAdmin2(user* raiz, ofstream& archivo) {
	if (raiz != NULL) {
		archivo << 0 << endl;
		archivo << raiz->usuario << endl;
		archivo << raiz->contra << endl;
		archivo << raiz->correo << endl;
		archivo << raiz->nombres << endl;
		archivo << raiz->apellidoP << endl;
		archivo << raiz->apellidoM << endl;
		char timeString[100];
		sprintf_s(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			raiz->nacimiento.wDay, raiz->nacimiento.wMonth, raiz->nacimiento.wYear,
			raiz->nacimiento.wHour, raiz->nacimiento.wMinute, raiz->nacimiento.wSecond);
		archivo << timeString << endl;
		archivo << raiz->genero << endl;
		archivo << raiz->admin << endl;
		archivo << raiz->foto << endl;

		guardarAdmin2(raiz->izq, archivo);
		guardarAdmin2(raiz->der, archivo);
	}
}
LRESULT CALLBACK ConfirmacionB(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		for (vuelo* i = inicio_vuelo; i != NULL; i = i->sig)
		{
			string tmp = to_string(i->clave);
			SendMessage(GetDlgItem(hwnd, IDC_LIST3), LB_ADDSTRING, 0, (LPARAM)tmp.c_str());
		}

		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)validador1);
		break;
		switch (LOWORD(wParam)) {
		case IDOK4:
			pivotePas->conBoleto = true;
			break;
		case IDOK3:
			pivotePas->conBoleto = false;
			break;
		case IDC_LIST3:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				indice = SendDlgItemMessage(hwnd, IDC_LIST3, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST3, LB_GETTEXT, indice, (LPARAM)tmp);
				auxB = inicio_b;
				auxV = inicio_vuelo;
				while (auxB != NULL && auxPas != NULL) {
					if (auxB->claveVuelo == auxV->clave) {
						while (auxPas != NULL) {
							if (strcmp(auxPas->nombre, auxB->pasajero) == 0) {
								SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)auxPas->nombre);
								auxPas = auxPas->sig;
							}
						}
						auxB = auxB->sig;
					}
				}
				break;
			}
			break;
		case IDC_LIST1:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				indice = SendDlgItemMessage(hwnd, IDC_LIST3, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST3, LB_GETTEXT, indice, (LPARAM)tmp);
				auxB = inicio_b;
				while (auxB != NULL && auxPas != NULL) {
					if (strcmp(auxB->pasajero, tmp) == 0) {
						while (auxPas != NULL) {
							if (strcmp(auxPas->nombre, tmp) == 0) {
								pivotePas = auxPas;
							}
						}
						break;
					}
					auxB = auxB->sig;
				}
				break;
			}
			break;
		}

	}

	case WM_COMMAND: {
		break;
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;

}
LRESULT CALLBACK Inicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MenuPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Usuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Pilotos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Vuelos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Boletos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Pasajeros(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Manifiesto(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void leerPiloto()
{
	string linea;
	ifstream archivo;
	archivo.open("pilotos.bin", ios::binary);
	if (archivo.fail())
	{
		return;
	}
	inicio_pilo = new piloto();
	auxP = inicio_pilo;
	while (!getline(archivo, linea).eof())
	{
		//auxU = new user();
		strcpy_s(auxP->usuario, linea.c_str());
		getline(archivo, linea);

		strcpy_s(auxP->horasVuelo, linea.c_str());
		getline(archivo, linea);

		strcpy_s(auxP->nombre, linea.c_str());
		getline(archivo, linea);
		strcpy_s(auxP->apellidoP, linea.c_str());
		getline(archivo, linea);
		strcpy_s(auxP->apellidoM, linea.c_str());
		getline(archivo, linea);
		strcpy_s(auxP->correo, linea.c_str());
		getline(archivo, linea);

		if (linea == "0") auxP->ocupado = 0;
		if (linea == "1") auxP->ocupado = 1;
		getline(archivo, linea);

		auxP->sig = new piloto();
		auxP->sig->ant = auxP;
		auxP = auxP->sig;
	}
	if (strcmp(auxP->usuario, "") == false) {
		auxP->ant->sig = NULL;
	}
	archivo.close();
}

void leerBoleto() {
	string linea;
	ifstream archivo;
	archivo.open("boleto.bin", ios::binary);
	if (archivo.fail()) {
		return;
	}
	inicio_b = new boleto();
	auxB = inicio_b;
	while (!getline(archivo, linea).eof()) {
		strcpy_s(auxB->usuario, linea.c_str());
		getline(archivo, linea);
		auxB->claveAsiento = stoi(linea);
		getline(archivo, linea);
		auxB->claveVuelo = stoi(linea);
		getline(archivo, linea);
		auxB->generacion = stoi(linea);
		getline(archivo, linea);
		auxB->tipo = stoi(linea);
		getline(archivo, linea);
		auxB->pago = stoi(linea);
		getline(archivo, linea);
		strcpy_s(auxB->pasajero, linea.c_str());
		getline(archivo, linea);
		auxB->pase = stoi(linea);
		getline(archivo, linea);
		sscanf_s(linea.c_str(), "%02hd/%02hd/%04hd %02hd:%02hd:%02hd",
			&auxB->abordo.wDay, &auxB->abordo.wMonth, &auxB->abordo.wYear,
			&auxB->abordo.wHour, &auxB->abordo.wMinute, &auxB->abordo.wSecond);
		getline(archivo, linea);
		auxB->precio = stoi(linea);
		auxB->sig = new boleto();
		auxB->sig->ant = auxB;
		auxB = auxB->sig;
	}
	if (strcmp(auxB->pasajero, "") == false) {
		auxB->ant->sig = NULL;
	}
	archivo.close();
}
void guardarBoleto() {
	ofstream archivo;
	archivo.open("boletos.bin", ios::binary);
	auxB = inicio_b;
	while (auxB != NULL)
	{
		archivo << auxB->usuario << endl;
		/*archivo << auxB->cod << endl; */
		archivo << auxB->claveAsiento << endl;
		archivo << auxB->claveVuelo << endl;
		archivo << auxB->generacion << endl;
		archivo << auxB->tipo << endl;
		archivo << auxB->pago << endl;
		archivo << auxB->pasajero << endl;
		archivo << auxB->pase << endl;
		char timeString[100];
		sprintf_s(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			auxB->abordo.wDay, auxB->abordo.wMonth, auxB->abordo.wYear,
			auxB->abordo.wHour, auxB->abordo.wMinute, auxB->abordo.wSecond);
		archivo << timeString << endl;
		archivo << auxB->precio;
		if (auxB->sig != NULL) archivo << endl;
		auxB = auxB->sig;
	}
	archivo.close();
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	/*Declaracion de variables */
	leerUsuarios();
	//leerPiloto();
	//leerVuelos();
	//leerPasajero();
	//leerBoleto();
	srand(time(0));
	HWND hwnd;
	MSG mensaje;
	hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Inicio);
	ShowWindow(hwnd, SW_SHOWDEFAULT);

	//BUCLE DE MENSAJES
	while (TRUE == GetMessage(&mensaje, 0, 0, 0))
	{
		TranslateMessage(&mensaje);
		DispatchMessage(&mensaje);
	}
	//delete auxP, inicio_pilo, final_pilo;

	guardarUsuarios();
	guardarPiloto();
	guardarVuelooo();
	guardarPasajero();
	guardarBoleto();
}
int elemento = 0;
HWND hPControl; HBITMAP hImagen;
//Ventana de acceso
LRESULT CALLBACK Inicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		archivo1.open("admin.bin", ios::binary);
		getline(archivo1, linea);
		if (linea == val) {
			//usuario
			getline(archivo1, linea);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)linea.c_str());
			//contra
			getline(archivo1, linea);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)linea.c_str());
		}
		archivo1.close();
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			bool status1 = false, status2 = false;
			GetDlgItemText(hwnd, IDC_EDIT1, validar1, sizeof(validar1));
			GetDlgItemText(hwnd, IDC_EDIT2, validar2, sizeof(validar2));

			// Buscar usuario en el árbol
			user* encontrado = buscarUsuario(raiz, validar1);
			if (encontrado != NULL) {
				status1 = true;
				strcpy_s(validador1, encontrado->usuario);
				if (strcmp(validar2, encontrado->contra) != 0) {
					status2 = false;
					MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK);
				}
				else {
					status2 = true;
				}
			}

			if (status1 && status2) {
				// Abrir el flujo de salida de archivo aquí y pasar como argumento
				ofstream archivo_salida("admin.bin", ios::binary);
				if (IsDlgButtonChecked(hwnd, IDC_CHECK1) == BST_CHECKED) {
					guardarAdmin1(raiz, archivo_salida);
				}
				else {
					guardarAdmin2(raiz, archivo_salida);
				}
				EndDialog(hwnd, LOWORD(wParam));
				DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)MenuPrincipal);
			}
			else if (status1 != 1) {
				MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK);
			}
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return 0;
}

LRESULT CALLBACK MenuPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	user* tmp = new user();
	switch (msg) {
	case WM_INITDIALOG:
		hPControl = GetDlgItem(hwnd, IDC_USUARIO1);
		hImagen = (HBITMAP)LoadImage(NULL, fotoDirU, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen);

		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)validador1);
		//Mandar la foto
		break;

	case WM_COMMAND:
		status = false;
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: {
			// Coloca la declaración de la variable dentro de un bloque
			user* encontrado = buscarUsuario(raiz, validador1);
			if (encontrado != NULL) {
				if (encontrado->admin != 1) {
					DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, (DLGPROC)Usuarios);
				}
				else {
					MessageBox(hwnd, "Solo los administradores pueden entrar", "AVISO", 0);
				}
			}
			else {
				MessageBox(hwnd, "Usuario no encontrado", "AVISO", 0);
			}
			break;
		}
		case IDC_BUTTON2:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, (DLGPROC)Pilotos);
			break;
		case IDC_BUTTON3:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG6), hwnd, (DLGPROC)Vuelos);
			break;
		case IDC_BUTTON4:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG7), hwnd, (DLGPROC)Pasajeros);
			break;
		case IDC_BUTTON5:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG8), hwnd, (DLGPROC)Boletos);
			break;
		case IDC_BUTTON6:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG9), hwnd, (DLGPROC)Manifiesto);
			break;
		case IDC_BUTTON8:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, (DLGPROC)ConfirmacionB);
			break;
		case IDC_BUTTON7:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return false;
}
void orden(user* nodo, HWND hwnd, int cosa) {
	if (nodo != nullptr) {
		orden(nodo->izq, hwnd, cosa);
		SendMessage(GetDlgItem(hwnd, cosa), LB_ADDSTRING, 0, (LPARAM)nodo->usuario);
		orden(nodo->der, hwnd, cosa);
	}
}
LRESULT CALLBACK Usuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		orden(raiz, hwnd, IDC_LIST1);

		/*for (user* i = inicio_usu; i != NULL; i = i->sig)
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)i->usuario);*/

		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)validador1);
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Usuario");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Administrador");

		break;

	case WM_COMMAND:
	{
		int elemento = LOWORD(wParam);
		switch (elemento)
		{
		case IDC_DELETE:

			break;
		case IDC_BUTTON7:
			GetDlgItemText(hwnd, IDC_EDIT2, usuP.usuario, sizeof(usuP.usuario));
			GetDlgItemText(hwnd, IDC_EDIT3, usuP.contra, sizeof(usuP.contra));
			GetDlgItemText(hwnd, IDC_EDIT4, usuP.nombres, sizeof(usuP.nombres));
			GetDlgItemText(hwnd, IDC_EDIT5, usuP.correo, sizeof(usuP.correo));
			GetDlgItemText(hwnd, IDC_EDIT6, usuP.apellidoP, sizeof(usuP.apellidoP));
			GetDlgItemText(hwnd, IDC_EDIT7, usuP.apellidoM, sizeof(usuP.apellidoM));
			if (usuP.contra[0] != NULL && usuP.nombres[0] != NULL && usuP.correo[0] != NULL && usuP.apellidoP[0] != NULL && usuP.apellidoM[0] != NULL) {
				user* nodoEncontrado = buscarUsuario(raiz, usuP.usuario); // Buscar el usuario en el árbol
				if (nodoEncontrado != NULL) { // Si se encontró el usuario
					// Actualizar los datos del nodo encontrado
					strcpy_s(nodoEncontrado->contra, usuP.contra);
					strcpy_s(nodoEncontrado->nombres, usuP.nombres);
					strcpy_s(nodoEncontrado->correo, usuP.correo);
					strcpy_s(nodoEncontrado->apellidoP, usuP.apellidoP);
					strcpy_s(nodoEncontrado->apellidoM, usuP.apellidoM);
					// Se puede continuar con la actualización de los demás datos según sea necesario
					MessageBox(hwnd, "Datos modificados", "AVISO", 0);
				}
				else {
					MessageBox(hwnd, "El usuario no existe", "AVISO", 0);
				}
			}
			else {
				MessageBox(hwnd, "Ingrese bien los datos", "AVISO", 0);
			}
			break;
		case IDC_LIST1:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				char texto[100] = {};
				int indice = 0;
				bool encontrado = false;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)texto);
				user* usuarioSeleccionado = buscarUsuario(raiz, texto); // Buscar el usuario en el árbol
				if (usuarioSeleccionado != NULL) { // Si se encontró el usuario
					// Mostrar los datos del usuario seleccionado en los controles de la interfaz
					EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)usuarioSeleccionado->usuario);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)usuarioSeleccionado->contra);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)usuarioSeleccionado->nombres);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)usuarioSeleccionado->correo);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)usuarioSeleccionado->apellidoP);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, 0, (LPARAM)usuarioSeleccionado->apellidoM);
					EnableWindow(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), false);
					SendDlgItemMessage(hwnd, IDC_DATETIMEPICKER1, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&usuarioSeleccionado->nacimiento);
					// Hombre
					EnableWindow(GetDlgItem(hwnd, IDC_RADIOH), false);
					EnableWindow(GetDlgItem(hwnd, IDC_RADIOM), false);
					if (usuarioSeleccionado->genero == 1) {
						SendDlgItemMessage(hwnd, IDC_RADIOH, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					}
					else {
						SendDlgItemMessage(hwnd, IDC_RADIOM, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					}
					HBITMAP fotoM = (HBITMAP)LoadImage(NULL, usuarioSeleccionado->foto, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
					SendMessage(GetDlgItem(hwnd, IDC_USUARIO2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoM);
					encontrado = true;
				}
				if (!encontrado) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", 0);
				}
				break;
			}
			break;
		case IDOK2:
		{
			strcpy(usuP.foto, fotoDirU);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, 100, (LPARAM)&usuP.usuario);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_GETTEXT, 100, (LPARAM)&usuP.contra);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_GETTEXT, 999, (LPARAM)&usuP.nombres);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_GETTEXT, 999, (LPARAM)&usuP.correo);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_GETTEXT, 999, (LPARAM)&usuP.apellidoP);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_GETTEXT, 999, (LPARAM)&usuP.apellidoM);
			SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), WM_GETTEXT, 999, (LPARAM)&usuP.nacimiento);
			int indice = SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_GETCURSEL, 0, 0);
			if (indice == 0) {
				usuP.admin = 1;
			}
			else {
				usuP.admin = 0;
			}

			if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
				status1 = false;
			}
			// Hombre
			if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_CHECKED && IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
				usuP.genero = 0;
				status1 = true;
			}
			// Mujer
			if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_CHECKED) {
				usuP.genero = 1;
				status1 = true;
			}

			if (usuP.usuario[0] != '\0' && usuP.contra[0] != '\0' && usuP.nombres[0] != '\0' && usuP.apellidoP[0] != '\0' && usuP.apellidoM[0] != '\0' && usuP.correo[0] != '\0' && status1)
			{
				agregarUsu(raiz, &usuP);
				MessageBox(NULL, "Registro hecho correctamente", "REGISTRO", MB_OK);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
				orden(raiz, hwnd, IDC_LIST1);
				//limpiarUsu(hwnd);
			}
			else
			{
				MessageBox(NULL, "Faltan datos por su registro", "REGISTRO", MB_OK);
			}
			break;
		}
		case ID_IMAGEN_AGREGAR_U:
			OPENFILENAME photo;
			ZeroMemory(&photo, sizeof(photo));
			photo.lStructSize = sizeof(photo);
			photo.hwndOwner = hwnd;
			//en vez de auxU->foto, poner el auxU->foto
			photo.lpstrFile = fotoDirU;
			photo.lpstrFile[0] = NULL;
			photo.nMaxFile = sizeof(fotoDirU);
			photo.lpstrFilter = "Imagen BitMap\0*.bmp*";
			photo.nFilterIndex = 0;
			photo.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&photo) == true)
			{
				HBITMAP fotoM = (HBITMAP)LoadImage(NULL, fotoDirU, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
				SendMessage(GetDlgItem(hwnd, IDC_USUARIO1), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)fotoM);
			}
			break;
		}
		break;
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}

LRESULT CALLBACK Pilotos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		for (piloto* i = inicio_pilo; i != NULL; i = i->sig)
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)i->nombre);
		//cambiar
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)validador1);
		break;

	case WM_CLOSE:
		EndDialog(hwnd, LOWORD(wParam));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case agregar:
			pivoteP = new piloto();
			GetDlgItemText(hwnd, IDC_EDIT2, pivoteP->horasVuelo, sizeof(pivoteP->horasVuelo));
			if (noTieneNum(pivoteP->horasVuelo))
				strcpy_s(pivoteP->horasVuelo, "\0");
			GetDlgItemText(hwnd, IDC_EDIT3, pivoteP->nombre, sizeof(pivoteP->nombre));
			if (contieneNumero(pivoteP->nombre))
				strcpy_s(pivoteP->nombre, "\0");
			GetDlgItemText(hwnd, IDC_EDIT4, pivoteP->apellidoP, sizeof(pivoteP->apellidoP));
			if (contieneNumero(pivoteP->apellidoP))
				strcpy_s(pivoteP->apellidoP, "\0");
			GetDlgItemText(hwnd, IDC_EDIT5, pivoteP->apellidoM, sizeof(pivoteP->apellidoM));
			if (contieneNumero(pivoteP->apellidoM))
				strcpy_s(pivoteP->apellidoM, "\0");
			GetDlgItemText(hwnd, IDC_EDIT6, pivoteP->correo, sizeof(pivoteP->correo));
			if (!contieneCorreo(pivoteP->correo))
				strcpy_s(pivoteP->correo, "\0");
			if (buscarPilo(inicio_pilo, final_pilo, pivoteP)) {
				MessageBox(hwnd, "Piloto ya dentro del sistema, favor de verificar el nombre", "Aviso", 0);
				pivoteP = NULL;
			}	//verifica que tenga informacion
			else if (pivoteP->apellidoM[0] != NULL && pivoteP->apellidoP[0] != NULL && pivoteP->correo[0] != NULL && pivoteP->horasVuelo[0] != NULL && pivoteP->nombre[0] != NULL) {
				agregarPilot(inicio_pilo, final_pilo, pivoteP);

				//borra lo que tienen los mensajes
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, 0);
				quickSort(inicio_pilo, final_pilo);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
				for (piloto* i = inicio_pilo; i != NULL; i = i->sig)
					SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)i->nombre);
				MessageBox(hwnd, "Piloto agregado con exito", "AVISO", MB_OK);
			}
			else MessageBox(hwnd, "Favor de llenar los datos de forma correcta", "AVISO", MB_OK);
			break;
		case IDC_LIST1:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
				EnableWindow(GetDlgItem(hwnd, agregar), false);
				EnableWindow(GetDlgItem(hwnd, eliminar), true);
				EnableWindow(GetDlgItem(hwnd, modificar), true);
				char texto[100] = {};
				int indice = 0;
				bool status = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)texto);
				auxP = inicio_pilo;
				while (auxP != NULL && status != 1) {
					if (strcmp(auxP->nombre, texto) != 0) auxP = auxP->sig;
					else status = 1;
				}
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)auxP->horasVuelo);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)auxP->nombre);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)auxP->apellidoP);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)auxP->apellidoM);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)auxP->correo);
				break;
			}
			break;
		case limpiar:
			auxP = NULL;
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwnd, agregar), true);
			EnableWindow(GetDlgItem(hwnd, eliminar), false);
			EnableWindow(GetDlgItem(hwnd, modificar), false);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, 0);
			break;
		case eliminar:
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
			GetDlgItemText(hwnd, IDC_EDIT2, nombre, sizeof(nombre));
			indice = SendDlgItemMessageA(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);
			eliminarPilot(inicio_pilo, nombre);
			EnableWindow(GetDlgItem(hwnd, agregar), true);
			EnableWindow(GetDlgItem(hwnd, eliminar), false);
			EnableWindow(GetDlgItem(hwnd, modificar), false);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
			for (piloto* i = inicio_pilo; i != NULL; i = i->sig)
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)i->nombre);
			MessageBox(hwnd, "Piloto eliminado", "Aviso", 0);
			break;
		case modificar:
			pivoteP = new piloto();
			GetDlgItemText(hwnd, IDC_EDIT2, pivoteP->horasVuelo, sizeof(pivoteP->horasVuelo));
			if (noTieneNum(pivoteP->horasVuelo))
				strcpy_s(pivoteP->horasVuelo, "\0");
			GetDlgItemText(hwnd, IDC_EDIT3, pivoteP->nombre, sizeof(pivoteP->nombre));
			if (contieneNumero(pivoteP->nombre))
				strcpy_s(pivoteP->nombre, "\0");
			GetDlgItemText(hwnd, IDC_EDIT4, pivoteP->apellidoP, sizeof(pivoteP->apellidoP));
			if (contieneNumero(pivoteP->apellidoP))
				strcpy_s(pivoteP->apellidoP, "\0");
			GetDlgItemText(hwnd, IDC_EDIT5, pivoteP->apellidoM, sizeof(pivoteP->apellidoM));
			if (contieneNumero(pivoteP->apellidoM))
				strcpy_s(pivoteP->apellidoM, "\0");
			GetDlgItemText(hwnd, IDC_EDIT6, pivoteP->correo, sizeof(pivoteP->correo));
			if (!contieneCorreo(pivoteP->correo))
				strcpy_s(pivoteP->correo, "\0");
			if (pivoteP->apellidoM[0] != NULL && pivoteP->apellidoP[0] != NULL && pivoteP->correo[0] != NULL && pivoteP->horasVuelo[0] != NULL && pivoteP->nombre[0] != NULL) {
				modifiPilo(inicio_pilo, auxP, pivoteP); SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
				for (piloto* i = inicio_pilo; i != NULL; i = i->sig)
					SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)i->nombre);
				MessageBox(hwnd, "Piloto modificado", "Aviso", 0);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
				EnableWindow(GetDlgItem(hwnd, agregar), true);
				EnableWindow(GetDlgItem(hwnd, eliminar), false);
				EnableWindow(GetDlgItem(hwnd, modificar), false);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, 0);
			}
			else MessageBox(hwnd, "Favor de llenar los datos de forma correcta", "AVISO", MB_OK);

			break;
		}
		break;
	}
	return false;
}

LRESULT CALLBACK Vuelos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		for (vuelo* i = inicio_vuelo; i != NULL; i = i->sig) {
			string tmp = to_string(i->clave);
			SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM)tmp.c_str());
		}
		for (piloto* i = inicio_pilo; i != NULL; i = i->sig)
			if (i->ocupado == false)
			{
				static int q = 0;
				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_INSERTSTRING, (WPARAM)q, (LPARAM)i->nombre);
				q++;
			}
		//cambiar
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)validador1);
		break;

	case WM_CLOSE:
		if (inicio_vuelo != NULL && inicio_vuelo->sig != NULL) heapSort(inicio_vuelo);
		EndDialog(hwnd, LOWORD(wParam));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			//limpia
		case IDCANCEL:
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, 0, 0);
			SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			SendMessage(GetDlgItem(hwnd, IDC_COMBO2), WM_SETTEXT, 0, 0);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT8), WM_SETTEXT, 0, 0);

			EnableWindow(GetDlgItem(hwnd, registrar), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), true);
			EnableWindow(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBO2), true);
			EnableWindow(GetDlgItem(hwnd, IDCANCEL2), false);
			SendDlgItemMessage(hwnd, cancelado, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			break;
			//cancela
		case IDCANCEL2:
			GetDlgItemText(hwnd, IDC_EDIT2, tmp, sizeof(tmp));
			for (vuelo* i = inicio_vuelo; i != NULL; i = i->sig) {
				string tmp2 = to_string(i->clave);
				if (strcmp(tmp, tmp2.c_str()) == 0) {
					i->estado = 0;
					SendDlgItemMessage(hwnd, cancelado, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					MessageBox(hwnd, "El vuelo a sido cancelado", "AVISO", MB_OK);
				}
			}
			break;
		case registrar:
			pivoteV = new vuelo();
			GetDlgItemText(hwnd, IDC_EDIT1, pivoteV->usuario, sizeof(pivoteV->usuario));
			clave = rand() % 9999;
			pivoteV->clave = clave;
			GetDlgItemText(hwnd, IDC_EDIT3, pivoteV->origen, sizeof(pivoteV->origen));
			if (contieneNumero(pivoteV->origen))
				strcpy_s(pivoteV->origen, "\0");
			GetDlgItemText(hwnd, IDC_EDIT4, pivoteV->destino, sizeof(pivoteV->destino));
			if (contieneNumero(pivoteV->destino))
				strcpy_s(pivoteV->destino, "\0");
			GetDlgItemText(hwnd, IDC_EDIT5, temp, sizeof(temp));
			if (noTieneNum(temp))
				pivoteV->turista = 0;
			else pivoteV->turista = stoi(temp);
			GetDlgItemText(hwnd, IDC_EDIT6, temp, sizeof(temp));
			if (noTieneNum(temp))
				pivoteV->ejecutivo = 0;
			else pivoteV->ejecutivo = stoi(temp);
			pivoteV->asientosTotales = pivoteV->turista + pivoteV->ejecutivo;
			SendDlgItemMessage(hwnd, IDC_DATETIMEPICKER2, DTM_GETSYSTEMTIME, 0, (LPARAM)&pivoteV->fecha);
			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_UNCHECKED) {
				status1 = false;
			}
			//estrecho
			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED &&
				IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_UNCHECKED) {
				pivoteV->tipoAvion = 0;
				status1 = true;
			}
			//ancho
			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_UNCHECKED &&
				IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
				pivoteV->tipoAvion = 1;
				status1 = true;
			}
			GetSystemTime(&pivoteV->fechaUsuario);
			GetDlgItemText(hwnd, IDC_COMBO2, pivoteV->piloto, sizeof(pivoteV->piloto));
			if (pilotoOcupado(pivoteV->piloto)) {
				strcpy_s(pivoteV->piloto, "\0");
			}
			if (seisMeses(pivoteV->fecha) == true)
				status1 = false;
			if (pivoteV->destino[0] != NULL && pivoteV->ejecutivo != 0 && esHoy(pivoteV->fecha, pivoteV->fechaUsuario) == false &&
				esAnterior(pivoteV->fecha, pivoteV->fechaUsuario) == false && pivoteV->origen[0] != NULL, pivoteV->piloto[0] != NULL &&
				pilotoOcupado(pivoteV->piloto) == false && status1 == true && pivoteV->turista != 0 && pivoteV->usuario[0] != NULL) {
				agregarVuelo(pivoteV);




				MessageBox(hwnd, "Vuelo registrado", "AVISO", MB_OK);
				for (vuelo* i = inicio_vuelo; i != NULL; i = i->sig) {
					string tmp = to_string(i->clave);
					SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM)tmp.c_str());
				}
				//borra lo que tienen los mensajes
				SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_RESETCONTENT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, 0);
				SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), WM_SETTEXT, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT8), WM_SETTEXT, 0, 0);

			}
			else MessageBox(hwnd, "Favor de llenar los datos de forma correcta", "AVISO", MB_OK);
			break;
		case IDC_LIST2:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				EnableWindow(GetDlgItem(hwnd, registrar), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBO2), false);
				EnableWindow(GetDlgItem(hwnd, cancelado), false);
				EnableWindow(GetDlgItem(hwnd, IDCANCEL2), true);



				char texto[100] = {};
				int clave = 0;
				bool status = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST2, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST2, LB_GETTEXT, indice, (LPARAM)texto);
				auxV = inicio_vuelo;
				while (auxV != NULL && status != 1) {
					string tmp2 = to_string(auxV->clave);
					if (strcmp(tmp2.c_str(), texto) != 0) auxV = auxV->sig;
					else status = 1;
				}
				string tmp2 = to_string(auxV->clave);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)tmp2.c_str());
				SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)auxV->usuario);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)auxV->origen);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)auxV->destino);
				tmp2 = to_string(auxV->turista);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)tmp2.c_str());
				tmp2 = to_string(auxV->ejecutivo);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)tmp2.c_str());
				tmp2 = to_string(auxV->asientosTotales);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, 0, (LPARAM)tmp2.c_str());
				SendDlgItemMessage(hwnd, IDC_DATETIMEPICKER2, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&auxV->fecha);
				SendDlgItemMessage(hwnd, IDC_DATETIMEPICKER1, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&auxV->fechaUsuario);
				if (auxV->tipoAvion == 0) {
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}
				if (auxV->tipoAvion == 1) {
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}
				if (auxV->estado == 0)
					SendDlgItemMessage(hwnd, cancelado, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT8), WM_SETTEXT, 0, (LPARAM)auxV->piloto);
				//cancelado
				if (auxV->estado == 0)
					SendDlgItemMessage(hwnd, cancelado, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
				//En proceso o concluido
				if (auxV->estado == 1 || auxV->estado == 2)
					SendDlgItemMessage(hwnd, cancelado, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			}
			break;
		}
		break;

	}
	return false;
}

LRESULT CALLBACK Pasajeros(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG:
	{


		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 100, (LPARAM)validador1);
		HWND hPControl = GetDlgItem(hwnd, IDC_USUARIO1);
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, fotoDirU, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen);

		break;
	}

	case WM_COMMAND:
	{
		int elemento = LOWORD(wParam);
		switch (elemento)
		{
		case IDOK2:
		{
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, 100, (LPARAM)&pasaP.usuario);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_GETTEXT, 100, (LPARAM)&pasaP.nombre);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_GETTEXT, 100, (LPARAM)&pasaP.nacionalidad);
			GetSystemTime(&pasaP.registro);
			SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER3), WM_GETTEXT, 100, (LPARAM)&pasaP.registro);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_GETTEXT, 100, (LPARAM)&pasaP.apellidoP);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_GETTEXT, 100, (LPARAM)&pasaP.apellidoM);
			SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), WM_GETTEXT, 999, (LPARAM)&pasaP.nacimiento);

			if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
				status1 = false;
			}
			//hombre = 0
			if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_CHECKED &&
				IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
				pasaP.genero = 0;
				status1 = true;
			}
			//mujer = 1
			if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED &&
				IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_CHECKED) {
				pasaP.genero = 1;
				status1 = true;
			}

			if (pasaP.usuario[0] != NULL && pasaP.nombre[0] != NULL && pasaP.apellidoP[0] && pasaP.apellidoM[0] != NULL && pasaP.nacionalidad[0] != NULL)
			{
				pasaP.conBoleto = true;

				agregarPasajero(inicio_pas, final_pas, pasaP.nombre, pasaP.apellidoP, pasaP.apellidoM, pasaP.nacimiento, pasaP.conBoleto, pasaP.genero, pasaP.nacionalidad, pasaP.usuario, pasaP.registro);

				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
				for (pasajero* i = inicio_pas; i != NULL; i = i->sig)
					SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)i->nombre);

				MessageBox(NULL, "Registro hecho correctamente", "REGISTRO", MB_OK);
				limpiarPasajero(hwnd);
			}
			else
			{
				pasaP.conBoleto = false;
				MessageBox(NULL, "Faltan datos por su registro", "REGISTRO", MB_OK);
			}
			break;
		}
		case IDC_LIST1:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), false);
				EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), false);

				EnableWindow(GetDlgItem(hwnd, agregar), false);

				if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
					status1 = false;
				}
				//hombre = 0
				if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_CHECKED &&
					IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
					pasaP.genero = 0;
					status1 = true;
				}
				//mujer = 1
				if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED &&
					IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_CHECKED) {
					pasaP.genero = 1;
					status1 = true;
				}

				char texto[100] = {};
				int indice = 0;
				bool status = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)texto);
				auxPas = inicio_pas;
				while (auxPas != NULL && status != 1) {
					if (strcmp(auxPas->nombre, texto) != 0) auxPas = auxPas->sig;
					else status = 1;
				}

				SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)&pasaP.usuario);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)&pasaP.nombre);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)&pasaP.nacionalidad);
				GetSystemTime(&pasaP.registro);
				SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER3), WM_SETTEXT, 0, (LPARAM)&pasaP.registro);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)&pasaP.apellidoP);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)&pasaP.apellidoM);
				SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), WM_SETTEXT, 0, (LPARAM)&pasaP.nacimiento);
				//Queda pendiente lo de los hombre y mujer
				break;
			}
			break;
		}
		break;
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}

LRESULT CALLBACK Boletos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 100, (LPARAM)validador1);
		HWND hPControl = GetDlgItem(hwnd, IDC_USUARIO1);
		HBITMAP hImagen = (HBITMAP)LoadImage(NULL, fotoDirU, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen);

		SendMessage(GetDlgItem(hwnd, IDC_COMBO4), CB_ADDSTRING, NULL, (LPARAM)"Menor de edad");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO4), CB_ADDSTRING, NULL, (LPARAM)"Adulto");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO4), CB_ADDSTRING, NULL, (LPARAM)"Mayor de edad");

		SendMessage(hPControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImagen);
		SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_ADDSTRING, NULL, (LPARAM)"Tarjeta de Credito");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_ADDSTRING, NULL, (LPARAM)"Tarjeta de Debito");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_ADDSTRING, NULL, (LPARAM)"Efectivo");
		SendMessage(GetDlgItem(hwnd, IDC_COMBO2), CB_ADDSTRING, NULL, (LPARAM)"Transferencia");
		auxPas = inicio_pas;
		for (int i = 0; auxPas != NULL; i++) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_INSERTSTRING, (WPARAM)i, (LPARAM)auxPas->nombre);
			auxPas = auxPas->sig;
		}
		auxV = inicio_vuelo;
		for (int i = 0; auxV != NULL; i++) {
			string tmp = to_string(auxV->clave);
			SendMessage(GetDlgItem(hwnd, IDC_COMBO3), CB_INSERTSTRING, (WPARAM)i, (LPARAM)tmp.c_str());
			auxV = auxV->sig;
		}
		break;
	}

	case WM_COMMAND:
	{
		int elemento = LOWORD(wParam);
		switch (elemento)
		{
		case IDC_BUTTON5:
			GetDlgItemText(hwnd, IDC_COMBO3, tmp, sizeof(tmp));
			auxV = inicio_vuelo;
			GetDlgItemText(hwnd, IDC_COMBO3, tmp, sizeof(tmp));
			while (auxV != NULL) {
				string cosa = to_string(auxV->clave);
				if (strcmp(tmp, cosa.c_str()) == false) {
					SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)auxV->origen);
					SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)auxV->destino);
					SendDlgItemMessage(hwnd, IDC_DATETIMEPICKER1, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&auxV->fecha);
					auxV = auxV->sig;
				}
				else auxV = auxV->sig;
			}
			break;
		case IDOK2:
		{
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_GETTEXT, 100, (LPARAM)&bP.usuario);
			int r = rand() % 9999 + 1000;
			char code[6];
			_itoa(r, code, 10);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 6, (LPARAM)&code);

			//Queda Pendiente Pase
			bP.claveAsiento = stoi(code);
			SendMessage(GetDlgItem(hwnd, IDC_COMBO1), WM_GETTEXT, 100, (LPARAM)tmp);
			strcpy_s(bP.pasajero, tmp);
			SendMessage(GetDlgItem(hwnd, IDC_COMBO4), WM_GETTEXT, 100, (LPARAM)tmp);
			if (strcmp(tmp, "Menor de edad") == 0) {
				bP.generacion = 0;
			}
			if (strcmp(tmp, "Adulto") == 0) {
				bP.generacion = 1;
			}
			if (strcmp(tmp, "Mayor de edad") == 0) {
				bP.generacion = 2;
			}
			SendMessage(GetDlgItem(hwnd, IDC_COMBO2), WM_GETTEXT, 100, (LPARAM)tmp);

			if (strcmp(tmp, "Tarjeta de Credito") == 0) {
				bP.pago = 0;
			}
			if (strcmp(tmp, "Tarjeta de Debito") == 0) {
				bP.pago = 1;
			}
			if (strcmp(tmp, "Transferencia") == 0) {
				bP.pago = 2;
			}
			if (strcmp(tmp, "Efectivo") == 0) {
				bP.pago = 3;
			}

			SendMessage(GetDlgItem(hwnd, IDC_COMBO3), WM_GETTEXT, 100, (LPARAM)tmp);
			bP.claveVuelo = stoi(tmp);
			//agria

			//credito, debito, transferencia, efectivo

			SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), WM_GETTEXT, 999, (LPARAM)&bP.abordo);
			if (IsDlgButtonChecked(hwnd, IDC_RADIO3) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, IDC_RADIO4) == BST_UNCHECKED) {
				status1 = false;
			}
			//Turista = 0
			if (IsDlgButtonChecked(hwnd, IDC_RADIO3) == BST_CHECKED &&
				IsDlgButtonChecked(hwnd, IDC_RADIO4) == BST_UNCHECKED) {
				bP.tipo = 0;

				status1 = true;
				bP.precio = 500;
			}
			//Ejectuvo = 1
			if (IsDlgButtonChecked(hwnd, IDC_RADIO3) == BST_UNCHECKED &&
				IsDlgButtonChecked(hwnd, IDC_RADIO4) == BST_CHECKED) {
				bP.tipo = 1;
				status1 = true;
				bP.precio = 1000;
			}
			/*SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 100, (LPARAM)&bP.precio);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_GETTEXT, 100, (LPARAM)&bP.precio); */

			agregarBoleto(inicio_b, final_b, bP.usuario, /*bP.cod,*/ bP.claveAsiento, bP.claveVuelo, bP.generacion, bP.tipo, bP.pago, bP.pasajero, bP.pase, bP.abordo, bP.precio);
			MessageBox(NULL, "Registro hecho correctamente, recuerde su codigo", "REGISTRO", MB_OK);
			limpiarBoleto(hwnd);
			/*if (pasaP.usuario[0] != NULL && pasaP.nombre[0] != NULL && pasaP.nacionalidad[0] != NULL)
			{
				agregarPasajero(inicio_pas, final_pas, pasaP.nombre, pasaP.nacimiento, pasaP.genero, pasaP.nacionalidad, pasaP.usuario, pasaP.registro);
				MessageBox(NULL, "Registro hecho correctamente", "REGISTRO", MB_OK);
				limpiarPasajero(hwnd);
			}
			else
			{
				MessageBox(NULL, "Faltan datos por su registro", "REGISTRO", MB_OK);
			}*/
			break;
		}
		case IDC_LIST1:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_GETTEXT, 100, (LPARAM)&bP.claveAsiento);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), WM_GETTEXT, 100, (LPARAM)&bP.pasajero);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO2), WM_GETTEXT, 100, (LPARAM)&bP.tipo);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO3), WM_GETTEXT, 100, (LPARAM)&bP.claveVuelo);
				SendMessage(GetDlgItem(hwnd, IDC_COMBO4), WM_GETTEXT, 100, (LPARAM)&bP.generacion);

				SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), WM_GETTEXT, 999, (LPARAM)&bP.abordo);

				EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);

				EnableWindow(GetDlgItem(hwnd, IDC_COMBO1), false);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBO2), false);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBO3), false);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBO4), false);
				EnableWindow(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), false);

				EnableWindow(GetDlgItem(hwnd, agregar), false);

				if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
					status1 = false;
				}
				//hombre = 0
				if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_CHECKED &&
					IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_UNCHECKED) {
					pasaP.genero = 0;
					status1 = true;
				}
				//mujer = 1
				if (IsDlgButtonChecked(hwnd, IDC_RADIOH) == BST_UNCHECKED &&
					IsDlgButtonChecked(hwnd, IDC_RADIOM) == BST_CHECKED) {
					pasaP.genero = 1;
					status1 = true;
				}

				char texto[100] = {};
				int indice = 0;
				bool status = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)texto);
				auxPas = inicio_pas;
				while (auxPas != NULL && status != 1) {
					if (strcmp(auxPas->nombre, texto) != 0) auxPas = auxPas->sig;
					else status = 1;
				}

				SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)&pasaP.usuario);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)&pasaP.nombre);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)&pasaP.nacionalidad);
				GetSystemTime(&pasaP.registro);
				SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER3), WM_SETTEXT, 0, (LPARAM)&pasaP.registro);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)&pasaP.apellidoP);
				SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)&pasaP.apellidoM);
				SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), WM_SETTEXT, 0, (LPARAM)&pasaP.nacimiento);

				break;
			}
			break;
		}

		break;
	}

	case WM_CLOSE:
	{
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	default:
		break;
	}
	return false;
}


LRESULT CALLBACK Manifiesto(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
		for (vuelo* i = inicio_vuelo; i != NULL; i = i->sig) {
			string tmp = to_string(i->clave);
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)tmp.c_str());
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_LIST1:
			bool status3 = 0;
			char texto[100];
			int h = 0;
			h = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)texto);
			int a = stoi(texto);
			for (vuelo* i = inicio_vuelo; i != NULL && status3 != 1; i = i->sig) {
				if (i->clave == a) {
					pivoteV = i;
					status3 = 1;
				}
			}
			for (boleto* i = inicio_b; i != NULL; i = i->sig) {
				if (i->claveVuelo == a) {
					SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM)i->pasajero);
				}
			}
			string tmp = to_string(pivoteV->clave);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT1), WM_SETTEXT, 0, (LPARAM)tmp.c_str());
			SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)pivoteV->origen);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)pivoteV->destino);
			tmp = to_string(pivoteV->asientosTotales);
			SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, 0, (LPARAM)tmp.c_str());
			break;
		}
		break;

	case WM_CLOSE:
	{
		EndDialog(hwnd, LOWORD(wParam));
		break;
	}
	}
	return 0;
}
void agregarUsu(user*& raiz, user* nuevoUsuario) {
	if (raiz == NULL) {
		raiz = nuevoUsuario;
	}
	else {
		if (strcmp(nuevoUsuario->usuario, raiz->usuario) < 0) {
			agregarUsu(raiz->izq, nuevoUsuario);
		}
		else {
			agregarUsu(raiz->der, nuevoUsuario);
		}
	}
}
//
//user* aux1 = inicioU;
//user* aux2 = NULL;

//	while (aux1 != NULL) {
//		aux2 = aux1;
//		aux1 = aux1->sig;
//	}
//
//	if (inicioU == aux1) {
//		inicioU = nuevo_nodo;
//
//	}
//	else {
//		aux2->sig = nuevo_nodo;
//		nuevo_nodo->ant = aux2;
//	}
//
//	nuevo_nodo->sig = aux1;
//	nuevo_nodo->ant = aux2;
//	finalU = nuevo_nodo;
//	return nodo;
//}


void eliminarUsu(user*& raiz, char usuario[]) {
	if (raiz == NULL) {
		MessageBox(NULL, "No existe este Usuario", "USERS", MB_OK);
		return;
	}
	if (strcmp(usuario, raiz->usuario) < 0) {
		eliminarUsu(raiz->izq, usuario);
	}
	else if (strcmp(usuario, raiz->usuario) > 0) {
		eliminarUsu(raiz->der, usuario);
	}
	else {
		user* nodoAEliminar = raiz;
		if (raiz->izq == NULL) {
			raiz = raiz->der;
		}
		else if (raiz->der == NULL) {
			raiz = raiz->izq;
		}
		else {
			user* reemplazo = raiz->der;
			user* padreReemplazo = raiz;
			while (reemplazo->izq != NULL) {
				padreReemplazo = reemplazo;
				reemplazo = reemplazo->izq;
			}
			if (padreReemplazo != raiz) {
				padreReemplazo->izq = reemplazo->der;
				reemplazo->der = raiz->der;
			}
			reemplazo->izq = raiz->izq;
			raiz = reemplazo;
		}
		delete nodoAEliminar;
	}
}
void limpiarUsu(HWND hwnd) {

	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), NM_KILLFOCUS, NULL, NULL);
	SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Usuario");
	SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_ADDSTRING, NULL, (LPARAM)"Admin");
	//Genero
}

void leerArbolAdmin(ifstream& archivo, user& raiz) {
	if (!archivo.is_open()) {
		cerr << "Error: No se puede abrir el archivo." << endl;
		return;
	}

	// Leer el estado del nodo (1 o 0)
	int estado;
	archivo >> estado;

	// Si el estado es 1, es un nodo con datos de usuario
	if (estado == 1) {
		// Leer los datos del usuario
		archivo.ignore(); // Ignorar el salto de línea
		archivo.getline(raiz.usuario, sizeof(raiz.usuario));
		archivo.getline(raiz.contra, sizeof(raiz.contra));
		archivo.getline(raiz.correo, sizeof(raiz.correo));
		archivo.getline(raiz.nombres, sizeof(raiz.nombres));
		archivo.getline(raiz.apellidoP, sizeof(raiz.apellidoP));
		archivo.getline(raiz.apellidoM, sizeof(raiz.apellidoM));

		char timeString[100];
		archivo.getline(timeString, sizeof(timeString));
		sscanf_s(timeString, "%02d/%02d/%04d %02d:%02d:%02d",
			&raiz.nacimiento.wDay, &raiz.nacimiento.wMonth, &raiz.nacimiento.wYear,
			&raiz.nacimiento.wHour, &raiz.nacimiento.wMinute, &raiz.nacimiento.wSecond);

		archivo >> raiz.genero;
		archivo >> raiz.admin;
		archivo.ignore(); // Ignorar el salto de línea
		archivo.getline(raiz.foto, sizeof(raiz.foto));

		// Leer subárboles izquierdo y derecho recursivamente
		raiz.izq = new user;
		raiz.der = new user;
		leerArbolAdmin(archivo, *(raiz.izq));
		leerArbolAdmin(archivo, *(raiz.der));
	}
	else {
		// Si el estado es 0, es un nodo vacío
		raiz.izq = NULL;
		raiz.der = NULL;
	}
}

user* buscarUsuario(user* raiz, const char* usuario) {
	if (raiz == NULL) {
		return NULL; // El árbol está vacío o el usuario no se encuentra
	}

	int comparacion = strcmp(usuario, raiz->usuario);
	if (comparacion == 0) {
		return raiz; // Se encontró el usuario en el nodo actual
	}
	else if (comparacion < 0) {
		return buscarUsuario(raiz->izq, usuario); // Buscar en el subárbol izquierdo
	}
	else {
		return buscarUsuario(raiz->der, usuario); // Buscar en el subárbol derecho
	}
}


void escribirYguardar(user* nodo, ofstream& archivo) {
	if (nodo != nullptr) {
		archivo << nodo->usuario << endl;
		archivo << nodo->contra << endl;
		archivo << nodo->correo << endl;
		archivo << nodo->nombres << endl;
		archivo << nodo->apellidoP << endl;
		archivo << nodo->apellidoM << endl;
		char timeString[100];
		sprintf_s(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			nodo->nacimiento.wDay, nodo->nacimiento.wMonth, nodo->nacimiento.wYear,
			nodo->nacimiento.wHour, nodo->nacimiento.wMinute, nodo->nacimiento.wSecond);
		archivo << timeString << endl;
		archivo << nodo->genero << endl;
		archivo << nodo->admin << endl;
		archivo << nodo->foto << endl;
	}
}

//txt
void guardarUsuarios() {
	ofstream archivo;
	archivo.open("users.txt");
	if (archivo.is_open()) {
		preorden(raiz, archivo);
		archivo.close();
	}
}



void leerUsuarios() {
	string linea;
	ifstream archivo("users.txt");
	if (archivo.fail()) {
		return;
	}
	while (!archivo.eof()) {
		user* nuevoUsuario = new user();
		getline(archivo, linea); strcpy_s(nuevoUsuario->usuario, linea.c_str());
		getline(archivo, linea); strcpy_s(nuevoUsuario->contra, linea.c_str());
		getline(archivo, linea); strcpy_s(nuevoUsuario->correo, linea.c_str());
		getline(archivo, linea); strcpy_s(nuevoUsuario->nombres, linea.c_str());
		getline(archivo, linea); strcpy_s(nuevoUsuario->apellidoP, linea.c_str());
		getline(archivo, linea); strcpy_s(nuevoUsuario->apellidoM, linea.c_str());
		getline(archivo, linea);
		sscanf_s(linea.c_str(), "%02hd/%02hd/%04hd %02hd:%02hd:%02hd",
			&nuevoUsuario->nacimiento.wDay, &nuevoUsuario->nacimiento.wMonth, &nuevoUsuario->nacimiento.wYear,
			&nuevoUsuario->nacimiento.wHour, &nuevoUsuario->nacimiento.wMinute, &nuevoUsuario->nacimiento.wSecond);
		getline(archivo, linea); nuevoUsuario->genero = stoi(linea);
		getline(archivo, linea); nuevoUsuario->admin = stoi(linea);
		getline(archivo, linea); strcpy_s(nuevoUsuario->foto, linea.c_str());

		agregarUsu(raiz, nuevoUsuario);
	}
	archivo.close();
}
// Funciones de Piloto
void agregarPiloto(piloto*& inicio_pilo, piloto*& final_pilo, char usuario[], char horasVuelo[], char nombre[], char apellidoP[], char apellidoM[], char correo[], bool ocupado)
{
	piloto* nuevo_nodo = new piloto();
	strcpy(nuevo_nodo->usuario, usuario);
	strcpy(nuevo_nodo->horasVuelo, horasVuelo);
	strcpy(nuevo_nodo->nombre, nombre);
	strcpy(nuevo_nodo->apellidoP, apellidoP);
	strcpy(nuevo_nodo->apellidoM, apellidoM);
	strcpy(nuevo_nodo->correo, correo);
	nuevo_nodo->ocupado, ocupado;

	piloto* aux1 = inicio_pilo;
	piloto* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicio_pilo == aux1) {
		inicio_pilo = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	final_pilo = nuevo_nodo;
}

void eliminarPiloto(piloto*& inicio_pilo, char usuario[])
{
	if (inicio_pilo != NULL) {
		piloto* aux_borrar;
		piloto* ante = NULL;
		aux_borrar = inicio_pilo;

		while (aux_borrar != NULL && strcmp(aux_borrar->usuario, usuario)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "No existe este Piloto", "PILOTOS", MB_OK);
		else if (ante == NULL) {
			inicio_pilo = inicio_pilo->sig;
			if (aux_borrar->sig != NULL)
				aux_borrar->sig->ant = NULL;
			delete aux_borrar;
		}
		else if (aux_borrar->sig == NULL) {
			ante->sig = aux_borrar->sig;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}

void limpiarPiloto(HWND hwnd)
{
	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, NULL, (LPARAM)L"");
}
//txt
void guardarPiloto()
{
	ofstream archivo;
	archivo.open("pilotos.bin", ios::binary);
	auxP = inicio_pilo;
	while (auxP != NULL)
	{
		archivo << auxP->usuario << endl;
		archivo << auxP->horasVuelo << endl;
		archivo << auxP->nombre << endl;
		archivo << auxP->apellidoP << endl;
		archivo << auxP->apellidoM << endl;
		archivo << auxP->correo << endl;
		archivo << auxP->ocupado;
		if (auxP->sig != NULL) archivo << endl;

		auxP = auxP->sig;
	}
	archivo.close();
}

// Funciones de Vuelo

void agregarVuelooo(vuelo*& inicio_vuelo, vuelo*& final_vuelo, int clave,
	char origen[], char destino[], char piloto[], SYSTEMTIME fecha, int turista,
	int ejecutivo, int asientosTotales, float menor, char usuario[], SYSTEMTIME fechaUsuario, bool tipoAvion, int estado)
{
	vuelo* nuevo_nodo = new vuelo();
	nuevo_nodo->clave, clave;
	strcpy(nuevo_nodo->origen, origen);
	strcpy(nuevo_nodo->destino, destino);
	strcpy(nuevo_nodo->piloto, piloto);
	nuevo_nodo->fecha, fecha;
	nuevo_nodo->turista, turista;
	nuevo_nodo->ejecutivo, ejecutivo;
	nuevo_nodo->asientosTotales, asientosTotales;
	nuevo_nodo->menor, menor;
	strcpy(nuevo_nodo->usuario, usuario);
	nuevo_nodo->fechaUsuario, fechaUsuario;
	nuevo_nodo->tipoAvion, tipoAvion;
	nuevo_nodo->estado, estado;

	vuelo* aux1 = inicio_vuelo;
	vuelo* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicio_vuelo == aux1) {
		inicio_vuelo = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	final_vuelo = nuevo_nodo;
}

void eliminarVuelooo(vuelo*& inicio_vuelo, char usuario[])
{
	if (inicio_vuelo != NULL) {
		vuelo* aux_borrar;
		vuelo* ante = NULL;
		aux_borrar = inicio_vuelo;

		while (aux_borrar != NULL && strcmp(aux_borrar->usuario, usuario)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "No existe este Vuelo", "VUELOS", MB_OK);
		else if (ante == NULL) {
			inicio_vuelo = inicio_vuelo->sig;
			if (aux_borrar->sig != NULL)
				aux_borrar->sig->ant = NULL;
			delete aux_borrar;
		}
		else if (aux_borrar->sig == NULL) {
			ante->sig = aux_borrar->sig;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}

void limpiarVuelooo(HWND hwnd)
{
	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, NULL, (LPARAM)L"");
}
//txt

void guardarVuelooo()
{
	ofstream archivo;
	archivo.open("vuelos.bin", ios::binary);
	auxV = inicio_vuelo;
	while (auxV != NULL)
	{
		archivo << auxV->clave << endl;
		archivo << auxV->origen << endl;
		archivo << auxV->destino << endl;
		archivo << auxV->piloto << endl;

		char timeString[100];
		sprintf_s(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			auxV->fecha.wDay, auxV->fecha.wMonth, auxV->fecha.wYear,
			auxV->fecha.wHour, auxV->fecha.wMinute, auxV->fecha.wSecond);
		archivo << timeString << endl;

		archivo << auxV->turista << endl;
		archivo << auxV->ejecutivo << endl;
		archivo << auxV->asientosTotales << endl;
		archivo << auxV->menor << endl;
		archivo << auxV->mayor << endl;
		archivo << auxV->usuario << endl;

		char timeString2[100];
		sprintf_s(timeString2, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			auxV->fechaUsuario.wDay, auxV->fechaUsuario.wMonth, auxV->fechaUsuario.wYear,
			auxV->fechaUsuario.wHour, auxV->fechaUsuario.wMinute, auxV->fechaUsuario.wSecond);
		archivo << timeString << endl;

		archivo << auxV->tipoAvion << endl;
		archivo << auxV->estado;
		if (auxV->sig != NULL) archivo << endl;
		auxV = auxV->sig;
	}
	archivo.close();
}

//// Funciones de Pasajero
void agregarPasajero(pasajero*& inicioPas, pasajero*& finalPas, char nombre[], char apellidoP[], char apellidoM[], SYSTEMTIME nacimiento, bool conBoleto, bool genero, char nacionalidad[], char usuario[], SYSTEMTIME registro)
{
	pasajero* nuevo_nodo = new pasajero();
	strcpy(nuevo_nodo->nombre, nombre);
	strcpy(nuevo_nodo->apellidoP, apellidoP);
	strcpy(nuevo_nodo->apellidoM, apellidoM);
	nuevo_nodo->nacimiento = nacimiento;
	nuevo_nodo->genero = genero;
	strcpy(nuevo_nodo->nacionalidad, nacionalidad);
	nuevo_nodo->registro = registro;
	strcpy(nuevo_nodo->usuario, usuario);

	pasajero* aux1 = inicioPas;
	pasajero* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicioPas == aux1) {
		inicioPas = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	finalPas = nuevo_nodo;
}

void eliminarPasajero(pasajero*& inicioPas, char nombre[])
{
	if (inicioPas != NULL) {
		pasajero* aux_borrar;
		pasajero* ante = NULL;
		aux_borrar = inicioPas;

		while (aux_borrar != NULL && strcmp(aux_borrar->nombre, nombre)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "No existe este Pasajero", "PASAJEROS", MB_OK);
		else if (ante == NULL) {
			inicioPas = inicioPas->sig;
			if (aux_borrar->sig != NULL)
				aux_borrar->sig->ant = NULL;
			delete aux_borrar;
		}
		else if (aux_borrar->sig == NULL) {
			ante->sig = aux_borrar->sig;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}

void limpiarPasajero(HWND hwnd) {
	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT6), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_EDIT7), WM_SETTEXT, NULL, (LPARAM)L"");
}

//txt
void guardarPasajero()
{
	ofstream archivo;
	archivo.open("pasajeros.bin", ios::binary);
	auxPas = inicio_pas;
	while (auxPas != NULL)
	{
		archivo << auxPas->nombre << endl;
		archivo << auxPas->apellidoP << endl;
		archivo << auxPas->apellidoM << endl;
		char timeString[100];
		sprintf_s(timeString, sizeof(timeString), "%02d/%02d/%04d %02d:%02d:%02d",
			auxPas->nacimiento.wDay, auxPas->nacimiento.wMonth, auxPas->nacimiento.wYear,
			auxPas->nacimiento.wHour, auxPas->nacimiento.wMinute, auxPas->nacimiento.wSecond);
		archivo << timeString << endl;
		archivo << auxPas->conBoleto << endl;
		archivo << auxPas->genero << endl;
		archivo << auxPas->nacionalidad << endl;
		char timeString2[100];
		sprintf_s(timeString2, sizeof(timeString2), "%02d/%02d/%04d %02d:%02d:%02d",
			auxPas->registro.wDay, auxPas->registro.wMonth, auxPas->registro.wYear,
			auxPas->registro.wHour, auxPas->registro.wMinute, auxPas->registro.wSecond);
		archivo << timeString2 << endl;
		archivo << auxPas->usuario;
		if (auxPas->sig != NULL)archivo << endl;
		auxPas = auxPas->sig;
	}
	archivo.close();
}

void leerPasajero() {
	string linea;
	ifstream archivo;
	archivo.open("pasajeros.bin", ios::binary);
	if (archivo.fail()) {
		return;
	}
	inicio_pas = new pasajero;
	auxPas = inicio_pas;
	while (!getline(archivo, linea).eof()) {
		strcpy_s(auxPas->nombre, linea.c_str());
		getline(archivo, linea);

		strcpy_s(auxPas->apellidoP, linea.c_str());
		getline(archivo, linea);

		strcpy_s(auxPas->apellidoM, linea.c_str());
		getline(archivo, linea);

		sscanf_s(linea.c_str(), "%02hd/%02hd/%04hd %02hd:%02hd:%02hd",
			&auxPas->nacimiento.wDay, &auxPas->nacimiento.wMonth, &auxPas->nacimiento.wYear,
			&auxPas->nacimiento.wHour, &auxPas->nacimiento.wMinute, &auxPas->nacimiento.wSecond);

		getline(archivo, linea);
		auxPas->conBoleto = stoi(linea);

		getline(archivo, linea);
		auxPas->genero = stoi(linea);

		getline(archivo, linea);
		strcpy_s(auxPas->nacionalidad, linea.c_str());

		getline(archivo, linea);
		sscanf_s(linea.c_str(), "%02hd/%02hd/%04hd %02hd:%02hd:%02hd",
			&auxPas->registro.wDay, &auxPas->registro.wMonth, &auxPas->registro.wYear,
			&auxPas->registro.wHour, &auxPas->registro.wMinute, &auxPas->registro.wSecond);
		getline(archivo, linea);
		strcpy_s(auxPas->usuario, linea.c_str());

		auxPas->sig = new pasajero();
		auxPas->sig->ant = auxPas;
		auxPas = auxPas->sig;
	}
	if (strcmp(auxPas->usuario, "") == false) {
		auxPas->ant->sig = NULL;
	}
	archivo.close();
}

// Funciones de Boletos
void agregarBoleto(boleto*& inicioB, boleto*& finalB, char usuario[], /*char cod[],*/ int claveAsiento, int claveVuelo, int generacion, bool tipo, int pago, char pasajero[], bool pase, SYSTEMTIME abordo, int precio)
{
	boleto* nuevo_nodo = new boleto();
	strcpy(nuevo_nodo->usuario, usuario);
	//strcpy(nuevo_nodo->cod, cod); 
	nuevo_nodo->claveAsiento = claveAsiento;
	nuevo_nodo->claveVuelo = claveVuelo;
	nuevo_nodo->generacion = generacion;
	nuevo_nodo->tipo = tipo;
	nuevo_nodo->pago = pago;
	strcpy(nuevo_nodo->pasajero, pasajero);
	nuevo_nodo->pase = pase;
	nuevo_nodo->abordo = abordo;
	nuevo_nodo->precio = precio;
	boleto* aux1 = inicioB;
	boleto* aux2 = NULL;

	while (aux1 != NULL) {
		aux2 = aux1;
		aux1 = aux1->sig;
	}

	if (inicioB == aux1) {
		inicioB = nuevo_nodo;

	}
	else {
		aux2->sig = nuevo_nodo;
		nuevo_nodo->ant = aux2;
	}

	nuevo_nodo->sig = aux1;
	nuevo_nodo->ant = aux2;
	finalB = nuevo_nodo;
}

void eliminarBoleto(boleto*& inicioB, char pasajero[])
{
	if (inicioB != NULL) {
		boleto* aux_borrar;
		boleto* ante = NULL;
		aux_borrar = inicioB;

		while (aux_borrar != NULL && strcmp(aux_borrar->pasajero, pasajero)) {
			ante = aux_borrar;
			aux_borrar = aux_borrar->sig;
		}
		if (aux_borrar == NULL)
			MessageBox(NULL, "No existe este Boleto de Pasajero", "Boletos", MB_OK);
		else if (ante == NULL) {
			inicioB = inicioB->sig;
			if (aux_borrar->sig != NULL)
				aux_borrar->sig->ant = NULL;
			delete aux_borrar;
		}
		else if (aux_borrar->sig == NULL) {
			ante->sig = aux_borrar->sig;
			delete aux_borrar;
		}
		else {
			ante->sig = aux_borrar->sig;
			aux_borrar->sig->ant = ante;
			delete aux_borrar;
		}
	}
}

void limpiarBoleto(HWND hwnd)
{
	SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_COMBO2), WM_SETTEXT, NULL, (LPARAM)L"");
	SendMessage(GetDlgItem(hwnd, IDC_COMBO4), WM_SETTEXT, NULL, (LPARAM)L"");

	//SendMessage(GetDlgItem(hwnd, IDC_RADIO3), BST_UNCHECKED, NULL, (LPARAM)L"");
	//SendMessage(GetDlgItem(hwnd, IDC_RADIO4), BST_UNCHECKED, NULL, (LPARAM)L"");
}