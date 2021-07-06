#include <stdio.h>
#include <malloc.h>
#include <png.h>

int width = 500;
int height = 300;
char * filename = "image.png";

FILE *fp = NULL;
png_structp png_ptr = NULL;
png_infop info_ptr = NULL;
png_bytep row = NULL;

void cleanup() {
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);
}


int main()
{

	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing in binary mode\n", filename);
		cleanup();
		return 1;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		cleanup();
		return 1;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		cleanup();
		return 1;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Could not create png\n");
		cleanup();
		return 1;
	}

	png_init_io(png_ptr, fp);

	// Write the png info header (8 bit RGBA)
	png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
		//	PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
			PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (4 bytes per pixel - RGBA)
	row = (png_bytep) malloc(4 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width ; x++) {
			row[4*x] = 0;
			row[4*x+1] = 100;
			row[4*x+2] = y%255;
			row[4*x+3] = x%255;
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	cleanup();
	return 0;
}
