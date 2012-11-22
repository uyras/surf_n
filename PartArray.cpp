/*
 * File:   PartArray.cpp
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 17:22
 */

#include "PartArray.h"
#include "config.cpp"
#include <cmath>
#include <iostream>
#include <time.h>
#include <stdlib.h>

std::vector < std::vector < std::vector < Part > > >::iterator l1Iterator;
std::vector < std::vector < Part > >::iterator l2Iterator;
std::vector < Part >::iterator l3Iterator;

PartArray::PartArray(int x, int y, int z) {
	this->E1 = this->E2 = 0;
	Part* temp;
	this->parts.resize(x);
	for (int i = 0; i < x; i++) {
		this->parts[i].resize(y);
		for (int j = 0; j < y; j++) {
			this->parts[i][j].resize(z);
			for (int k = 0; k < z; k++) {
				temp = &this->parts[i][j][k];

				//координаты частицы (для удобства)
				temp->pos.x = i;
				temp->pos.y = j;
				temp->pos.z = k;

				//абсолютные координаты частицы
				temp->absPos.x = i * dstep;
				temp->absPos.y = j * dstep;
				temp->absPos.z = k * dstep;
			}
		}
	}
}

void PartArray::calcM() {
	Part* temp;
	l1Iterator = this->parts.begin();
	while (l1Iterator != this->parts.end()) {
		l2Iterator = (*l1Iterator).begin(); //по оси Y только первый элемент

		l3Iterator = (*l2Iterator).begin();
		while (l3Iterator != (*l2Iterator).end()) {
			temp = l3Iterator.base();
			temp->m.x = kv * vol * temp->axis.x * I0s;
			temp->m.y = kv * vol * temp->axis.y * I0s;
			temp->m.z = kv * vol * temp->axis.z * I0s;
			++l3Iterator;
		}
		++l1Iterator;
	}
}

Part* PartArray::getElem(int x, int y, int z) {
	return &this->parts.at(x).at(y).at(z);
}

void PartArray::calcInteraction(int x, int y, int z) {
	Part* elem = &this->parts[x][y][z]; //элемент, над которым производятся операции
	elem->interaction = vect(0, 0, 0);
	elem->intMod = 0;
	double r, r2, r5, part, dx, dy, dz;
	l1Iterator = this->parts.begin();
	while (l1Iterator != this->parts.end()) {
		l2Iterator = (*l1Iterator).begin(); //по оси Y только первый элемент

		l3Iterator = (*l2Iterator).begin();
		while (l3Iterator != (*l2Iterator).end()) {
			if (x != (*l3Iterator).pos.x || y != (*l3Iterator).pos.y || z != (*l3Iterator).pos.z) { //не считать взаимодействие частицы на себя
				r = (*l3Iterator).absPos.space(elem->absPos);
				r2 = r * r; //радиус в кубе
				r5 = r2 * r * r * r; //радиус в пятой
				dx = elem->absPos.x - (*l3Iterator).absPos.x;
				dy = elem->absPos.y - (*l3Iterator).absPos.y;
				dz = elem->absPos.z - (*l3Iterator).absPos.z;
				part = (*l3Iterator).m.x * dx + (*l3Iterator).m.y * dy + (*l3Iterator).m.z * dz;
				elem->interaction.x += ((*l3Iterator).m.x * r2 - (3 * part * dx)) / r5;
				elem->interaction.y += ((*l3Iterator).m.y * r2 - (3 * part * dy)) / r5;
				elem->interaction.z += ((*l3Iterator).m.z * r2 - (3 * part * dz)) / r5;
				//elem->interaction.z += (3 * part * dz) / r5 - (*l3Iterator).m.z * r2 / r3;
			}
			++l3Iterator;
		}
		++l1Iterator;
	}
	elem->intMod = elem->interaction.length();
}

void PartArray::calcInteraction() {
	int i, j, k;
	for (i = 0; i < N; i++) {
		j = 0;
		//for (j = 0; j < N; j++) {
		for (k = 0; k < M; k++) {
			this->calcInteraction(i, j, k);
		}
		//}
	}
}

void PartArray::calcEnergy1() {
	this->E1 = 0;
	int i, j, k;
	for (i = 0; i < N; i++) {
		j = 0;
		//for (j = 0; j < N; j++) {
		for (k = 0; k < M; k++) {
			this->E1 += this->calcEnergy1(i, j, k);
		}
		//}
	}
	this->E1 *= 0.5;
}

