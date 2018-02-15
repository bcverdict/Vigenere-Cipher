#include <iostream>
#include <fstream>
#include "Executive.h"
using namespace std;
int main(int argc, char * argv[])
{
	Executive * E = new Executive();
	//E->Take(argv[1]);
	E->Take("Test.txt");
	//delete E;
	return 0;
}
