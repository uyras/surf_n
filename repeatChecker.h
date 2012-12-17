/*
 * File:   repeatChecker.h
 * Author: uyras
 *
 * Created on 7 Декабрь 2012 г., 14:41
 */

#ifndef REPEATCHECKER_H
#define	REPEATCHECKER_H

#include <queue>

using namespace std;

class repeatChecker {
public:
	repeatChecker(short int);

	/**
	 * Проверить элемент на предмет повторений в очереди
     * @param elem
     * @return
     */
	bool check(double);
	void clear();
private:
	queue<double> _memory;
	int _size;
};

#endif	/* REPEATCHECKER_H */

