#include<iostream>
#include<fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>

using namespace std;

int N = 100, total = N  * N; //count of particles
double
I0s = 500, //намагниченность насыщения в гаусс,
		hc = 50, //критическое поле в эрстедах,
		vol = 0.5, //объем в микронах кубических,
		dstep = 0.00001, //параметр решетки в микронах (множитель)
		kv = 1E-12, //перевод к сантиметрам кубическим и к сантиметрам (множитель для перевода),
		md; //модуль магнитного момента
//векторы

struct vect {
	double x, y, z;
};

//класс частицы

class Part {
public:
	vect pos; //координаты, для удобства в относительных величинах
	vect absPos; //абсолютные координаты
	vect axis; //ось частицы
	vect m; //магнитный момент
	vect interaction; //поле взаимодействия
	double intMod; //модуль (длина) поля взаимодействия

	Part();
	/**
	 * Считает взаимодействия между этой частицей и остальным массивом частиц
	 * @param
	 */
	void calcInteraction(vector < vector < vector < Part > > >*);
};

/**
 * Скалярное произведение двух векторов
 * @param a первый вектор
 * @param b второй вектор
 * @return
 */
double scalar(vect a, vect b) {
	return
	a.x * b.x +
			//a.y * b.y +
			a.z * b.z;
}

/**
 * Находит расстояние между двумя векторами
 * @param a
 * @param b
 * @return
 */
double space(vect a, vect b) {
	return sqrt(
			(a.x - b.x)*(a.x - b.x) +
			//(a.y - b.y)*(a.y - b.y) +
			(a.z - b.z)*(a.z - b.z)
			);
}

/**
 * длина вектора
 * @return
 */
double length(vect a) {
	return sqrt(
			a.x * a.x +
			//a.y * a.y +
			a.z * a.z
			);
}

