#pragma once
#include <atomic>

class BlockScheduler {
private:
	int total_blocks_x;
	int total_blocks_y;
	std::atomic<int> next_block;
	int block_size=40;

public:
	BlockScheduler(int width, int height) ;
	bool GetNextBlock(int& block_x, int& block_y, int& start_x, int& start_y, int& end_x, int& end_y, int width, int height);


};