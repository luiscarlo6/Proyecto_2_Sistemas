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

char *NombreDirectorio = NULL;
char NombreDirectorioActual[MAX];

int Es_Directorio(char* Nombre);

void Procesar_Directorio(char* Nombre);

int NumHijos=0;




int main(int argc, char **argv) {
  
  static int i=0;
  
  if (argc < 2 || argc > 2) {
	 printf("\nError en la linea de argumentos\n\n");
	 exit(1);
  }
  
  if(!Es_Directorio(argv[1])){
	 printf("El argumento %s\tNO ES UN DIRECTORIO",argv[1]);
	 exit(1);
  }
  
  Procesar_Directorio(argv[1]);
  
  for(i=0; i!=NumHijos; i++){
	 wait();
  }
  
  printf("SOY EL MAIN \t%s\n",NombreDirectorio);
  
  free(NombreDirectorio);
  
  return 0;
}



int Es_Directorio(char* Nombre){
  
  struct stat statbuf;
  /**/
  if (lstat(Nombre, &statbuf) == 1) {
	 fprintf(stderr, " No se pudo aplicar stat sobre el archivo %s: %s \n", Nombre, strerror(errno));
	 exit(1);
  }
  
  /*verifico si es directorio*/
  if ( statbuf.st_mode & S_IFDIR){
	 printf("%s\t es un directorio\n",Nombre);
	 return 1;
  }
  else{
	 printf("%s\t no es un directorio\n",Nombre);
	 return 0;
  }
  
  return -1;
  
}


void Procesar_Directorio(char* Nombre){
  
  DIR *dirp;
  struct dirent *direntp;
  
  //char *NombreDirectorio = NULL;
  
  
  int Tam = -1; /*variable que contendra el size de argv[1]*/
  int i = 0; /*Contador multipropositos*/
  int Tam1 = -1; /*variable que contendra el size de NombreDirectorio*/
  
  Tam = strlen(Nombre);
  
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
  strncpy(NombreDirectorio, Nombre, Tam);
  
  printf("HOLA SOY %s\t%d\n",NombreDirectorio,getppid());
  
  
  /*se abre el directorio*/
  if ((dirp = opendir(NombreDirectorio)) == NULL) {
	 fprintf(stderr, "No se pudo abrir el directorio %s: %s", NombreDirectorio, strerror(errno));
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
		
		if(Es_Directorio(NombreDirectorioActual)){
		  
		  if(fork() == 0 ){
			 
			 Procesar_Directorio(NombreDirectorioActual);
			 
			 for(i=0;i!=NumHijos;i++){
				wait();
			 }
			 
			 printf("SOY \t%s\n",NombreDirectorio);
			 free(NombreDirectorio);
			 exit(0);
			 
		  }else{
			 
			 NumHijos++;
			 
		  }
		}
		
	 }/*fin de if*/
  }/*fin del for*/
  
  closedir(dirp);
}
