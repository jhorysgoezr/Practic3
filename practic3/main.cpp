#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


using namespace std;
void convertirABinario(int numero, char* binario);
void invertirBits(char* bloque, int tamanio);
void desplazarBits(char* bloque, int tamanio);
void desplazarBitsDerecha(char* bloque, int tamanio);
void leerArchivoYConvertirEnBloques(const string& nombreArchivoEntrada, const string& nombreArchivoSalida,
                                    int n, int metodo );
void decodificarArchivo(const string& nombreArchivoEntrada, const string& nombreArchivoSalida,
                        int n, int metodo );
string encriptarClave(const& claveOriginal, int metodo );
string desencriptarClave(const string& claveEncriptada,int metodo);



int main()
{
    cout << "Hello World!" << endl;
    return 0;
}

// Función para convertir un carácter ASCII en su representación binaria de 8 bits
void convertirABinario(int numero, char* binario) {
    for (int i = 7; i >= 0; --i) {
        binario[7 - i] = (numero & (1 << i)) ? '1' : '0';
    }
    binario[8] = '\0';
}

// funcion para invertir bits
void invertirBits(char* bloque, int tamanio) {
    for (int i = 0; i < tamanio; ++i) {
        bloque[i] = (bloque[i] == '1') ? '0' : '1';
    }
}

// funcion para desplazar bits hacia la izquierda
void desplazarBits(char* bloque, int tamanio) {
    if (tamanio <= 1) return;

    char temp = bloque[0];
    for (int i = 0; i < tamanio - 1; ++i) {
        bloque[i] = bloque[i + 1];
    }
    bloque[tamanio - 1] = temp;
}

// funcion para desplazar bits hacia la derecha (para decodificación)
void desplazarBitsDerecha(char* bloque, int tamanio) {
    if (tamanio <= 1) return;

    char temp = bloque[tamanio - 1];
    for (int i = tamanio - 1; i > 0; --i) {
        bloque[i] = bloque[i - 1];
    }
    bloque[0] = temp;
}

//funcion para leer el archivo y convertirlo en bloques
void leerArchivoYConvertirEnBloques(const string& nombreArchivoEntrada, const string& nombreArchivoSalida,
                                    int n, int metodo ){
    ifstream archivo(nombreArchivoEntrada, ios::in);

    if (!archivo){
        cout<<"Error al abrir el archivo de entrada. "<<endl;
        return;
    }

    char caracter;
    char binario[9];
    string bitsTotales = "";

    while (archivo.get(caracter)){
        convertirABinario((int)caracter, binario);
        bitsTotales += binario;
    }

    archivo.close();

    ofstream archivoSalida(nombreArchivoSalida);
    if (!archivoSalida){
        cout<<"Error al crear el archivo de salida."<<endl;
        return;
    }

    for (size_t i = 0; i < bitsTotales.size(); i += n){
        char bloque [n +1];
        int tamanioBloque = 0;

        for (size_t j = i; j < i + n && j <bitsTotales.size(); ++j){
            bloque[tamanioBloque++] = bitsTotales[j];
        }
        bloque[tamanioBloque] = '\0';
        if(metodo == 1){
            invertirBits(bloque, tamanioBloque);
        }
        else if (metodo == 2){
            desplazarBits(bloque, tamanioBloque);
        }
        archivoSalida<<bloque;
    }
    archivoSalida.close();
}

//Funcion para decodificar archivo
void decodificarArchivo(const string& nombreArchivoEntrada, const string& nombreArchivoSalida,
                        int n, int metodo ){
    ifstream archivo(nombreArchivoEntrada, ios::in);

    if(!archivo){
        cout<<"Error al abrir el archivo de entrada."<<endl;
        return;
    }

    string bitsTotales = "";
    char bloque[n + 1];

    while(archivo.read(bloque, n)){
        bloque[n] = '\0';
        if (metodo == 1){
            invertirBits(bloque, n);
        }
        else if (metodo == 2){
            desplazarBitsDerecha(bloque, n);
        }
        bitsTotales += bloque;
    }
    archivo.close();

    ofstream archivosalida(nombreArchivoSalida);

    if(!archivosalida){
        cout<<"Problema para crear el archivo de salida."<<endl;
        return;
    }

    for (size_t i = 0; i < bitsTotales.size(); i += 8){
        if (i + 8 <= bitsTotales.size()){
            int valor = 0;
            for (int j = 0; j<8; ++j){
                valor = (valor << 1) | (bitsTotales[i+j] - '0');
            }
            archivosalida <<static_cast<char>(valor);
        }
    }
    archivosalida.close();
}

string encriptarClave(const string& claveOriginal, int metodo){
    string claveEncriptada;

    for (char c: claveOriginal){
        char binario[9];
        convertirABinario(static_cast<int>(c), binario);

        if (metodo == 1){
            invertirBits(binario, 8);
        }
        else if (metodo == 2){
            desplazarBits(binario, 8);
        }

        claveEncriptada += binario;
    }
    return claveEncriptada;
}

string desencriptarClave(const string& claveEncriptada,int metodo ){
    string claveDesencriptada;

    for(size_t i = 0; i < claveEncriptada.size(); i += 8){
        char bloque[9];

        for (size_t j= 0; j < 8 && i + j <claveEncriptada.size(); ++j){
            bloque[j] = claveEncriptada[i + j];
        }
        bloque[8] = '\0';

        if (metodo == 1){
            invertirBits(bloque, 8);
        }
        else if (metodo == 2){
            desplazarBitsDerecha(bloque, 8);
        }

        int valor = stoi(string(bloque), nullptr, 2);
        claveDesencriptada += static_cast<char>(valor);
    }
    return claveDesencriptada;
}
