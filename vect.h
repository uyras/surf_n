/*
 * File:   vect.h
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 15:55
 */

#ifndef VECT_H
#define	VECT_H

class vect {
public:
	double x, y, z;

	/**
	 * Скалярное произведение двух векторов
	 * @param a первый вектор
	 * @param b второй вектор
	 * @return
	 */
	double scalar(vect);

	double space(vect b);
	vect radius(vect b); //возвращает радиус-вектор
	double length();
	void setXYZ(double x, double y, double z);
	void rotate();
	vect();
	vect(double x, double y, double z);
	vect& operator=(const vect&);
	virtual ~vect();
private:
	double _length;

};

#endif	/* VECT_H */

