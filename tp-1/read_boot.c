#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char first_byte;//Indica si la particion esta activa 0x80 o inactiva 0x00
    unsigned char start_chs[3];//Direccion chs donde inicia la particion
    unsigned char partition_type;//Tipo de particion, 0x01 para FAT12
    unsigned char end_chs[3];//Direccion chs donde termina la particion
    char start_sector[4];//Direccion inicio de la particion en LBA
    char length_sectors[4];//Cantidad de sectores totales de la particion
} __attribute((packed)) PartitionTable;//Esquema de particionado MBR 

typedef struct {
    unsigned char jmp[3];//bytes 0-2 
    char oem[8];//bytes 3-10 
    unsigned short sector_size; //bytes 11-12, bytes por sector
    //Completamos con datos FAT Boot Sector
	unsigned char sector_per_cluster; // byte 13
    unsigned short reserved_sectors; // bytes 14-15
    unsigned char num_fats; // byte 16, en FAT12 es 2 --> redundancia
    unsigned short max_root_entries; // bytes 17-18, maximo de archivos en raiz
    unsigned short total_sectors; // bytes 19-20, total de sectores en la particion
    unsigned char media_type; // byte 21, tipo de medio, 0xF8 para FAT12
    unsigned short fat_size; // bytes 22-23, tamaño de cada FAT 
    unsigned short sectors_per_track; // bytes 24-25, cantidad de sectores por pista
    unsigned short num_heads; // bytes 26-27, cantidad de cabezales
    unsigned int hidden_sectors; // bytes 28-31, cantidad de sectores ocultos
    unsigned int total_sectors_extension; // bytes 32-35, total de sectores en la particion (0 si alcanzaron los bytes 19-20)
    unsigned char drive_number; // byte 36, numero de unidad logica
    unsigned char reserved; //byte 37 no usado
    unsigned char boot_signature; // byte 38, firma 0x38 para validar los siguientes 3 campos
    //
    unsigned int volume_id;//bytes 39-42, ID del volumen
    char volume_label[11];//bytes 43-53, nombre del volumen
    char fs_type[8]; // bytes 54-61 Type en ascii
    char boot_code[448];//bytes 62-509
    unsigned short boot_sector_signature;//bytes 510-511, firma de arranque 0xAA55
} __attribute((packed)) Fat12BootSector;//Estructura del Boot sector FAT12

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    
    fseek(in, 0x1BE , SEEK_SET); // Ir al inicio de la tabla de particiones. Byte Range 446-461 Partition Table Entry #1 446 = 0x1BE 
    fread(pt, sizeof(PartitionTable), 4, in); // leo entradas 
    
    for(i=0; i<4; i++) {        
        printf("Partiion type: %d\n", pt[i].partition_type);
        if(pt[i].partition_type == 1) {
            printf("Encontrado FAT12 %d\n", i);
            break;
        }
    }
    
    if(i == 4) {
        printf("No se encontró filesystem FAT12, saliendo ...\n");
        return -1;
    }
    
    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);
    
    printf("  Jump code: %02X:%02X:%02X\n", bs.jmp[0], bs.jmp[1], bs.jmp[2]);
    printf("  OEM code: [%.8s]\n", bs.oem);
    printf("  sector_size: %d\n", bs.sector_size);
	// Completamos con datos Fat12BootSector
    printf("  sector_per_cluster: %d\n", bs.sector_per_cluster);
    printf("  reserved_sectors: %d\n", bs.reserved_sectors);
    printf("  num_fats: %d\n", bs.num_fats);
    printf("  Max number of files in the Root Directory: %d\n", bs.max_root_entries);
    printf("  total_sectors: %d\n", bs.total_sectors);
    printf("  media_type: 0x%02X\n", bs.media_type);
    printf("  fat_size: %d\n", bs.fat_size);
    printf("  sectors_per_track: %d\n", bs.sectors_per_track);
    printf("  num_heads: %d\n", bs.num_heads);
    printf("  hidden_sectors: %d\n", bs.hidden_sectors);
    printf("  total_sectors_extension: %d\n", bs.total_sectors_extension);
    printf("  drive_number: %02X\n", bs.drive_number);
    printf("  boot_signature: %02X\n", bs.boot_signature);

    printf("  volume_id: 0x%08X\n", bs.volume_id);
    printf("  Volume label: [%.11s]\n", bs.volume_label);
    printf("  Filesystem type: [%.8s]\n", bs.fs_type);
    printf("  Boot sector signature: 0x%04X\n", bs.boot_sector_signature);
    
    fclose(in);
    return 0;
}
