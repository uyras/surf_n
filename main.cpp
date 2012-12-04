#include<iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "Part.h"
#include "vect.h"
#include "config.cpp"
#include "PartArray.h"
#include "distributionLaw.h"

using namespace std;

double md;

vector<int> calcDiff(vector < double >* parts, vector<int> step) {
	int stepSize = step.size(); //размер шага
	int partsSize = (*parts).size();
	vector <int> res(stepSize + 1, 0);
	double temp; //временныый элемент частицы, для ускорения кода
	for (int i = 0; i < partsSize; i++) {
		temp = (*parts)[i];
		//проверяем первый элемент и меньше
		if (temp < step[0])
			res[0]++;
		//проверяем остальные
		for (int m = 1; m < stepSize; m++)
			if (temp >= step[m - 1] && temp < step[m])
				res[m]++;
		//в бесконечность и далее! ))
		if (temp > step[stepSize - 1])
			res[stepSize]++;
	}
	return res;
}

void printDiff(double min, double max, int steps, vector<double>* elems, char* fName) {
	ofstream f(fName);
	//проверяем и выдаем распределение значений
	cout << endl << endl << endl;
	vector<int> step;

	min = floor(min / 100.) * 100.;
	for (int i = (int) min; i <= max; i += steps) {
		step.push_back(i);
	}

	//считаем распределение
	vector<int> raspr;
	raspr = calcDiff(elems, step);

	//всё что ниже минимальной точки
	cout << "less " << step[0] << "\t" << (float) raspr[0] / total << endl;
	f << "less " << step[0] << "\t" << (float) raspr[0] / total << endl;

	//все что между интервалами
	for (int i = 1; i < step.size(); i++) {
		cout << step[i - 1] << " to " << step[i] << "\t" << (float) raspr[i] / total << endl;
		f << step[i - 1] << " to " << step[i] << "\t" << (float) raspr[i] / total << endl;
	}

	//все что выше максимальной точки
	cout << "more " << step.back() << "\t" << (float) raspr[step.size()] / total << endl;
	f << "more " << step.back() << "\t" << (float) raspr[step.size()] / total << endl;
}

void writefile(vector<double> &history, const char*file) {
	vector<double>::iterator iter;
	fstream f;
	f.open(file, fstream::trunc | fstream::out);
	iter = history.begin();
	while (iter != history.end()) {
		f << (*iter)*1e06 << endl;
		iter++;
	}
	f.close();
}

int main() {
	cout << fixed << setprecision(15); //устанавливаем формат вывода, 15 знаков после запятой
ofstream f("d:\\2.txt");
	distributionLaw distribution;
	int* distrVal;
	double* lables;
	PartArray parts(N, 1, M); //инициализируем 3-х мерный массив parts
	parts.setLines(); //устанавливаем магнитные моменты
	parts.calcEnergy1();



	distribution.values = parts.getHZVector();
	distrVal = distribution.calculate(100);
	lables = distribution.getLables(100);
	for (int i=0;i<100;i++){
		f<<lables[i]<<"\t"<<distrVal[i]<<endl;
	}
}


