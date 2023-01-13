#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
char calc( char* gen, int* pas, char* var, int k );
int clean( char* gen );
int inver( char* gen, int* pas, char* var, int k, int i );
int pro( char* gen, int* pas, char* var, int k, int i );
int cumm( char* gen, int* pas, char* var, int k, int i );
int main( ) {
	setlocale( LC_ALL, "RUS" );
	system( "title Логический калькулятор " );
	int kolvo = 5;
	char* varia = ( char* ) calloc( kolvo, sizeof( int ) );
	varia [0] = '&'; varia [1] = '|'; varia [2] = '('; varia [3] = ')'; varia [4] = '!';
	printf( "Символы операций:\n" );
	printf( "Конъюкция - &, дизъюнкция - |, инверсия - !. +скобки\n" );
	printf( "Ввод символов переменных не ограничен(но длина формулы без = не более 8)\n" );
	printf( "Примеры выражений: k&(O|!t), m&r|a&k\n" );
	printf( "Введите уравнение с '=' >>> " );
	char str [100]; int lenth = 0;
	gets( str );
	for (int i = 0; i < sizeof( str ); i++) { if (str [i] == '=') break;	lenth = i + 1; }
	int flag = 0;
	char* general = ( char* ) calloc( lenth, sizeof( char ) );//	ОШИБКА!!!

	for (int i = 0; i < lenth; i++) general [i] = str [i];
	for (int i = 0; i < lenth; i++) {
		flag = 0;
		for (int j = 0; j < kolvo; j++) if (varia [j] == general [i]) flag = 1;
		if (flag != 1) {
			varia = ( char* ) realloc( varia, sizeof( char ) * ( ++kolvo ) );
			varia [kolvo - 1] = general [i];
		}
	}
	for (int i = 0; i < kolvo - 5; i++) varia [i] = varia [i + 5];
	//printf("%d\n",kolvo );
	kolvo -= 5;
	int* pass = ( int* ) calloc( kolvo, sizeof( int ) );
	for (int i = 0; i < kolvo; i++) printf( "| %c ", varia [i] );
	printf( "|  РЕЗУЛЬТАТ\n" );
	for (int i = 0; i < pow( 2, kolvo ); i++) {
		for (int g = 0; g < kolvo; g++) {
			int r = floor( i / pow( 2, kolvo - g - 1 ) );
			pass [g] = r % 2;
			printf( "| %d ", pass [g] );
		}
		if (calc( general, pass, varia, kolvo ) == '0') printf( "|  ЛОЖЬ\n" );
		else printf( "|  ИСТИНА\n" );
		for (int i = 0; i < lenth; i++) general [i] = str [i];
	}
}
char calc( char* gen, int* pas, char* var, int k ) {
	//заполнение формулы по текущим значениям
	for (int i = 0; i < sizeof( gen ); i++) {
		for (int j = 0; j < k; j++) {
			if (var [j] == gen [i]) {
				if (pas [j] == 0)  gen [i] = '0';
				else gen [i] = '1';
			}
		}
	}//поиск скобок, без скобок работает шикарно
	//printf( "\n" ); for (int t = 0; t < 13; t++)printf( "%c ", gen [t] ); printf( "\n" );
	for (int i = 0; i < sizeof( gen ); i++) {
		if (gen [i] == ')') {
			//printf( "\n" ); for (int t = 0; t < 11; t++)printf( "%c ", gen [t] ); printf( "\n" );
			for (int j = i; j > 0; j--) {
				if (gen [j] == '(') {
					//printf("--%d %d--", i, j );
					prioritet( gen, pas, var, k, j, i + 1 );
					//printf( "\n" ); for (int t = 0; t < 11; t++)printf( "%c ", gen [t] ); printf( "\n" );
					gen [j] = '?';
					gen [j + 2] = '?';
					//printf( "\n" ); for (int t = 0; t < 11; t++)printf( "%c ", gen [t] ); printf( "\n" );
					clean( gen );
					i = 0;
				}
			}
		}
	}
	prioritet( gen, pas, var, k, 0, sizeof( gen ) );
	if (gen [0] == '(') return gen [1];
	else gen [0];
}
int clean( char* gen ) {
	int trig = 0;
	for (int i = 0; i < sizeof( gen ); i++) if (gen [i] == '?') {
		trig++;
		break;
	}
	if (trig != 0) {
		for (int i = sizeof( gen ) - 1; i > 0; i--) {//сдвиг на лишнее
			if (gen [i] == '?') {
				for (int j = i; j < sizeof( gen ) - 1; j++) gen [j] = gen [j + 1];
			}
		}
		for (int i = sizeof( gen ) - 1; i > 0; i--) {//унитожаем лишнее
			if (gen [i] == gen [i - 1]) gen [i] = ' ';
		}
	}
}
int pro( char* gen, int* pas, char* var, int k, int i ) {
	int tem1 = 0, tem2 = 0;
	if (gen [i - 1] == '0') tem1 = 0;
	else tem1 = 1;
	if (gen [i + 1] == '0') tem2 = 0;
	else tem2 = 1;
	if (tem1 == 1 && tem2 == 1) gen [i - 1] = '1';
	else gen [i - 1] = '0';
	//printf( "!%c ", gen [i-1] );
	gen [i] = '?';
	gen [i + 1] = '?';
	clean( gen );
}
int cumm( char* gen, int* pas, char* var, int k, int i ) {
	int tem1 = 0, tem2 = 0;
	if (gen [i - 1] == '0') tem1 = 0;
	else tem1 = 1;
	if (gen [i + 1] == '0') tem2 = 0;
	else tem2 = 1;
	if (tem1 == 0 && tem2 == 0) gen [i - 1] = '0';
	else gen [i - 1] = '1';
	//printf( "!%c ", gen [i - 1] );
	gen [i] = '?';
	gen [i + 1] = '?';
	clean( gen );
}
int inver( char* gen, int* pas, char* var, int k, int i ) {
	int tem = 0;
	if (gen [i + 1] == '0') tem = 0;
	else tem = 1;
	if (tem == 0)  gen [i] = '1';
	else gen [i] = '0';
	gen [i + 1] = '?';
	clean( gen );
}
int prioritet( char* gen, int* pas, char* var, int k, int strt, int endf ) {
	for (int i = strt; i < endf; i++) {
		int trig = 0;
		for (int i = strt; i < endf; i++) {
			if (gen [i] == '!') {
				trig++;
				break;
			}
		}
		if (gen [i] == '!') { inver( gen, pas, var, k, i ); i = strt; trig++; }
		if (trig == 0) {
			for (int i = strt; i < endf; i++) {
				if (gen [i] == '&') {
					trig++;
					break;
				}
			}
			if (gen [i] == '&') { pro( gen, pas, var, k, i ); i = strt; trig++; }
			if (trig == 0) {
				for (int i = strt; i < endf; i++) {
					if (gen [i] == '|') {
						trig++;
						break;
					}
				}
				if (gen [i] == '|') { cumm( gen, pas, var, k, i ); i = strt; }
			}
		}
	}
}