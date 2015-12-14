#include "Matrix.h"


Matrix::Matrix()
{
	SetMatrix(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

Matrix::Matrix( double a, double b, double c, double d, double e, double f )
{
	SetMatrix(a, b, c, d, e, f);
}


Matrix::~Matrix()
{
}

void Matrix::SetMatrix(double a, double b, double c, double d, double e, double f)
{
	m_a = a;
	m_b = b;
	m_c = c;
	m_d = d;
	m_e = e;
	m_f = f;
}

void Matrix::SetMatrix(double mat[6])
{
	SetMatrix(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5]);
}

void Matrix::GetMatrix(double *pa, double *pb, double *pc, double *pd, double *pe, double *pf)
{
	*pa = m_a;
	*pb = m_b;
	*pc = m_c;
	*pd = m_d;
	*pe = m_e;
	*pf = m_f;
}

void Matrix::GetMatrix(double mat[6])
{
	mat[0] = m_a;
	mat[1] = m_b;
	mat[2] = m_c;
	mat[3] = m_d;
	mat[4] = m_e;
	mat[5] = m_f;
}

void Matrix::Translate(double x, double y)
{
	m_e += (x * m_a);
	m_f += (y * m_d);
}

bool operator==(Matrix &a, Matrix &b)
{
	return a.m_a == b.m_a &&
		a.m_b == b.m_b &&
		a.m_c == b.m_c &&
		a.m_d == b.m_d &&
		a.m_e == b.m_e &&
		a.m_f == b.m_f;
}

bool operator!=(Matrix &a, Matrix &b)
{
	return !(a == b);
}