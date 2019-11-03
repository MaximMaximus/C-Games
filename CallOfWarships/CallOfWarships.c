#include "pch.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

//enum Bool { FALSE, TRUE };
enum Schiff { UBOOT = 85, FREGATTE = 70, SCHLACHTSCHIFF = 83 };
HANDLE wHnd;
HANDLE rHnd;

// Loading
void loadingAnimation() {
	int i = 0;
	int j = 20;
	short loading = TRUE;
		while (loading) {
		system("CLS");
		printf("KI ZUG WIRD BERECHNET!\n");
		printf("+--------------------+\n|");
		printf("%*s", i, "");
		printf("%c", 178);
		printf("%*s", j, "|");
		printf("\n+--------------------+\n");
		i++, j--;
		Sleep(10);
		if (i == 20) {
			loading = FALSE;
		}
	}
}

// Hit
void hit() {
	printf("\a");
}

// Spielfeld zeichnen
char print_feld(char feldnpc[10][10], char feldspieler[10][10], int *lebenNpc, int *lebenSpieler) {
	int i = 0;
	int j = 0;
	printf("\t      Spielfeld Gegner\n");
	printf("\ty/x | 1 2 3 4 5 6 7 8 9 10\n");
	printf("\t----+---------------------\n");
	for (i = 0; i < 10; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("\t%-4d|", i + 1);
		for (j = 0; j < 10; j++) {
			if(feldnpc[i][j] == 'X') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20), printf(" %c", feldnpc[i][j]), SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			else if(feldnpc[i][j] == 'O') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31), printf(" %c", feldnpc[i][j]), SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 17), printf(" %c", feldnpc[i][j]);
		}
		printf("\n");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("\t----+---------------------\n");
	printf("\tGegner-Leben: \n\t");
	for (i = 0; i < 17; i++) {
		if (i < *lebenNpc) printf("%c ", 3);
		else printf("X ");
	}
	printf("\n");

	printf("\n\t    Spielfeld Spieler\n");
	printf("\ty/x | 1 2 3 4 5 6 7 8 9 10\n");
	printf("\t----+---------------------\n");
	for (i = 0; i < 10; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		printf("\t%-4d|", i + 1);
		for (j = 0; j < 10; j++) {
			if(feldspieler[i][j] == 'X') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20), printf(" %c", feldspieler[i][j]), SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			else if(feldspieler[i][j] == 'O') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 31), printf(" %c", feldspieler[i][j]), SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			else if(feldspieler[i][j] == 'U' || feldspieler[i][j] == 'F' || feldspieler[i][j] == 'S') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 20), printf(" %c", feldspieler[i][j]), SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 17), printf(" %c", feldspieler[i][j]);
		}
		printf("\n");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("\t----+---------------------\n");
	printf("\tSpieler-Leben: \n\t");
	for (i = 0; i < 17; i++) {
		if (i < *lebenSpieler) printf("%c ", 3);
		else printf("X ");
	}
	printf("\n\n");
}

// Spielereingabe
int spielereingabe(int *y, int *x) {
	short eingabe = TRUE;
	while (eingabe) {
		printf("\tBitte geben Sie X und Y ein: \n\tX Y:\n\t");
		fflush(stdin);
		scanf_s("%d %d", x, y);
		if ((10 >= *y && 1 <= *y) && (10 >= *x && 1 <= *x)) eingabe = FALSE;
		else printf("\nFalsche Eingabe!\n");
	}
}

// Angriff Spieler
char angriff_spieler(char feldNpcHidden[10][10], char feldNpc[10][10], int *lebenNpc) {
	int y = 0;
	int x = 0;
	short eingabeSpieler = TRUE;

	while (eingabeSpieler) {
		spielereingabe(&y, &x);
		y -= 1;
		x -= 1;
		if (feldNpc[y][x] == 'X' || feldNpc[y][x] == 'O') {
			printf("\tDieses Feld wurde schon angegeriffen!\n");
			continue;
		}
		if (feldNpcHidden[y][x] != 0) feldNpc[y][x] = 'X', hit(), *lebenNpc -= 1, eingabeSpieler = FALSE;
		if (feldNpcHidden[y][x] == 0) feldNpc[y][x] = 'O', eingabeSpieler = FALSE;
	}
}

