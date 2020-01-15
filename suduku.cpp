#include <iostream>
#include <string> 
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <math.h>
#include <fstream>
#include <istream>
#include <algorithm>
#include <vector> 
#include <string.h>
using namespace std;

#define N 1000000

char inputsudoku[99999999];
int Input[N][9],Output[N][9];
int out_num = 0;
int sudoku[9][9];

void makesudoku(int num);
void solvesuduku(string filepath);
int check(int c);
void dfs(int n);
void OutputFile(string filepath);

int main(int argc, char** argv)
{
	if (argc != 3)
		printf("参数数目不正确，请重新输入\n");
	if (strcmp(argv[1], "-c") == 0)
	{
		int n = atoi(argv[2]);
		if (n<1 || n>N)	printf("数独数目不正确\n");
		else {
			makesudoku(n);
			OutputFile("sudoku.txt");
		}
	}
	else if (strcmp(argv[1], "-s") == 0)
	{
		solvesuduku(argv[2]);
		OutputFile("solute_sudoku.txt");
	}

	else printf("输入参数格式不正确\n");
	return 0;
}


void makesudoku(int num)
{
	int i, j;
	out_num = 0;
	memset(Output, 0, sizeof(Output));
	int firstrow[9] = { 5,1,2,3,4,6,7,8,9 };
	int move[9] = { 0,3,6,1,4,7,2,5,8 };
	do
	{
		//通过移动第一行生成数独
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
			{
				sudoku[i][j] = firstrow[(j + move[i]) % 9];
			}
		}
		//通过交换生成数独
		int s[9] = { 0,1,2,3,4,5,6,7,8 };
		do
		{
			do
			{
				do
				{
					for (i = 0; i < 9; i++)
						for (j = 0; j < 9; j++)
							Output[i + 9 * out_num][j] = sudoku[s[i]][j];
					out_num++;
					if (out_num >= num)	return;  //数独总数
				} while (next_permutation(s + 1, s + 3)); //交换1到2行
			} while (next_permutation(s + 3, s + 6));//交换3到5行
		} while (next_permutation(s + 6, s + 9));//交换6到8行

	} while (next_permutation(firstrow + 1, firstrow + 9));
}

void solvesuduku(string filepath)
{
	int i, j, k = 0, n = 0;
	out_num = 0;
	memset(Input, 0, sizeof(Input));
	memset(Output, 0, sizeof(Output));
	ifstream ReadFile;
	ReadFile.open(filepath.c_str());
	if (ReadFile.is_open() == FALSE)
		printf("读取数独文件失败，路径错误\n");
	while (!ReadFile.eof())
		ReadFile >> inputsudoku[n++];//读取文件中的字符 
	n = n / 9; //n表示一个有多少行 
	for (i = 0; i < n; i++)
		for (j = 0; j < 9; j++)
			Input[i][j] = inputsudoku[k++] - '0';//转换成数字且存进矩阵中 
	n = n / 9; //n表示一个有多少个数独 
	for (k = 0; k < n; k++)
	{
		memset(sudoku, 0, sizeof(sudoku));
		for (i = 0; i < 9; i++)
			for (j = 0; j < 9; j++)
				sudoku[i][j] = Input[i + (n - 1) * 9][j];
		dfs(0);
	}
	return;
}

void dfs(int n)
{
	int x = n / 9, y = n % 9;
	int i, j;
	if (n == 81)
	{
		for (i = 0; i < 9; i++)
			for (j = 0; j < 9; j++)
				Output[i + 9 * out_num][j] = sudoku[i][j];
		n++;
		return;
	}

	if (sudoku[x][y] == 0)     //如果这个数等于0，就要试探
	{
		for (i = 1; i <= 9; i++)
		{
			sudoku[x][y] = i;      //试探
			if (check(n))
				dfs(n + 1);    //递归
			sudoku[x][y] = 0;       //回溯
		}
		return;
	}
	else
		dfs(n + 1);

}

int check(int n)
{
	int x = n / 9, y = n % 9;
	int i, j;
	for (i = 0; i < 9; i++)
		if (sudoku[i][y] == sudoku[x][y] && x != i)	return 0;
	for (j = 0; j < 9; j++)
		if (sudoku[x][j] == sudoku[x][y] && y != j)	return 0;

	int dx = x / 3 * 3, dy = y / 3 * 3;
	for (i = dx; i < dx + 3; i++)
		for (j = dy; j < dy + 3; j++)
			if (sudoku[i][j] == sudoku[x][y] && i != x && j != y)	return 0;
	return 1;
}

void OutputFile(string filepath)
{
	ofstream WriteFile(filepath.c_str());
	if (WriteFile.fail())
	{
		printf("创建数独文件失败\n");
		return;
	}
	int n, i, j;
	for (n = 0; n < out_num; n++)
	{
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
			{
				WriteFile << (Output[n * 9 + i][j]);
				if (j == 8)
					WriteFile << ("\n");
				else
					WriteFile << (" ");
			}
		}
		WriteFile << ("\n");
	}
	printf("sudoku.txt已成功生成");
}
