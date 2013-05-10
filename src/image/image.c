
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils\list.h"

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

typedef struct ImageStruct_t
{
	Image_t* img;
	char* filename;
	struct list_head next;
}ImageStruct_t;

static struct list_head g_image_list;
static int firstTime = 1;

static Image_t* searchImagesLoadeds(char* name)
{
	struct list_head *p;
	ImageStruct_t* img;

	list_for_each(p, &g_image_list)
	{
		img = list_entry(p, ImageStruct_t, next);
		if(strcmp(img->filename, name) == 0)
		{
			return img->img;
		}
	}

	return 0;
}

static void addImage(Image_t* img, char* name)
{
	ImageStruct_t* node = (ImageStruct_t*)malloc(sizeof(ImageStruct_t));

	node->img = img;
	node->filename = name;
	list_add(&node->next, &g_image_list);
}

#ifdef __cplusplus
extern "C"  
#endif  

int ImageLoad_PNG(Image_t** image, char* name)
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
	Image_t* img_tmp;

	if(firstTime)
	{
		firstTime = 0;
		INIT_LIST_HEAD(&g_image_list);
	}
	
	img_tmp = searchImagesLoadeds(name);
	if(img_tmp != 0)
	{
		*image = img_tmp;
		return 1;
	}
	img_tmp = (Image_t*)malloc(sizeof(Image_t));
	*image = img_tmp;

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

	img_tmp->width = info_ptr->width;
	img_tmp->hight = info_ptr->height;
	switch (info_ptr->color_type) {
		case PNG_COLOR_TYPE_RGBA:
			img_tmp->hasAlpha = 1;
			break;
		case PNG_COLOR_TYPE_RGB:
			img_tmp->hasAlpha = 0;
			break;
		default:
			printf("Color type %s not supported \n", info_ptr->color_type);
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(fp);
			return 0;
	}
	row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	img_tmp->texture = (unsigned char*) malloc(row_bytes * img_tmp->hight);

	row_pointers = png_get_rows(png_ptr, info_ptr);

	for (i = 0; i < img_tmp->hight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(img_tmp->texture+(row_bytes * (img_tmp->hight-1-i)), row_pointers[i], row_bytes);
	}

	/* Clean up after the read,
		* and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	/* Close the file */
	fclose(fp);

	glGenTextures(1, &img_tmp->texture_handler);
	glBindTexture(GL_TEXTURE_2D, img_tmp->texture_handler);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img_tmp->width,
		img_tmp->hight, 0, img_tmp->hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
		img_tmp->texture);
			 

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
	
	addImage(img_tmp, name);
	
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