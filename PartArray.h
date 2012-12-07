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
	double E1, E2;

	/**
	 * Создает массив частиц размером X,Y,Z
	 * @param x
	 * @param y
	 * @param z
	 */
	PartArray(int x, int y, int z);

	PartArray(char* file);

	/**
	 * рассчитывает магнитный момент для каждой частицы в системе
	 */
	void calcM();

	/**
	 * Считает общюю проекцию магнитного момента Mz во всей системе
     * @return Mz всей системы
     */
	double calcMZTotal();

	/**
	 * Рассчитать поле взаимодействия для частицы стоящей на позиции X,Y,Z и записывает её в параметр Interaction частицы
	 * @param elem ссылка на элемент, над которым производить взаимодействие
	 */
	void calcInteraction(Part* elem);

	/**
	 * Считать поле взаимодействия только для соседних 8 элементов
	 * @param elem
	 * @return
	 */
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
	 * @param elem ссылка на элемент, для которого считать энергию
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

	void save(char* file);

	void load(char* file);

	/**
	 * возвращает вектор(массив) энергий каждой частицы
	 */
	std::vector<double> getEVector();

	/**
	 * возвращает вектор(массив) энергий взаимодействия каждой частицы
	 */
	std::vector<double> getHVector();

	std::vector<double> getHZVector();
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
	void setLines(); //одна линия вверх, одна вниз
	void setAllUp(); //все вверх
	void setRandomZ(); //случайно распределяет вдоль оси Z

	//просчет массива, возвращает набор энергий на каждом шаге
	std::vector<double> processStepXYZ(); //перебор в порядке объявления (сначала Z потом Y потом X)
	std::vector<double> processStepZYX(); //перебор решетки поперёк
	std::vector<double> processRandom(); //из набора неравновесных частиц случайным образом выбирается одна и перебирается
	std::vector<double> processMaxH(); //выбирается максимально неустойчивая частица и переворачивается
	std::vector<double> processGroupMaxH(); //выбирается группа максимально неустойчивых частиц и переворачиваются разом
	std::vector<double> processGroupStep(); //все неустойчивые частицы вращаются разом
	std::vector<double> processFromCenter(int x, int y, int z); //обработка из центра к краям, в параметрах координаты центральной частицы
	std::vector<double> processHEffective(); //переворот только тех частиц, для которых энергия выше среднего


private:
	std::vector < Part > parts;
	int x, y, z;
};

#endif	/* PARTARRAY_H */

