/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"


uint8_t get_bit(uint8_t x, uint8_t n) {
	return (x >> n) & 1;
}

// 判定给定 (row, col) 位置的像素在输出图中应该是什么颜色。
// 不应修改入参 image，并且需要为返回的 Color 分配新的内存（由调用方负责释放）。
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	Color target_color = image->image[row][col];
	uint8_t blue_color = target_color.B;
	uint8_t s_bit = get_bit(blue_color, 0);
	uint8_t true_color_value = s_bit == 1 ? 255 : 0;
	Color *true_color = malloc(sizeof(Color));
	if (true_color == NULL) {
		fprintf(stderr, "Error: Could not allocate memory for color\n");
		return NULL;
	}
	true_color->R = true_color->G = true_color->B = true_color_value;
	return true_color;
}

// 给定一张图像，构造并返回一张新的图像，
// 新图像的每个像素由原图对应像素 B 通道的最低有效位 (LSB) 决定：
//   - 若 LSB 为 0，则该像素为黑色 (0, 0, 0)
//   - 若 LSB 为 1，则该像素为白色 (255, 255, 255)
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	Image *new_image = malloc(sizeof(Image));
	if (new_image == NULL) {
		fprintf(stderr, "Error: Could not allocate memory for image\n");
		return NULL;
	}
	new_image->rows = image->rows;
	new_image->cols = image->cols;


	new_image->image = malloc(sizeof(Color*) * image->rows);
	if (new_image->image == NULL) {
		fprintf(stderr, "Error: Could not allocate memory for image\n");
		return NULL;
	}

	for (uint32_t i = 0; i < image->rows; i++) {
		new_image->image[i] = malloc(sizeof(Color) * image->cols);
		if (new_image->image[i] == NULL) {
			fprintf(stderr, "Error: Could not allocate memory for image\n");
			return NULL;
		}
	}
	
	for (uint32_t i = 0; i < image->rows; i++) {
		for (uint32_t j = 0; j < image->cols; j++) {
			Color *true_color = evaluateOnePixel(image, i, j);
			if (true_color == NULL) {
				fprintf(stderr, "Error: Could not evaluate one pixel\n");
				return NULL;
			}
			new_image->image[i][j] = *true_color;
			free(true_color);
		}
	}
	return new_image;
}

/*
从指定文件中读取一个 PPM P3 格式的图像，
然后向标准输出（例如使用 printf）打印一张新的图像：
对每个像素，如果原图 B 通道的最低有效位 (LSB) 为 0，则输出像素为黑色；
如果 LSB 为 1，则输出像素为白色。

参数说明：
- argc：命令行参数的个数。
- argv：命令行参数列表，期望的输入如下：
    * argv[0]：程序名（由系统自动填充）。
    * argv[1]：输入文件名，对应一个 PPM P3 格式的文件
              （文件扩展名不一定是 .ppm）。

错误处理：
- 如果输入参数不正确、malloc 失败或发生其他错误，
  应当以退出码 -1 结束程序。
- 否则 main 函数返回 0。

注意：返回之前必须释放所有分配过的内存！
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
    	exit(-1);
	}

	char *input_file = argv[1];
	Image *input_img = readData(input_file);
	if (input_img == NULL) {
		fprintf(stderr, "can not read %s as 'Image' type\n", input_file);
    	exit(-1);
	}

	Image *decoded_img = steganography(input_img);
	if (decoded_img == NULL) {
		fprintf(stderr, "can not convert %s\n", input_file);
		freeImage(input_img);
		exit(-1);
	}

	writeData(decoded_img);
	freeImage(decoded_img);
	freeImage(input_img);

	return 0;
}
