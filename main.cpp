#include <cstdlib>
#include <iostream>
#include <iomanip> //для форматного вывода значений на экран
#include <fstream>
#include <vector>
#include <cmath>
#include "config.cpp"
#include "Part.h"
#include "vect.h"
#include "PartArray.h"
#include "distributionLaw.h"

using namespace std;

int main() {
	cout << fixed << setprecision(15); //устанавливаем формат вывода, 15 знаков после запятой

	//пример работы классов
	PartArray parts("d:\\1.txt");
	parts.draw();
	parts.processGroupStep();
	parts.draw();
}