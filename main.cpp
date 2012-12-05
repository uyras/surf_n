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
	PartArray parts(N, 1, M); //инициализируем 3-х мерный массив parts, N и M берутся из конфигов
	parts.setRandomZ(); //устанавливаем магнитные моменты в случайном порядке
	parts.draw(); //рисуем в консоли их конфигурацию
	parts.cout(); //выдаем параметры на экран для каждой частицы в виде таблицы
	parts.processStep(); //обрабатываем систему частиц, расставляя их в выгодной конфигурации
	parts.draw(); //рисуем в консоли их новую конфигурацию

	//пример работы класса закона распределения
	distributionLaw distribution; //создаем класс, который считает закон распределения
	int* distrVal; //сюда будут сохранены данные закона распределения
	double* lables; //тут будут значения по оси X для каждого интервала из distrVal
	distribution.values = parts.getHZVector(); //к примеру получим все Hz для каждой частицы. для них будет рассчитан закон
	distrVal = distribution.calculate(10); //считаем закон распределения для 10 частиц
	lables = distribution.getLables(10); //тут понятно

	ofstream f("d:\\1.txt"); //открываем файл для записи
	for (int i=0;i<10;i++){ //сохраняем всё это в файл для построения в какой-нибудь программе графиков
		f<<lables[i]<<"\t"<<distrVal[i]<<endl;
	}
}


