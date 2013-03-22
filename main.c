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
#include<time.h>
#include<signal.h>

#include <sys/dir.h>
#include <sys/syscall.h>
#include <assert.h>

#define MAX 1000

int main(int argc, char **argv) {
  
  DIR *dirp;
  struct dirent *direntp;
  struct stat statbuf;
  
  char *NombreDirectorio = NULL;
  char NombreDirectorioActual[MAX];
  
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
  for( direntp = readdir (dirp); direntp != NULL; direntp = readdir (dirp)){	 
	 
	 /*Para no caer en ciclo infinito se descartan los directorios "." y ".."*/
	 if(strcmp (direntp->d_name, ".") != 0 &&  strcmp (direntp->d_name, "..") != 0){
		
		/*inicializo la variable en un string vacio*/
		 for (i = 0; i != MAX; i++) {
			 NombreDirectorioActual[i] = '\0';
		 }
		
		/*coloco el directorio padre*/
		strcpy(NombreDirectorioActual, NombreDirectorio);
		/*añado la direccion del actual*/
		strcat(NombreDirectorioActual, "/");
		strcat(NombreDirectorioActual, direntp->d_name);
		
		/**/
		if (lstat(NombreDirectorioActual, &statbuf) == 1) {
		  fprintf(stderr, " No se pudo aplicar stat sobre el archivo %s: %s \n", direntp->d_name, strerror(errno));
		  exit(1);
		}
		
		/*verifico si es directorio*/
		if ( statbuf.st_mode & S_IFDIR){
		  printf("%s es un directorio\n",direntp->d_name);
		}
		else{
		  printf("%s no es un directorio\n",direntp->d_name);
		}
		
	 }/*fin de if*/
  }/*fin del for*/
  
  closedir(dirp);

  return 0;
}
