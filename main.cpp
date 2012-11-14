#include<iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>

using namespace std;

int N = 6, total = N * N*N; //count of particles
double
I0s = 500, //намагниченность насыщения в гаусс,
		hc = 50, //критическое поле в эрстедах,
		vol = 0.5, //объем в микронах кубических,
		dstep = 1E-4, //параметр решетки в микронах
		kv = 1E-12, //перевод к сантиметрам кубическим и к сантиметрам,
		md; //модуль магнитного момента
//векторы

struct vect {
	double x, y, z;
};

//класс частицы

class Part {
public:
	int x, y, z; //координаты, для удобства в относительных величинах
	double absX, absY, absZ; //абсолютные координаты
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
			a.y * b.y +
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
			(a.y - b.y)*(a.y - b.y) +
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
			a.y * a.y +
			a.z * a.z
			);
}

int main() {
	Part* temp; //временныый элемент частицы, для ускорения кода

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
				temp->x = i;
				temp->y = j;
				temp->z = k;

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

	//выполнение рассчетов
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				temp = &parts[i][j][k];
				temp->calcInteraction(&parts);
				temp->intMod = length(temp->interaction);
			}
		}
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

void Part::calcInteraction(vector < vector < vector < Part > > >* parts) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				if (i != this->x && j != this->y && k != this->z) { //не считать взаимодействие частицы на себя
					this->interaction.x += 1;
					this->interaction.y += 1;
					this->interaction.z += 1;
				}
			}
		}
	}
}
