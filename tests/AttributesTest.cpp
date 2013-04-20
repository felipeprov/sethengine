#include "gtest/gtest.h"
extern "C"
{
#include "attributes/attributes.h"
}

TEST(Attributes, CreateInt)
{
	int* value;
	Attribute_t* att = Attribute_NewInteger(39);
	value = Attribute_ToInteger(att);

	ASSERT_EQ(*value, 39);

	Attribute_Free(att);
}

TEST(Attributes, CreateDouble)
{
	double* value;
	Attribute_t* att = Attribute_NewDouble(64.1259);
	value = Attribute_ToDouble(att);

	ASSERT_EQ(*value, 64.1259);

	Attribute_Free(att);
}

TEST(Attributes, CreateString)
{
	char* value;
	Attribute_t* att = Attribute_NewString("Testing");
	value = Attribute_ToString(att);

	ASSERT_STREQ(value, "Testing");

	Attribute_Free(att);
}

TEST(Attributes, CreateVector)
{
	Vector2d_t* value;
	Attribute_t* att = Attribute_NewVector(3.14,15.31);
	value = Attribute_ToVector2d(att);

	ASSERT_EQ(value->x, 3.14);
	ASSERT_EQ(value->y, 15.31);

	Attribute_Free(att);
}
