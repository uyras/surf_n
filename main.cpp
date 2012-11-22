#include<iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "Part.h"
#include "vect.h"
#include "config.cpp"
#include "PartArray.h"

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

int main() {
	Part* temp; //временныый элемент частицы, для ускорения кода

	//инициализируем 3-х мерный массив parts
	PartArray parts(N, 1, M);
	parts.setAntiferr();

	float minH = 0, maxH = 0, averH = 0; //минимальное максимальное и среднее значения модуля поля взаимодействия
	float minHx = 0, maxHx = 0, averHx = 0; //тут понятно по подобию
	float minHz = 0, maxHz = 0, averHz = 0;
	double mTotal = 0;
	vector<double> intMod, intX, intZ;
	bool first = true;
	int tick = 0;
	//выполнение рассчетов
	parts.calcInteraction();
	//	for (int i = 0; i < N; i++) {
	//		int j = 0;
	//		//for (int j = 0; j < N; j++) {
	//		for (int k = 0; k < M; k++) {
	//			parts.calcInteraction(i, j, k);
	//			temp = parts.getElem(i, j, k);
	//
	//			intMod.push_back(temp->intMod);
	//			intX.push_back(temp->interaction.x);
	//			intZ.push_back(temp->interaction.z);
	//
	//			mTotal += temp->m.length();
	//
	//			if (first) {
	//				minH = maxH = averH = temp->intMod;
	//				minHx = maxHx = averHx = temp->interaction.x;
	//				minHz = maxHz = averHz = temp->interaction.z;
	//				first = false;
	//			} else {
	//				if (minH > temp->intMod) minH = temp->intMod;
	//				if (maxH < temp->intMod) maxH = temp->intMod;
	//				averH = (averH * tick + temp->intMod) / (tick + 1);
	//
	//				if (minHx > temp->interaction.x) minHx = temp->interaction.x;
	//				if (maxHx < temp->interaction.x) maxHx = temp->interaction.x;
	//				averHx = (averHx * tick + temp->interaction.x) / (tick + 1);
	//
	//				if (minHz > temp->interaction.z) minHz = temp->interaction.z;
	//				if (maxHz < temp->interaction.z) maxHz = temp->interaction.z;
	//				averHz = (averHz * tick + temp->interaction.z) / (tick + 1);
	//			}
	//			tick++;
	//		}
	//		//}
	//	}
	parts.calcEnergy1();
	parts.calcEnergy2();
	parts.cout();
	parts.draw();
	bool rotated = true;
	while (rotated) {
		rotated = false;
		for (int k = 0; k < M; k++) {
			int j = 0;
			//for (int j = 0; j < N; j++) {
			for (int i = 0; i < N; i++) {
				temp = parts.getElem(i, j, k);
				if (temp->intMod > hc && temp->interaction.scalar(temp->m) < 0) {
					cout << "rotate with x=" << i << "; z=" << k << endl;
					temp->m.rotate();
					temp->axis.rotate();
					parts.calcInteraction();
					rotated = true;

				} else {
					cout << "normal with x=" << i << "; z=" << k << endl;
				}
				if (rotated) break;
			}
			if (rotated) break;
		}
	}
	parts.calcEnergy1();
	parts.calcEnergy2();
	parts.cout();
	parts.draw();

	//printDiff(minH, maxH, 50, &intMod, "d:\\H.txt");
	//printDiff(minHx, maxHx, 50, &intX, "d:\\Hx.txt");
	//printDiff(minHz, maxHz, 50, &intZ, "d:\\Hz.txt");

	cout << "MTotal:\t" << mTotal << endl;
	cout << "total:\t" << total;
}