// Angriff NPC
char angriff_npc(char feldSpieler[10][10], int *lebenSpieler, int *lebenNpc, int *lastHitX, int *lastHitY, short *lastHit, int *npcVerfehlt) {
	short eingabeNPC = TRUE;
	short randSwitch = TRUE;
	int hitTest = 0;
	int randDir = 0;
	int newHit[4] = {0};
	int yZufall = rand() % 10;
	int xZufall = rand() % 10;
	//loadingAnimation();
	while (eingabeNPC) {
		yZufall = rand() % 10;
		xZufall = rand() % 10;
		if (*lastHit==TRUE) {
			hitTest = 0;
			if (*lastHitY-1 > -1 && feldSpieler[*lastHitY-1][*lastHitX] != 'X' || feldSpieler[*lastHitY-1][*lastHitX] != 'O') newHit[0] = 1, hitTest++;
			if (*lastHitX+1 < 10 && feldSpieler[*lastHitY][*lastHitX+1] != 'X' || feldSpieler[*lastHitY][*lastHitX+1] != 'O') newHit[1] = 1, hitTest++;
			if (*lastHitY+1 < 10 && feldSpieler[*lastHitY+1][*lastHitX] != 'X' || feldSpieler[*lastHitY+1][*lastHitX] != 'O') newHit[2] = 1, hitTest++;
			if (*lastHitX-1 > -1 && feldSpieler[*lastHitY][*lastHitX-1] != 'X' || feldSpieler[*lastHitY][*lastHitX-1] != 'O') newHit[3] = 1, hitTest++;
			if (hitTest == 0) {
				*lastHit = FALSE;
				break;
			} else {
				while(randSwitch) {
					randDir = rand() % 4;
					if (newHit[randDir] != 0) {
						switch(randDir) {
							case 0: yZufall = *lastHitY-1, xZufall = *lastHitX, randSwitch = FALSE; break;
							case 1: yZufall = *lastHitY, xZufall = *lastHitX+1, randSwitch = FALSE; break;
							case 2: yZufall = *lastHitY+1, xZufall = *lastHitX, randSwitch = FALSE; break;
							case 3: yZufall = *lastHitY, xZufall = *lastHitX-1, randSwitch = FALSE; break;
							default: break;
						}
					}
				}
			}
		}
		if(*npcVerfehlt == 3) {
			*lastHit = FALSE;
			*npcVerfehlt = 0;
		}
		if (feldSpieler[yZufall][xZufall] == 'X' || feldSpieler[yZufall][xZufall] == 'O') continue;
		if (feldSpieler[yZufall][xZufall] != 0) feldSpieler[yZufall][xZufall] = 'X', hit(), *lastHitX = xZufall, *lastHitY = yZufall, *lastHit = TRUE, *npcVerfehlt = 0, *lebenSpieler -= 1, eingabeNPC = FALSE;
		if (feldSpieler[yZufall][xZufall] == 0) feldSpieler[yZufall][xZufall] = 'O', *npcVerfehlt+=1, eingabeNPC = FALSE;
	}
}

