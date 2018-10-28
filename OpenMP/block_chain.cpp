#include "block_chain.h"
#include "sha256.h"
#include <omp.h>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;
constexpr int num_threads = 8;
// Note that _time would normally be set to the time of the block's creation.
// This is part of the audit a block chain.  To enable consistent results
// from parallelisation we will just use the index value, so time increments
// by one each time: 1, 2, 3, etc.
block::block(uint32_t index, const string &data)
: _index(index), _data(data), hash_found(false), _time(static_cast<long>(index))
{
	_nonce = make_shared<atomic<uint64_t>>(0);
}

double block::mine_block(uint32_t difficulty) noexcept
{
    string str(difficulty, '0');
    auto start = system_clock::now();

	
	#pragma omp parallel num_threads(num_threads) default(none) shared(difficulty)
	{
		calculate_hash(difficulty);
	}

    auto end = system_clock::now();
    duration<double> diff = end - start;

    cout << "Block mined: " << _hash << " in " << diff.count() << " seconds" << endl;
	return diff.count();
}

void block::calculate_hash(uint32_t difficulty) noexcept
{
	const string str(difficulty, '0');
	while (!hash_found)
	{
		stringstream ss;
		#pragma omp critical
		{
			ss << _index << _time << _data << ++(*_nonce) << prev_hash;
		}
		string temp = sha256(ss.str());
		if (temp.substr(0, difficulty) == str) 
		{
			#pragma omp critical
			{
			hash_found = true;
			}
			_hash = temp;
		}
	}
}
std::string block::calculate_hash() noexcept
{
		stringstream ss;
		#pragma critical
		{
			ss << _index << _time << _data << ++(*_nonce) << prev_hash;
		}
		return sha256(ss.str());
}

block_chain::block_chain()
{
    _chain.emplace_back(block(0, "Genesis Block"));
    _difficulty = 3;
}

double block_chain::add_block(block &&new_block) noexcept
{
    new_block.prev_hash = get_last_block().get_hash();
    auto time = new_block.mine_block(_difficulty);
    _chain.push_back(new_block);
	return time;
}