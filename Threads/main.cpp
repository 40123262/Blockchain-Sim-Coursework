#include <iostream>
#include <string>
#include <fstream>
#include "block_chain.h"


using namespace std;

int main()
{
    block_chain bchain;
	ofstream data("10000.csv", ofstream::out);

	for (int j = 0; j < 5; ++j)
	{
		for (uint32_t i = 1; i < 10000u; ++i)
		{
			cout << "Mining block " << i << "..." << endl;
			auto time = bchain.add_block(block(i, string("Block ") + to_string(i) + string(" Data")));
			data << time << ",";
		}
		data << endl;
	}
	data << endl;
	data.close();
    return 0;
}