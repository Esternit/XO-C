#include <stdio.h>
#include <stdlib.h>
#include <clocale>
#include <cstring>
#include <Windows.h>

#pragma warning(disable : 4996)
constexpr auto MYSIZE=3;
constexpr auto X='X';
constexpr auto O='O';
constexpr auto SPACE = ' ';

char* CreateMap(int size);
void DestroyMap(char* map);
void PrintMap(char* map, int size);
bool CheckInput(int x, int y, char* map, int size);
bool IsMapFull(char* commonMap, int mapsize);
char checkWinOnMap(char* commonMap, int size);
typedef struct cort {
	int x;
	int y;
};
cort* optimalCompStep(char* commonMap, int mapsize, char choise, char computerchoise);
cort* CompStep(char* commonMap, int mapsize, char choise, char computerchoise);

void InitColor();
// основная функция, в которой совмещаются все функции
int main()
{
	setlocale(LC_ALL, "Russian");
	int mapsize = 0;
	char choise, computerchoise;
	while (1) {
		printf("%s", "Выберите крестиками или ноликами вы будете играть:");
		choise = fgetc(stdin);
		if (choise != X and choise != O) {
			printf("%s", "Введите X или O");
		}
		else {
			if (choise == X) {
				computerchoise = O;
				break;
			}
			else {
				computerchoise = X;
				break;
			}
		}
	}
	while (1) {
		char mapsizestr[3];
		fgets(mapsizestr, 3, stdin);
		mapsize = atoi(mapsizestr);
		if (mapsize < 3 || mapsize>10) {
			printf("%s", "Введите число от 3 до 10:");
		}
		else {
			break;
		}

	}
	char* commonMap = CreateMap(mapsize);
	PrintMap(commonMap, mapsize);
	while (1) {
		int x = 1;
		int y = 1;
		char sx[5];
		char readlline[4];
		char sy[5];
		while (1) {
			printf("%s", "Ваш ход по x:");
			fgets(sx, 5, stdin);
			printf("%s", "Ваш ход по y:");
			fgets(sy, 5, stdin);
			x = atoi(sx);
			y = atoi(sy);
			if (0 <= x && x < mapsize && 0 <= y && y < mapsize) {
				if (CheckInput(x, y, commonMap, mapsize) != true)
					printf("%s \n", "Это место уже занято");
				else
					break;				
			}
			else
				printf("%s %d \n", "Введите числа от 0 до", mapsize);
		}
		commonMap[mapsize * x + y] = choise;
		PrintMap(commonMap, mapsize);
		char check = checkWinOnMap(commonMap, mapsize);
		if (check != NULL){
			printf("%s %c", "Победил", check);
			printf("%s", "нажмите enter для завершения программы");
			fgets(readlline, 4, stdin);
			exit(0);
		}
		cort* ret = optimalCompStep(commonMap, mapsize, choise, computerchoise);
		if (ret != NULL) {
			int x = (*ret).x;
			int y = (*ret).y;
			commonMap[mapsize * x + y] = computerchoise;
		}
		else {
			cort* ret = CompStep(commonMap, mapsize, choise, computerchoise);
			int x = (*ret).x;
			int y = (*ret).y;
			commonMap[mapsize * x + y] = computerchoise;
		}
		PrintMap(commonMap, mapsize);
		char check1 = checkWinOnMap(commonMap, mapsize);
		if (check1 != NULL) {
			printf("%s %c \n", "Победил", check1);
			printf("%s", "нажмите enter для завершения программы");
			fgets(readlline, 4, stdin);
			exit(0);
		}


		if (IsMapFull(commonMap,mapsize) == true){
			printf("%s", "Ничья");
			exit(0);
		}

	}
}
// создаёт массив , заполненный пробелами( получает размер массива)
char* CreateMap(int size) {
	char* map = (char*)malloc(size * size);
	for (int x=0; x < size; x++) {
		for (int y=0; y < size; y++) {
			map[size * x + y] = SPACE;
		}
	}
	return map;
}
// освобождает память, занятую полем( на вход получает поле)
void DestroyMap(char* map) {
	free(map);
}
// рисует поле(на вход получает масиив и размер поля)
void PrintMap(char* map, int size) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	char strheader[80] = { 0 };
	int count = 0;

	char* str = (char*)malloc(80);
	for (int e = 0; e < size; e++) {
		char newe[3];
		itoa(e,newe,10);
		strheader[count] = ' '; 
		count++;
		strheader[count] = newe[0];
		count++;
	}
	char newstrheader[100] = { 0 };
	newstrheader[0] = ' ';
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE| BACKGROUND_GREEN);
	strcat(newstrheader, strheader);
	strcat(newstrheader, " Y");
	printf("%s\n", newstrheader);
	SetConsoleTextAttribute(hOut, BACKGROUND_GREEN|BACKGROUND_RED| BACKGROUND_BLUE);
	
	for (int x=0; x < size; x++) {
		int count = 2;
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_GREEN);
		printf("%d", x);
		SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		for (int y=0; y < size; y++) {
			printf("%s", "|");
			char simbol;
			simbol = map[size * x + y];
			if (simbol == X) {
				SetConsoleTextAttribute(hOut,FOREGROUND_RED);
			}
			else if (simbol == O) {
				SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
			}

			printf("%c", simbol);
			SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);

		}
		printf("%c", '|');
		SetConsoleTextAttribute(hOut,BACKGROUND_GREEN);
		printf("%c\n", ' ');
		SetConsoleTextAttribute(hOut, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
	}
	char strfooter[80] = { 0 };
	int c = 0;
	for (; c < size * 2; c++) {
		strfooter[c] = ' ';
	}
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | BACKGROUND_GREEN);
	strfooter[0] = 'X';
	strfooter[c] = ' ';
	printf("%s  \n", strfooter);
	SetConsoleTextAttribute(hOut,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	free(str);
}
// проверяет занято ли место на поле( на вход получает данные, введённые по х , данные, введённые по y, поле, размер поля)
bool CheckInput(int x, int y, char* map,int size) {
	if (map[size * x + y] != X && map[size * x + y] != O) 
		return true;
	else
		return false;
}
// проверяет заполнено ли поле полностью( на вход получает поле и его размер)
bool IsMapFull(char* commonMap, int mapsize) {
	int count = 0;
	for (int x = 0; x < mapsize; x++) {
		for (int y = 0; y < mapsize; y++) {
			if (commonMap[mapsize * x + y] != SPACE) {
				count = count + 1;
			}
		}
	}
	if (count == mapsize*mapsize)
		return true;
	else
		return false;
}
// проверяет победил ли кто то из пользователей( на вход получает поле , использует несколько функций)
char CheckWin(char* map) {
	int count = 0;
	for (int x = 0; x < MYSIZE; x++) {
		char rightchoise=NULL;
		for (int y = 0; y < MYSIZE; y++) {
			if (rightchoise == NULL) {
				if (map[MYSIZE * x + y] != SPACE) {
					rightchoise = map[MYSIZE * x + y];
				}
			}
			if (map[MYSIZE * x + y] != rightchoise)
				break;
			count = count + 1;
		}
		if (count == MYSIZE)
			return rightchoise;
		count = 0;
	}
	for (int x = 0; x < MYSIZE; x++) {
		char rightchoise = NULL;
		for (int y = 0; y < MYSIZE; y++) {
			if (rightchoise == NULL) {
				if (map[MYSIZE * y +x] != SPACE) {
					rightchoise = map[MYSIZE * y + x];
				}
			}
			if (map[MYSIZE * y + x] != rightchoise)
				break;
			count = count + 1;
		}
		if (count == MYSIZE)
			return rightchoise;
		count = 0;
	}
	char rightchoise = NULL;
	count = 0;
	for (int x=0; x < MYSIZE; x++) {
		if (rightchoise == NULL) {
			if (map[MYSIZE * x + x] != SPACE)
				rightchoise = map[MYSIZE * x + x];
		}
		if (map[MYSIZE * x + x] == rightchoise)
			count += 1;
	}
	if (count == MYSIZE)
		return rightchoise;

	rightchoise = NULL;
	count = 0;
	for (int x=0; x < MYSIZE; x++) {
		if (rightchoise == NULL) {
			if (map[MYSIZE * x + ((MYSIZE-1)-x)] != SPACE)
				rightchoise = map[MYSIZE * x + ((MYSIZE - 1) -x)];
		}
		if (map[MYSIZE * x + ((MYSIZE - 1) -x)] == rightchoise)
			count += 1;
	}
	if (count == MYSIZE)
		return rightchoise;
	return NULL;
}
// проверяет победил ли ктото на поле, размером 3 на 3)
char checkWinOnMap(char* commonMap,int size) {
	for (int xb = 0; xb < size-(MYSIZE-1); xb++) {
		for (int yb=0; yb < size- (MYSIZE - 1); yb++) {
			char* smallMap = CreateMap(MYSIZE);
				for (int x = 0; x < MYSIZE; x++) {
					for (int y = 0; y < MYSIZE; y++) {
						smallMap[MYSIZE * x + y] = commonMap[size * (x + xb) + (y + yb)];
					}
				}
				char check;
				if ((check = CheckWin(smallMap)) != NULL)
					return check;
				DestroyMap(smallMap);
		}
	}
	return NULL;
}
// ищет победный ход для компьютера( на вход получает поле, фигурку человека, фигурку компьютера)
cort* findCompWinPos3x3(char* map, char choise, char computerchoise) {
	cort* ret=(cort*)malloc(sizeof(cort));
	for (int x = 0; x < MYSIZE; x++) {
		for (int y = 0; y < MYSIZE; y++) {
			if (map[MYSIZE * x + y] == SPACE) {
				map[MYSIZE * x + y] = computerchoise;
				if (CheckWin(map) == computerchoise) {
					map[MYSIZE * x + y] = SPACE;
					(*ret).x = x;
					(*ret).y = y;
					return ret;
				}
				else
					map[MYSIZE * x + y] = SPACE;
			}
		}

	}
	return NULL;
}
// не даёт человеку победить( на вход получает поле, фигурку человека, фигурку компьютера)
cort* findHumanWinPos3x3(char* map, char choise, char computerchoise) {
	cort* ret = (cort*)malloc(sizeof(cort));
	for (int x = 0; x < MYSIZE; x++) {
		for (int y = 0; y < MYSIZE; y++) {
			if (map[MYSIZE * x + y] == SPACE) {
				map[MYSIZE * x + y] = choise;
				if (CheckWin(map) == choise) {
					map[MYSIZE * x + y] = SPACE;
					(*ret).x = x;
					(*ret).y = y;
					return ret;
				}
				else
					map[MYSIZE * x + y] = SPACE;
			}
		}

	}
	return NULL;
}
// совмещает две функции, ищущие победный ход человека и компьютера( на вход получает поле, размер поля,  фигурку человека, фигурку компьютера)
cort* optimalCompStep(char* commonMap, int mapsize, char choise, char computerchoise) {
	for (int xb = 0; xb < mapsize - (MYSIZE-1); xb++) {
		for (int yb = 0; yb < mapsize - (MYSIZE - 1); yb++) {
			char* smallMap = CreateMap(MYSIZE);
			for (int x = 0; x < MYSIZE; x++) {
				for (int y = 0; y < MYSIZE; y++) {
					smallMap[MYSIZE * x + y] = commonMap[mapsize * (x + xb) + (y + yb)];
				}
			}
			if (findCompWinPos3x3(smallMap, choise, computerchoise) != NULL) {
				cort* ret = findCompWinPos3x3(smallMap, choise, computerchoise);
				(*ret).x += xb;
				(*ret).y += yb;
				return ret;
			}
		}
	}
	for (int xb = 0; xb < mapsize - (MYSIZE - 1); xb++) {
		for (int yb = 0; yb < mapsize - (MYSIZE - 1); yb++) {
			char* smallMap = CreateMap(MYSIZE);
			for (int x = 0; x < MYSIZE; x++) {
				for (int y = 0; y < MYSIZE; y++) {
					smallMap[MYSIZE * x + y] = commonMap[mapsize * (x + xb) + (y + yb)];
				}
			}
			if (findHumanWinPos3x3(smallMap, choise, computerchoise) != NULL) {
				cort* ret = findHumanWinPos3x3(smallMap, choise, computerchoise);
				(*ret).x += xb;
				(*ret).y += yb;
				return ret;
			}
		}
	}
	return NULL;
}
// сичтает, сколько фигурок на поле( на вход получает поле и размер поля)
int amountOfXO(char* commonmap,int mapsize) {
	int count = 0;
	for (int x = 0; x < mapsize; x++) {
		for (int y = 0; y < mapsize; y++) {
			if (commonmap[mapsize * x + y] != SPACE)
				count = count + 1;
		}
	}
	return count;
}
// ищет оптимальный ход для компьютера, если человека не надо блокировать и нет выигрышной комбинации( на вход получает поле, размер поля,  фигурку человека, фигурку компьютера)
cort* CompStep(char* commonMap, int mapsize, char choise, char computerchoise) {
	cort* ret = (cort*)malloc(sizeof(cort));
	int anchoise = amountOfXO(commonMap, mapsize);
	if (anchoise == 1) {
		int center = mapsize / 2;
		if (commonMap[mapsize * center + center] != SPACE) {
			(*ret).x = center - 1;
			(*ret).y = center;
			return ret;
		}
		else {
			(*ret).x = center;
			(*ret).y = center;
			return ret;
		}
	}
	else if(anchoise>=2) {
		int xpos ;
		int ypos ;
		for (int x = 0; x < mapsize; x++) {
			for (int y = 0; y < mapsize; y++) {
				if (commonMap[mapsize * x + y] == computerchoise) {
					 xpos = x;
					 ypos = y;
				}
			}
		}
		for (int x=0; x < mapsize; x++) {
			for (int y=0; y < mapsize; y++) {
				if (xpos - 1 <= x && x <= xpos + 1 && ypos - 1 <= y && y <= ypos + 1) {
					if (commonMap[mapsize * x + y] == SPACE) {
						(*ret).x = x;
						(*ret).y = y;
						return ret;
					}
				}
			}
		}
	}
}
// функция, создающая цвета
void InitColor() {
	//HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleMode(hOut, 7);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	BOOL ret = SetConsoleMode(hOut, dwMode);
	DWORD retdw = GetLastError();

}