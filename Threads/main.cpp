#include <iostream>
#include <string>
#include <fstream>
#include "block_chain.h"


using namespace std;

int main()
{
    block_chain bchain;

		for (uint32_t i = 1; i < 100u; ++i)
		{
			cout << "Mining block " << i << "..." << endl;
			bchain.add_block(block(i, string("Block ") + to_string(i) + string(" Data")));
		}

    return 0;
}