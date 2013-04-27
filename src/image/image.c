
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C"  {
#endif 
#include "png.h"
#include "pnginfo.h"


#ifdef __cplusplus
}
#endif 
#include "image.h"
#include <string.h>

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL


#ifdef __cplusplus
extern "C"  
#endif  
int ImageLoad_PNG(Image_t* image, char* name)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;
	char buffer[8];
	GLenum error;
	unsigned int row_bytes;
	png_bytepp row_pointers;
	int i;

	fp = fopen(name, "rb+");
	
	
	
	if (fp == NULL)
		return 0;

	//fread(buffer, 1, 8, fp);

		/* Create and initialize the png_struct
		* with the desired error handler
		* functions.  If you want to use the
		* default stderr and longjump method,
		* you can supply NULL for the last
		* three parameters.  We also supply the
		* the compiler header file version, so
		* that we know if the application
		* was compiled with a compatible version
		* of the library.  REQUIRED
		*/
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return 0;
	}
		/* Allocate/initialize the memory
		* for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return 0;
	}

	/* Set error handling if you are
		* using the setjmp/longjmp method
		* (this is the normal method of
		* doing things with libpng).
		* REQUIRED unless you  set up
		* your own error handlers in
		* the png_create_read_struct()
		* earlier.
		*/
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* Free all of the memory associated
			* with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(fp);
		/* If we get here, we had a
			* problem reading the file */
		return 0;
	}

	/* Set up the output control if
		* you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* If we have already
		* read some of the signature */
	//png_set_sig_bytes(png_ptr, sig_read);

	/*
		* If you have enough memory to read
		* in the entire image at once, and
		* you need to specify only
		* transforms that can be controlled
		* with one of the PNG_TRANSFORM_*
		* bits (this presently excludes
		* dithering, filling, setting
		* background, and doing gamma
		* adjustment), then you can read the
		* entire image (including pixels)
		* into the info structure with this
		* call
		*
		* PNG_TRANSFORM_STRIP_16 |
		* PNG_TRANSFORM_PACKING  forces 8 bit
		* PNG_TRANSFORM_EXPAND forces to
		*  expand a palette into RGB
		*/

	//fread(buffer, 1, 8, fp);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, 0);

	image->width = info_ptr->width;
	image->hight = info_ptr->height;
	switch (info_ptr->color_type) {
		case PNG_COLOR_TYPE_RGBA:
			image->hasAlpha = 1;
			break;
		case PNG_COLOR_TYPE_RGB:
			image->hasAlpha = 0;
			break;
		default:
			printf("Color type %s not supported \n", info_ptr->color_type);
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(fp);
			return 0;
	}
	row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	image->texture = (unsigned char*) malloc(row_bytes * image->hight);

	row_pointers = png_get_rows(png_ptr, info_ptr);

	for (i = 0; i < image->hight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(image->texture+(row_bytes * (image->hight-1-i)), row_pointers[i], row_bytes);
	}

	/* Clean up after the read,
		* and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	/* Close the file */
	fclose(fp);

	glGenTextures(1, &image->texture_handler);
	glBindTexture(GL_TEXTURE_2D, image->texture_handler);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, image->width,
		image->hight, 0, image->hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
		image->texture);
			 

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_2D);
   // glShadeModel(GL_FLAT);


	glBindTexture(GL_TEXTURE_2D, 0);

	error = glGetError();
	if(error != GL_NO_ERROR)
		printf("OpenGL Error %d %s\n", error, (const char*)glewGetErrorString(error));
	/* That's it */
	return 1;

}

#ifdef __cplusplus
extern "C" 
#endif  
void ImageSetTexture(Image_t* img)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, img->texture_handler);
}