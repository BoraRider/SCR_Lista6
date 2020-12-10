#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){

    int plik;
    char nazwa[128];
    char dodaj[8];
    struct stat statbuff;
    int dlugosc;
    char *mapped_mem;

    while(1){
        printf("Podaj nazwe pliku do otwarcia\n");
        scanf("%s", nazwa);

        plik = open(nazwa, O_RDWR | O_CREAT, 0755);
        if(plik < 0){
            printf("Nie udalo sie otworzyc pliku\n");
            return 1;
        }

        if(fstat(plik, &statbuff) < 0){
            printf("Blad pliku: nie udalo sie odczytac rozmiaru\n");
            return 1;
        }

        for(int i = 0;i<8;i++){
            dodaj[i]='0';
        }
        dlugosc = strlen(dodaj);

        ftruncate(plik, statbuff.st_size+dlugosc);
        mapped_mem = mmap(NULL, statbuff.st_size+dlugosc, PROT_WRITE | PROT_READ, MAP_SHARED, plik, 0);

        strcat(mapped_mem, dodaj);
        msync(mapped_mem, statbuff.st_size+dlugosc, MS_SYNC);

        close(plik);
    }
    munmap(mapped_mem, statbuff.st_size+dlugosc);
    return 0;
}
