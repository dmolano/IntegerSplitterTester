// IntegerSplitterTester.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>

#define INTEGER_SPLITTER_NULL 0
#define INTEGER_SPLITTER_NO_ERROR 0
#define INITIAL_STATE_SPLITTER 0
#define SEPARATOR_STATE_SPLITTER 1
#define DIGIT_STATE_SPLITTER 2
#define ERROR_STATE_SPLITTER 3
#define MAX_DIGITS_LENGTH 5 // 99999
#define BASE_10 10
#define LAST_SEPARATOR 0

typedef struct IntegerSplit { //es el nombre de la estructura
	int integer;
	int separator;
	struct IntegerSplit* next;
}IntegerSplitType;//es el tipo de dato para declarar la estructura

typedef IntegerSplitType* IntegerSplitPtr;

//
// integerSplitter_Init
//
int integerSplitter_Init(IntegerSplitPtr* integerSplitPtr) {
	IntegerSplitPtr integerSplitListIndex = *integerSplitPtr;
	while (integerSplitListIndex != INTEGER_SPLITTER_NULL) {
		IntegerSplitPtr next = integerSplitListIndex->next;
		free((void*)integerSplitListIndex);
		integerSplitListIndex = next;
	}
	*integerSplitPtr = integerSplitListIndex;
	if (integerSplitListIndex != NULL) {
		return INTEGER_SPLITTER_NO_ERROR + 1;
	}
	return INTEGER_SPLITTER_NO_ERROR;
}

//
// integerSplitter_New
//
IntegerSplitPtr integerSplitter_New(int integer, int separator) {
	IntegerSplitPtr integerSplitPtrNew = (IntegerSplitPtr)malloc(sizeof(IntegerSplitType));
	integerSplitPtrNew->integer = integer;
	integerSplitPtrNew->separator = separator;
	integerSplitPtrNew->next = INTEGER_SPLITTER_NULL;
	return integerSplitPtrNew;
}

//
// integerSplitter_Add
//
IntegerSplitPtr integerSplitter_Add(IntegerSplitPtr* integerSplitPtr, int integer, int separator) {
	IntegerSplitPtr integerSplitPtrNew = INTEGER_SPLITTER_NULL;
	if (*integerSplitPtr == INTEGER_SPLITTER_NULL) {
		*integerSplitPtr = integerSplitter_New(integer, separator);
		integerSplitPtrNew = *integerSplitPtr;
	}
	else {
		IntegerSplitPtr integerSplitPtrIndex = *integerSplitPtr;
		while (integerSplitPtrIndex->next != INTEGER_SPLITTER_NULL) {
			integerSplitPtrIndex = integerSplitPtrIndex->next;
		}
		integerSplitPtrNew = integerSplitter_New(integer, separator);
		integerSplitPtrIndex->next = integerSplitPtrNew;
	}
	return integerSplitPtrNew;
}

//
// integerSplitter_AddDigit
//
int integerSplitter_AddDigit(int* digitsIndex, char* digits, int character) {
	int result;
	if ((*digitsIndex) < MAX_DIGITS_LENGTH) {
		digits[(*digitsIndex)++] = character;
		result = DIGIT_STATE_SPLITTER;
	}
	else {
		result = ERROR_STATE_SPLITTER;
	}
	return result;
}

//
// integerSplitter_addStoredData
//
int integerSplitter_addStoredData(char* digits, IntegerSplitPtr* integerSplitList, int character, int* digitsIndex) {
	int state;
	char* endptr;
	int length = strtol(digits, &endptr, BASE_10);
	if (length > 0) {
		integerSplitter_Add(integerSplitList, length, character);
		memset(digits, 0, MAX_DIGITS_LENGTH);
		*digitsIndex = 0;
		state = SEPARATOR_STATE_SPLITTER;
	}
	else {
		state = ERROR_STATE_SPLITTER;
	}
	return state;
}

