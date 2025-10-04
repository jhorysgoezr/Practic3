#include <iostream>

using namespace std;
void convertirABinario(int numero, char* binario);
void invertirBits(char* bloque, int tamanio);
void desplazarBits(char* bloque, int tamanio);
void desplazarBitsDerecha(char* bloque, int tamanio);


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

