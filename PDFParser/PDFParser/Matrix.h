#pragma once

class Matrix
{
public:
	Matrix();
	Matrix(double a, double b, double c, double d, double e, double f);
	~Matrix();

	void SetMatrix(double a, double b, double c, double d, double e, double f);
	void SetMatrix(double mat[6]);
	void GetMatrix(double *pa, double *pb, double *pc, double *pd, double *pe, double *pf);
	void GetMatrix(double mat[6]);
	void Translate(double x, double y);

	double GetA() { return m_a; }
	double GetB() { return m_b; }
	double GetC() { return m_c; }
	double GetD() { return m_d; }
	double GetE() { return m_e; }
	double GetF() { return m_f; }

protected:
	double m_a;
	double m_b;
	double m_c;
	double m_d;
	double m_e;
	double m_f;


public:
	friend bool operator==(Matrix &a, Matrix &b);
	friend bool operator!=(Matrix &a, Matrix &b);
};

