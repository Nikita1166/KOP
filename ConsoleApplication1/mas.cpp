#include "stdafx.h"
#include <iostream>

bool massiv(int x1, int y1, int x2, int y2, int N)	{
	bool flag = true;
	int mas[10][4];
	if (N > 0)	{
		for (int i = 0; i < N; i++)	{
			if (((x1 >= mas[i][0]) && (x2 <= mas[i][2])) &&
				((y1 >= mas[i][1]) && (y2 <= mas[i][3])))	{
				flag = false;
				break;
			}
		}
	}
	if (flag == true)	{
		mas[N][0] = x1 - 30;
		mas[N][1] = y1 - 30;
		mas[N][2] = x2 + 30;
		mas[N][3] = y2 + 30;
	}
	return flag;
}