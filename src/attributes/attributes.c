#include "attributes.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "math/vector2d.h"

/***********************************************
@function
************************************************/
int* Attribute_ToInteger(Attribute_t* att_)
{
	int* ptr;
	assert(att_->type == AT_INT);
	ptr = (int*)att_->data;

	return ptr;
}

/***********************************************
@function
************************************************/
double* Attribute_ToDouble(Attribute_t* att_)
{
	double* ptr;
	assert(att_->type == AT_DOUBLE);
	ptr = (double*)att_->data;

	return ptr;
}

/***********************************************
@function
************************************************/
char* Attribute_ToString(Attribute_t* att_)
{
	char* ptr;
	assert(att_->type == AT_STRING);
	ptr = (char*)att_->data;

	return ptr;
}

/***********************************************
@function
************************************************/
Vector2d_t* Attribute_ToVector2d(Attribute_t* att_)
{
	Vector2d_t* ptr;
	assert(att_->type == AT_VECTOR2D);
	ptr = (Vector2d_t*)att_->data;

	return ptr;
}

void* Attribute_ToPointer(Attribute_t* att_)
{
	return att_->data;
}

/***********************************************
@function
************************************************/
Attribute_t* Attribute_NewInteger(int value_)
{
	Attribute_t* att = (Attribute_t*)malloc(sizeof(Attribute_t));
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = value_;
	att->data = ptr;

	att->type = AT_INT;

	return att;
}

/***********************************************
@function
************************************************/
Attribute_t* Attribute_NewDouble(double value_)
{
	Attribute_t* att = (Attribute_t*)malloc(sizeof(Attribute_t));
	double* ptr = (double*)malloc(sizeof(double));
	*ptr = value_;
	att->data = ptr;

	att->type = AT_DOUBLE;

	return att;
}

/***********************************************
@function
************************************************/
Attribute_t* Attribute_NewString(char* value_)
{
	Attribute_t* att = (Attribute_t*)malloc(sizeof(Attribute_t));
	char* str = (char*)malloc(sizeof(char) * strlen(value_) + 1);

	strcpy(str, value_);
	att->data = str;
	att->type = AT_STRING;
	return att;
}

extern Attribute_t* Attribute_NewPointer(void* value)
{
	Attribute_t* att = (Attribute_t*)malloc(sizeof(Attribute_t));

	att->data = value;
	att->type = AT_POINTER;

	return att;
}
/***********************************************
@function
************************************************/
Attribute_t* Attribute_NewVector(double x, double y)
{
	Attribute_t* att = (Attribute_t*)malloc(sizeof(Attribute_t));
	Vector2d_t* vector = (Vector2d_t*)malloc(sizeof(Vector2d_t));

	vector->x = x;
	vector->y = y;
	att->data = vector;
	att->type = AT_VECTOR2D;

	return att;
}


void Attribute_UpdateString(Attribute_t* att, char* value_)
{
	char* str = (char*)malloc(sizeof(char) * strlen(value_) + 1);
	free(att->data);

	strcpy(str, value_);
	att->data = str;
	att->type = AT_STRING;
}

void Attribute_SetPointer(Attribute_t* att_, void* ptr)
{
	att_->data = ptr;
}
/***********************************************
@function
************************************************/
void Attribute_Free(Attribute_t* att_)
{
	free(att_->data);
	free(att_);
}
