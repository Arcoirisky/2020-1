# P1-SO-2020
Veamos si ahora les explico un poco lo que tienen, si ven que uso o o estoy usando funciones ya hechas para hacer otras cosas, no importa, es porque soy tonta. Funcionan y no hay que tocarlos, asi que no importa realmente. 

LES SUPLICO no me cambien lo que ya tengo ;--; porque pueden generar un caos en cadena...
# Variables globales

* extern char* diskName;
* extern Disco* diskMount;
* extern int BLOCK_SIZE;

# cr_API.h
Contiene las funciones generales, solo las generales pedidas por el enunciado.

## void cr_mount(char* diskname)
Asigna el nombre del disco que usaremos durante toda la ejecucion del programa en la variable externa diskName.
Si el nombre esta mal, te avisará y se acabará el programa. 
SOLO SE USA UNA VEZ y es al inicio del programa. ES DISTINTO A LA FUNCION QUE MANEJA LA ESTRUCTURA

## void cr_bitmap(unsigned disk, bool hex)
Básicamente es lo que dice el enunciado, te imprime el bitmap de una particion con disk entre 1..4, si disk es 0 te imprime el bitmap de todo el disco. 

si hex es true entonces imprime el bitmap en hexa, sino lo imprime en binario.

UTILIZA EL DISCO .bin no la estructura

## int cr_exists(unsigned int disk, char* filename)
Según un nombre y la particion, busca en la estructura montada si existe o no el archivo filename en la partición específicada con disk. 

disk debe ser un numero entre 1..4, sino te dira que estas mal y no hará nada.

## void cr_ls(unsigned disk)
Imprime los archivos en una particion asignada por disk entre 1..4

Si disk es igual a 0 entonces te imprime todo el disco

Tengo 1 funcion extra que me ayuda a imprimir el bitmap, me da paja explicarlas pero solo tienen uso aquí. Tampoco quiero que me la modifiquen asi que no la menciono.

# filesfunctions.h
## int cr_unload(unsigned int disk, char* orig, char* dest)
Descarga los archivos ubicados en el .bin a tu computador

si disk == 0, entonces descarga todo el disco TODO. no me calente la cabeza con carpetas, te descarga los archivos con los nombres antepuestos a su particion <particion>_<nombre archivo>

si orig == NULL, entonces confio (porque no he manejado errores) de que disk estara entre 1..4. Descarga todos los archivos de una particion.

caso contrario entonces descargo el archivo con nombre indicado por orig ubicado en el disco disk, si es que es posible descargarlo. 

Tengo funciones extras que me ayudan a descargar un archivo, me da paja explicarlas pero solo tienen uso aquí. Tampoco quiero que me la modifiquen asi que no la menciono.

# funcionesStructs.h
## int tamanio_File(crFILE* archivo)
Me retorna el tamaño de un archivo

## int cantidad_refArchivo(crFILE* archivo)
Me retorna la cantidad de referencias de un archivo

## int particion_Archivo(crFILE* archivo)
Me retorna la particion de un archivo, OJO que esto existe por culpa de los softlinks. En el caso de 1/Baroque.mp3 ubicado en la particion 4, te dirá que la partición es 1 porque EL LINK NO ESTA ROTO. 

Si quieren ver la particion de un archivo deben verlo en la estructura.

## int size_Archivo2(unsigned char* bytes, int inicio, int leer)
TE ENTREGA EL VALOR BINARIO DE LO QUE LEES. Es ideal para leer los punteros que son 1 número compuesto de 4 bytes.

El nombre es confuso, pero lee una cantidad "leer" de bytes desde un buff "bytes" desde "inicio". 

## int punteroIndirectoSimple(crFILE* archivo)
Te saca el puntero de indireccion simple de un archivo. En general existe indireccion simple si esta funcioncion retorna distinto a cero. 

## int pos_indice_block(unsigned char* Directorio)
Te retorna el puntero del bloque indice del BLOQUE DIRECTORIO, Directorio es un buff cargado con el bloque directorio del .bin

Lo uso en una funcion externa, por si solo no hace mucho si no saben sacar el bloque directorio. No lo tengo guardado en ninguna parte porque me parecio innecesario.

## void decifrar_infoFiles(Disco* disco)
Esta funcion la uso al momento de montar si no me equivoco. 
Lo que hace es reconocer el tipo de archivo (SoftLink, HardLink, NormalFile), digo si el link esta conectado, etc.

Básicamente rellena la informacion restante para los archivos al momento de montar mi estructura guardada en DiskMount. 

## void leer_bloqueDireccion(crFILE* archivo)
Funcion que me sirvió para entender como funciona el bloqueDireccion de un archivo. No recuerdo si la use en otra parte. 

## bool string_equals(char* string1, char* string2)
Compara string1 y string2, retorna True si son iguales, false caso contrario. 

## bool softLinkRec(crFILE* archivo)
Reconoce si un archivo es un softLink o no

# leerArchivo.h

## crFILE* cr_open(unsigned disk, char* filename, char mode)
Hasta donde la deje, solo funciona en readMode. No sobrepensé más allá de eso porque lo que queria era leer los archivos del .bin

## int cr_read(crFILE* file_desc, void* buffer, int nbytes)
leo los nbytes de un archivo y lo guardo en el buffer. Si me quedo sin bytes que leer entonces no leo nada más. 

## int cr_close(crFILE* file_desc)
Cierro los archivos abiertos. Limpio las estructuras que uso para la lectura de los archivos. 

## crFILE* obtenerFile(unsigned disk, char* filename)
A partir de un filename y un disk entre 1..4 retorno el crFile correspondiente.

## void leer_punterosDireccion(crFILE* archivo, bool indirectoSimple)
cargo en mi estructura creada SOLO al momento de abrir un archivo los punteros a los bloques de datos.
Si necesitan una funcion que te retorne los punteros a otro lado, pueden hacer su propia versión de esta funcion pero esto solo funciona al momento de abrir un archivo.

## void cargar_bloqueSig(crFILE* archivo)
Esto lo uso para la lectura. Carga el bloque de datos para leerlo directamente desde mi estructura. 
Tienen que tener cuidado cuando lo llaman porque solo lo uso para la lectura. Insisto en que solo lo uso para la LECTURA. porque en caso de que carguen el bloque siguiente sin un archivo abierto en modo lectura, no funcionara. 

Si cargan el siguiente bloque y luego leen, entonces leeran un bloque menos.

LEAN EL CODIGO y si necesitan algo que tiene otras funciones, hagan las propias y que solo funcione en sus funciones.
Para alivianar el choque de codigo.

#  structs.h

## crFILE* montar_archivo(char* filename, int dir_BloqueIndice)
crea una estructura crFILE a partir de un filename y el puntero del bloqueIndice

## Disco* montar_disco()
Monto mi super estructura guardada en diskMount basada en el .bin

## void imprimir_disco(Disco* disk)
Imprimo mi disco completo segun la estructura

## void desmontar_disco(Disco* disk)
Libero la memoria pedida al montar_disco()
