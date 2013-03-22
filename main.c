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

#include<dirent.h>
#include<errno.h>
#include <time.h>



int main(int argc, char **argv) {
  
  DIR *dirp;
  struct dirent *direntp;
  struct stat statbuf;
  
  char *NombreDirectorio = NULL;
  
  int Tam = -1; /*variable que contendra el size de argv[1]*/
  int i = 0; /*Contador multipropositos*/
  int Tam1 = -1; /*variable que contendra el size de NombreDirectorio*/
  
  if (argc < 2 || argc > 2) {
	 printf("\nError en la linea de argumentos\n\n");
	 exit(1);
  }
  
  Tam = strlen(argv[1]);
  
  Tam1 = Tam + 1;
  
  NombreDirectorio = (char *) malloc(Tam1 * sizeof(char));
  
  /*Validamos la llamada*/
  if (NombreDirectorio == NULL) {
	 printf("\nError\n\n");
	 exit(1);
  }
  
  /*Inicializamod el arreglo*/
  for (i = 0; i != Tam1; i++) {
	 NombreDirectorio[i] = '\0';
  }
  
  /*Copiamos los n elementos de argv[1] en NombreDirectorio*/
  strncpy(NombreDirectorio, argv[1], Tam);
  
  
  /*se abre el directorio*/
  if ((dirp = opendir(argv[1])) == NULL) {
	 fprintf(stderr, "No se pudo abrir el directorio %s: %s", argv[1], strerror(errno));
	 exit(1);
  }
  
  /*se lee el contenido del directorio*/
  while ((direntp = readdir (dirp)) != NULL){
	 
	 
	 if (stat(direntp->d_name, &statbuf) == 1) {
		fprintf(stderr, " No se pudo aplicar stat sobre el archivo %s: %s \n", direntp->d_name, strerror(errno));
		exit(1);
	 }
	 if (S_ISDIR(statbuf.st_mode)){
		printf("%s es un directorio\n",direntp->d_name);
	 }
	 else{
		printf("%s no es un directorio\n", direntp->d_name);
	 }
  }
  
closedir(dirp);



/*
 *	printf("Tamano = %d\n", strlen(NombreDirectorio));
 *	printf("%s\n", NombreDirectorio);
 */
return 0;
}
