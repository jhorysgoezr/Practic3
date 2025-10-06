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
bool validarAccesoAdministrador();
void registrarUsuario();
void mostrarUsuarios();
bool iniciarSesionUsuario(string& cedulaUsuario);
void actualizarSaldo(const string& cedula, double nuevoSaldo);
double obtenerSaldo(const string& cedula);
void mostrarSaldo(const string& cedula);
void realizarRetiro(const string& cedula);


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

bool validarAccesoAdministrador(){
    const string nombreArchivoClave ="sudo.txt";

    ifstream archivoClave(nombreArchivoClave);

    if(!archivoClave.is_open()){
        cout<<"Error al abrir el archivo de claves."<<endl;
        return false;
    }

    string claveEncriptada;
    getline(archivoClave, claveEncriptada);
    archivoClave.close();

    string claveIngresada;
    cout<<"Ingrese la clave de administrador: ";
    cin>>claveIngresada;

    int metodoSeleccionado = 1;
    string claveIngresadaEncriptada = encriptarClave(claveIngresada, metodoSeleccionado);

    if (claveIngresadaEncriptada == claveEncriptada){
        cout<<"Clave correcta."<<endl;
        return true;
    }
    else {
        cout<<"Clave incorrecta."<<endl;
        return false;
    }
}

void registrarUsuario(){
    string cedula, clave;
    double saldo;
    const string archivoUsuarios = "usuarios.txt";

    cout<<"\nRegistro de nuevo usuario"<<endl;
    cout<<"Ingrese numero de cedula: ";
    cin>>cedula;
    cout<<"Ingrese clave del usuario: ";
    cin>>clave;
    cout<<"Ingrese el saldo incial (COP): ";
    cin>>saldo;

    stringstream ss;
    ss <<cedula<<","<<clave<<","<<saldo;
    string datosUsuario = ss.str();

    string datosEncriptados = encriptarClave(datosUsuario, 1);

    ofstream archivo(archivoUsuarios, ios::app);
    if(!archivo){
        cout<<"Error al abrir el archivo de usuarios."<<endl;
        return;
    }

    archivo <<datosEncriptados << endl;
    archivo.close();

    cout<<"Usuario registrado exitosamente."<<endl;
}

void mostrarUsuarios(){
    const string archivoUsuarios ="usuarios.txt";
    ifstream archivo(archivoUsuarios);

    if(!archivo){
        cout<<"No hay usuarios registrados."<<endl;
        return;
    }

    string lineaEncriptada;
    int contador = 1;

    cout<<"\nListado de usuarios registrados:"<<endl;
    cout<<"---------------------------------"<<endl;

    while(getline(archivo, lineaEncriptada)){
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);

        stringstream ss(lineaDesencriptada);
        string cedula, clave, saldo;

        getline(ss, cedula, ',');
        getline(ss, clave, ',');
        getline(ss, saldo, ',');

        cout<<"Usuario: "<<contador<<":"<<endl;
        cout<<"Cedula: "<<cedula<<endl;
        cout<<"Saldo: "<<saldo<<" COP"<<endl;
        cout<<"--------------------------"<<endl;

        contador++;
    }

    archivo.close();
}

bool iniciarSesionUsuario(string& cedulaUsuario){
    string cedula, clave;
    cout<<"Ingrese su numero de cedula: ";
    cin>>cedula;
    cout<<"Ingrese su numero de clave: ";
    cin>>clave;

    const string archivoUsuarios = "usuarios.txt";
    ifstream archivo(archivoUsuarios);

    if(!archivo){
        cout<<"Error al abrir el archivo de usuairos."<<endl;
        return false;
    }

    string lineaEncriptada;
    while (getline(archivo, lineaEncriptada)){
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);
        stringstream ss(lineaDesencriptada);
        string cedulaArchivo, claveArchivo;

        getline(ss, cedulaArchivo, ',');
        getline(ss, claveArchivo, ',');

        if (cedula == cedulaArchivo && clave==claveArchivo){
            cedulaUsuario = cedula;
            archivo.close();
            return true;
        }
    }
    archivo.close();
    cout<<"Credenciales incorrectas."<<endl;
    return false;
}

