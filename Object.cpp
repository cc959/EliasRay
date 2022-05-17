#include "Object.h"

void Object::setType(int type)
{
    data &= (1u << 31) | (1u << 30);
    data |= type;
}

void Object::makeMin()
{
    data &= UINT32_MAX - (1u << 31);
}

void Object::makeMax()
{
    data |= (1u << 31);
}

void Object::setNegative(bool b)
{
    data &= UINT32_MAX - (1u << 30);
    data |= uint(b) << 30;
}