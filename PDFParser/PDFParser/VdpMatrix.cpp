#include "VdpMatrix.h"

VdpMatrix::VdpMatrix()
{
	double matrix[MATRIX_SIZE] = { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
	this->SetMatrix(matrix);
}

VdpMatrix::~VdpMatrix()
{
}

VdpMatrix::VdpMatrix(double matrix[MATRIX_SIZE])
{
	this->SetMatrix(matrix);
}

void VdpMatrix::SetMatrix(double matrix[MATRIX_SIZE])
{
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		m_matrix[i] = matrix[i];
	}
}

void VdpMatrix::GetMatrix(double matrix[MATRIX_SIZE])
{
	for (int i = 0; i < MATRIX_SIZE; i++)
	{
		matrix[i] = m_matrix[i];
	}
}