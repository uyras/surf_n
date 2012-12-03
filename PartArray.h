/*
 * File:   PartArray.h
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 17:22
 */

#ifndef PARTARRAY_H
#define	PARTARRAY_H
#include <vector>
#include "Part.h"

class PartArray {
public:
	double E1,E2;
	/**
	 * Создает массив частиц размером X,Y,Z
     * @param x
     * @param y
     * @param z
     */
	PartArray(int x, int y, int z);

	/**
	 * рассчитывает магнитный момент для вссех частиц в системе
     */
	void calcM();

	/**
	 * Рассчитать поле взаимодействия для частицы стоящей на позиции X,Y,Z и записывает её в параметр Interaction частицы
     * @param elem ссылка на элемент, над которым производить взаимодействие
     */
	void calcInteraction(Part* elem);

	vect calcInteractionNeighb(Part* elem);

	/**
	 * рассчитывает поле взаимодействия для каждой частицы в системе
     */
	void calcInteraction();

	/**
	 * рассчитывает внутреннюю энергию во всей системе частиц (метод подсчета через магнитные моменты)
     */
	void calcEnergy1();

	/**
	 * рассчитывает внутреннюю энергию для определенной частицы
     * @param x координата частицы X
     * @param y координата частицы Y
     * @param z координата частицы Z
     * @return энергия
     */
	double calcEnergy1(Part* elem);

	/**
	 * рассчитывает внутреннюю энергию во всей системе частиц (метод скалярного произведения H и E)
     */
	void calcEnergy2();

	/**
	 * выдает все парамеры частиц на экран
     */
	void cout();

	/**
	 * рисует конфигурацию массива частиц в консоли
     */
	void draw();

	/**
	 * Получает определенную частицу из массива и возвращает ссылку на эту частицу
     * @param x координата X частицы
     * @param y координата Y частицы
     * @param z координата Z частицы
     * @return ссылка на частицу
     */
	Part* getElem(int x, int y, int z);

	//настройка начальных конфигураций
	void setAntiferr(); //антиферромсагнетизм (шахматный порядок)
	void setAllUp(); //все вверх
	void setRandomZ(); //случайно распределяет вдоль оси Z

	//просчет массива, возвращает набор энергий на каждом шаге
	std::vector<double> processStep(); //последовательный перебор каждой неравновесной частицы
//	std::vector<double> processStep2();
	std::vector<double> processRandom(); //из набора неравновесных частиц случайным образом выбирается одна и перебирается
	std::vector<double> processMaxH(); //выбирается максимально неустойчивая частица и переворачивается
	std::vector<double> processGroupMaxH(); //выбирается группа максимально неустойчивых частиц и переворачиваются разом
	std::vector<double> processGroupStep(); //все неустойчивые частицы вращаются разом

private:
	std::vector < Part > parts;
	int x,y,z;
};

#endif	/* PARTARRAY_H */

