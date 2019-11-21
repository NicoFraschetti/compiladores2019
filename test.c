/* Contiene los casos de test para el compilador C-TDS
 *
 */
#include <stdio.h>
#include "libtest.c"



/* Retorna el "n-esimo" numero de la serie de Fibonacci. */
int fibonacci(int n)
{
    int i;
    int term1, term2;
    int nextTerm;
    
    term1 = 0;
    term2 = 1;
    i = 1;
    while (i < n)
    {
        
        nextTerm = term1 + term2;
        term1 = term2;
        term2 = nextTerm;
        i = i+1;
    }
    
    if (n == 0 || n < 0){
        return 0;
    }
    else{
        return term2;
    }
}

// retorna el factorial de v, con v hasta 15
int factorial (int v){
     int limit;
    limit = 15;
    if ( v > limit){
        return -1;
    }
    {//nuevo bloque
         int c;
         int fact;
        c = 0;
        fact = 1;
        while (c<v){
            c = c+1;
            fact = fact*c;
        }
        return fact;
    }
}


// retorna el n esimo primo
int nthprime (int n) {
     int i;
     int divs;
    i = 0;
    divs = 2;
    n = n +1;
    while ( n > 0) {
         int divides;
        divides = 0;
        i = i + 1;
        while ( !divides && divs < i) {
            if ( i % divs == 0)
            { divides = 1; }
            else
            { divs = divs + 1; }
        }
        divs = 2;
        if ( ! divides)
        { n = n - 1; }
    }
    return i;
}



// retorna el maximo comun divisor de a y b,
int gcd (int a, int b) {
	 int i;
	 int result;
	i=1;
	result=i;
	while ( i < (a+b)) {
		if (((a % i) == 0) &&  ((b%i) ==0 ))
        { result = i; }
		i = i + 1;
	}
	return result;
}

/* Calcula el valor del entero m elevado a la n*/
int potencia(int m, int n){
	 int i;
	 int acum;
     int breakVAR= 0;
	acum=1;
	i=1;
	while (!breakVAR){
		if(i<n || i==n){
			acum=acum * m;
			i=i+1;
		}
		else { breakVAR=1;}
	}
	return(acum);
}


/* Calcula el valor del real m elevado a la n*/
int potenciaI(int m, int n){
	int i;
	int acum;
	acum=1;
	i=1;
	while (i<n || i==n){
        acum=acum * m;
        i=i+1;
	}
	return(acum);
}

// retorna la representacion de un numero en binario

int int_to_bin(int number){
     int binary = 0;
     int counter = 0;
    while(number > 0){
         int remainder = number % 2;
        number = number / 2;
        binary = binary + potenciaI(10, counter) * remainder;
        counter= counter +1;
    }
    return binary;
}

// invoca varias funciones
int test(){
     int aux = 2;
    
    aux= gcd(factorial(3),factorial(4));
    aux = nthprime(gcd(factorial(3),aux));
    print_int(aux);

    return ( aux );
}

// invoca test
void test1(){
    float aux;
    
    aux = 1;
    print_int(potencia(nthprime(4),aux +1));
    print_int(test());
    return;
}

// funcion main
int main (){
    
    int x;
    int i = 0;
    
    
    init_input();
    
    // test factorial entero
    printString(2);
    x= get_int() ; // lee la cantidad de veces que ejecutara la funcion
    i = 0;
    while (i<x){
        int aux;
        aux= get_int(); // lee los datos para invocar a la funcion
        printStringFactorialValor(aux); // imprime: Factorial(aux)=
        aux = factorial(aux);
        print_int(aux);
        i = i + 1;
    }
    printString(1);
    
    
    
    // test PotenciaI entero
    printString(3);
    x= get_int() ; // lee la cantidad de veces que ejecutara la funcion
    
    i = 0;
    while (i<x){
        int aux1;
        int aux2;
        int res;

        aux1= get_int(); // lee los datos para invocar a la funcion
        aux2= get_int();
        printStringPotenciaIValor(aux1,aux2); // imprime: PotenciaI(aux1,aux2) =
        res = potenciaI(aux1, aux2);
        print_int(res);
        i = i + 1;
    }
    printString(1);
    
    
    // test Potencia entero
    printString(4);
    x= get_int() ; // lee la cantidad de veces que ejecutara la funcion
    
    i = 0;
    while (i<x){
        int aux1;
        int aux2;
        int res;
        aux1= get_int(); // lee los datos para invocar a la funcion
        aux2= get_int();
        printStringPotenciaValor(aux1,aux2); // imprime: Potencia(aux1,aux2) =
        res = potencia(aux1,aux2);
        print_int(res);
        i = i + 1;
    }
    printString(1);
    
    // test nthprime entero
    printString(5);
    
    x=get_int(); // lee la cantidad de veces que ejecutara la funcion
    i = 0;
    while (i<x){
        int aux;
        aux=get_int(); // lee los datos para invocar a la funcion
        printStringNthprimeValor(aux); // imprime: Nthprime(aux) =
        aux = nthprime(aux);
        print_int(aux);
        i =i + 1;
    }
    printString(1);
    
    
    // test int2bin entero
    printString(7);
    x = get_int(); // lee la cantidad de veces que ejecutara la funcion
    i = 0;
    while (i<x){
        int aux;
        aux=get_int(); // lee los datos para invocar a la funcion
        printStringInt2BinValor(aux); // imprime: Int2Bin(aux) =
        aux = int_to_bin(aux);
        print_int(aux);
        i = i+ 1;
    }
    printString(1);
    
    // test gcd entero
    printString(8);
    
    x= get_int(); // lee la cantidad de veces que ejecutara la funcion
    i = 0;
    while (i<x){
        int aux1;
        int aux2;
        int res;

        aux1=get_int(); // lee los datos para invocar a la funcion
        aux2=get_int(); // lee los datos para invocar a la función
        printStringGCDValor(aux1, aux2); // imprime: GCD(aux1,aux2)=
        res = gcd(aux1,aux2);
        print_int(res);
        i = i + 1;
    }
    printString(1);
    
    // test Fibonacci 
    printString(6);
    x= get_int(); // lee la cantidad de veces que ejecutara la funcion
    i = 0;
    while (i<x){
        int aux1;
        int res;
        aux1=get_int(); // lee los datos para invocar a la funcion
        printStringFibonacciValor(aux1); // imprime: Fibonacci(aux1)=
        res =fibonacci(aux1);
        print_int(res);
        i = i + 1;
    }
    printString(1);
    
    
    
    // test test
    printString(9);
    test();
    printString(1);
    
    
    // test test1
    printString(10);
    test1();
    printString(1);
    
    close_input();
    return 1;
    
    
} // endmain


