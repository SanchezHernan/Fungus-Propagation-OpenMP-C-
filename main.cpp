#include <omp.h>
#include <iostream>
#include <time.h>
#include <string.h>

using namespace std;

class Arbol {
    char estado;
    char edad;
    bool heridas;
    int semana;
    public:
        Arbol();
        Arbol(char, char, bool, int);
        char getEstado();
        char getEdad();
        bool getHeridas();
        int getSemana();
        void setEstado(char);
        void setEdad(char);
        void setSemana(int);
};

Arbol::Arbol () {
    estado= 'V';
    edad='J';
    heridas=false;
    semana=0;
};

Arbol::Arbol (char a, char b, bool c, int d){
    estado = a;
    edad = b;
    heridas = c;
    semana = d;
}

char Arbol::getEstado(){ return estado; }
char Arbol::getEdad(){ return edad; }
bool Arbol::getHeridas(){ return heridas; }
int Arbol::getSemana(){ return semana; }
void Arbol::setEstado(char a) { estado = a; }
void Arbol::setEdad(char a) { edad = a; }
void Arbol::setSemana(int a) { semana = a; }

bool calcularHeridas(char edadArbol){
    bool heridas = false;
    int porcentaje = rand() % 100;
    if (edadArbol == 'J'){
        if (porcentaje < 15) heridas = true;
    }
    else if (edadArbol == 'A'){
        if (porcentaje < 5) heridas = true;
    }
    else
        if (porcentaje < 30) heridas = true;
    return heridas;
};

Arbol *crearMatriz(int m, int n){
    Arbol *matriz = new Arbol[m*n];
    char estado;
    char edad;
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            int r = rand() % 1000;
            if (r < 500){
                estado = 'V';
            } else if (r >= 507){
                if (rand() % 2 == 0){
                    estado = 'B';
                } else
                    estado = 'A';
            } else if  (502 <= r and r < 507) { estado = 'N'; }
            else { estado = 'R'; }
            r = rand() % 100;
            if (r < 54) { edad = 'A'; }
            else if (r < 84) { edad = 'J'; }
            else { edad = 'V'; }
            bool heridas = calcularHeridas(edad);
            Arbol arbol = {estado, edad, heridas, 0};
            matriz[i*m+j] = arbol;
        }
    }
    return matriz;
}


float calcularProbContagio(int vecinosCS, Arbol arbol){
    int porcVCS = vecinosCS * 100 / 8;
    int suscept;
    if (arbol.getEdad() == 'J') { suscept = 30; }
    else if (arbol.getEdad() == 'A') { suscept = 20; }
    else suscept = 50;
    if (arbol.getHeridas()){
        suscept += 15;
    }
    return (porcVCS + suscept) * 0.6 + 5;
}


void printArray(Arbol* arr, int m, int n){
    for (int i=0; i<m; i++){
        for (int j=0; j<n; j++){
            printf("%c,", arr[i * n + j].getEstado());
        }
        printf("\n");
    }
};


int getPos(int f, int c, int m){
    return m*f+c;
}


