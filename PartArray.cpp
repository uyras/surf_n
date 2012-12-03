/*
 * File:   PartArray.cpp
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 17:22
 */

#include "PartArray.h"
#include "Part.h"
#include "config.cpp"
#include <cmath>
#include <iostream>
#include <time.h>
#include <stdlib.h>


using namespace std;

PartArray::PartArray(int x, int y, int z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->E1 = this->E2 = 0; //обнуляем энергии системы
	Part* temp; //временная частица
	this->parts.resize(x * y * z);
	int i, j, k;
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			for (k = 0; k < z; k++) {
				temp = this->getElem(i, j, k);

				//координаты частицы (для удобства)
				temp->pos.x = i;
				temp->pos.y = j;
				temp->pos.z = k;

				//абсолютные координаты частицы
				temp->absPos.x = (double) i * dstep;
				temp->absPos.y = (double) j * dstep;
				temp->absPos.z = (double) k * dstep;
			}
		}
	}
}

void PartArray::calcM() {
	std::vector < Part >::iterator iterator1;
	Part* temp;
	iterator1 = this->parts.begin();
	while (iterator1 != this->parts.end()) {
		temp = iterator1.base();
		temp->m.x = kv * vol * temp->axis.x * I0s;
		temp->m.y = kv * vol * temp->axis.y * I0s;
		temp->m.z = kv * vol * temp->axis.z * I0s;
		++iterator1;
	}
}

Part* PartArray::getElem(int x, int y, int z) {
	return &this->parts.at(x * (this->y * this->z) + y * this->y + z);
}

void PartArray::calcInteraction(Part* elem) {
	std::vector < Part >::iterator iterator1;
	vect rvect(0,0,0);
	elem->interaction.x = 0;
	elem->interaction.y = 0;
	elem->interaction.z = 0;
	elem->intMod = 0;
	double r, r2, r5, part, dx, dy, dz;
	iterator1 = this->parts.begin();
	while (iterator1 != this->parts.end()) {
		if (elem->pos.x != (*iterator1).pos.x || elem->pos.y != (*iterator1).pos.y || elem->pos.z != (*iterator1).pos.z) { //не считать взаимодействие частицы на себя
			rvect = (*iterator1).absPos.radius(elem->absPos);
			r = rvect.length();
			r2 = r * r; //радиус в кубе
			r5 = r2 * r * r * r; //радиус в пятой
			dx = elem->absPos.x - (*iterator1).absPos.x;
			dy = elem->absPos.y - (*iterator1).absPos.y;
			dz = elem->absPos.z - (*iterator1).absPos.z;
			part = (*iterator1).m.x * dx + (*iterator1).m.y * dy + (*iterator1).m.z * dz;
			elem->interaction.x += ((*iterator1).m.x * r2 - (3 * part * dx)) / r5;
			elem->interaction.y += ((*iterator1).m.y * r2 - (3 * part * dy)) / r5;
			elem->interaction.z += ((*iterator1).m.z * r2 - (3 * part * dz)) / r5;
			//elem->interaction.z += (3 * part * dz) / r5 - (*l3Iterator).m.z * r2 / r3;
		}
		++iterator1;
	}
	elem->intMod = elem->interaction.length();
}

void PartArray::calcInteraction() {
	std::vector < Part >::iterator iterator2;
	iterator2 = this->parts.begin();
	while (iterator2 != this->parts.end()) {
		this->calcInteraction(iterator2.base());
		iterator2++;
	}
}

vect PartArray::calcInteractionNeighb(Part* elem) {
	vect interaction(0, 0, 0);
	double r, r2, r5, part, dx, dy, dz;
	vector<Part*> parts;
	std::vector < Part* >::iterator iter;

	int x = elem->pos.x;
	int y = elem->pos.y;
	int z = elem->pos.z;

	parts.push_back(this->getElem(x - 1, 1, z - 1));
	parts.push_back(this->getElem(x - 1, 1, z));
	parts.push_back(this->getElem(x - 1, 1, z + 1));
	parts.push_back(this->getElem(x, 1, z - 1));
	parts.push_back(this->getElem(x, 1, z + 1));
	parts.push_back(this->getElem(x + 1, 1, z - 1));
	parts.push_back(this->getElem(x + 1, 1, z));
	parts.push_back(this->getElem(x + 1, 1, z + 1));

	iter = parts.begin();
	while (iter != parts.end()) {
		r = (*iter)->absPos.space(elem->absPos);
		r2 = r * r; //радиус в кубе
		r5 = r2 * r * r * r; //радиус в пятой
		dx = elem->absPos.x - (*iter)->absPos.x;
		dy = elem->absPos.y - (*iter)->absPos.y;
		dz = elem->absPos.z - (*iter)->absPos.z;
		part = (*iter)->m.x * dx + (*iter)->m.y * dy + (*iter)->m.z * dz;
		interaction.x += ((*iter)->m.x * r2 - (3 * part * dx)) / r5;
		interaction.y += ((*iter)->m.y * r2 - (3 * part * dy)) / r5;
		interaction.z += ((*iter)->m.z * r2 - (3 * part * dz)) / r5;
		++iter;
	}
	return interaction;
}

