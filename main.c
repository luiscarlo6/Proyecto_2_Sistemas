/**
 *	@file: main
 * @
 *	@ 
 *
 */

/**Librerias a incluir*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h> 
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


int main(int argc, char **argv)
{
  
  char *NombreDirectorio = NULL;
  
  int Tam = -1;	/*variable que contendra el size de argv[1]*/
  int i = 0;	/*Contador multipropositos*/
  int Tam1 = -1;	/*variable que contendra el size de NombreDirectorio*/
  
  Tam = strlen(argv[1]);
  
  Tam1 = Tam + 1;
  
  NombreDirectorio = (char *) malloc(Tam1 * sizeof(char));
  
  /*Validamos la llamada*/
  if (NombreDirectorio==NULL){
	 printf("\nError\n\n");
	 exit(1);
  }
  
  /*Inicializamod el arreglo*/
  for(i=0; i!=Tam1; i++){
	 NombreDirectorio[i]='\0';
  }
  
  /*Copiamos los n elementos de argv[1] en NombreDirectorio*/
  strncpy(NombreDirectorio,argv[1],Tam);
  
  
  printf("Tamano = %d\n", strlen(NombreDirectorio));
  printf("%s\n",NombreDirectorio);
  
  
  return 0;
}
