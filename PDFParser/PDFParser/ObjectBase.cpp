#include "ObjectBase.h"




ObjectBase::ObjectBase(EObjectType eObjType)
{
	m_eObjType = eObjType;
	m_bVariable = true;
}


ObjectBase::~ObjectBase()
{
}

void ObjectBase::SetTransformMatrix(Matrix cm)
{
	m_cm = cm;
}

Matrix ObjectBase::GetTransformMatrix()
{
	return m_cm;
}

void ObjectBase::SetVariableData(bool bVariable)
{
	m_bVariable = bVariable;
}

void ObjectBase::SetConstant()
{
	m_bVariable = false;
}

bool ObjectBase::IsVariableData()
{
	return m_bVariable;
}

EObjectType ObjectBase::GetObjectType()
{
	return m_eObjType;
}

bool operator== (ObjectBase &obj1, ObjectBase &obj2)
{

	if (obj1.GetObjectType() == obj2.GetObjectType())
	{
		return obj1.GetTransformMatrix() == obj2.GetTransformMatrix();
	}

	return false;
}

bool operator!= (ObjectBase &obj1, ObjectBase &obj2)
{
	return !(obj1 == obj2);
}