#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
char calc( char* gen,int len, int* pas, char* var, int k, char * op );
void clean( char* gen );
void NOT( char* gen, int* pas, char* var, int k, int i );
void AND( char* gen, int* pas, char* var, int k, int i );
void OR ( char* gen, int* pas, char* var, int k, int i );
void XOR( char* gen, int* pas, char* var, int k, int i );
void IMP( char* gen, int* pas, char* var, int k, int i );
void EQU( char* gen, int* pas, char* var, int k, int i );
void prioritet( char* gen, int* pas, char* var, int k, int strt, int endf,  char * op );
int main( ) {
	setlocale( LC_ALL, "RUS" );
	system( "title Логический калькулятор " );
	char operations [8] = { '!', '&', '|', '^', '>', '~', '(', ')'};
	char* oper = operations;
	printf( "Символы операций:\n" ); 	printf( "NOT - !, AND - &, OR - |, XOR - ^, IMP - >, EQU - ~,  +скобки\n" ); 	printf( "Примеры выражений: k&(O|!t), m&r|a&k\n" ); 	printf( "Введите уравнение с '=' >>> " );
	char str [100]; int lenth = 0;
	gets( str );
	for (int i = 0; i < sizeof( str ); i++) { if (str [i] == '=') break;	lenth = i + 1; }
	char formula [50] = { ' ' }; char * fo = formula;
	int kolvo=0; char* varia = ( char* ) calloc( kolvo, sizeof( int ) );
	for (int i = 0; i < lenth; i++) formula [i] = str [i];
	int flag = 0;
	for (int i = 0; i < sizeof(formula); i++) {
		if (formula [i] == ' ') break;
		flag = 0;
		for (int j = 0; j < sizeof( operations ); j++) {
			if (operations [j] == formula [i]) { 
				flag = 1;
				break;
			}
		}
		if (flag != 1) {
			for (int j = 0; j < kolvo;j++) if (varia [j] == formula [i]) {
				flag = 1;
				break;
			}
			if (flag != 1) {
				varia = ( char* ) realloc( varia, sizeof( char ) * ( ++kolvo ) );
				varia [kolvo - 1] = formula [i];
			}
		}
	}kolvo--;
	int* pass = ( int* ) calloc( kolvo, sizeof( int ) );
	for (int i = 0; i < kolvo; i++) printf( "| %c ", varia [i] );
	printf( "|  РЕЗУЛЬТАТ\n" );
	for (int i = 0; i < pow( 2, kolvo ); i++) {
		for (int g = 0; g < kolvo; g++) {
			int r = floor( i / pow( 2, kolvo - g - 1 ) );
			pass [g] = r % 2;
			printf( "| %d ", pass [g] );
		}
		if (calc( fo,lenth, pass, varia, kolvo, oper ) == '0') printf( "|  ЛОЖЬ\n" );
		else printf( "|  ИСТИНА\n" );
		for (int i = 0; i < lenth; i++) formula [i] = str [i];
	}
}
char calc( char* gen,int len, int* pas, char* var, int k, char * op ) {
	//заполнение формулы по текущим значениям
	for (int i = 0; i < len; i++) { 
		for (int j = 0; j < k; j++) {
			if (var [j] == gen [i]) {
				if (pas [j] == 0)  gen [i] = '0';
				else gen [i] = '1';
				break;
			}
		}
	} 
	//поиск скобок и выполнение операций в них
	for (int i = 0; i < len; i++) {
		if (gen [i] == ')') for (int j = i; j >= 0; j--) {
			if (gen [j] == '(') {
				prioritet( gen, pas, var, k, j, i, op );
				gen [j] = '?';
				gen [j+2] = '?';
				clean(gen );
				i=0; j=0;
			}
		}
	}//выполнение остатков
	prioritet( gen, pas, var, k, 0, len, op );
	return gen [0];
}
void clean( char* gen) {
	int trig = 0; int konec=0;
	for (int i = 0; i < 50; i++){
		if (gen [i] == '?') trig++;
		if (gen[i]!=' ') konec++;
	}
	//сдвиг на лишнее
	for (int i = konec; i >= 0; i--) {
		if (gen [i] == '?') {
			for (int j = i; j < konec; j++) gen [j] = gen [j + 1];
		}
	}
	//чистим лишнее
	while (1) {
		gen[konec-trig]= ' ';
		trig--;
		if (trig==0) break;
	}	
}
void AND( char* gen, int* pas, char* var, int k, int i) {
	int tem1 = 0, tem2 = 0;
	if (gen [i - 1] == '0') tem1 = 0;
	else tem1 = 1;
	if (gen [i + 1] == '0') tem2 = 0;
	else tem2 = 1;
	if (tem1 == 1 && tem2 == 1) gen [i - 1] = '1';
	else gen [i - 1] = '0';
	gen [i] = '?';
	gen [i + 1] = '?';
	clean( gen);
}
void OR ( char* gen, int* pas, char* var, int k, int i) {
	int tem1 = 0, tem2 = 0;
	if (gen [i - 1] == '0') tem1 = 0;
	else tem1 = 1;
	if (gen [i + 1] == '0') tem2 = 0;
	else tem2 = 1;
	if (tem1 == 0 && tem2 == 0) gen [i - 1] = '0';
	else gen [i - 1] = '1';
	gen [i] = '?';
	gen [i + 1] = '?';
	clean( gen);
}
void XOR( char* gen, int* pas, char* var, int k, int i) {
	int tem1 = 0, tem2 = 0;
	if (gen [i - 1] == '0') tem1 = 0;
	else tem1 = 1;
	if (gen [i + 1] == '0') tem2 = 0;
	else tem2 = 1;
	if (tem1 == tem2 ) gen [i - 1] = '0';
	else gen [i - 1] = '1';
	gen [i] = '?';
	gen [i + 1] = '?';
	clean( gen);
}
void IMP( char* gen, int* pas, char* var, int k, int i) {
	int tem1 = 0, tem2 = 0;
	if (gen [i - 1] == '0') tem1 = 0;
	else tem1 = 1;
	if (gen [i + 1] == '0') tem2 = 0;
	else tem2 = 1;
	if (tem1 == 1 && tem2==0) gen [i - 1] = '0';
	else gen [i - 1] = '1';
	gen [i] = '?';
	gen [i + 1] = '?';
	clean( gen);
}
void EQU( char* gen, int* pas, char* var, int k, int i) {
	int tem1 = 0, tem2 = 0;
	if (gen [i - 1] == '0') tem1 = 0;
	else tem1 = 1;
	if (gen [i + 1] == '0') tem2 = 0;
	else tem2 = 1;
	if (tem1 == tem2 ) gen [i - 1] = '1';
	else gen [i - 1] = '0';
	gen [i] = '?';
	gen [i + 1] = '?';
	clean( gen);
}
void NOT( char* gen, int* pas, char* var, int k, int i) {
	int tem = 0;
	if (gen [i + 1] == '0') tem = 0;
	else tem = 1;
	if (tem == 0)  gen [i] = '1';
	else gen [i] = '0';
	gen [i + 1] = '?';
	clean( gen);
}
void prioritet( char* gen, int* pas, char* var, int k, int strt, int endf, char * op) {
	int gg=0;
	while (gg < 6) {
		for (int i = strt; i < endf; i++) {
			int trig = 0;
			if (gen [i] == op [gg]) {
				switch (gg)
				{
				case 0:
					NOT( gen, pas, var, k, i);  
					break; 
				case 1:
					AND( gen, pas, var, k, i);
					break;
				case 2:
					OR( gen, pas, var, k, i);
					break;
				case 3:
					XOR( gen, pas, var, k, i);
					break;
				case 4:
					IMP( gen, pas, var, k, i);
					break;
				case 5:
					EQU( gen, pas, var, k, i);
					break;
				default:
					break;
				}
				i = strt; 	trig++;
			}
			if (trig == 0 && i == endf - 1) { gg++; }
		}
	}
}