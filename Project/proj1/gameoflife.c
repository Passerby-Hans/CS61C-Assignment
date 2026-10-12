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
  /*
  位号:   17 16 15 14 13 12 11 10  9 │  8  7  6  5  4  3  2  1  0
        └────────── 活细胞表 ────────┘ └────────── 死细胞表 ───────┘
        s=1 时查这半边（9+n）      s=0 时查这半边（n）
        rule >> 9+n & 1                 rule >> n & 1
  */

  uint8_t next_R = 0;
  uint8_t next_G = 0;
  uint8_t next_B = 0;

  for (int b = 0; b < 24; b++) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (i == 0 && j == 0) continue;
        int nr = (row + i + image->rows) % image->rows;
        int nc = (col + j + image->cols) % image->cols;
        if (b / 8 == 0) {
          if ((image->image[nr][nc].R >> b) & 1) {
            count++;
          }
        } else if (b / 8 == 1) {
          if ((image->image[nr][nc].G >> (b - 8)) & 1) {
            count++;
          }
        } else if (b / 8 == 2) {
          if ((image->image[nr][nc].B >> (b - 16)) & 1) {
            count++;
          }
        } else {
          fprintf(stderr, "Error: Invalid bit index %d\n", b);
          exit(-1);
        }
      }
    }
    if (b / 8 == 0) {
      uint8_t current_R_bit = (image->image[row][col].R >> b) & 1;
      if (current_R_bit) {
        next_R |= ((rule >> (9 + count)) & 1) << b;
      } else {
        next_R |= ((rule >> count) & 1) << b;
      }
    } else if (b / 8 == 1) {
      uint8_t current_G_bit = (image->image[row][col].G >> (b - 8)) & 1;
      if (current_G_bit) {
        next_G |= ((rule >> (9 + count)) & 1) << (b - 8);
      } else {
        next_G |= ((rule >> count) & 1) << (b - 8);
      }
    } else if (b / 8 == 2) {
      uint8_t current_B_bit = (image->image[row][col].B >> (b - 16)) & 1;
      if (current_B_bit) {
        next_B |= ((rule >> (9 + count)) & 1) << (b - 16);
      } else {
        next_B |= ((rule >> count) & 1) << (b - 16);
      }
    } else {
      fprintf(stderr, "Error: Invalid bit index %d\n", b);
      exit(-1);
    }
  }
  // 分配新的 Color 内存并返回
  Color *newColor = (Color *)malloc(sizeof(Color));
  if (!newColor) {
    fprintf(stderr, "Error: Failed to allocate memory for new color\n");
    exit(-1);
  }
  newColor->R = next_R;
  newColor->G = next_G;
  newColor->B = next_B;
  return newColor;
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