int getVecinosInfectados(Arbol *matriz, int i, int j, int m, int n){
    int infectados = 0;
    Arbol vectorVecinos[8];
    int vectorVecinosLength;

    //creo el vectorVecinos segun la posicion de i y j
    if (i == 0){
        if (j == 0){
            vectorVecinos[0] = matriz[1];
            vectorVecinos[1] = matriz[getPos(1, 0, m)];
            vectorVecinos[2] = matriz[getPos(1, 1, m)];
            vectorVecinosLength = 3;
        }
        else if (j >= n-1){
            vectorVecinos[0] = matriz[getPos(0, j-1, m)];
            vectorVecinos[1] = matriz[getPos(1, j-1, m)];
            vectorVecinos[2] = matriz[getPos(1, j, m)];
            vectorVecinosLength = 3;
        }
        else{
            vectorVecinos[0] = matriz[getPos(0, j-1, m)];
            vectorVecinos[1] = matriz[getPos(0,j+1,m)];
            vectorVecinos[2] = matriz[getPos(1,j-1,m)];
            vectorVecinos[3] = matriz[getPos(i,j,m)];
            vectorVecinos[4] = matriz[getPos(1,j+1,m)];
            vectorVecinosLength = 5;
        }
    }
    else if (i == m-1){
        if (j == 0){
            vectorVecinos[0] = matriz[getPos(i-1,0,m)];
            vectorVecinos[1] = matriz[getPos(i-1,1,m)];
            vectorVecinos[2] = matriz[getPos(i,1,m)];
            vectorVecinosLength = 3;
        }
        else if (j >= n-1){
            vectorVecinos[0] = matriz[getPos(i-1,j-1,m)];
            vectorVecinos[1] = matriz[getPos(i-1,j,m)];
            vectorVecinos[2] = matriz[getPos(i,j-1,m)];
            vectorVecinosLength = 3;
        }
        else{
            vectorVecinos[0] = matriz[getPos(i-1,j-1,m)];
            vectorVecinos[1] = matriz[getPos(i-1,j,m)];
            vectorVecinos[2] = matriz[getPos(i-1,j+1,m)];
            vectorVecinos[3] = matriz[getPos(i,j-1,m)];
            vectorVecinos[4] = matriz[getPos(i,j+1,m)];
            vectorVecinosLength = 5;
        }
    }
    else if (j == 0){
        vectorVecinos[0] = matriz[getPos(i-1,j,m)];
        vectorVecinos[1] = matriz[getPos(i-1,j+1,m)];
        vectorVecinos[2] = matriz[getPos(i,j+1,m)];
        vectorVecinos[3] = matriz[getPos(i+1,j,m)];
        vectorVecinos[4] = matriz[getPos(i+1,j+1,m)];
        vectorVecinosLength = 5;
    }
    else if (j == n-1){
        vectorVecinos[0] = matriz[getPos(i-1,j-1,m)];
        vectorVecinos[1] = matriz[getPos(i-1,j,m)];
        vectorVecinos[2] = matriz[getPos(i,j-1,m)];
        vectorVecinos[3] = matriz[getPos(i+1,j-1,m)];
        vectorVecinos[4] = matriz[getPos(i+1,j,m)];
        vectorVecinosLength = 5;
    }
    else{
        vectorVecinos[0] = matriz[getPos(i-1,j-1,m)];
        vectorVecinos[1] = matriz[getPos(i-1,j,m)];
        vectorVecinos[2] = matriz[getPos(i-1,j+1,m)];
        vectorVecinos[3] = matriz[getPos(i,j-1,m)];
        vectorVecinos[4] = matriz[getPos(i,j+1,m)];
        vectorVecinos[5] = matriz[getPos(i+1,j-1,m)];
        vectorVecinos[6] = matriz[getPos(i+1,j,m)];
        vectorVecinos[7] = matriz[getPos(i+1,j+1,m)];
        vectorVecinosLength = 8;
    }

    //recorro el vectorVecinos y cuento los infectados entre los vecinos
    for (int i = 0; i < vectorVecinosLength; i++){
        if (vectorVecinos[i].getEstado() == 'R')
            infectados += 1;
    }
    return infectados;
}


Arbol arbolSano(Arbol arbol, int vecinosCS){
    Arbol newArbol = arbol;
    int probContagio = calcularProbContagio(vecinosCS, newArbol);
    int r = rand() % 100;
    if (r < probContagio){
        newArbol.setEstado('N');
        newArbol.setSemana(0);
    }
    return newArbol;
}


Arbol arbolConEsporas(Arbol arbol){
    Arbol newArbol = arbol;
    if (arbol.getSemana() >= 5)
        newArbol.setEstado('R');
    newArbol.setSemana(arbol.getSemana() + 1);
    return newArbol;
}


