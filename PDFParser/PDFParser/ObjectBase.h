#pragma once

#include <vector>
#include <podofo/podofo.h>

#include "Matrix.h"



using namespace PoDoFo;

typedef enum EObjectType
{
	eTextObj = 0,
	eImageObj
};

class ObjectBase
{
public:
	ObjectBase(EObjectType eObjType);
	virtual ~ObjectBase() = 0;

	void SetTransformMatrix(Matrix cm);
	Matrix GetTransformMatrix();

	void SetVariableData(bool bVariable);
	void SetConstant();
	bool IsVariableData();

	EObjectType GetObjectType();

protected:
	EObjectType m_eObjType;
	Matrix m_cm;
	bool m_bVariable;

public:
	friend bool operator== (ObjectBase &obj1, ObjectBase &obj2);
	friend bool operator!= (ObjectBase &obj1, ObjectBase &obj2);
};

typedef std::vector<ObjectBase*> PageObjects;
typedef std::vector<PageObjects*> DocObjects;
