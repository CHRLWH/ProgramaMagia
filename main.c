#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiciones de constantes para tamanos maximos y nombre del archivo
#define MAX_NOMBRE 50      // Tamano maximo para el nombre del mago
#define MAX_HECHIZO 30     // Tamano maximo para el hechizo favorito
#define ARCHIVO_MAGICO "magos.bin"  // Nombre del archivo binario donde se guardara la lista

// Definicion de la estructura Mago, que contiene los datos de un mago
typedef struct {
    char nombre[MAX_NOMBRE];          // Cadena de caracteres para el nombre
    int nivelMagico;                  // Entero que representa el nivel magico (debe estar entre 1 y 100)
    char hechizoFavorito[MAX_HECHIZO];  // Cadena de caracteres para el hechizo favorito
} Mago;


/*
    Funcion leerEntero:
    --------------------
    Esta funcion solicita al usuario que ingrese un numero entero, mostrando el mensaje recibido
    como argumento. Se valida que el valor ingresado sea un entero y que este se encuentre dentro del rango [min, max].
    Si el usuario ingresa un dato invalido (por ejemplo, una letra) o un numero fuera del rango, se muestra
    un mensaje de error y se solicita nuevamente la entrada.
*/
int leerEntero(const char* mensaje, int min, int max) {
    int valor;      // Variable donde se almacenara el numero ingresado
    int result;     // Variable para almacenar el resultado de scanf
    do {
        // Muestra el mensaje de solicitud
        printf("%s", mensaje);
        result = scanf("%d", &valor);
        // Limpiar el buffer de entrada hasta encontrar un salto de linea
        while (getchar() != '\n')

        // Si el valor ingresado no es un numero o esta fuera del rango, se muestra un mensaje de error
        if (result != 1 || valor < min || valor > max) {
            printf("Error: Debes ingresar un numero entre %d y %d.\n", min, max);
            result = 0; // Se establece result en 0 para continuar el ciclo
        }
    } while (result != 1);
    return valor;   // Retorna el valor valido ingresado por el usuario
}


/*
    Funcion crearMago:
    -------------------
    Esta funcion permite al usuario ingresar los datos de un mago (nombre, nivel magico y hechizo favorito).
    Se valida que las cadenas ingresadas no esten vacias y que el nivel magico este entre 1 y 100.
    Retorna una estructura Mago con los datos ingresados.
*/
Mago crearMago() {
    Mago nuevoMago;  // Variable de tipo Mago para almacenar los datos ingresados

    // Solicita el nombre del mago y se asegura de que no este vacio
    do {
        printf("\nIngrese el nombre del mago: ");
        scanf(" %[^\n]", nuevoMago.nombre);
    } while (strlen(nuevoMago.nombre) == 0);

    // Solicita el nivel magico utilizando la funcion leerEntero para validar la entrada
    nuevoMago.nivelMagico = leerEntero("Ingrese el nivel magico (1-100): ", 1, 100);

    // Solicita el hechizo favorito y se asegura de que no este vacio
    do {
        printf("Ingrese el hechizo favorito: ");
        scanf(" %[^\n]", nuevoMago.hechizoFavorito);
    } while (strlen(nuevoMago.hechizoFavorito) == 0);

    return nuevoMago;  // Retorna el mago creado
}


/*
    Funcion imprimirMago:
    ----------------------
    Esta funcion recibe una estructura Mago y muestra en pantalla sus datos
    (nombre, nivel magico y hechizo favorito).
*/
void imprimirMago(Mago mago) {
    printf("\nNombre: %s\n", mago.nombre);
    printf("Nivel Magico: %d\n", mago.nivelMagico);
    printf("Hechizo Favorito: %s\n", mago.hechizoFavorito);
}


/*
    Funcion guardarEnArchivoMagico:
    --------------------------------
    Esta funcion guarda en un archivo binario la lista de magos.
    Se escribe primero el numero de magos (para conocer la cantidad al leer el archivo)
    y luego se escriben los datos de cada mago en formato binario.
    Si el archivo no se puede abrir, se muestra un mensaje de error.
*/
Mago *guardarEnArchivoMagico(Mago *magos, int numMagos) {
    FILE* archivo = fopen(ARCHIVO_MAGICO, "wb");  // Se abre el archivo en modo escritura binaria
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo para guardar.\n");

    }else {
        // Se escribe el numero de magos al inicio del archivo
        fwrite(&numMagos, sizeof(int), 1, archivo);
        // Se escriben los datos de la lista de magos
        fwrite(magos, sizeof(Mago), numMagos, archivo);
        fclose(archivo);  // Se cierra el archivo

        printf("\nLista de magos guardada correctamente en %s.\n", ARCHIVO_MAGICO);
        return magos;
    }
}

