/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: Could not open file %s\n", filename);
		return NULL;
	}
	
	char file_type[16];
	uint32_t rows, cols, max_value;
	fscanf(file, "%s", file_type);
	fscanf(file, "%u %u", &cols, &rows);
	fscanf(file, "%u", &max_value);

	Image *img = malloc(sizeof(Image));
	if (img == NULL) {
		fprintf(stderr, "Error: Could not allocate memory for image\n");
		return NULL;
	}
	img->rows = rows;
	img->cols = cols;
	img->image = malloc(sizeof(Color*) * rows);
	if (img->image == NULL) {
		fprintf(stderr, "Error: Could not allocate memory for image\n");
		return NULL;
	}
	for (uint32_t i = 0; i < rows; i++) {
		img->image[i] = malloc(sizeof(Color) * cols);
		if (img->image[i] == NULL) {
			fprintf(stderr, "Error: Could not allocate memory for image\n");
			return NULL;
		}
	}

	for (uint32_t i = 0; i < rows; i++) {
		for (uint32_t j = 0; j < cols; j++) {
			fscanf(file, "%hhu %hhu %hhu", &img->image[i][j].R, &img->image[i][j].G, &img->image[i][j].B);
		}
	}
	fclose(file);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL) {
		fprintf(stderr, "Error: Image is NULL\n");
		return;
	}
	printf("P3\n");
	printf("%u %u\n", image->cols, image->rows);
	printf("255\n");
	for (uint32_t i = 0; i < image->rows; i++) {
		for (uint32_t j = 0; j < image->cols; j++) {
			printf("%3hhu %3hhu %3hhu", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
			if (j < image->cols - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL) return;

	for (uint32_t i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}