void PartArray::calcEnergy1() {
	std::vector < Part >::iterator iterator2;
	this->E1 = 0;
	iterator2 = this->parts.begin();
	while (iterator2 != this->parts.end()) {
		this->E1 += this->calcEnergy1(iterator2.base());
		iterator2++;
	}
	this->E1 *= 0.5;
}

double PartArray::calcEnergy1(Part* elem) {
	std::vector < Part >::iterator iterator1;
	double r, r2, r5, E = 0;
	vect rij;
	iterator1 = this->parts.begin();
	while (iterator1 != this->parts.end()) {
		if (elem->pos.x != (*iterator1).pos.x || elem->pos.y != (*iterator1).pos.y || elem->pos.z != (*iterator1).pos.z) { //не считать взаимодействие частицы на себя
			rij = (*iterator1).absPos.radius(elem->absPos);
			r = rij.length();
			r2 = r * r; //радиус в кубе
			r5 = r2 * r * r * r; //радиус в пятой
			E +=
					(((*iterator1).m.scalar(elem->m) * r2)
					-
					(3 * elem->m.scalar(rij) * (*iterator1).m.scalar(rij))) / r5;
		}
		++iterator1;
	}
	return E;
}

void PartArray::calcEnergy2() {
	std::vector < Part >::iterator iterator2;
	this->E2 = 0;
	iterator2 = this->parts.begin();
	while (iterator2 != this->parts.end()) {
		this->E2 -= (*iterator2).interaction.scalar((*iterator2).m);
		iterator2++;
	}
	this->E2 *= 0.5;
}

void PartArray::cout() {
	std::vector < Part >::iterator iterator1;
	std::cout << "X\tY\tZ\tMx\tMy\tMz\t\tHx\tHy\tHz\t|H|" << std::endl;
	iterator1 = this->parts.begin();
	while (iterator1 != this->parts.end()) {
		std::cout
				<< (*iterator1).absPos.x << "\t"
				<< (*iterator1).absPos.y << "\t"
				<< (*iterator1).absPos.z << "\t"
				<< (*iterator1).m.x << "\t"
				<< (*iterator1).m.y << "\t"
				<< (*iterator1).m.z << "\t\t"
				<< (*iterator1).interaction.x << "\t"
				<< (*iterator1).interaction.y << "\t"
				<< (*iterator1).interaction.z << "\t"
				<< (*iterator1).intMod << std::endl;
		++iterator1;
	}
	std::cout << "E1 : " << this->E1 << "; E2 : " << this->E2 << std::endl;
}

void PartArray::draw() {
	std::cout << std::endl;
	Part* temp;
	int i, j = 0, k;
	for (i = 0; i < this->x; i++) {
		for (k = 0; k < this->z; k++) {
			temp = this->getElem(i, j, k);
			if (temp->m.z > 0)
				std::cout << "i ";
			else
				std::cout << "! ";
		}
		std::cout << std::endl;
	}
}

void PartArray::setAntiferr() {
	std::vector < Part >::iterator iterator1;
	iterator1 = this->parts.begin();
	while (iterator1 != this->parts.end()) {
		if ((int) ((*iterator1).pos.x + (*iterator1).pos.z) % 2 == 1) (*iterator1).axis.z = -1.;
		else (*iterator1).axis.z = 1;
		(*iterator1).axis.y = 0.;
		(*iterator1).axis.x = 0.;

		++iterator1;
	}
	this->calcM();

	this->calcInteraction();
	this->calcEnergy1();
	this->calcEnergy2();
}