/*
    Funcion leerDesdeArchivoMagico:
    ---------------------------------
    Esta funcion abre el archivo binario que contiene la lista de magos.
    Primero lee el numero de magos y luego reserva memoria dinamica para cargar la lista.
    Se leen los datos de cada mago y se retorna un puntero al arreglo de magos.
    Se incluye control de errores en caso de que el archivo no exista o la lectura falle.
*/
Mago* leerDesdeArchivoMagico(int* numMagos) {
    FILE* archivo = fopen(ARCHIVO_MAGICO, "rb");  // Se abre el archivo en modo lectura binaria
    if (!archivo) {
        printf("\nNo se encontro el archivo \"%s\". Asegurate de haber guardado la lista antes.\n", ARCHIVO_MAGICO);
        return NULL;
    }

    // Se lee el numero de magos almacenado en el archivo
    if (fread(numMagos, sizeof(int), 1, archivo) != 1) {
        printf("\nError al leer el numero de magos.\n");
        fclose(archivo);
        return NULL;
    }

    // Se reserva memoria dinamica para almacenar la lista de magos
    Mago* magos = (Mago*)malloc((*numMagos) * sizeof(Mago));
    if (!magos) {
        printf("\nNo se pudo asignar memoria.\n");
        fclose(archivo);
        return NULL;
    }

    // Se leen los datos de los magos
    if (fread(magos, sizeof(Mago), *numMagos, archivo) != *numMagos) {
        printf("\nError al leer los datos de los magos.\n");
        free(magos);
        fclose(archivo);
        return NULL;
    }

    fclose(archivo);  // Se cierra el archivo

    // Se imprimen los datos de cada mago leido del archivo
    printf("\nLista de magos cargada exitosamente desde \"%s\":\n", ARCHIVO_MAGICO);
    for (int i = 0; i < *numMagos; i++) {
        printf("\nMago %d:\n", i + 1);
        printf("Nombre: %s\n", magos[i].nombre);
        printf("Nivel Magico: %d\n", magos[i].nivelMagico);
        printf("Hechizo Favorito: %s\n", magos[i].hechizoFavorito);
    }

    return magos;
}

/*
    Funcion modificarNivelMagico:
    ------------------------------
    Esta funcion permite modificar el nivel magico de un mago existente en la lista.
    Primero muestra la lista de magos disponibles (con su indice) y solicita al usuario
    que seleccione el numero del mago a modificar. Luego, se pide el nuevo nivel magico,
    validando que se encuentre en el rango permitido. Si el nuevo nivel es menor que el actual,
    se muestra una advertencia.
*/
void modificarNivelMagico(Mago* magos, int numMagos) {
    int indice, nuevoNivel;

    // Verifica que exista al menos un mago registrado
    if (numMagos == 0) {
        printf("\nNo hay magos registrados.\n");
        return;
    }

    // Muestra la lista de magos disponibles con su indice
    printf("\nMagos disponibles:\n");
    for (int i = 0; i < numMagos; i++) {
        printf("%d. %s (Nivel: %d)\n", i + 1, magos[i].nombre, magos[i].nivelMagico);
    }

    // Se solicita el indice del mago a modificar y se valida que este en rango
    do {
        printf("\nSeleccione el numero del mago a modificar (1-%d): ", numMagos);
        scanf("%d", &indice);
        // Se limpia el buffer de entrada
        while (getchar() != '\n');

        if (indice < 1 || indice > numMagos) {
            printf("Error: Numero invalido. Intente nuevamente.\n");
        }
    } while (indice < 1 || indice > numMagos);

    // Se solicita el nuevo nivel magico utilizando la funcion leerEntero
    nuevoNivel = leerEntero("Ingrese el nuevo nivel magico (1-100): ", 1, 100);

    // Si el nuevo nivel es menor que el nivel actual, se muestra una advertencia
    if (nuevoNivel < magos[indice - 1].nivelMagico) {
        printf("Advertencia: ¡El nivel magico ha disminuido!\n");
    }

    // Se actualiza el nivel magico del mago seleccionado
    magos[indice - 1].nivelMagico = nuevoNivel;
    printf("\nNivel magico actualizado.\n");
}