vector<int> calcDiff(vector < double >* parts, vector<int> step) {
	int stepSize = step.size(); //размер шага
	int partsSize = (*parts).size();
	vector <int> res(stepSize + 1, 0);
	double temp; //временныый элемент частицы, для ускорения кода
	for (int i = 0; i < partsSize; i++) {
			temp =  (*parts)[i];
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

void printDiff(double min, double max, int steps, vector<double>* elems, char* fName){
	ofstream f(fName);
	//проверяем и выдаем распределение значений
	cout << endl << endl << endl;
	vector<int> step;

	min = floor(min/100.) * 100.;
	for (int i=(int)min;i<=max;i+=steps){
		step.push_back(i);
	}

	//считаем распределение
	vector<int> raspr;
	raspr = calcDiff(elems, step);

	//всё что ниже минимальной точки
	cout << "less " << step[0] << "\t" << (float)raspr[0]/total << endl;
	f << "less " << step[0] << "\t" << (float)raspr[0]/total << endl;

	//все что между интервалами
	for (int i = 1; i < step.size(); i++) {
		cout << step[i-1] << " to " << step[i] << "\t" << (float)raspr[i]/total << endl;
		f << step[i-1] << " to " << step[i] << "\t" << (float)raspr[i]/total << endl;
	}

	//все что выше максимальной точки
	cout << "more " << step.back() << "\t" << (float)raspr[step.size()]/total << endl;
	f << "more " << step.back() << "\t" << (float)raspr[step.size()]/total << endl;
}

int main() {
	Part* temp; //временныый элемент частицы, для ускорения кода

	ofstream f("d:\\file.txt");

	//инициализируем 3-х мерный массив parts
	vector < vector < vector < Part > > > parts;
	parts.resize(N);
	for (int i = 0; i < N; i++) {
		parts[i].resize(N);
		for (int j = 0; j < N; j++) {
			parts[i][j].resize(N);
		}
	}

	//задание начальных значений
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				temp = &parts[i][j][k];
				//координаты частицы (для удобства)
				temp->pos.x = i;
				temp->pos.y = j;
				temp->pos.z = k;

				//абсолютные координаты частицы
				temp->absPos.x = i * dstep;
				temp->absPos.y = j * dstep;
				temp->absPos.z = k * dstep;

				//магн. моменты
				temp->m.x = kv * vol * temp->axis.x * I0s;
				temp->m.y = kv * vol * temp->axis.y * I0s;
				temp->m.z = kv * vol * temp->axis.z * I0s;
			}
		}
	}
	cout << "X\tY\tZ\tMx\tMy\tMz\tHx\tHy\tHz\t|H|" << endl;
	f << "X\tY\tZ\tMx\tMy\tMz\tHx\tHy\tHz\t|H|" << endl;
	float minH=0,maxH=0,averH=0; //минимальное максимальное и среднее значения модуля поля взаимодействия
	float minHx=0,maxHx=0,averHx=0; //тут понятно по подобию
	float minHz=0,maxHz=0,averHz=0;
	vector<double> intMod,intX,intZ;
	bool first = true;
	int tick = 0;
	//выполнение рассчетов
	for (int i = 0; i < N; i++) {
		int j = 0;
		//for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
			temp = &parts[i][j][k];
			temp->calcInteraction(&parts);
			temp->intMod = length(temp->interaction);

			intMod.push_back(temp->intMod);
			intX.push_back(temp->interaction.x);
			intZ.push_back(temp->interaction.z);

			if (first){
				minH = maxH = averH = temp->intMod;
				minHx = maxHx = averHx = temp->interaction.x;
				minHz = maxHz = averHz = temp->interaction.z;
				first = false;
			} else {
				if (minH > temp->intMod) minH=temp->intMod;
				if (maxH < temp->intMod) maxH=temp->intMod;
				averH =  ( averH*tick + temp->intMod ) / (tick+1);

				if (minHx > temp->interaction.x) minHx=temp->interaction.x;
				if (maxHx < temp->interaction.x) maxHx=temp->interaction.x;
				averHx =  ( averHx*tick + temp->interaction.x ) / (tick+1);

				if (minHz > temp->interaction.z) minHz=temp->interaction.z;
				if (maxHz < temp->interaction.z) maxHz=temp->interaction.z;
				averHz =  ( averHz*tick + temp->interaction.z ) / (tick+1);
			}
			cout
					<< temp->absPos.x << "\t"
					<< temp->absPos.y << "\t"
					<< temp->absPos.z << "\t"
					<< temp->m.x << "\t"
					<< temp->m.y << "\t"
					<< temp->m.z << "\t"
					<< temp->interaction.x << "\t"
					<< temp->interaction.y << "\t"
					<< temp->interaction.z << "\t"
					<< temp->intMod << endl;
			f
					<< temp->absPos.x << "\t"
					<< temp->absPos.y << "\t"
					<< temp->absPos.z << "\t"
					<< temp->m.x << "\t"
					<< temp->m.y << "\t"
					<< temp->m.z << "\t"
					<< temp->interaction.x << "\t"
					<< temp->interaction.y << "\t"
					<< temp->interaction.z << "\t"
					<< temp->intMod << endl;
			tick++;
		}
		//}
	}

	printDiff(minH,maxH,1000,&intMod,"d:\\H.txt");
	printDiff(minHx,maxHx,1000,&intX,"d:\\Hx.txt");
	printDiff(minHz,maxHz,1000,&intZ,"d:\\Hz.txt");

	cout << "total: " << total;
	system("pause");
}

Part::Part() {
	//оси частиц направлены вдоль оси Z
	this->axis.x = .0;
	this->axis.y = .0;
	this->axis.z = 1.;

	//взаимодействие частиц пока ноль
	this->interaction.x = this->interaction.y = this->interaction.z = this->intMod = 0;
}

void Part::calcInteraction(vector < vector < vector < Part > > >* parts) {
	Part* temp; //временныый элемент частицы, для ускорения кода
	for (int i = 0; i < N; i++) {
		int j = 0;
		//for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
			if (i != this->pos.x || j != this->pos.y || k != this->pos.z) { //не считать взаимодействие частицы на себя
				temp = &(*parts)[i][j][k];
				double r = space(this->absPos, temp->absPos);
				double r3 = pow(r, 3);
				double r5 = pow(r, 5);
				double x = this->absPos.x - temp->absPos.x;
				double y = this->absPos.y - temp->absPos.y;
				double z = this->absPos.z - temp->absPos.z;
				double part = 3 * (temp->m.x * x + temp->m.y * y + temp->m.z * z);
				this->interaction.x += (part * x) / r5 - temp->m.x / r3;
				this->interaction.y += (part * y) / r5 - temp->m.y / r3;
				this->interaction.z += (part * z) / r5 - temp->m.z / r3;
			}
			//}
		}
	}
}
