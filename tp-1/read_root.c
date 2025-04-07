#include <stdio.h>
#include <stdlib.h>

#define UNUSED 0x00
#define FIRST_BYTE_0xE5 0x05
#define DOT_DIR 0x2E
#define DELETED_FILE 0xE5

#define ARCHIVE 0x20
#define DIRECTORY 0x10

typedef struct {// PARTICION
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    char starting_cluster[4];
    char file_size[4];
} __attribute((packed)) PartitionTable;

typedef struct { // BOOT SECTOR ESTRUCTURA FAT32
    unsigned char jmp[3];//3B--Bien
    char oem[8];//8B--Bien
    unsigned short sector_size;//2B
    unsigned char sectors_per_cluster;//1B
    unsigned short reserved_sectors;//2B
    unsigned char number_of_fats;//1B
    unsigned short root_dir_entries;//2B
    unsigned short total_sectors;//2B
    unsigned char media_descriptor;//1B
    unsigned short fat_size_sectors;//2B
    unsigned short sectors_per_track;//2B
    unsigned short number_of_heads;//2B
    unsigned int hidden_sectors;//4B
    unsigned int large_sector_count;//4B
    unsigned char drive_number;//1B
    unsigned char reserved;//1B
    unsigned char boot_signature;//1b
    unsigned int volume_id;//4B
    char volume_label[11];//11B
    char fs_type[8]; //8B
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct { // ENTRADA ESTRUCTURA FAT32
	unsigned char filename[8]; //8B
    char extension[3]; //3B
    unsigned char attributes; //1B
    unsigned char reserved; //1B
    unsigned char creation_time_msegs; //1B
    unsigned short creation_time; //2B
    unsigned short creation_date; //2B
    unsigned short last_access_date; //2B
    unsigned short high_cluster; //2B
    unsigned short last_mod_time; //2B
    unsigned short last_mod_date; //2B
    unsigned short start_cluster; //2B
    unsigned int file_size; //4B
    						//32
} __attribute((packed)) Fat12Entry;

void print_file_info(Fat12Entry *entry, FILE *f, Fat12BootSector bs);

void content_directory(Fat12Entry *entry, FILE *f, Fat12BootSector bs){
	printf("-------Archivos del directorio:-------\n");
	
	Fat12Entry directory_entry;
	unsigned long directory_index = ftell(f);//ftell retorna la posicion actual del puntero del archivo
	int cluster_size_byte = bs.sector_size * bs.sectors_per_cluster; //se calcula el total de clusters en bytes 512*2
	int cluster_content_file = entry->start_cluster;
	unsigned short num_entries = cluster_size_byte / sizeof(Fat12Entry);// 512/32= total de entradas que entran en el cluster
	
	long int offset_content = sizeof(Fat12BootSector) 
    + (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) * bs.sector_size /*calculo para moverse al directorio raiz*/
    + bs.root_dir_entries * sizeof(Fat12Entry) 
    + ((cluster_content_file - 2)* cluster_size_byte);
    
    fseek(f,offset_content,SEEK_SET);// Muevo el puntero al contenido del directorio
   	
   	for(int i = 0; i < num_entries; i++){
   		fread(&directory_entry, sizeof(directory_entry), 1, f);
   		print_file_info(&directory_entry, f, bs);
   	}
    
    fseek(f, directory_index, SEEK_SET);//Se restablece la posicion del puntero
    printf("-------------------Fin-------------------\n\n");
}

void content_archive(Fat12Entry *entry, FILE *f, Fat12BootSector bs){
	printf("***Contenido***");
	if(entry->filename[0]==DELETED_FILE){
		printf("\n ARCHIVO VACIO.\n\n");
		printf("*********\n\n");
		return;
	}
	unsigned long file_index = ftell(f);//ftell retorna la posicion actual del puntero del archivo
	
	int cluster_size_byte = bs.sector_size * bs.sectors_per_cluster; //se calcula el total de clusters en bytes 512*2
	int cluster_content_file = entry->start_cluster;
	char file_content[cluster_size_byte + 1]; //Contenido del archivo
	
	long int offset_content = sizeof(Fat12BootSector) 
    + (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) * bs.sector_size /*calculo para moverse al directorio raiz*/
    + bs.root_dir_entries * sizeof(Fat12Entry) 
    + ((cluster_content_file - 2)* cluster_size_byte);
    
    fseek(f, offset_content, SEEK_SET);// Muevo el puntero al contenido del directorio
    fread(file_content, entry->file_size, 1, f);
    file_content[entry->file_size] = '\0'; // se agrega caracter nulo para que ascii detecte que termino el array
    
	printf("\n%s\n\n", file_content);
 	fseek(f, file_index, SEEK_SET);//Se restablece la posicion del puntero
 	printf("*********\n\n");
}

void print_file_info(Fat12Entry *entry, FILE *f, Fat12BootSector bs) {

    switch(entry->filename[0]) {
    case UNUSED: 
        return; 
    case DELETED_FILE: //Indica que el archivo fue borrado 
    	if(entry-> attributes == ARCHIVE){
    		printf("\nEstructura del archivo borrado:\n");
    		printf("\nNombre: [?%.7s]\n", entry->filename+1);
    		printf("Extension: [%.3s]\n", entry->extension);
        	printf("Atributos : %X\n",entry->attributes);
        	printf("Reservado : %X\n",entry->reserved);
        	printf("Hora de creación(en milisegundos) : %X\n",entry->creation_time_msegs);
        	printf("Hora de creacion : %X\n",entry->creation_time);
        	printf("Fecha de creacion : %X\n",entry->creation_date);
        	printf("Ultima fecha de acceso : %X\n",entry->last_access_date);
       		printf("Alto num de cluster inicial : %X\n",entry->high_cluster);
        	printf("Hora de ultima modificacion : %X\n",entry->last_mod_time);
        	printf("Fecha de ultima modificacion : %X\n",entry->last_mod_date);
        	printf("Bajo num de cluster inicial: %X\n",entry->start_cluster);
        	printf("Tamaño de archivo(B) : %X\n",entry->file_size);
        	content_archive(entry,f,bs);
    	}
        break;
    case FIRST_BYTE_0xE5: // Completar los ...//0x05 Indica que el byte inicial es 0xE5 
        return;
    case DOT_DIR://Se usa para entrada especial de directorio(? O Entradas de puintos
        return;
    default:
    	if(entry-> attributes == DIRECTORY){
    		printf("\nDirectorio: [%.8s.%.3s]\n",entry-> filename, entry->extension);
         	content_directory(entry,f,bs);
    	}else if(entry-> attributes == ARCHIVE){
        	printf("\nArchivo:[%.8s.%.3s]\n\n",entry-> filename, entry->extension); 
        	content_archive(entry, f, bs);
        }
    }
    
}

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;
  
	fseek(in, 0x1BE, SEEK_SET);//CON 0x1BE VOY PRIMERA PARTICION
	fread(pt, sizeof(PartitionTable), 4, in); // Lee cuatro veces la estructura en el sistema de archivos 16B x 4(particiones)
    
    for(i=0; i<4; i++) {        
        if(pt[i].partition_type == 1) { //busca las particion o las particiones que tengan un sistema de archivo = 0x01 (FAT12)
            printf("Encontrada particion FAT12 %d\n", i);
            break;
        }
    }
    
    if(i == 4) {
        printf("No encontrado filesystem FAT12, saliendo...\n");
        return -1;
    }
    
   //----------------------
    
    fseek(in, 0, SEEK_SET); // ABRE EL SISTEMA DE ARCHIVO DESDE EL INICIO
	fread(&bs, sizeof(Fat12BootSector), 1, in); // Lee el boot selector del sistema de archivos
    
    printf("En  0x%lx, sector size %d, FAT size %d sectors, %d FATs\n\n", 
      	ftell(in), bs.sector_size, bs.fat_size_sectors, bs.number_of_fats);
  //----------------------
      
    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) *
          bs.sector_size, SEEK_CUR); //(1-1+2*2)*512 =2048 calculo para moverse al directorio raiz
    
    printf("Root dir_entries %d \n", bs.root_dir_entries);// 512
    for(i=0; i<bs.root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);
        print_file_info(&entry, in, bs);
    }
    
    printf("\nLeido Root directory, ahora en 0x%lx\n", ftell(in)); 
    fclose(in);
    return 0;
}
