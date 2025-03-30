#include <stdio.h>
#include <stdlib.h>


// el MBR es el primer sector fisico del disco y mide 512bytes,
// puede contener informacion de hasta 4 particiones distintas,
// en el contexto del archivo, si seguimos la tabla de la teoria,
// deberiamos saltear los primeros 446 bytes (BootStrap Code)
// e ir saltando de a 16bytes hasta terminar las particiones.

int main() {
    //puntero al archivo test.img, de donde buscaremos leer el mbr
    FILE * in = fopen("test.img", "rb");

    //declaramos como enteros sin signo variable indice, el inicio del sector y el largo de los sectores
    unsigned int i, start_sector, length_sectors;

    fseek(in, 446, SEEK_SET); // Voy al inicio. Saltamos el BootSrtap Code (446bytes)

    //for de 4 ciclos, uno por cada posible particion
    for(i=0; i<4; i++) { 
        
        //Muestra el BootFlag 
        printf("Partition entry %d: First byte %02X\n", i, fgetc(in));
        //fgetc lee de a byte, en particular al que apunta ln, y lo incrementa en 1.
        
        //Muestra La dirección CHS de inicio
        printf("  Comienzo de partición en CHS: %02X:%02X:%02X\n", fgetc(in), fgetc(in), fgetc(in));
        //utiliza fgetc como en el anterior, esta vez en 3 ocasiones ya que son 3 bytes.

        //Muestra el Tipo de partición
        printf("  Partition type 0x%02X\n", fgetc(in));

        //Muestra La dirección CHS de finalizacion
        printf("  Fin de partición en CHS: %02X:%02X:%02X\n", fgetc(in), fgetc(in), fgetc(in));
        
        //Actualiza las variables para mostrar:
        //1) la direccion LBA(inicio de la particion), 2) el tamaño de la particion en sectores.
        fread(&start_sector, 4, 1, in);
        fread(&length_sectors, 4, 1, in);
        //fread tiene una sintaxis tal que: 
        //fread(void buffer, tamaño_elemento, numero_elementos, Puntero_al_archivo);
        //Por lo que, en ambos casos leen 1 elemento de 4bytes, lo guardan en la variable
        //y finalmente actualizan el puntero 4 bytes (numero_elementos * tamaño_elemento).

        //Muestra la direccion LBA(inicio de la particion)
        printf("  Dirección LBA relativa 0x%08X, de tamaño en sectores %d\n", start_sector, length_sectors);
    }
    
    fclose(in);
    return 0;
}
