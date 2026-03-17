#include "stdafx.h"
#include <iostream>
#include <ctime>
#include "Fire.h"

void Fire::Update() {
    float currentTime = (float)clock() / CLOCKS_PER_SEC;

	this->u_Time = currentTime * 1.5f;
}

void Fire::sendSpecificData() {
	if (this->shader->sr->U_Time != -1){ 
		//std::cout << this->u_Time << " " << this->shader->sr->U_Time <<  '\n';
		glUniform1f(this->shader->sr->U_Time,this->u_Time);
	}

	if (this->shader->sr->U_DispMax != -1) {
		//std::cout << this->u_DispMax << " " << this->shader->sr->U_DispMax << '\n';
		glUniform1f(this->shader->sr->U_DispMax, this->u_DispMax);

	}
}