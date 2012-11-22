/*
 * File:   Part.cpp
 * Author: uyras
 *
 * Created on 20 Ноябрь 2012 г., 15:50
 */

#include <vector>
#include "Part.h"
#include "config.cpp"

Part::Part() {
	//оси частиц направлены вдоль оси Z
	this->axis.x = .0;
	this->axis.y = .0;
	this->axis.z = .0;

	//взаимодействие частиц пока ноль
	this->interaction.x = this->interaction.y = this->interaction.z = this->intMod = 0;
}