void PartArray::setAllUp() {
	std::vector < Part >::iterator iterator1;
	iterator1 = this->parts.begin();
	while (iterator1 != this->parts.end()) {
		(*iterator1).axis.z = 1.;
		(*iterator1).axis.y = 0.;
		(*iterator1).axis.x = 0.;
		++iterator1;
	}
	this->calcM();

	this->calcInteraction();
	this->calcEnergy1();
	this->calcEnergy2();
}

void PartArray::setRandomZ() {
	std::vector < Part >::iterator iterator1;
	srand(time(NULL));
	iterator1 = this->parts.begin();
	while (iterator1 != this->parts.end()) {
		if (rand() % 2 == 1) (*iterator1).axis.z = 1.;
		else (*iterator1).axis.z = -1.;
		(*iterator1).axis.y = 0.;
		(*iterator1).axis.x = 0.;
		++iterator1;
	}
	this->calcM();

	this->calcInteraction();
	this->calcEnergy1();
	this->calcEnergy2();
}

std::vector<double> PartArray::processStep() {
	std::vector<Part>::iterator iter;
	std::vector<double> history;
	iter = this->parts.begin();
	while (iter != this->parts.end()) {
		if ((*iter).intMod > hc && (*iter).interaction.scalar((*iter).m) < 0) {
			history.push_back(this->E2);
			//std::cout << "rotate with x=" << (*iter).pos.x << "; z=" << (*iter).pos.z << std::endl;
			(*iter).m.rotate();
			(*iter).axis.rotate();
			this->calcInteraction();
			this->calcEnergy2();
			iter = this->parts.begin();
		} else {
			//std::cout << "normal with x=" << (*iter).pos.x << "; z=" << (*iter).pos.z << std::endl;
			iter++;
		}
	}
	return history;
}

//std::vector<double> PartArray::processStep2() {
//	Part* iter;
//	std::vector<double> history;
//	Part* temp;
//	int i, j = 0, k;
//	bool rotated = true;
//	for (i = 0; i < this->x; i++) {
//		for (k = 0; k < this->z; k++) {
//			iter = this->getElem(k,j,i);
//			if ((*iter).intMod > hc && (*iter).interaction.scalar((*iter).m) < 0) {
//				history.push_back(this->E2);
//				//std::cout << "rotate with x=" << (*iter).pos.x << "; z=" << (*iter).pos.z << std::endl;
//				(*iter).m.rotate();
//				(*iter).axis.rotate();
//				this->calcInteraction();
//				this->calcEnergy2();
//				iter = this->parts.begin();
//			} else {
//				//std::cout << "normal with x=" << (*iter).pos.x << "; z=" << (*iter).pos.z << std::endl;
//				iter++;
//			}
//		}
//
//	}
//	return history;
//}

std::vector<double> PartArray::processRandom() {
	std::vector<Part>::iterator iter; //итератор для перебора массива частиц
	Part* rElem; //тут будет храниться случайная частица
	srand(time(NULL));
	std::vector<Part*> unstable; //хранит нестабильные частицы
	std::vector<double> history; //история переворота
	int rNum = 0;
	bool hasUnstable = true;
	while (hasUnstable) {
		unstable.clear(); //чистим массив нестабильных частиц
		//собираем все неустойчивые элементы в один массив
		iter = this->parts.begin();
		while (iter != this->parts.end()) {
			if ((*iter).intMod > hc && (*iter).interaction.scalar((*iter).m) < 0)
				unstable.push_back(iter.base());
			iter++;
		}
		//если найдены частицы, выбираем одну из них рэндомно и переворачиваем
		if (!unstable.empty()) {
			hasUnstable = true;
			rNum = rand() % unstable.size(); //получаем случайный номер переворачиваемой частицы
			rElem = unstable.at(rNum);
			rElem->axis.rotate();
			rElem->m.rotate();
			this->calcInteraction();
			//сохраняем новую энергию системы в историю
			this->calcEnergy2();
			history.push_back(this->E2);
			draw();
			//std::cout << "rotate with x=" << rElem->pos.x << "; z=" << rElem->pos.z << std::endl;
		} else {
			hasUnstable = false;
		}
	}
	return history;
}

