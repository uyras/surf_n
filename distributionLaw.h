/*
 * File:   distributionLaw.h
 * Author: Отморозок
 *
 * Created on 4 Декабрь 2012 г., 15:29
 */

#ifndef DISTRIBUTIONLAW_H
#define	DISTRIBUTIONLAW_H
#include <vector>
#include <cmath>

using namespace std;
template <typename arrType> class distributionLaw {
public:
	distributionLaw(int count, arrType array);
	int* calculate();
	double* getLables(int count);
	vector<double> values; //значения, для которых рассчитан закон распределения
	int* counts; //массив количества попаданий значения в интервал
	double* probabilities;

private:
	double _max();
	double _min();
};

#endif	/* DISTRIBUTIONLAW_H */

