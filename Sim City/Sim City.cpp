// Sim City.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Field.h"
#include "Resolution.h"
#include "Parameters.h"

using namespace std;

int main()
{
	Field myField(50, 50);
	myField.generateInsAndOuts(2);
	myField.show_ins_and_outs();

	cout << "FIN" << endl;

    return 0;
}