void actualizarSaldo(const string& cedula, double nuevoSaldo) {
    const string archivoUsuarios = "usuarios.txt";
    const string archivoTemp = "temp.txt";
    ifstream archivo(archivoUsuarios);
    ofstream archivoTemporal(archivoTemp);

    if (!archivo || !archivoTemporal) {
        cout << "Error al acceder a los archivos." << endl;
        return;
    }

    string lineaEncriptada;
    while (getline(archivo, lineaEncriptada)) {
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);
        stringstream ss(lineaDesencriptada);
        string cedulaArchivo, clave, saldoStr;

        getline(ss, cedulaArchivo, ',');
        getline(ss, clave, ',');
        getline(ss, saldoStr, ',');

        if (cedula == cedulaArchivo) {
            // Crear nueva línea con saldo actualizado
            stringstream nuevaLinea;
            nuevaLinea << cedulaArchivo << "," << clave << "," << nuevoSaldo;
            string lineaNueva = nuevaLinea.str();
            string lineaNuevaEncriptada = encriptarClave(lineaNueva, 1);
            archivoTemporal << lineaNuevaEncriptada << endl;
        } else {
            archivoTemporal << lineaEncriptada << endl;
        }
    }

    archivo.close();
    archivoTemporal.close();

    // Reemplazar el archivo original con el temporal
    remove(archivoUsuarios.c_str());
    rename(archivoTemp.c_str(), archivoUsuarios.c_str());
}

// Función para obtener el saldo actual
double obtenerSaldo(const string& cedula) {
    const string archivoUsuarios = "usuarios.txt";
    ifstream archivo(archivoUsuarios);

    if (!archivo) {
        cout << "Error al abrir el archivo de usuarios." << endl;
        return -1;
    }

    string lineaEncriptada;
    while (getline(archivo, lineaEncriptada)) {
        string lineaDesencriptada = desencriptarClave(lineaEncriptada, 1);
        stringstream ss(lineaDesencriptada);
        string cedulaArchivo, clave, saldoStr;

        getline(ss, cedulaArchivo, ',');
        getline(ss, clave, ',');
        getline(ss, saldoStr, ',');

        if (cedula == cedulaArchivo) {
            archivo.close();
            return stod(saldoStr);
        }
    }

    archivo.close();
    return -1;
}

// Función modificada para mostrar el saldo
void mostrarSaldo(const string& cedula) {
    double saldoActual = obtenerSaldo(cedula);
    if (saldoActual >= 0) {
        // Cobrar comisión por consulta
        double nuevoSaldo = saldoActual - 1000;
        if (nuevoSaldo >= 0) {
            actualizarSaldo(cedula, nuevoSaldo);
            cout << "Saldo anterior: $" << saldoActual << " COP" << endl;
            cout << "Cargo por consulta: $1000 COP" << endl;
            cout << "Saldo actual: $" << nuevoSaldo << " COP" << endl;
        } else {
            cout << "Saldo insuficiente para realizar la consulta." << endl;
        }
    } else {
        cout << "Error: No se encontro la informacion del usuario." << endl;
    }
}

// Función para realizar retiro
void realizarRetiro(const string& cedula) {
    double saldoActual = obtenerSaldo(cedula);
    if (saldoActual < 0) {
        cout << "Error al obtener el saldo." << endl;
        return;
    }

    double montoRetiro;
    cout << "Ingrese el monto a retirar: $";
    cin >> montoRetiro;

    // Verificar si hay saldo suficiente (incluyendo la comisión)
    if (saldoActual >= (montoRetiro + 1000)) {
        double nuevoSaldo = saldoActual - montoRetiro - 1000;
        actualizarSaldo(cedula, nuevoSaldo);
        cout << "\nTransacción exitosa:" << endl;
        cout << "Saldo anterior: $" << saldoActual << " COP" << endl;
        cout << "Monto retirado: $" << montoRetiro << " COP" << endl;
        cout << "Cargo por transaccion: $1000 COP" << endl;
        cout << "Saldo actual: $" << nuevoSaldo << " COP" << endl;
    } else {
        cout << "Saldo insuficiente para realizar el retiro." << endl;
        cout << "Recuerde que cada transaccion tiene un costo de $1000 COP." << endl;
    }
}
