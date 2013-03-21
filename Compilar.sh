#!/bin/sh
#programa que limpia el directorio de archivos terminales
#compila los archivos, cambia los permisos de ejecucion
rm -rf *~
echo rm -rf *~
chmod -R 400 *.c *.h *.o makefile
echo chmod -R 400 *.c *.h *.o makefile
make
echo make
chmod -R 700 Main
echo chmod -R 700 Main

