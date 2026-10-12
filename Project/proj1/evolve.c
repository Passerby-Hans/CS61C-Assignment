/************************************************************************
**
** NAME:        evolve.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1 —— 多代演化驱动器（非评分文件，
**              仅供本地观察生命游戏长时间行为；评分只使用 imageloader.c
**              和 gameoflife.c）。
**
** 用法:        ./evolve [file] [rule] [gens] [delay_ms] [mode]
**              delay_ms  可选，帧间隔毫秒数，默认 150
**              mode      可选：ansi（默认，终端彩色动画）/ ppm（最终代打印到 stdout）
**
** 渲染:        ANSI 24 位真彩色背景块，每个细胞染成它自己的 RGB 颜色。
**              图太宽时自动抽样（stride）并压缩细胞宽度，保证塞进终端。
**
** 构建说明:    gameoflife.c 自带 main，与本文件冲突，
**              Makefile 里用 -Dmain=graded_main 把它的入口符号改名后再链接。
**
**************************************************************************/

#define _POSIX_C_SOURCE 200809L   /* nanosleep 在 -std=c99 下需要此特性宏 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>
#include <time.h>
#include "imageloader.h"

extern Image *life(Image *image, uint32_t rule);

static int cursor_hidden = 0;

/* atexit/Ctrl-C 时恢复终端：光标可见、颜色复位 */
static void restore_terminal(void)
{
	if (cursor_hidden) {
		printf("\x1b[?25h\x1b[0m\n");
		fflush(stdout);
	}
}

static void msleep(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
	nanosleep(&ts, NULL);
}

static void render_ansi(const Image *img, int gen, uint32_t rule)
{
	uint32_t stride = img->cols / 100 + 1;              /* >100 列时抽样 */
	int width = (img->cols / stride > 60) ? 1 : 2;      /* 细胞宽度，近似正方形 */
	printf("\x1b[Hgen %d   rule 0x%" PRIx32 "   stride %u\n",
	       gen, rule, (unsigned)stride);
	for (uint32_t r = 0; r < img->rows; r += stride) {
		for (uint32_t c = 0; c < img->cols; c += stride) {
			Color px = img->image[r][c];
			/* 背景色转义：\x1b[48;2;R;G;Bm，随后印空格即得色块 */
			printf("\x1b[48;2;%u;%u;%um", (unsigned)px.R, (unsigned)px.G, (unsigned)px.B);
			for (int k = 0; k < width; k++)
				putchar(' ');
		}
		printf("\x1b[0m\n");
	}
	fflush(stdout);
}

int main(int argc, char **argv)
{
	if (argc < 4 || argc > 6) {
		fprintf(stderr, "usage: %s filename rule generations [delay_ms] [ansi|ppm]\n", argv[0]);
		fprintf(stderr, "rule is a hex number beginning with 0x; Life is 0x1808.\n");
		return -1;
	}

	char *input_file = argv[1];
	uint32_t rule = (uint32_t)strtol(argv[2], NULL, 16);
	int generations = atoi(argv[3]);
	long delay_ms = (argc >= 5) ? atol(argv[4]) : 150;
	int ansi = 1;
	if (argc >= 6) {
		if (strcmp(argv[5], "ppm") == 0) ansi = 0;
		else if (strcmp(argv[5], "ansi") != 0) {
			fprintf(stderr, "mode must be ansi or ppm\n");
			return -1;
		}
	}
	if (generations < 0 || delay_ms < 0) {
		fprintf(stderr, "generations and delay must be non-negative\n");
		return -1;
	}

	Image *cur = readData(input_file);
	if (cur == NULL) {
		fprintf(stderr, "can not read %s as 'Image' type\n", input_file);
		return -1;
	}

	if (ansi) {
		atexit(restore_terminal);
		signal(SIGINT, exit);          /* Ctrl+C 也走 atexit，恢复光标 */
		cursor_hidden = 1;
		printf("\x1b[2J\x1b[?25h\x1b[?25l");   /* 清屏一次；之后每帧只归位，防闪烁 */
		fflush(stdout);
	}

	for (int gen = 0; gen < generations; gen++) {
		if (ansi) {
			render_ansi(cur, gen, rule);
			msleep(delay_ms);
		}
		Image *next = life(cur, rule);
		if (next == NULL) {
			freeImage(cur);
			return -1;
		}
		freeImage(cur);   // 先算 next，再放 cur，指针才能安全接力
		cur = next;
	}

	if (ansi)
		render_ansi(cur, generations, rule);
	else
		writeData(cur);
	freeImage(cur);
	return 0;
}
