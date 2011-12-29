#include <iostream>
#include "matrixCube.hpp"

using namespace std;


int main() {
	/*
	matrixCube<4> test00;
	matrixCube<4> test01;
	matrixCube<4> test02;
	matrixCube<4> test03;
	matrixCube<4> test04;
	matrixCube<4> test05;
	matrixCube<4> test105;
	matrixCube<4> test1;
	matrixCube<3> testA;
	cout << "test00:\n" << test00.rotate(0, 1, 1);
	cout << "test01:\n" << test01.rotate(1, 1, 1);
	cout << "test02:\n" << test02.rotate(2, 1, 1);
	cout << "test03:\n" << test03.rotate(3, 1, 1);
	cout << "test04:\n" << test04.rotate(4, 1, 1);
	cout << "test05:\n" << test05.rotate(5, 1, 1);
	cout << "test05:\n" << test05.rotate(5, 1, 1);
	cout << "test105:\n";
	cout << test105.rotate(1, 1, 1);
	cout << test105.rotate(0,2,1);
	cout << test105.rotate(5,1,1);
	cout << "test1:\n" << test1.rotate(1,4,1);
	cout << "testA:\n" << testA;
	testA.matrix[0][0][0] = 0;
	cout << testA << testA.rotate(2,1,1);

	cout << "####################\n";
	*/
	matrixCube<4> testReal;
	cin >> testReal;
	cout << testReal;

	return 0;
}

