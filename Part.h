/*
 * File:   Part.h
 * Author: uyras
 * класс частицы
 * Created on 20 Ноябрь 2012 г., 15:50
 */

#ifndef PART_H
#define	PART_H
#include <vector>
#include "vect.h"


class Part {
public:
	vect pos; //координаты, для удобства в относительных величинах
	vect absPos; //абсолютные координаты
	vect axis; //ось частицы
	vect m; //магнитный момент
	vect interaction; //поле взаимодействия
	double intMod; //модуль (длина) поля взаимодействия

	Part();
};

#endif	/* PART_H */

