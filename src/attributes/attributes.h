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
		AT_DOUBLE,
		AT_POINTER
	};

	typedef struct Attribute_t
	{
		void* data;
		int   type; 
	}Attribute_t;

	/******************** Prototypes ********************************/


#ifdef __cplusplus
extern "C" {  
#endif  

	extern int*			Attribute_ToInteger(Attribute_t* att_);
	extern double* 		Attribute_ToDouble(Attribute_t* att_);
	extern char*  		Attribute_ToString(Attribute_t* att_);
	extern Vector2d_t* 	Attribute_ToVector2d(Attribute_t* att_);
	extern void*		Attribute_ToPointer(Attribute_t* att_);

	extern Attribute_t* Attribute_NewInteger(int value_);
	extern Attribute_t* Attribute_NewDouble(double value_);
	extern Attribute_t* Attribute_NewString(char* value_);
	extern Attribute_t* Attribute_NewVector(double x, double y);
	extern Attribute_t* Attribute_NewPointer(void* value);

	extern void Attribute_UpdateString(Attribute_t* att, char* value_);
	extern void Attribute_SetPointer(Attribute_t* att_, void* ptr);
	extern void Attribute_Free(Attribute_t* att_);

#ifdef __cplusplus
}
#endif

#endif