/*
 * File:   repeatChecker.cpp
 * Author: uyras
 *
 * Created on 7 Декабрь 2012 г., 14:41
 */

#include "repeatChecker.h"
#include "config.cpp"
#include <queue>

using namespace std;

repeatChecker::repeatChecker(short int size = repeatCheckerMemorySize) {
	this->_size = size;
	for (int i = 0; i < this->_size; i++) {
		this->_memory.push(0);
	}
}

bool repeatChecker::check(double elem) {
	for (int i = 0; i < this->_size; i++) {
		if (this->_memory.front() == elem) //если элемент встретился на пути, то возвращаем true
			return true;
		else { //иначе кладем проверенный элемент в конец
			this->_memory.push(this->_memory.front());
			this->_memory.pop();
		}
	}
	this->_memory.push(elem);
	this->_memory.pop();
	return false;
}

void repeatChecker::clear(){
	for (int i = 0; i < this->_size; i++) {
		this->_memory.push(0);
		this->_memory.pop();
	}
}