std::vector<double> PartArray::processMaxH() {
	std::vector<double> history; //история переворота
	std::vector<Part>::iterator iter; //итератор для перебора массива частиц
	Part* mElem = NULL; //тут будет храниться максимальная частица. по умолчанию максимальная - первая
	bool hasUnstable = true;
	while (hasUnstable) {
		hasUnstable = false;
		mElem = NULL; //чистим максимум
		iter = this->parts.begin();
		while (iter != this->parts.end()) {
			if ((*iter).intMod > hc && (*iter).interaction.scalar((*iter).m) < 0) {
				hasUnstable = true;
				//на первом шаге цикла нужно просто присвоить максимуму значение, а на остальных уже сравнивать
				if (mElem == NULL) {
					mElem = iter.base();
				} else {
					if (mElem->intMod < (*iter).intMod)
						mElem = iter.base();
				}
			}
			iter++;
		}

		//если найдена нестабильная частица - вращаем её
		if (hasUnstable) {
			mElem->axis.rotate();
			mElem->m.rotate();
			this->calcInteraction();
			//сохраняем новую энергию системы в историю
			this->calcEnergy2();
			history.push_back(this->E2);
			draw();
			//std::cout << "rotate with x=" << mElem->pos.x << "; z=" << mElem->pos.z << std::endl;
		}
	}
	return history;
}

std::vector<double> PartArray::processGroupMaxH() {
	std::vector<double> history; //история переворота
	std::vector<Part*> unstable; //все нестабильные частицы здесь при просчете
	std::vector<Part>::iterator iter; //итератор для перебора массива частиц
	std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
	int maxH = 0.; //максимальный модуль поля взаимодействия
	bool hasUnstable = true; //есть нестабильные частицы или нет
	while (hasUnstable) {
		int rSize = 0; //счетчик, нужен для баловства
		hasUnstable = false;
		unstable.clear(); //чистим набор нестабильных частиц

		maxH = 0;

		//step 1 - сначала находим максимальный магнитный момент системы
		iter = this->parts.begin();
		while (iter != this->parts.end()) {
			if ((*iter).intMod > hc && (*iter).interaction.scalar((*iter).m) < 0) {
				unstable.push_back(iter.base());
				hasUnstable = true;
				if ((int) (*iter).intMod > maxH)
					maxH = (int) (*iter).intMod;
			}
			iter++;
		}

		//step 2 - переворачиваем ТОЛЬКО частицы с максимальной нестабильностью
		iter2 = unstable.begin();
		while (iter2 != unstable.end()) {
			if ((int) (*iter2)->intMod == maxH) {
				(*iter2)->axis.rotate();
				(*iter2)->m.rotate();
				rSize++;
			}
			iter2++;
		}

		//step 3 - если чтото было перевернуто - обновляем поля взаимодействия
		if (hasUnstable) {
			std::cout << "rotate group from " << rSize << " elements" << std::endl;
			this->calcInteraction();
			//сохраняем новую энергию системы в историю
			this->calcEnergy2();
			history.push_back(this->E2);
			draw();
		}
	}

	return history;
}

std::vector<double> PartArray::processGroupStep() {
	std::vector<double> history; //история переворота
	std::vector<Part*> unstable; //все нестабильные частицы здесь при просчете
	std::vector<Part>::iterator iter; //итератор для перебора массива частиц
	std::vector<Part*>::iterator iter2; //итератор для перебора нестабильных частиц
	bool hasUnstable = true; //есть нестабильные частицы или нет
	while (hasUnstable) {
		hasUnstable = false;
		unstable.clear(); //чистим набор нестабильных частиц

		//step 1 - сначала находим все неустойчивые элементы
		iter = this->parts.begin();
		while (iter != this->parts.end()) {
			if ((*iter).intMod > hc && (*iter).interaction.scalar((*iter).m) < 0) {
				unstable.push_back(iter.base());
				hasUnstable = true;
			}
			iter++;
		}

		//step 2 - переворачиваем ТОЛЬКО частицы с максимальной нестабильностью
		iter2 = unstable.begin();
		while (iter2 != unstable.end()) {
			(*iter2)->axis.rotate();
			(*iter2)->m.rotate();
			iter2++;
		}

		//step 3 - если чтото было перевернуто - обновляем поля взаимодействия
		if (hasUnstable) {
			//std::cout<<"rotate group from "<<unstable.size()<<" elements"<<std::endl;
			this->calcInteraction();
			//сохраняем новую энергию системы в историю
			this->calcEnergy2();
			history.push_back(this->E2);
			draw();
		}
	}

	return history;
}