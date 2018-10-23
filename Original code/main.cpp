#include <iostream>
#include <string>
#include "block_chain.h"
#include <fstream>

using namespace std;

int main()
{
    block_chain bchain;
	ofstream data("initial_data.csv", ofstream::out);

    for (uint32_t i = 1; i < 100u; ++i)
    {
        cout << "Mining block " << i << "..." << endl;
        auto time = bchain.add_block(block(i, string("Block ") + to_string(i) + string(" Data")));
		data << time << ",";
	}
	data << endl; 
	data.close();
    return 0;
}