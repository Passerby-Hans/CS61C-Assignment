/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

// 判定 (row, col) 位置的细胞下一代应该是什么颜色，并为返回的 Color 分配新的内存
//（由调用方负责释放）。
// 注意：你需要读取该细胞的 8 个邻居。网格采用「环绕（wrap-around）」拓扑：
// 即最上一行与最下一行视为相邻，最左一列与最右一列视为相邻。
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
}

// 生命游戏的主体逻辑：给定一张图像和一条规则，
// 计算并返回演化一代后的新图像。
// 这里大部分代码可以直接从 steganography.c 中复用。
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
}

/*
从指定文件中读取一个 .ppm 图像，计算其在生命游戏规则下演化一代后的新图像，
然后将新图像打印到标准输出。

参数说明：
- argc：命令行参数的个数。
- argv：命令行参数列表，期望的输入如下：
    * argv[0]：程序名（由系统自动填充）。
    * argv[1]：输入文件名，对应一个 .ppm 文件。
    * argv[2]：规则的十六进制字符串，例如 "0x1808"。
              注意这是字符串形式，可以使用 strtol 进行转换。

错误处理：
- 如果输入参数不正确、malloc 失败或发生其他错误，
  应当以退出码 -1 结束程序。
- 否则 main 函数返回 0。

注意：返回之前必须释放所有分配过的内存！

提示：可以先从 steganography.c 复制代码作为起点。
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
}
