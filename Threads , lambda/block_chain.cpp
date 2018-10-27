#include "block_chain.h"
#include "sha256.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

// Note that _time would normally be set to the time of the block's creation.
// This is part of the audit a block chain.  To enable consistent results
// from parallelisation we will just use the index value, so time increments
// by one each time: 1, 2, 3, etc.
block::block(uint32_t index, const string &data)
: _index(index), _data(data), _time(static_cast<long>(index))
{
	_nonce = make_shared<atomic<uint64_t>>(0);
}

double block::mine_block(uint32_t difficulty) noexcept
{
    string str(difficulty, '0');
    auto start = system_clock::now();


	/*
    while (_hash.substr(0, difficulty) != str)
    {
		++(*_nonce)
        _hash = calculate_hash();
    }
	*/

	auto num_threads = 8;
	vector<thread> threads;
	auto lambda_hash = [difficulty, str, this]
	{
		while (!hash_found)
		{
			stringstream ss;
			ss << _index << _time << _data << ++(*_nonce) << prev_hash;
			string temp = sha256(ss.str());
			if (temp.substr(0, difficulty) == str) {
				hash_found = true;
				_hash = temp;
			}
		}
	};

	for (auto i = 0u; i < num_threads; ++i) 
	{
		threads.push_back(std::thread(lambda_hash));
	}
	for (auto &thread : threads) 
			thread.join();

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
		ss << _index << _time << _data << ++(*_nonce) << prev_hash;
		string temp = sha256(ss.str());
		if (temp.substr(0, difficulty) == str) {
			hash_found = true;
			_hash = temp;
		}
	}
}

block_chain::block_chain()
{
    _chain.emplace_back(block(0, "Genesis Block"));
    _difficulty = 6;
}

double block_chain::add_block(block &&new_block) noexcept
{
    new_block.prev_hash = get_last_block().get_hash();
    auto time = new_block.mine_block(_difficulty);
    _chain.push_back(new_block);
	return time;
}