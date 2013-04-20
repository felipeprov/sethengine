#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

	/******************** Includes ************************************/
	#include <stdint.h>
	#include "math/vector2d.h"

	/******************** Types ***************************************/
	enum AttributeType_e
	{
		AT_INT = 0,
		AT_STRING,
		AT_VECTOR2D,
		AT_DOUBLE
	};

	typedef struct Attribute_t
	{
		void* data;
		int   type; 
	}Attribute_t;

	/******************** Prototypes ********************************/

	extern int*			Attribute_ToInteger(Attribute_t* att_);
	extern double* 		Attribute_ToDouble(Attribute_t* att_);
	extern char*  		Attribute_ToString(Attribute_t* att_);
	extern Vector2d_t* 	Attribute_ToVector2d(Attribute_t* att_);

	extern Attribute_t* Attribute_NewInteger(int value_);
	extern Attribute_t* Attribute_NewDouble(double value_);
	extern Attribute_t* Attribute_NewString(char* value_);
	extern Attribute_t* Attribute_NewVector(double x, double y);

	extern void Attribute_Free(Attribute_t* att_);

#endif