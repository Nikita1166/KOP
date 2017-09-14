#include "stdafx.h"
#include <iostream>

int position_x(int x, bool turn) {
	if (turn == false)	{
		x = (int)ceil(x / 15);
		if (x % 2 == 0)	{
			x -= 1;
		}
		x *= 15;
	}
	else {
		x = (int)ceil(x / 30);
		x *= 30;
	}

	return x;
}
int position_y(int y, bool turn) {
	if (turn == false)	{
		y = (int)ceil(y / 30);
		y *= 30;
	}
	else
	{
		y = (int)ceil(y / 15);
		if (y % 2 == 0)	{
			y -= 1;
		}
		y *= 15;
	}
	return y;
}