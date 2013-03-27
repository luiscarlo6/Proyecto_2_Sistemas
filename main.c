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
#define TP 2
#define READ 0
#define WRITE 1

char *NombreDirectorio = NULL;
char NombreDirectorioActual[MAX];

int Es_Directorio(char* Nombre);

void Procesar_Directorio(char* Nombre);

int NumHijos = 0;

int fleer = -1;
int fescribir = -1;

struct Pipe {
	int Read[TP];
	int Write[TP];
};

typedef struct Pipe PIPE;

PIPE *Array = NULL;
int SIZE = 1;

int main(int argc, char **argv) {

	static int i = 0;

	if (argc < 2 || argc > 2) {
		printf("\nError en la linea de argumentos\n\n");
		exit(1);
	}

	if (!Es_Directorio(argv[1])) {
		printf("El argumento %s\tNO ES UN DIRECTORIO", argv[1]);
		exit(1);
	}

	//   Array = (PIPE *)malloc(SIZE * sizeof(PIPE));
	//   
	//   if(Array == NULL){
	// 	 printf("ERROR");
	// 	 exit(1);
	//   }

	//printf("\n");
	//printf("\n%s\n",argv[1]);

	Procesar_Directorio(argv[1]);

	for (i = 0; i != NumHijos; i++) {
		wait();
	}

	printf("\nSOY EL MAIN \t%s\n", NombreDirectorio);

	free(NombreDirectorio);

	return 0;
}

int Es_Directorio(char* Nombre) {

	struct stat statbuf;

	if (lstat(Nombre, &statbuf) == 1) {
		fprintf(stderr, " No se pudo aplicar stat sobre el archivo %s: %s \n",
				Nombre, strerror(errno));
		exit(1);
	}

	/*verifico si es directorio*/
	if (statbuf.st_mode & S_IFDIR) {
		//printf("%s\t es un directorio\t con %d elementos\n", Nombre, statbuf.st_nlink - 2);
		//printf("%d\t Contenido\n",statbuf.st_nlink);
		return 1;
	} else {
		//printf("%s\t no es un directorio\n",Nombre);
		return 0;
	}

	return -1;

}

int Numeros_Sub_Directorio(char* Nombre) {

	struct stat statbuf;

	if (lstat(Nombre, &statbuf) == 1) {
		fprintf(stderr, " No se pudo aplicar stat sobre el archivo %s: %s \n",
				Nombre, strerror(errno));
		exit(1);
	}

	return (statbuf.st_nlink - 2);
}

void Procesar_Directorio(char* Nombre) {

	DIR *dirp;
	struct dirent *direntp;
	int K = 0;
	int Tam = -1; /*variable que contendra el size de argv[1]*/
	int i = 0; /*Contador multipropositos*/
	int Tam1 = -1; /*variable que contendra el size de NombreDirectorio*/
	int Signal = 1;

	int readbytes;
	char buffer[512];

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

	//printf("HOLA SOY %s\t%d\n",NombreDirectorio,getppid());

	/*se abre el directorio*/
	if ((dirp = opendir(NombreDirectorio)) == NULL) {
		fprintf(stderr, "No se pudo abrir el directorio %s: %s",
				NombreDirectorio, strerror(errno));
		exit(1);
	}

	if ((Signal == 1) & (Signal)) {
		Signal = 0;

		int NumeroDePipe = Numeros_Sub_Directorio(NombreDirectorio);

		if (NumeroDePipe > 0) {

			Array = (PIPE *) malloc(NumeroDePipe * sizeof(PIPE));

		} else {
			Array = (PIPE *) malloc(1 * sizeof(PIPE));

		}

		if (Array == NULL) {
			printf("\nError\n\n");
			exit(1);
		}
	}

	/*se lee el contenido del directorio*/
	for (direntp = readdir(dirp), K = 0; direntp != NULL;
			direntp = readdir(dirp)) {

		/*Para no caer en ciclo infinito se descartan los directorios "." y ".."*/
		if (strcmp(direntp->d_name, ".") != 0
				&& strcmp(direntp->d_name, "..") != 0) {

			/*inicializo la variable en un string vacio*/
			for (i = 0; i != MAX; i++) {
				NombreDirectorioActual[i] = '\0';
			}

			/*coloco el directorio padre*/
			strcpy(NombreDirectorioActual, NombreDirectorio);
			/*añado la direccion del actual*/
			strcat(NombreDirectorioActual, "/");
			strcat(NombreDirectorioActual, direntp->d_name);

			if (Es_Directorio(NombreDirectorioActual)) {

				printf("\nSOY \t%s\n", NombreDirectorioActual);
				pipe(Array[K].Read);
				pipe(Array[K].Write);
				NumHijos++;
				/*si soy hijo*/
				if (fork() == 0) {
					close(Array[NumHijos - 1].Read[WRITE]);	//cerramos el lado de escritura
					close(Array[NumHijos - 1].Write[READ]);	//cerramos el lado de lectura

					//sleep(5);
					fleer = Array[NumHijos - 1].Read[READ];
					fescribir = Array[NumHijos - 1].Write[WRITE];
					char TEMP[512];
					char Aux[20];

					strcpy(TEMP, "HOLA SOY TU HIJO JEJEJ  ");
					sprintf(Aux, "(%d ,%d)", getppid(), getpid());
					strcat(TEMP, Aux);

					write(fescribir, TEMP, 100);

					read(fleer, buffer, 100);
					//printf("SOY \t%s\n",NombreDirectorioActual);
					//printf("\t\tMI mensaje es %s\ty mi padre es:%d\n\n",buffer,getppid());

					free(Array);
					Array = NULL;
					NumHijos = 0;

					Procesar_Directorio(NombreDirectorioActual);

					for (i = 0; i != NumHijos; i++) {
						wait();
					}

					free(NombreDirectorio);
					exit(0);

				} else {
					K++;

					close(Array[K - 1].Read[READ]);	//cerramos el lado de escritura
					close(Array[K - 1].Write[WRITE]);//cerramos el lado de lectura

					char AUX[512];
					for (i = 0; i != 512; i++) {
						buffer[i] = '\0';
						AUX[i] = '\0';
					}
					sprintf(AUX, "%d", NumHijos);

					strcat(buffer, "HOLA HIJO MIO ");

					strcat(buffer, AUX);
					//while( (readbytes=read( b[0], buffer, SIZE )) > 0)
					//printf("VOY A ENVIARLE A MI HIJO %d, \"%s\"\n",K,buffer);
					write(Array[K - 1].Read[WRITE], buffer, 100);
					read(Array[K - 1].Write[READ], buffer, 100);
					//printf("MENSAJE DE MI HIJO\t\"%s\"\n",buffer);
					wait();
				}
			}
		}/*fin de if*/
	}/*fin del for*/

	closedir(dirp);
}
