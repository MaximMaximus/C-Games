#include "pch.h"
#include <stdio.h>
#include <time.h>


void print_feld(int feld[9][9]) {
	int i = 0;
	int j = 0;
	printf("\n     SUDOKU MASTERS     \n\n");
	printf("  | 1 2 3  4 5 6  7 8 9\n");
	printf("--+--------------------\n");
	for (i = 0; i < 9; i++) {
		printf("%d |", i + 1);
		for (j = 0; j < 9; j++) {
			if (j % 3 == 0) printf(" ");
			printf("%d ", feld[i][j]);
		}
		printf("\n");
		if (i == 2 || i == 5) printf("  |\n");

	}
}

int check_feld(int feld[9][9], int zeile, int spalte, int wert) {
	int z = 0;
	int s = 0;

	// vertical und horizontal prüfen
	for (s = 0, z = 0; s < 9, z < 9; s++, z++) {
		if (feld[zeile - 1][s] == wert || feld[z][spalte - 1] == wert) {
			return 0;
		}
	}

	// Quadrant prüfen
	for (s = ((spalte - 1) / 3) * 3; s < ((spalte - 1) / 3) * 3 + 3; s++) {
		for (z = ((zeile - 1) / 3) * 3; z < ((zeile - 1) / 3) * 3 + 3; z++) {
			if (feld[z][s] == wert) {
				return 0;
			}
		}
	}

	feld[zeile - 1][spalte - 1] = wert;

	return 1;
}

int main() {
	int zeile = 0;
	int spalte = 0;
	int wert = 0;

	int feld[9][9] = {
		{0,1,8, 7,0,0, 0,0,0},
		{0,2,0, 6,0,0, 4,8,5},
		{0,3,4, 5,0,0, 7,0,2},
		{0,0,0, 0,0,0, 6,3,1},
		{0,0,0, 0,0,0, 0,0,0},
		{1,8,5, 0,0,0, 0,0,0},
		{2,0,7, 0,0,3, 1,4,0},
		{3,4,6, 0,0,8, 0,7,0},
		{0,0,0, 0,0,2, 5,6,0},
	};

	do {
		system("CLS");
		print_feld(feld);

		printf("\nBitte geben Sie die Zeile, Spalte und den Wert ein:\nZ S W:\n");
		scanf_s("%d %d %d", &zeile, &spalte, &wert);

		if (check_feld(feld, zeile, spalte, wert) == 0 && wert != 0) {
			printf("Fehler!\n");
			//sleep(500);
			printf("\a\a\n");
		}
		else {
			printf("\a\n");
		}

	} while (wert != 0);

	return 0;
}
