Comandos útiles:

Para compilar el programa corra el siguiente comando:
make IPC=* , donde * puede ser fifo, shmem, mqueue o sockets.

Para ejecutar el servidor corra el siguiente comando:
./serverGeneric &

Para ejecutar un solo cliente corra el siguiente comando:
./clientGeneric /path , donde path es la ruta del archivo a ejecutar.

Para ejecutar muchos clientes al mismo tiempo corra el siguiente comando:
./clients nro , donde nro es la cantidad de clientes a ejecutar.

Para finalizar el servidor corra el siguiente comando:
./kill

Para eliminar los ejecutables generados corra el siguiente comando:
make clean  

++IMPORTANTE++
La salida de la ejecucion se almacenara en la carpeta "exit", ubicada en el root de la aplicacion.