double PartArray::calcEnergy1(int x, int y, int z) {
	Part* elem = &this->parts[x][y][z]; //элемент, над которым производятся операции
	double r, r2, r5, E = 0;
	vect rij;
	l1Iterator = this->parts.begin();
	while (l1Iterator != this->parts.end()) {
		l2Iterator = (*l1Iterator).begin(); //по оси Y только первый элемент

		l3Iterator = (*l2Iterator).begin();
		while (l3Iterator != (*l2Iterator).end()) {
			if (x != (*l3Iterator).pos.x || y != (*l3Iterator).pos.y || z != (*l3Iterator).pos.z) { //не считать взаимодействие частицы на себя
				rij = elem->absPos.radius((*l3Iterator).absPos);
				r = rij.length();
				r2 = r * r; //радиус в кубе
				r5 = r2 * r * r * r; //радиус в пятой
				E +=
						(((*l3Iterator).m.scalar(elem->m) * r2)
						-
						(3 * elem->m.scalar(rij) * (*l3Iterator).m.scalar(rij))) / r5;
			}
			++l3Iterator;
		}
		++l1Iterator;
	}
	return E;
}

void PartArray::calcEnergy2() {
	this->E2 = 0;
	int i, j, k;
	Part* elem;
	for (i = 0; i < N; i++) {
		j = 0;
		//for (j = 0; j < N; j++) {
		for (k = 0; k < M; k++) {
			elem = &this->parts[i][j][k];
			this->E2 -= elem->interaction.scalar(elem->m);
		}
		//}
	}
	this->E2 /= 2.;
}

void PartArray::cout() {
	std::cout << "X\tY\tZ\tMx\tMy\tMz\t\tHx\tHy\tHz\t|H|" << std::endl;
	l1Iterator = this->parts.begin();
	while (l1Iterator != this->parts.end()) {
		l2Iterator = (*l1Iterator).begin(); //по оси Y только первый элемент

		l3Iterator = (*l2Iterator).begin();
		while (l3Iterator != (*l2Iterator).end()) {
			std::cout
					<< (*l3Iterator).absPos.x << "\t"
					<< (*l3Iterator).absPos.y << "\t"
					<< (*l3Iterator).absPos.z << "\t"
					<< (*l3Iterator).m.x << "\t"
					<< (*l3Iterator).m.y << "\t"
					<< (*l3Iterator).m.z << "\t\t"
					<< (*l3Iterator).interaction.x << "\t"
					<< (*l3Iterator).interaction.y << "\t"
					<< (*l3Iterator).interaction.z << "\t"
					<< (*l3Iterator).intMod << std::endl;

			++l3Iterator;
		}
		++l1Iterator;
	}
	std::cout << "E1 : " << this->E1 << "; E2 : " << this->E2 << std::endl;
}

void PartArray::draw() {
	Part* temp;
	int i, j = 0, k;
	for (i = 0; i < N; i++) {
		for (k = 0; k < M; k++) {
			temp = &this->parts[i][j][k];
			if (temp->m.z > 0)
				std::cout << "i ";
			else
				std::cout << "! ";
		}
		std::cout << std::endl;
	}
}

void PartArray::setAntiferr() {
	l1Iterator = this->parts.begin();
	while (l1Iterator != this->parts.end()) {
		l2Iterator = (*l1Iterator).begin(); //по оси Y только первый элемент

		l3Iterator = (*l2Iterator).begin();
		while (l3Iterator != (*l2Iterator).end()) {
			if ((int)((*l3Iterator).pos.x + (*l3Iterator).pos.y) % 2 == 1) (*l3Iterator).axis.z = -1.;
			else (*l3Iterator).axis.z = 1;
			(*l3Iterator).axis.y = 0.;
			(*l3Iterator).axis.x = 0.;
			++l3Iterator;
		}
		++l1Iterator;
	}
	this->calcM();
}

void PartArray::setAllUp() {
	l1Iterator = this->parts.begin();
	while (l1Iterator != this->parts.end()) {
		l2Iterator = (*l1Iterator).begin(); //по оси Y только первый элемент

		l3Iterator = (*l2Iterator).begin();
		while (l3Iterator != (*l2Iterator).end()) {
			(*l3Iterator).axis.z = 1.;
			(*l3Iterator).axis.y = 0.;
			(*l3Iterator).axis.x = 0.;
			++l3Iterator;
		}
		++l1Iterator;
	}
	this->calcM();
}

void PartArray::setRandomZ() {
	srand(time(NULL));
	l1Iterator = this->parts.begin();
	while (l1Iterator != this->parts.end()) {
		l2Iterator = (*l1Iterator).begin(); //по оси Y только первый элемент

		l3Iterator = (*l2Iterator).begin();
		while (l3Iterator != (*l2Iterator).end()) {
			if (rand() % 2 == 1) (*l3Iterator).axis.z = 1.;
			else (*l3Iterator).axis.z = -1.;

			(*l3Iterator).axis.y = 0.;
			(*l3Iterator).axis.x = 0.;

			++l3Iterator;
		}
		++l1Iterator;
	}
	this->calcM();
}