/*
    Funcion ordenarMagos:
    -----------------------
    Esta funcion ordena el arreglo de magos en orden descendente de acuerdo
    al nivel magico utilizando Bubble sort.
*/
void ordenarMagos(Mago* magos, int numMagos) {
    for (int i = 0; i < numMagos - 1; i++) {
        for (int j = 0; j < numMagos - i - 1; j++) {
            // Si el mago actual tiene un nivel menor que el siguiente, se intercambian
            if (magos[j].nivelMagico < magos[j + 1].nivelMagico) {
                Mago temp = magos[j];
                magos[j] = magos[j + 1];
                magos[j + 1] = temp;
            }
        }
    }
}


/*
    Funcion main:
    --------------
    Esta es la funcion principal del programa, que implementa un menu interactivo
    con las siguientes opciones:
      1. Registrar magos manualmente.
      2. Modificar niveles magicos.
      3. Guardar lista en archivo.
      4. Leer magos desde archivo.
      5. Mostrar magos ordenados (de mayor a menor).
      6. Salir del programa.

    Se validan las entradas y se gestionan correctamente los recursos de memoria.
*/
int main() {
    int opcion, numMagos = 0;
    Mago* magos = NULL;  // Inicialmente el puntero a la lista de magos es NULL

    // Bucle infinito para mostrar el menu hasta que el usuario decida salir
    while (1) {
        // Se muestra el menu interactivo
        printf("\nMENU MAGICO\n");
        printf("1. Registrar magos manualmente\n");
        printf("2. Modificar niveles magicos\n");
        printf("3. Guardar lista en archivo\n");
        printf("4. Leer magos desde archivo\n");
        printf("5. Mostrar magos ordenados\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        // Se limpia el buffer de entrada
        while (getchar() != '\n');

        // Se procesa la opcion seleccionada
        switch (opcion) {
            case 1:
                // Si ya hay una lista creada, se libera la memoria para evitar fugas
                if (magos) {
                    free(magos);
                }
                printf("\nIngrese el numero de magos a registrar: ");
                // Se utiliza leerEntero para validar que se ingrese un numero entre 1 y 100
                numMagos = leerEntero("", 1, 100);
                // Se reserva memoria dinamica para la lista de magos
                magos = (Mago*)malloc(numMagos * sizeof(Mago));
                if (!magos) {
                    printf("Error: No se pudo asignar memoria.\n");
                    exit(1);
                }
                // Se registran los datos de cada mago
                for (int i = 0; i < numMagos; i++) {
                    printf("\nRegistrando mago %d:", i + 1);
                    magos[i] = crearMago();
                }
                printf("\nLista creada exitosamente.\n");
                break;

            case 2:
                // Se llama a la funcion para modificar el nivel magico de un mago
                modificarNivelMagico(magos, numMagos);
                break;

            case 3:
                // Se guarda la lista de magos en el archivo, si la lista existe
                if (magos) {
                    guardarEnArchivoMagico(magos, numMagos);
                } else {
                    printf("\nNo hay magos registrados.\n");
                }
                break;

            case 4:
                // Se libera la lista actual, si existe, antes de cargar desde el archivo
                if (magos) {
                    free(magos);
                }
                // Se carga la lista de magos desde el archivo
                magos = leerDesdeArchivoMagico(&numMagos);
                break;

            case 5:
                // Se muestra la lista de magos ordenada por nivel magico (de mayor a menor)
                if (!magos) {
                    printf("\nNo hay magos registrados.\n");
                } else {
                    ordenarMagos(magos, numMagos);
                    printf("\nLista de magos ordenada:\n");
                    for (int i = 0; i < numMagos; i++) {
                        imprimirMago(magos[i]);
                    }
                }
                break;

            case 6:
                // Opcion de salida: se libera la memoria y se termina el programa
                printf("\nSaliendo...\n");
                if (magos) {
                    free(magos);
                }
                return 0;

            default:
                // Opcion invalida, se muestra mensaje de error y se pide reingreso
                printf("\nOpcion invalida. Intente nuevamente.\n");
        }
    }
}
