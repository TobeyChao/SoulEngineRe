#include "SMatrix4x4.h"
#include "SQuaternion.h"
#include "SVector3.h"
#include "SVector4.h"

namespace Soul
{
	namespace Core
	{
		SMatrix4x4::SMatrix4x4()
			:
			mat()
		{
			memset(mat, 0, sizeof(mat));
		}
		SMatrix4x4& SMatrix4x4::operator=(const SMatrix4x4& mat4x4)
		{
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					mat[i][j] = mat4x4.mat[i][j];
				}
			}
			return *this;
		}
		SMatrix4x4 SMatrix4x4::operator+(const SMatrix4x4& mat4x4) const
		{
			SMatrix4x4 Mat4x4;
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					Mat4x4.mat[i][j] = mat[i][j] + mat4x4.mat[i][j];
				}
			}

			return Mat4x4;
		}
		SMatrix4x4& SMatrix4x4::operator+=(const SMatrix4x4& mat4x4)
		{
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					mat[i][j] = mat[i][j] + mat4x4.mat[i][j];
				}
			}
			return *this;
		}
		SMatrix4x4 SMatrix4x4::operator-(const SMatrix4x4& mat4x4) const
		{

			SMatrix4x4 Mat4x4;
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					Mat4x4.mat[i][j] = mat[i][j] - mat4x4.mat[i][j];
				}
			}
			return Mat4x4;
		}
		SMatrix4x4& SMatrix4x4::operator-=(const SMatrix4x4& mat4x4)
		{
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					mat[i][j] = mat[i][j] - mat4x4.mat[i][j];
				}
			}
			return *this;
		}
		SMatrix4x4 SMatrix4x4::operator*(const SMatrix4x4& mat4x4) const
		{
			SMatrix4x4 Mat4x4;
			for (size_t i = 0; i < matrixRow; i++)
				for (size_t j = 0; j < matrixColumn; j++)
					Mat4x4.mat[i][j] = 0;
			size_t i, j;
			for (size_t r = 0; r < matrixRow; r++)
			{
				for (size_t c = 0; c < matrixColumn; c++)
				{
					for (i = 0, j = 0; i < matrixColumn && j < matrixRow; j++, i++)
					{
						Mat4x4.mat[r][c] += mat[r][j] * mat4x4.mat[j][c];
					}
				}
			}
			return Mat4x4;
		}
		bool SMatrix4x4::operator==(const SMatrix4x4& mat4x4) const
		{
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					if (mat[i][j] != mat4x4.mat[i][j])
					{
						return false;
					}
				}
			}
			return true;
		}
		bool SMatrix4x4::operator!=(const SMatrix4x4& mat4x4) const
		{
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					if (mat[i][j] != mat4x4.mat[i][j])
					{
						return true;
					}
				}
			}
			return false;
		}
		/*************non-member-function*************/
		std::ostream& operator<<(std::ostream& output, const SMatrix4x4& mat4x4)
		{
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					output << mat4x4.mat[i][j] << "\t";
				}
				std::cout << std::endl;
			}
			return output;
		}
		void VectorSinCos(SVector3* pSin, SVector3* pCos, const SVector3& angle)
		{
			pSin->x = sinf(angle.x);
			pSin->x = sinf(angle.y);
			pSin->x = sinf(angle.z);

			pCos->x = cosf(angle.x);
			pCos->x = cosf(angle.y);
			pCos->x = cosf(angle.z);
		}
		void ScalarSinCos(float* pSin, float* pCos, float angle)
		{
			*pCos = cosf(angle);
			*pSin = sinf(angle);
		}
		SMatrix4x4 Matrix4x4Identity()
		{
			SMatrix4x4 mat4x4;
			for (size_t i = 0; i < matrixRow; i++)
			{
				mat4x4.mat[i][i] = 1;
			}
			return mat4x4;
		}
		SMatrix4x4 MatrixRotationRollPitchYaw(float pitch, float yaw, float roll)
		{
			SQuaternion q = QuaternionRotationObjectToInertial(pitch, yaw, roll);

			//From Quaternion To Matrix
			SMatrix4x4 matrix = Matrix4x4Identity();
			matrix.mat[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
			matrix.mat[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
			matrix.mat[0][2] = 2.0f * (q.x * q.z - q.w * q.y);

			matrix.mat[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
			matrix.mat[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
			matrix.mat[1][2] = 2.0f * (q.y * q.z + q.w * q.x);

			matrix.mat[2][0] = 2.0f * (q.x * q.z + q.w * q.y);
			matrix.mat[2][1] = 2.0f * (q.y * q.z - q.w * q.x);
			matrix.mat[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
			return matrix;
		}
		SMatrix4x4 MatrixPerspectiveFovLH(float fovy, float aspect, float nearZ, float farZ)
		{
			float SinFov;
			float CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * fovy);

			float yScale = CosFov / SinFov;
			float xScale = yScale / aspect;
			float fRange = farZ / (farZ - nearZ);

			SMatrix4x4 mat4x4;
			mat4x4.mat[0][0] = xScale;
			mat4x4.mat[1][1] = yScale;
			mat4x4.mat[2][2] = fRange;
			mat4x4.mat[2][3] = 1;
			mat4x4.mat[3][2] = -fRange * nearZ;

			return mat4x4;
		}
		SMatrix4x4 MatrixOrthographicLH(float width, float height, float nearZ, float farZ)
		{
			float fRange = 1.0f / (farZ - nearZ);

			SMatrix4x4 mat4x4;
			mat4x4.mat[0][0] = 2.0f / width;
			mat4x4.mat[0][1] = 0.0f;
			mat4x4.mat[0][2] = 0.0f;
			mat4x4.mat[0][3] = 0.0f;

			mat4x4.mat[1][0] = 0.0f;
			mat4x4.mat[1][1] = 2.0f / height;
			mat4x4.mat[1][2] = 0.0f;
			mat4x4.mat[1][3] = 0.0f;

			mat4x4.mat[2][0] = 0.0f;
			mat4x4.mat[2][1] = 0.0f;
			mat4x4.mat[2][2] = fRange;
			mat4x4.mat[2][3] = 0.0f;

			mat4x4.mat[3][0] = 0.0f;
			mat4x4.mat[3][1] = 0.0f;
			mat4x4.mat[3][2] = -fRange * nearZ;
			mat4x4.mat[3][3] = 1.0f;
			return mat4x4;
		}
		SMatrix4x4 MatrixLookAtLH(const SVector3& eye, const SVector3& at, const SVector3& up)
		{
			SVector3 axisZ = at - eye;
			Normalize(axisZ);
			SVector3 axisX = up.Cross(axisZ);
			Normalize(axisX);
			SVector3 axisY = axisZ.Cross(axisX);
			SVector3 ngeye = -eye;
			float tx = axisX.Dot(ngeye);
			float ty = axisY.Dot(ngeye);
			float tz = axisZ.Dot(ngeye);
			SMatrix4x4 mat4x4;
			mat4x4.mat[0][0] = axisX.x;
			mat4x4.mat[0][1] = axisX.y;
			mat4x4.mat[0][2] = axisX.z;
			mat4x4.mat[0][3] = tx;
			mat4x4.mat[1][0] = axisY.x;
			mat4x4.mat[1][1] = axisY.y;
			mat4x4.mat[1][2] = axisY.z;
			mat4x4.mat[1][3] = ty;
			mat4x4.mat[2][0] = axisZ.x;
			mat4x4.mat[2][1] = axisZ.y;
			mat4x4.mat[2][2] = axisZ.z;
			mat4x4.mat[2][3] = tz;
			mat4x4.mat[3][3] = 1;
			MatrixTranspose(mat4x4);
			return mat4x4;
		}
		SMatrix4x4 MatrixInvViewMatrix(const SMatrix4x4& matrix)
		{
			SMatrix4x4 mat4x4 = matrix;
			mat4x4.mat[0][0] = matrix.mat[0][0];
			mat4x4.mat[0][1] = matrix.mat[1][0];
			mat4x4.mat[0][2] = matrix.mat[2][0];
			mat4x4.mat[1][0] = matrix.mat[0][1];
			mat4x4.mat[1][1] = matrix.mat[1][1];
			mat4x4.mat[1][2] = matrix.mat[2][1];
			mat4x4.mat[2][0] = matrix.mat[0][2];
			mat4x4.mat[2][1] = matrix.mat[1][2];
			mat4x4.mat[2][2] = matrix.mat[2][2];
			mat4x4.mat[3][0] = 0;
			mat4x4.mat[3][1] = 0;
			mat4x4.mat[3][2] = 0;
			mat4x4.mat[3][3] = 1;
			return mat4x4;
		}
		void MatrixTranspose(SMatrix4x4& mat4x4)
		{
			float tmp;
			for (size_t i = 0; i < matrixRow; i++)
			{
				for (size_t j = 0; j < matrixColumn; j++)
				{
					if (i <= j)
					{
						continue;
					}
					if (mat4x4.mat[i][j] == mat4x4.mat[j][i])
					{
						continue;
					}
					else
					{
						tmp = mat4x4.mat[j][i];
						mat4x4.mat[j][i] = mat4x4.mat[i][j];
						mat4x4.mat[i][j] = tmp;
					}
				}
			}
		}
		SMatrix4x4 MatrixTranslation(const SVector3& pos)
		{
			return MatrixTranslation(pos.x, pos.y, pos.z);
		}
		SMatrix4x4 MatrixTranslation(float offset_x, float offset_y, float offset_z)
		{
			SMatrix4x4 mat4x4 = Matrix4x4Identity();
			mat4x4.mat[3][0] = offset_x;
			mat4x4.mat[3][1] = offset_y;
			mat4x4.mat[3][2] = offset_z;
			return mat4x4;
		}
		SMatrix4x4 MatrixRotationX(float angle)
		{
			float sin, cos;
			SMatrix4x4 mat4x4 = Matrix4x4Identity();
			ScalarSinCos(&sin, &cos, angle);
			mat4x4.mat[1][1] = cos;
			mat4x4.mat[1][2] = sin;
			mat4x4.mat[2][1] = -sin;
			mat4x4.mat[2][2] = cos;
			return mat4x4;
		}
		SMatrix4x4 MatrixRotationY(float angle)
		{
			float sin, cos;
			SMatrix4x4 mat4x4 = Matrix4x4Identity();
			ScalarSinCos(&sin, &cos, angle);
			mat4x4.mat[0][0] = cos;
			mat4x4.mat[0][2] = -sin;
			mat4x4.mat[2][0] = sin;
			mat4x4.mat[2][2] = cos;
			return mat4x4;
		}
		SMatrix4x4 MatrixRotationZ(float angle)
		{
			float sin, cos;
			SMatrix4x4 mat4x4 = Matrix4x4Identity();
			ScalarSinCos(&sin, &cos, angle);
			mat4x4.mat[0][0] = cos;
			mat4x4.mat[0][1] = sin;
			mat4x4.mat[1][0] = -sin;
			mat4x4.mat[1][1] = cos;
			return mat4x4;
		}
		SMatrix4x4 MatrixRotationAboutAxis(const SVector3& axis, float angle)
		{
			return SMatrix4x4();
		}
		SMatrix4x4 MatrixScaling(float scale_x, float scale_y, float scale_z)
		{
			SMatrix4x4 mat4x4 = Matrix4x4Identity();
			mat4x4.mat[0][0] = scale_x;
			mat4x4.mat[1][1] = scale_y;
			mat4x4.mat[2][2] = scale_z;
			return mat4x4;
		}
		SMatrix4x4 MatrixShadow(const SVector4& plane, const SVector4& light)
		{
			SVector4 planeVec = plane;
			SMatrix4x4 mat4x4 = Matrix4x4Identity();
			planeVec.PlaneNormalize();
			float dot = Dot(planeVec, light);
			planeVec = -planeVec;
			mat4x4.mat[0][0] = dot + planeVec.x * light.x;
			mat4x4.mat[0][1] = planeVec.x * light.y;
			mat4x4.mat[0][2] = planeVec.x * light.z;
			mat4x4.mat[0][3] = planeVec.x * light.w;
			mat4x4.mat[1][0] = planeVec.y * light.x;
			mat4x4.mat[1][1] = dot + planeVec.y * light.y;
			mat4x4.mat[1][2] = planeVec.y * light.z;
			mat4x4.mat[1][3] = planeVec.y * light.w;
			mat4x4.mat[2][0] = planeVec.z * light.x;
			mat4x4.mat[2][1] = planeVec.z * light.y;
			mat4x4.mat[2][2] = dot + planeVec.z * light.z;
			mat4x4.mat[2][3] = planeVec.z * light.w;
			mat4x4.mat[3][0] = planeVec.w * light.x;
			mat4x4.mat[3][1] = planeVec.w * light.y;
			mat4x4.mat[3][2] = planeVec.w * light.z;
			mat4x4.mat[3][3] = dot + planeVec.w * light.w;
			return mat4x4;
		}
		SVector3 operator*(const SVector3& vec3, const SMatrix4x4& matrix)
		{
			return SVector3(
				vec3.x * matrix.mat[0][0] + vec3.y * matrix.mat[1][0] + vec3.z * matrix.mat[2][0] + matrix.mat[3][0],
				vec3.x * matrix.mat[0][1] + vec3.y * matrix.mat[1][1] + vec3.z * matrix.mat[2][1] + matrix.mat[3][1],
				vec3.x * matrix.mat[0][2] + vec3.y * matrix.mat[1][2] + vec3.z * matrix.mat[2][2] + matrix.mat[3][2]);
		}
	}
}