//
// integerSplitter_Split
//
IntegerSplitPtr integerSplitter_Split(const char* string) {
	IntegerSplitPtr integerSplitList = INTEGER_SPLITTER_NULL;
	size_t lengthString = strlen(string);
	size_t index;
	int state;
	char digits[MAX_DIGITS_LENGTH + 1];
	int digitsIndex;
	for (index = 0, state = INITIAL_STATE_SPLITTER; (index < lengthString) && (state != ERROR_STATE_SPLITTER); index++) {
		int character = string[index];
		switch (state) {
		case INITIAL_STATE_SPLITTER:
			memset(digits, 0, MAX_DIGITS_LENGTH);
			digitsIndex = 0;
			if (isdigit(character)) {
				// Almaceno el número, si puedo hacerlo.
				state = integerSplitter_AddDigit(&digitsIndex, digits, character);
			}
			else if (!(iswalnum(character) || iswspace(character))) {
				state = ERROR_STATE_SPLITTER;
			}
			else {
				state = ERROR_STATE_SPLITTER;
			}
			break;
		case SEPARATOR_STATE_SPLITTER:
			if (isdigit(character)) {
				// Almaceno el número, si puedo hacerlo.
				state = integerSplitter_AddDigit(&digitsIndex, digits, character);
			}
			else if (!(iswalnum(character) || iswspace(character))) {
				state = ERROR_STATE_SPLITTER;
			}
			else {
				state = ERROR_STATE_SPLITTER;
			}
			break;
		case DIGIT_STATE_SPLITTER:
			if (isdigit(character)) {
				// Almaceno el número, si puedo hacerlo.
				state = integerSplitter_AddDigit(&digitsIndex, digits, character);
			}
			else if (!(iswalnum(character) || iswspace(character))) {
				state = integerSplitter_addStoredData(digits, &integerSplitList, character, &digitsIndex);
			}
			else {
				state = ERROR_STATE_SPLITTER;
			}
			break;
		default:
			state = ERROR_STATE_SPLITTER;
			break;
		}
	}
	if ((state != ERROR_STATE_SPLITTER) && (index == lengthString)) {
		state = integerSplitter_addStoredData(digits, &integerSplitList, LAST_SEPARATOR, &digitsIndex);
	}
	if (state == ERROR_STATE_SPLITTER) {
		integerSplitter_Init(&integerSplitList);
	}
	return integerSplitList;
}

void test(const char* string) {
	IntegerSplitPtr integerSplitList = INTEGER_SPLITTER_NULL;
	std::cout << "Hello World!\n";
	if (integerSplitter_Init(&integerSplitList) == INTEGER_SPLITTER_NO_ERROR) {
		std::cout << "integerSplitList inicializado.\n";
	}
	else {
		std::cout << "integerSplitList no inicializado.\n";
	}
	integerSplitList = integerSplitter_Split(string);
	if (integerSplitList == INTEGER_SPLITTER_NULL) {
		std::cout << "integerSplitList no cargado.\n";
	}
	else {
		IntegerSplitPtr integerSplitListIndex = integerSplitList;
		while (integerSplitListIndex != INTEGER_SPLITTER_NULL) {
			char buffer[200];
			sprintf_s(buffer, 200, "%d-%d\n", integerSplitListIndex->integer, integerSplitListIndex->separator);
			std::cout << "---\n";
			std::cout << buffer;
			integerSplitListIndex = integerSplitListIndex->next;
		}
		if (integerSplitter_Init(&integerSplitList) == INTEGER_SPLITTER_NO_ERROR) {
			std::cout << "integerSplitList inicializado.\n";
		}
		else {
			std::cout << "integerSplitList no inicializado.\n";
		}
	}
}

int main()
{
	test("");
	test("0");
	test(";1");
	test(";");
	test(";;");
	test("1;");
	test(";1");
	test(" ");
	test("1;2;3;04");
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln