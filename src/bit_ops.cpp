#include <stdint.h>

uint8_t set_bit(uint8_t num, uint8_t position)
{
	int mask = 1 << position;
	return num | mask;
}

bool get_bit(uint8_t num, uint8_t position)
{
	bool bit = num & (1 << position);
	return bit;
}

uint8_t flip_bit(uint8_t num, uint8_t position)
{
	int mask = 1 << position;
	return num ^ mask;
}

uint8_t clear_bit(uint8_t num, uint8_t position)
{
	int mask = 1 << position;
	return num & ~mask;
}