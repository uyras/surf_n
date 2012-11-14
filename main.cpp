#include<iostream>
#include<fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>

using namespace std;

int N = 6, total = N * N * N; //count of particles
double
I0s = 500, //намагниченность насыщения в гаусс,
		hc = 50, //критическое поле в эрстедах,
		vol = 0.5, //объем в микронах кубических,
		dstep = 0.0001, //параметр решетки в микронах (множитель)
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
double length(vect a){
	return sqrt(
			a.x * a.x +
			//a.y * a.y +
			a.z * a.z
			);
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
				temp->absPos.x =  i * dstep;
				temp->absPos.y =  j * dstep;
				temp->absPos.z =  k * dstep;

				//магн. моменты
				temp->m.x = kv * vol * temp->axis.x * I0s;
				temp->m.y = kv * vol * temp->axis.y * I0s;
				temp->m.z = kv * vol * temp->axis.z * I0s;
			}
		}
	}
	cout << "X\tY\tZ\tMx\tMy\tMz\tHx\tHy\tHz\t|H|" << endl;
	f << "X\tY\tZ\tMx\tMy\tMz\tHx\tHy\tHz\t|H|" << endl;
	//выполнение рассчетов
	for (int i = 0; i < N; i++) {
		int j=0;
		//for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				temp = &parts[i][j][k];
				temp->calcInteraction(&parts);
				temp->intMod = length(temp->interaction);
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
			}
		//}
		}

	//так как каждый элемент решетки имеет электрон - то доступ к каждому электрону с координатами {x,y,z}
	//будет происходить по parts[x][y][z]
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

void Part::calcInteraction(vector < vector < vector < Part > > >* parts2) {
	vector < vector < vector < Part > > > parts = *parts2;
	Part* temp; //временныый элемент частицы, для ускорения кода
	for (int i = 0; i < N; i++) {
		int j=0;
		//for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				if (i != this->pos.x || j != this->pos.y || k != this->pos.z) { //не считать взаимодействие частицы на себя
					temp = &parts[i][j][k];
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
