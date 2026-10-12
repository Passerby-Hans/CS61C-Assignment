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


// 把 Color 的 24 个位平面打包成一个 24 位整数（R 在最低字节），
// 之后所有通道按统一的位号 b 处理，无需任何通道分支。
static uint32_t pack24(const Color *c)
{
	return (uint32_t)c->R | ((uint32_t)c->G << 8) | ((uint32_t)c->B << 16);
}

// 判定 (row, col) 位置的细胞下一代应该是什么颜色，并为返回的 Color 分配新的内存
//（由调用方负责释放）。
// 注意：你需要读取该细胞的 8 个邻居。网格采用「环绕（wrap-around）」拓扑：
// 即最上一行与最下一行视为相邻，最左一列与最右一列视为相邻。
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	// 八个方向的RGB值3合1
	// 每个邻居只做一次回绕取模和一次打包
	uint32_t neighbors[8];
	int k = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			int nr = (row + i + image->rows) % image->rows;
			int nc = (col + j + image->cols) % image->cols;
			neighbors[k++] = pack24(&image->image[nr][nc]);
		}
	}

	uint32_t self = pack24(&image->image[row][col]);
	uint32_t next = 0;
	for (int b = 0; b < 24; b++) {
		int count = 0;
		for (int m = 0; m < 8; m++) {
			count += (neighbors[m] >> b) & 1;
		}
		int s = (self >> b) & 1;
		// 18 位规则表：低 9 位管死细胞，高 9 位管活细胞，下标 9*s + n
		next |= ((rule >> (9 * s + count)) & 1) << b;
	}

	Color *newColor = malloc(sizeof(Color));
	if (!newColor) {
		fprintf(stderr, "Error: Failed to allocate memory for new color\n");
		exit(-1);
	}
	newColor->R = next & 0xFF;
	newColor->G = (next >> 8) & 0xFF;
	newColor->B = (next >> 16) & 0xFF;
	return newColor;
}

// 生命游戏的主体逻辑：给定一张图像和一条规则，
// 计算并返回演化一代后的新图像。
// 这里大部分代码可以直接从 steganography.c 中复用。
Image *life(Image *image, uint32_t rule)
{
	Image *newImage = malloc(sizeof(Image));
	if (!newImage) {
		fprintf(stderr, "Error: Failed to allocate memory for new image\n");
		exit(-1);
	}
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = malloc(newImage->rows * sizeof(Color *));
	if (!newImage->image) {
		fprintf(stderr, "Error: Failed to allocate memory for new image rows\n");
		free(newImage);
		exit(-1);
	}
	for (uint32_t i = 0; i < newImage->rows; i++) {
		newImage->image[i] = malloc(newImage->cols * sizeof(Color));
		if (!newImage->image[i]) {
			fprintf(stderr, "Error: Failed to allocate memory for new image columns\n");
			for (uint32_t j = 0; j < i; j++) {
				free(newImage->image[j]);
			}
			free(newImage->image);
			free(newImage);
			exit(-1);
		}
		for (uint32_t j = 0; j < newImage->cols; j++) {
			Color *newColor = evaluateOneCell(image, i, j, rule);
			newImage->image[i][j] = *newColor;   // 结构体整体赋值 = 逐成员拷贝
			free(newColor);
		}
	}
	return newImage;
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
	if (argc != 3) {
		fprintf(stderr, "usage: %s filename rule\n", argv[0]);
		fprintf(stderr, "filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		fprintf(stderr, "rule is a hex number beginning with 0x; Life is 0x1808.\n");
    	exit(-1);
	}

	char *input_file = argv[1];
	Image *input_img = readData(input_file);
	if (input_img == NULL) {
		fprintf(stderr, "can not read %s as 'Image' type\n", input_file);
    	exit(-1);
	}
	uint32_t rule = strtol(argv[2], NULL, 16);
	Image *output_img = life(input_img, rule);
	writeData(output_img);

	freeImage(input_img);
	freeImage(output_img);

	return 0;
}
