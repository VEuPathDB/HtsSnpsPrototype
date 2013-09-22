#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
main(int argc, char *argv[]) {
	FILE *f1;
	FILE *f2;
  int8_t *strain1;
  int8_t *strain2;

  int32_t id1;
  int32_t *id1p = &id1;
  char a1;  // allele
  char *a1p = &a1;
  char p1;  // product
  char *p1p = &p1;

  int32_t id2;
  int32_t *id2p = &id2;
  char a2;  // allele
  char *a2p = &a2;
  char p2;  // product
  char *p2p = &p2;

	time_t now;
  struct tm *current;

	//	fprintf(stderr, "start: unionLocations %s %s\n", argv[1], argv[2]);

	if ( argc != 3 ) {
		printf( "usage: %s file1 file2\n", argv[0] );
		return -1;
	}

	f1 = fopen(argv[1], "rb");
	if (f1 == 0) {
		printf( "Can't open file1 '%s' \n", argv[1] );
		return -1;
	}
	f2 = fopen(argv[2], "rb");
	if (f2 == 0) {
		printf( "Can't open file2 '%s' \n", argv[2] );
		return -1;
	}

	int f1got;
	int f2got;
	f1got = readStrainRow(f1, id1p, a1p, p1p);
	f2got = readStrainRow(f2, id2p, a2p, p2p);

	while(1 == 1) {
		while (id1 < id2 && f1got != 0) { 
			f1got = writeStrainRowAndReadNext(f1, id1p, a1p, p1p, strain1);
		}
		while (id2 < id1 && f2got != 0) {
			f2got = writeStrainRowAndReadNext(f2, id2p, a2p, p2p, strain2);
		}
		if (id1 == id2) {
			f1got = writeStrainRowAndReadNext(f1, id1p, a1p, p1p, strain1);
			f2got = writeStrainRowAndReadNext(f2, id2p, a2p, p2p, strain2);
		}
		if (f1got == 0 && f2got ==0) break;
		if (f1got == 0) f2got = writeStrainRowAndReadNext(f2, id2p, a2p, p2p, strain2);
		if (f2got == 0) f1got = writeStrainRowAndReadNext(f1, id1p, a1p, p1p, strain1);
	}
	fclose(f1);
	fclose(f2);
	return 0;
}

int readStrainRow(FILE *file, int32_t *id, int8_t *allele, char *product) {
	fread(id, 4, 1, file);  
	fread(allele, 1, 1, file); 
	return fread(product, 1, 1, file);
}

int writeStrainRowAndReadNext(FILE *file, int32_t *id, int8_t *allele, char *product, int8_t *strain) {
	fwrite(id, 4, 1, stdout);  
	fwrite(allele, 1, 1, stdout); 
	fwrite(product, 1, 1, stdout);
	fwrite(strain, 1, 1, stdout);
	return readStrainRow(file, id, allele, product);
}