Arbol arbolConSintomas(Arbol arbol){
    Arbol newArbol = arbol;
    newArbol.setSemana(arbol.getSemana() + 1);
    if (rand() % 100 < 90)
        newArbol.setEstado('A');
    return newArbol;
}


Arbol arbolConTratamiento(Arbol arbol){
    //porc = { 'joven': 1, 'adulto': 10, 'viejo': 45 }
    Arbol newArbol = arbol;
    int r = rand() % 100;
    int porc;
    if (arbol.getEdad() == 'J')
        porc = 1;
    else if (arbol.getEdad() == 'A')
        porc = 10;
    else
        porc = 45;
    if (arbol.getSemana() < 8){
        if (r < porc){
            newArbol.setEstado('V');
            newArbol.setSemana(0);
        }
        else
            newArbol.setSemana(newArbol.getSemana() + 1);
    }
    else{
        if (arbol.getEdad() == 'J' or arbol.getEdad() == 'A')
            newArbol.setEstado('B');
        else{
            newArbol.setEstado('V');
            newArbol.setEdad('J');
        }
        newArbol.setSemana(0);
    }
    return newArbol;
}


Arbol arbolPodado(Arbol arbol){
    Arbol newArbol = arbol;
    newArbol.setSemana(arbol.getSemana() + 1);
    if (arbol.getSemana() == 7)
        newArbol.setEstado('V');
    return newArbol;
}



Arbol* cambiarEstado(Arbol *matriz, int fi, int fs, int m){
    Arbol newArbol;
    Arbol* nuevaMatriz = new Arbol[m * (fs - fi)];
    for (int i = fi; i < fs; i++){
        for (int j = 0; j <  m; j++){
            Arbol arbol = matriz[m*i+j];
            char estado = arbol.getEstado();
            if (estado == 'V'){
                int vecinosCS = getVecinosInfectados(matriz, i, j, m, (fs-fi));
                newArbol = arbolSano(arbol, vecinosCS);
            }
            else if (estado == 'N'){
                newArbol = arbolConEsporas(arbol);
            }
            else if (estado == 'R'){
                newArbol = arbolConSintomas(arbol);
            }
            else if (estado == 'A'){
                newArbol = arbolConTratamiento(arbol);
            }
            else{
                newArbol = arbolPodado(arbol);
            }
            nuevaMatriz[m*(i-fi)+j] = newArbol;
        }
    }
    //delete[] matriz; //para secuencial
    return nuevaMatriz;
}




int main()
{
    srand(time(NULL));
    int m = 1496;
    int tid, fi, fs;
    int di = m / 8; //8 = num threads
    Arbol *matriz = crearMatriz(m, m);
    Arbol *newMatriz;
    int porcentaje = rand() % 3900;
    printf("semanas: %d \n", porcentaje + 96);

    for (int vueltas = 0; vueltas < porcentaje + 96; vueltas++){
        #pragma omp parallel private(tid, fi, fs, newMatriz)
        {
            //printf("semanas: %d\n", omp_get_thread_num());
            tid = omp_get_thread_num();
            fi = tid * di;
            fs = (tid + 1) * di;
            newMatriz = cambiarEstado(matriz, fi, fs, m);
            for (int i = 0; i < di; i++){
                for (int j = 0; j < m; j++)
                    matriz[(i+tid*di)*m+j] = newMatriz[i*m+j];
            }
            delete [] newMatriz;
        }
    }
    /* Secuencial:
    Arbol *matriz = crearMatriz(m, m);
    int porcentaje = rand() % 600;
    printf("semanas: %d, casillas: %d \n", porcentaje + 96, m);
    for (int vueltas = 0; vueltas < porcentaje + 96; vueltas++){
        matriz = cambiarEstado(matriz, 0, m, m);
    }
    */
    printf("Finish");
    return 0;
}
