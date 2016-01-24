#pragma once

#define MATRIX_SIZE		6

class VdpMatrix
{
public:
	VdpMatrix();
	VdpMatrix(double matrix[MATRIX_SIZE]);
	~VdpMatrix();

	void SetMatrix(double matrix[MATRIX_SIZE]);
	void GetMatrix(double matrix[MATRIX_SIZE]);

	inline double GetXScale(void)
	{
		return m_matrix[0];
	}
	inline double GetYScale(void)
	{
		return m_matrix[3];
	}
	inline double GetXTranslate(void)
	{
		return m_matrix[4];
	}
	inline double GetYTranslate(void)
	{
		return m_matrix[5];
	}
	inline double GetXRotation(void)
	{
		return m_matrix[1];
	}
	inline double GetYRotation(void)
	{
		return m_matrix[2];
	}

	bool operator==(VdpMatrix & other)
	{
		return (*this).m_matrix[0] == other.m_matrix[0] &&
			(*this).m_matrix[1] == other.m_matrix[1] &&
			(*this).m_matrix[2] == other.m_matrix[2] &&
			(*this).m_matrix[3] == other.m_matrix[3] &&
			(*this).m_matrix[4] == other.m_matrix[4] &&
			(*this).m_matrix[5] == other.m_matrix[5];
	}

	bool operator!=(VdpMatrix & other)
	{
		return !((*this) == other);
	}

private:
	double m_matrix[MATRIX_SIZE];
};

