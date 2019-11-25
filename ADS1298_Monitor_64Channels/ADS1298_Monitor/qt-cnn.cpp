/*************************************************
main.cpp
Author:Mitom
Date:2019-05-30
Description:矩阵类
**************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <random>
#include <time.h>
#include <ctime>

#include "Matrix.hpp"
#include "Tensor.hpp"
#include "Filter.hpp"
#include "utils.hpp"

#include "tinyxml/tinyxml.h"

#define TIXML_USE_STL

using namespace std;

void run()
{


}

void showBias(vector<double> bias){
	for(int i=0; i<bias.size(); i++){
		cout << bias[i]<< endl;
	}
}



