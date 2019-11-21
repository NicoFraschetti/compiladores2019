/* Funciones auxiliares para realizar el test del compilador C-TDS.
 * Permite leer los datos de entrada e imprimir la salida para cada funcion a ejecutar. 
 * Asume que los datos de entrada se encuentran en el archivo "input".
 * El archivo input contiene una linea por cada funcion que se invoca (en el orden que se invocan). 
 * Cada linea es una lista de numeros (enteros y/o reales). 
 * El primer numero indica la cantidad de ejecuciones de cada funcion seguido de los parametros que se 
 * necesitan para la invocación.
 * */


#include <stdio.h>

FILE *fp;  
char buffer[100];

// abre el archivo con los datos de entrada
int init_input(){
	fp = fopen ( "input", "r" );
    return 1;
}

// cierra el archivo
int close_input(){
	fclose(fp);
    return 1;
}

// lee un dato entero del archivo de entrada
int get_int(){
	int x;	
   	fscanf(fp, "%s" ,buffer); 
    sscanf(buffer, "%d", &x);
    return x;
}

// imprime un entero en la salida
int print_int(int x){
       printf("%d\n",x);
       return x;	
	}

// lee un dato real del archivo de entrada
float get_float(){
	float x;	
   	fscanf(fp, "%s" ,buffer);
    sscanf(buffer, "%f", &x);
    return x;
}

// imprime un real en la salida
void print_float(float x){
       printf("%f\n",x);	
	}
	
// imprime un string en la salida	
int printString(int op){
	    
        if (op==1) 	printf("%s\n\n","---------------------------------------------------------");

        if (op==2) 	printf("\n%s\n\n","Factorial ----------------------------------");
        
        if (op==3) 	printf("\n%s\n\n","PotenciaI----------------------------------");
        	        
        if (op==4) 	printf("\n%s\n\n","Potencia ----------------------------------");
	
        if (op==5) 	printf("\n%s\n\n","Nthprime ----------------------------------");

        if (op==6) 	printf("\n%s\n\n","Fibonacci ---------------------------------");        	
	
        if (op==7) 	printf("\n%s\n\n","Int2Bin ----------------------------------");
	
        if (op==8) 	printf("\n%s\n\n","GCD ----------------------------------");

        if (op==9) 	printf("\n%s\n\n","test----------------------------------");        	
	
        if (op==10) 	printf("\n%s\n\n","test1----------------------------------");
	
    return 0;

	}


int printStringFactorialValor(int x){
    printf("\n%s%d%s","Factorial(",x, ")= ");
    return 0;
}

int printStringNthprimeValor(int x){
    printf("\n%s%d%s","Nthprime(",x, ")= ");
    return 0;
}

int printStringFibonacciValor(int x){
    printf("\n%s%d%s","Fibonacci(",x, ")= ");
    return 0;
}


int printStringInt2BinValor(int x){
    printf("\n%s%d%s","Int2Bin(",x, ")= ");
    return 0;
}

int printStringGCDValor(int x, int y){
    printf("\n%s%d%s%d%s","GCD(",x, ",", y, ")= ");
    return 0;
}
int printStringPotenciaValor(int x, int y){
    printf("\n%s%d%s%d%s","Potencia(",x, ",", y, ")= ");
    return 0;
}

int printStringPotenciaIValor(int x, int y){
    printf("\n%s%d%s%d%s","PotenciaI(",x, ",", y, ")= ");
    return 0;
}

