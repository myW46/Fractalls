#include "BlockScheduler.h"
#include <algorithm>

BlockScheduler::BlockScheduler(int width, int height ){
	total_blocks_x = (width + block_size - 1) / block_size;
	total_blocks_y = (height + block_size - 1) / block_size;
	next_block = 0;
	this->block_size = block_size;

}
bool BlockScheduler::GetNextBlock(int& block_x, int& block_y,
	int& start_x, int& start_y,
	int& end_x, int& end_y, int width, int height) {
	int index = next_block.fetch_add(1);
	if (index >= total_blocks_x * total_blocks_y) {
		return false;
	}
	block_x = index % total_blocks_x;
	block_y = index / total_blocks_x;

	start_x = block_x * block_size;
	start_y = block_y * block_size;
	end_x = std::min(start_x+block_size, width);
	end_y = std::min(start_y+block_size, height);
	return true;
}