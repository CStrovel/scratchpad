#include "Numerics.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	Integer* i = new Integer("435");
	cout << i << endl;
	i->print();
}