char feld_generieren(char feld[10][10], char *text) {
	int generieren = 10;
	int y = 0;
	int x = 0;
	int schiff = 0;
	int direction = 0;

	int schlacht = 2;
	int freg = 3;
	int ubo = 5;

	while (generieren > 0) {
		system("CLS");
		printf("Call of Warships\n");
		printf("%s %d\n", text, 100 - generieren * 10);

		y = rand() % 10;
		x = rand() % 10;
		schiff = rand() % 3;
		direction = rand() % 2 + 1;

		if (feld[y][x] != 0) continue;
		if (feld[y][x] == 0) {
			if (schiff == 0 && ubo != 0) {
				feld[y][x] = UBOOT;
				ubo--;
				generieren--;
			}
			if (schiff == 1 && freg != 0) {
				if (direction == 1) {
					if (y - 1 > -1 && feld[y - 1][x] == 0) feld[y][x] = FREGATTE, feld[y - 1][x] = FREGATTE, freg--, generieren--;
					else if (y + 1 < 10 && feld[y + 1][x] == 0) feld[y][x] = FREGATTE, feld[y + 1][x] = FREGATTE, freg--, generieren--;
					else break;
				}
				if (direction == 2) {
					if (x - 1 > -1 && feld[y][x - 1] == 0) feld[y][x] = FREGATTE, feld[y][x - 1] = FREGATTE, freg--, generieren--;
					else if (x + 1 < 10 && feld[y][x + 1] == 0) feld[y][x] = FREGATTE, feld[y][x + 1] = FREGATTE, freg--, generieren--;
					else break;
				}
			}
			if (schiff == 2 && schlacht != 0) {
				if (direction == 1) {
					if (y - 2 > -1 && feld[y - 1][x] == 0 && feld[y - 2][x] == 0) feld[y][x] = SCHLACHTSCHIFF, feld[y - 1][x] = SCHLACHTSCHIFF, feld[y - 2][x] = SCHLACHTSCHIFF, schlacht--, generieren--;
					else if (y + 2 < 10 && feld[y + 1][x] == 0 && feld[y + 2][x] == 0) feld[y][x] = SCHLACHTSCHIFF, feld[y + 1][x] = SCHLACHTSCHIFF, feld[y + 2][x] = SCHLACHTSCHIFF, schlacht--, generieren--;
					else break;
				}
				if (direction == 2) {
					if (x - 2 > -1 && feld[y][x - 1] == 0 && feld[y][x - 2] == 0) feld[y][x] = SCHLACHTSCHIFF, feld[y][x - 1] = SCHLACHTSCHIFF, feld[y][x - 2] = SCHLACHTSCHIFF, schlacht--, generieren--;
					else if (x + 2 < 10 && feld[y][x + 1] == 0 && feld[y][x + 2] == 0) feld[y][x] = SCHLACHTSCHIFF, feld[y][x + 1] = SCHLACHTSCHIFF, feld[y][x + 2] = SCHLACHTSCHIFF, schlacht--, generieren--;
					else break;
				}
			}
		}
	}
}

int main() {
	// Grüne Farbe
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	// Conslen Größe ändern
	// Set up the handles for reading/writing:
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	// Change the window title:
	//SetConsoleTitle("Call Of Warships");
	// Set up the required window size:
	SMALL_RECT windowSize = { 0, 0, 50, 50 };
	SetConsoleWindowInfo(wHnd, 1, &windowSize);
	// Change the console window size:
	// Create a COORD to hold the buffer size:
	COORD bufferSize = { 10, 10 };
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	short neuesSpiel = TRUE;

	do {
		short spiel = TRUE;
		int npcVerfehlt = 0;
		short lastHit = FALSE;
		int lastHitX = 0;
		int lastHitY = 0;
		int lebenSpieler = 17;
		int lebenNpc = 17;

		char feldNpc[10][10] = {
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
		};
		char feldNpcHidden[10][10] = {
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
		};
		char feldSpieler[10][10] = {
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
			{0,0,0,0,0, 0,0,0,0,0},
		};

		srand(time(NULL));
		feld_generieren(feldSpieler, "Loading Data");
		feld_generieren(feldNpcHidden, "Loading KI");

		do {
			srand(time(NULL));
			system("CLS");
			//printf("lasthit %d, npcVerfehlt %d", lastHit, npcVerfehlt);
			printf("\t\tCall of Warships\n\n");
			printf("U: Uboot\tF: Fregatte\tS: Schlachtschiff\n");
			printf("X: Treffer\tO: Verfehlt\n\n");
			print_feld(feldNpc, feldSpieler, &lebenNpc, &lebenSpieler);
			angriff_spieler(feldNpcHidden, feldNpc, &lebenNpc);
			angriff_npc(feldSpieler, &lebenSpieler, &lebenNpc, &lastHitX, &lastHitY, &lastHit, &npcVerfehlt);
			if (lebenSpieler == 0) spiel = FALSE;
			if (lebenNpc == 0) spiel = FALSE;
		} while (spiel);

		system("CLS");
		print_feld(feldNpc, feldSpieler, &lebenNpc, &lebenSpieler);

		if (lebenSpieler == 0) printf("Sie haben verloren!\n");
		if (lebenNpc == 0) printf("Sie haben gewonnen!\n");
		printf("Neues Spiel starten?\nY / N:\n");
		fflush(stdin);
		char a = getchar();
		if ( a == 'y' || a == 'Y' ) continue;
		if ( a == 'n' || a == 'N' ) neuesSpiel = FALSE;
		if ( a != 'y' || a != 'Y' ) neuesSpiel = FALSE;
	} while (neuesSpiel);
	printf("See Ya!\n");
	fflush(stdin);
	getchar();
	return 0;
}
