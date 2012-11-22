/*
 * File:   vect.cpp
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 15:55
 */

#include "vect.h"
#include "Part.h"
#include <cmath>
#include <locale.h>

vect::vect() {
}

vect::vect(double x, double y, double z) {
	this->setXYZ(x, y, z);
}

vect::~vect() {
}

double vect::scalar(vect b) {
	return (this->x * b.x) + (this->z * b.z);
}

/**
 * Находит расстояние между двумя векторами
 * @param a
 * @param b
 * @return
 */
double vect::space(vect b) {
	return sqrt(
			(this->x - b.x)*(this->x - b.x) +
			//(a.y - b.y)*(a.y - b.y) +
			(this->z - b.z)*(this->z - b.z)
			);
}

vect vect::radius(vect b) {
	return vect(this->x - b.x, this->y - b.y, this->z - b.z);
}

double vect::length() {
	return this->_length = sqrt(
			this->x * this->x +
			//a.y * a.y +
			this->z * this->z
			);
}

void vect::setXYZ(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
	//this->_length = NULL;
}

void vect::rotate() {
	this->x *= -1;
	this->y *= -1;
	this->z *= -1;
}

vect& vect::operator=(const vect& a){
	this->x = a.x;
	this->x = a.y;
	this->x = a.x;
}