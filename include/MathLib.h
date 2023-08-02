// Credit to Sanjay Madhav

#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <cmath>
#include <memory.h>
#include <limits>
#include <fstream>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>

namespace junebug
{
	const float Pi = 3.1415926535f;
	const float TwoPi = Pi * 2.0f;
	const float PiOver2 = Pi / 2.0f;
	const float Infinity = std::numeric_limits<float>::infinity();
	const float NegInfinity = -std::numeric_limits<float>::infinity();

	enum RoundDir
	{
		None = 0,
		Up = 1,
		Down = -1,
		Closest = 2
	};

	template <typename T>
	T Round(T value, RoundDir dir = RoundDir::Closest)
	{
		switch (dir)
		{
		case RoundDir::None:
			return value;
		case RoundDir::Up:
			return ceil(value);
		case RoundDir::Down:
			return floor(value);
		case RoundDir::Closest:
			return round(value);
		default:
			return value;
		}
	}

	inline bool FloatsEqual(float a, float b, float epsilon = 0.0001f)
	{
		return fabs(a - b) < epsilon;
	}

	template <typename T>
	[[nodiscard]] inline int Sign(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	[[nodiscard]] inline float ToRadians(float degrees)
	{
		return degrees * Pi / 180.0f;
	}

	[[nodiscard]] inline float ToDegrees(float radians)
	{
		return radians * 180.0f / Pi;
	}

	[[nodiscard]] inline bool NearZero(float val, float epsilon = 0.001f)
	{
		if (fabs(val) <= epsilon)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	[[nodiscard]] T Max(const T &a, const T &b)
	{
		return (a < b ? b : a);
	}

	template <typename T>
	[[nodiscard]] T Min(const T &a, const T &b)
	{
		return (a < b ? a : b);
	}

	template <typename T>
	[[nodiscard]] T Clamp(const T &value, const T &lower, const T &upper)
	{
		return Min(upper, Max(lower, value));
	}

	[[nodiscard]] inline float Abs(float value)
	{
		return fabs(value);
	}

	[[nodiscard]] inline float Cos(float angle)
	{
		return cosf(angle);
	}

	[[nodiscard]] inline float Sin(float angle)
	{
		return sinf(angle);
	}

	[[nodiscard]] inline float Tan(float angle)
	{
		return tanf(angle);
	}

	[[nodiscard]] inline float Asin(float value)
	{
		return asinf(value);
	}

	[[nodiscard]] inline float Acos(float value)
	{
		return acosf(value);
	}

	[[nodiscard]] inline float Atan2(float y, float x)
	{
		return atan2f(y, x);
	}

	[[nodiscard]] inline float Cot(float angle)
	{
		return 1.0f / Tan(angle);
	}

	[[nodiscard]] inline float Lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	[[nodiscard]] inline float Sqrt(float value)
	{
		return sqrtf(value);
	}

	[[nodiscard]] inline float Fmod(float numer, float denom)
	{
		return fmod(numer, denom);
	}
}

using namespace junebug;

// 2D Vector
template <typename T = float>
struct Vec2
{
public:
	T x = 0;
	T y = 0;

	Vec2()
	{
	}

	explicit Vec2(T inX, T inY)
		: x(inX), y(inY)
	{
	}

	explicit Vec2(T *inArray)
		: x(inArray[0]), y(inArray[1])
	{
	}

	template <typename U>
	explicit Vec2(const Vec2<U> inVec)
		: x(static_cast<T>(inVec.x)), y(static_cast<T>(inVec.y))
	{
	}

	// Set both components in one line
	void Set(T inX, T inY)
	{
		x = inX;
		y = inY;
	}

	// Printable
	[[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const Vec2 &v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}

	// Vector addition (a + b)
	[[nodiscard]] friend Vec2 operator+(const Vec2 &a, const Vec2 &b)
	{
		return Vec2(a.x + b.x, a.y + b.y);
	}

	// Vector subtraction (a - b)
	[[nodiscard]] friend Vec2 operator-(const Vec2 &a, const Vec2 &b)
	{
		return Vec2(a.x - b.x, a.y - b.y);
	}

	// Vector equality (a == b)
	[[nodiscard]] friend bool operator==(const Vec2 &a, const Vec2 &b)
	{
		return a.x == b.x && a.y == b.y;
	}
	// Vector inequality (a != b)
	[[nodiscard]] friend bool operator!=(const Vec2 &a, const Vec2 &b)
	{
		return a.x != b.x || a.y != b.y;
	}
	// Vector less-than (a < b)
	[[nodiscard]] friend bool operator<(const Vec2 &a, const Vec2 &b)
	{
		return a.x < b.x && a.y < b.y;
	}
	// Vector less-than-equals (a <= b)
	[[nodiscard]] friend bool operator<=(const Vec2 &a, const Vec2 &b)
	{
		return a.x <= b.x && a.y <= b.y;
	}
	// Vector greater-than (a > b)
	[[nodiscard]] friend bool operator>(const Vec2 &a, const Vec2 &b)
	{
		return a.x > b.x && a.y > b.y;
	}
	// Vector greater-than-equals (a >= b)
	[[nodiscard]] friend bool operator>=(const Vec2 &a, const Vec2 &b)
	{
		return a.x >= b.x && a.y >= b.y;
	}

	// Component-wise multiplication
	// (a.x * b.x, ...)
	template <typename U>
	[[nodiscard]] friend Vec2 operator*(const Vec2 &a, const Vec2<U> &b)
	{
		return Vec2(a.x * b.x, a.y * b.y);
	}

	// Scalar multiplication
	[[nodiscard]] friend Vec2 operator*(const Vec2 &vec, float scalar)
	{
		return Vec2(vec.x * scalar, vec.y * scalar);
	}

	// Scalar multiplication
	[[nodiscard]] friend Vec2 operator*(float scalar, const Vec2 &vec)
	{
		return Vec2(vec.x * scalar, vec.y * scalar);
	}

	// Component-wise division
	// (a.x / b.x, ...)
	template <typename U>
	[[nodiscard]] friend Vec2 operator/(const Vec2 &a, const Vec2<U> &b)
	{
		return Vec2(a.x / b.x, a.y / b.y);
	}

	// Scalar division
	[[nodiscard]] friend Vec2 operator/(const Vec2 &vec, float scalar)
	{
		return Vec2(vec.x / scalar, vec.y / scalar);
	}

	// Scalar *=
	Vec2 &operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	// Vec *=
	Vec2 &operator*=(Vec2 vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	// Scalar /=
	Vec2 &operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	// Vector +=
	Vec2 &operator+=(const Vec2 &right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}
	// Vector +
	template <typename U>
	[[nodiscard]] Vec2 operator+(const Vec2<U> &right) const
	{
		return Vec2(x + right.x, y + right.y);
	}

	// Vector -=
	Vec2 &operator-=(const Vec2 &right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}
	// Vector -
	template <typename U>
	[[nodiscard]] Vec2 operator-(const Vec2<U> &right) const
	{
		return Vec2(x - right.x, y - right.y);
	}

	// Length squared of vector
	[[nodiscard]] float LengthSq() const
	{
		return (x * x + y * y);
	}

	// Length of vector
	[[nodiscard]] float Length() const
	{
		return (junebug::Sqrt(LengthSq()));
	}

	// Normalize this vector
	void Normalize()
	{
		float length = Length();
		if (NearZero(length))
			return;
		x /= length;
		y /= length;
	}

	// Check if the vector is equal to zero
	[[nodiscard]] bool IsZero() const
	{
		return FloatsEqual(x, 0.0f) && FloatsEqual(y, 0.0f);
	}

	// Round the vector
	[[nodiscard]] Vec2 Floor()
	{
		return Vec2(floor(x), floor(y));
	}
	[[nodiscard]] Vec2 Ceil()
	{
		return Vec2(ceil(x), ceil(y));
	}
	[[nodiscard]] Vec2 Round(RoundDir dir = RoundDir::Closest)
	{
		x = junebug::Round(x, dir);
		y = junebug::Round(y, dir);
		return *this;
	}

	// Normalize the provided vector
	[[nodiscard]] static Vec2 Normalize(const Vec2 &vec)
	{
		Vec2 temp = vec;
		temp.Normalize();
		return temp;
	}

	// Minimum of two vectors
	[[nodiscard]] static Vec2 Min(const Vec2 &a, const Vec2 &b)
	{
		return Vec2(junebug::Min(a.x, b.x), junebug::Min(a.y, b.y));
	}
	// Maximum of two vectors
	[[nodiscard]] static Vec2 Max(const Vec2 &a, const Vec2 &b)
	{
		return Vec2(junebug::Max(a.x, b.x), junebug::Max(a.y, b.y));
	}

	// Dot product between two vectors (a dot b)
	[[nodiscard]] static float Dot(const Vec2 &a, const Vec2 &b)
	{
		return (a.x * b.x + a.y * b.y);
	}

	// Lerp from A to B by f
	[[nodiscard]] static Vec2 Lerp(const Vec2 &a, const Vec2 &b, float f)
	{
		return Vec2(a + f * (b - a));
	}

	// Reflect V about (normalized) N
	[[nodiscard]] static Vec2 Reflect(const Vec2 &v, const Vec2 &n)
	{
		return v - 2.0f * Vec2::Dot(v, n) * n;
	}

	// Transform vector by matrix
	[[nodiscard]] static Vec2 Transform(const Vec2 &vec, const class Matrix3 &mat, float w = 1.0f);

	// Get distance between two points
	[[nodiscard]] static float Distance(const Vec2 &a, const Vec2 &b)
	{
		return (a - b).Length();
	}

	[[nodiscard]] static Vec2 LengthDir(float length, float angle)
	{
		return Vec2(length * junebug::Cos(ToRadians(angle)), -length * junebug::Sin(ToRadians(angle)));
	}

	template <typename U = float>
	[[nodiscard]] inline static Vec2 Dir(U angle)
	{
		return LengthDir(1.0f, (float)angle);
	}

	static const Vec2<T> Zero;
	static const Vec2<T> One;
	static const Vec2<T> UnitX;
	static const Vec2<T> UnitY;
	static const Vec2<T> NegUnitX;
	static const Vec2<T> NegUnitY;
};

namespace junebug
{
	typedef std::vector<Vec2<float>> Vertices;
	typedef std::shared_ptr<Vertices> VerticesPtr;
}

// 3D Vector
template <typename T = float>
struct Vec3
{
public:
	T x;
	T y;
	T z;

	Vec3()
		: x(0.0f), y(0.0f), z(0.0f)
	{
	}

	explicit Vec3(T inX, T inY, T inZ)
		: x(inX), y(inY), z(inZ)
	{
	}

	explicit Vec3(T *inArray)
		: x(inArray[0]), y(inArray[1]), z(inArray[2])
	{
	}

	// Cast to a const float pointer
	const T *GetAsFloatPtr() const
	{
		return reinterpret_cast<const T *>(&x);
	}

	// Set all three components in one line
	void Set(T inX, T inY, T inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	}

	// Printable
	[[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const Vec3 &v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	// Vector addition (a + b)
	[[nodiscard]] friend Vec3 operator+(const Vec3 &a, const Vec3 &b)
	{
		return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	// Vector subtraction (a - b)
	[[nodiscard]] friend Vec3 operator-(const Vec3 &a, const Vec3 &b)
	{
		return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	// Component-wise multiplication
	[[nodiscard]] friend Vec3 operator*(const Vec3 &left, const Vec3 &right)
	{
		return Vec3(left.x * right.x, left.y * right.y, left.z * right.z);
	}

	// Scalar multiplication
	[[nodiscard]] friend Vec3 operator*(const Vec3 &vec, float scalar)
	{
		return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar multiplication
	[[nodiscard]] friend Vec3 operator*(T scalar, const Vec3 &vec)
	{
		return Vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar *=
	Vec3 &operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	// Vector +=
	Vec3 &operator+=(const Vec3 &right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	// Vector -=
	Vec3 &operator-=(const Vec3 &right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	// Length squared of vector
	[[nodiscard]] T LengthSq() const
	{
		return (x * x + y * y + z * z);
	}

	// Length of vector
	[[nodiscard]] float Length() const
	{
		return (junebug::Sqrt(LengthSq()));
	}

	// Normalize this vector
	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
	}

	// Normalize the provided vector
	[[nodiscard]] static Vec3 Normalize(const Vec3 &vec)
	{
		Vec3 temp = vec;
		temp.Normalize();
		return temp;
	}

	// Dot product between two vectors (a dot b)
	[[nodiscard]] static float Dot(const Vec3 &a, const Vec3 &b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	// Cross product between two vectors (a cross b)
	[[nodiscard]] static Vec3 Cross(const Vec3 &a, const Vec3 &b)
	{
		Vec3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}

	// Lerp from A to B by f
	[[nodiscard]] static Vec3 Lerp(const Vec3 &a, const Vec3 &b, float f)
	{
		return Vec3(a + f * (b - a));
	}

	// Reflect V about (normalized) N
	[[nodiscard]] static Vec3 Reflect(const Vec3 &v, const Vec3 &n)
	{
		return v - 2.0f * Vec3::Dot(v, n) * n;
	}

	[[nodiscard]] static Vec3 Transform(const Vec3 &vec, const class Matrix4 &mat, float w = 1.0f);
	// This will transform the vector and renormalize the w component
	[[nodiscard]] static Vec3 TransformWithPerspDiv(const Vec3 &vec, const class Matrix4 &mat, float w = 1.0f);

	// Transform a Vec3 by a quaternion
	[[nodiscard]] static Vec3 Transform(const Vec3 &v, const class Quaternion &q);

	// Get distance between two points
	[[nodiscard]] static float Distance(const Vec3 &a, const Vec3 &b)
	{
		return (a - b).Length();
	}

	static const Vec3<T> Zero;
	static const Vec3<T> One;
	static const Vec3<T> UnitX;
	static const Vec3<T> UnitY;
	static const Vec3<T> UnitZ;
	static const Vec3<T> NegUnitX;
	static const Vec3<T> NegUnitY;
	static const Vec3<T> NegUnitZ;
	static const Vec3<T> Infinity;
	static const Vec3<T> NegInfinity;
};

// 3x3 Matrix
class Matrix3
{
public:
	float mat[3][3];

	Matrix3()
	{
		*this = Matrix3::Identity;
	}

	explicit Matrix3(float inMat[3][3])
	{
		memcpy(mat, inMat, 9 * sizeof(float));
	}

	// Cast to a const float pointer
	const float *GetAsFloatPtr() const
	{
		return reinterpret_cast<const float *>(&mat[0][0]);
	}

	// Printable
	[[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const Matrix3 &m)
	{
		os << "[[" << m.mat[0][0] << ", " << m.mat[0][1] << ", " << m.mat[0][2] << "]" << std::endl;
		os << " [" << m.mat[1][0] << ", " << m.mat[1][1] << ", " << m.mat[1][2] << "]" << std::endl;
		os << " [" << m.mat[2][0] << ", " << m.mat[2][1] << ", " << m.mat[2][2] << "]]" << std::endl;
		return os;
	}

	// Matrix multiplication
	[[nodiscard]] friend Matrix3 operator*(const Matrix3 &left, const Matrix3 &right)
	{
		Matrix3 retVal;
		// row 0
		retVal.mat[0][0] =
			left.mat[0][0] * right.mat[0][0] +
			left.mat[0][1] * right.mat[1][0] +
			left.mat[0][2] * right.mat[2][0];

		retVal.mat[0][1] =
			left.mat[0][0] * right.mat[0][1] +
			left.mat[0][1] * right.mat[1][1] +
			left.mat[0][2] * right.mat[2][1];

		retVal.mat[0][2] =
			left.mat[0][0] * right.mat[0][2] +
			left.mat[0][1] * right.mat[1][2] +
			left.mat[0][2] * right.mat[2][2];

		// row 1
		retVal.mat[1][0] =
			left.mat[1][0] * right.mat[0][0] +
			left.mat[1][1] * right.mat[1][0] +
			left.mat[1][2] * right.mat[2][0];

		retVal.mat[1][1] =
			left.mat[1][0] * right.mat[0][1] +
			left.mat[1][1] * right.mat[1][1] +
			left.mat[1][2] * right.mat[2][1];

		retVal.mat[1][2] =
			left.mat[1][0] * right.mat[0][2] +
			left.mat[1][1] * right.mat[1][2] +
			left.mat[1][2] * right.mat[2][2];

		// row 2
		retVal.mat[2][0] =
			left.mat[2][0] * right.mat[0][0] +
			left.mat[2][1] * right.mat[1][0] +
			left.mat[2][2] * right.mat[2][0];

		retVal.mat[2][1] =
			left.mat[2][0] * right.mat[0][1] +
			left.mat[2][1] * right.mat[1][1] +
			left.mat[2][2] * right.mat[2][1];

		retVal.mat[2][2] =
			left.mat[2][0] * right.mat[0][2] +
			left.mat[2][1] * right.mat[1][2] +
			left.mat[2][2] * right.mat[2][2];

		return retVal;
	}

	Matrix3 &operator*=(const Matrix3 &right)
	{
		*this = *this * right;
		return *this;
	}

	// Create a scale matrix with x and y scales
	[[nodiscard]] static Matrix3 CreateScale(float xScale, float yScale)
	{
		float temp[3][3] =
			{
				{xScale, 0.0f, 0.0f},
				{0.0f, yScale, 0.0f},
				{0.0f, 0.0f, 1.0f},
			};
		return Matrix3(temp);
	}

	[[nodiscard]] static Matrix3 CreateScale(const Vec2<float> &scaleVector)
	{
		return CreateScale(scaleVector.x, scaleVector.y);
	}

	// Create a scale matrix with a uniform factor
	[[nodiscard]] static Matrix3 CreateScale(float scale)
	{
		return CreateScale(scale, scale);
	}

	// Create a rotation matrix about the Z axis
	// theta is in radians
	[[nodiscard]] static Matrix3 CreateRotation(float theta)
	{
		float temp[3][3] =
			{
				{junebug::Cos(theta), junebug::Sin(theta), 0.0f},
				{-junebug::Sin(theta), junebug::Cos(theta), 0.0f},
				{0.0f, 0.0f, 1.0f},
			};
		return Matrix3(temp);
	}

	// Create a translation matrix (on the xy-plane)
	[[nodiscard]] static Matrix3 CreateTranslation(const Vec2<float> &trans)
	{
		float temp[3][3] =
			{
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{trans.x, trans.y, 1.0f},
			};
		return Matrix3(temp);
	}

	static const Matrix3 Identity;
};

// 4x4 Matrix
class Matrix4
{
public:
	float mat[4][4];

	Matrix4()
	{
		*this = Matrix4::Identity;
	}

	explicit Matrix4(float inMat[4][4])
	{
		memcpy(mat, inMat, 16 * sizeof(float));
	}

	// Cast to a const float pointer
	const float *GetAsFloatPtr() const
	{
		return reinterpret_cast<const float *>(&mat[0][0]);
	}

	// Printable
	[[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const Matrix4 &m)
	{
		os << "[[" << m.mat[0][0] << ", " << m.mat[0][1] << ", " << m.mat[0][2] << ", " << m.mat[0][3] << "]" << std::endl;
		os << " [" << m.mat[1][0] << ", " << m.mat[1][1] << ", " << m.mat[1][2] << ", " << m.mat[1][3] << "]" << std::endl;
		os << " [" << m.mat[2][0] << ", " << m.mat[2][1] << ", " << m.mat[2][2] << ", " << m.mat[2][3] << "]" << std::endl;
		os << " [" << m.mat[3][0] << ", " << m.mat[3][1] << ", " << m.mat[3][2] << ", " << m.mat[3][3] << "]]";
		return os;
	}

	// Matrix multiplication (a * b)
	[[nodiscard]] friend Matrix4 operator*(const Matrix4 &a, const Matrix4 &b)
	{
		Matrix4 retVal;
		// row 0
		retVal.mat[0][0] =
			a.mat[0][0] * b.mat[0][0] +
			a.mat[0][1] * b.mat[1][0] +
			a.mat[0][2] * b.mat[2][0] +
			a.mat[0][3] * b.mat[3][0];

		retVal.mat[0][1] =
			a.mat[0][0] * b.mat[0][1] +
			a.mat[0][1] * b.mat[1][1] +
			a.mat[0][2] * b.mat[2][1] +
			a.mat[0][3] * b.mat[3][1];

		retVal.mat[0][2] =
			a.mat[0][0] * b.mat[0][2] +
			a.mat[0][1] * b.mat[1][2] +
			a.mat[0][2] * b.mat[2][2] +
			a.mat[0][3] * b.mat[3][2];

		retVal.mat[0][3] =
			a.mat[0][0] * b.mat[0][3] +
			a.mat[0][1] * b.mat[1][3] +
			a.mat[0][2] * b.mat[2][3] +
			a.mat[0][3] * b.mat[3][3];

		// row 1
		retVal.mat[1][0] =
			a.mat[1][0] * b.mat[0][0] +
			a.mat[1][1] * b.mat[1][0] +
			a.mat[1][2] * b.mat[2][0] +
			a.mat[1][3] * b.mat[3][0];

		retVal.mat[1][1] =
			a.mat[1][0] * b.mat[0][1] +
			a.mat[1][1] * b.mat[1][1] +
			a.mat[1][2] * b.mat[2][1] +
			a.mat[1][3] * b.mat[3][1];

		retVal.mat[1][2] =
			a.mat[1][0] * b.mat[0][2] +
			a.mat[1][1] * b.mat[1][2] +
			a.mat[1][2] * b.mat[2][2] +
			a.mat[1][3] * b.mat[3][2];

		retVal.mat[1][3] =
			a.mat[1][0] * b.mat[0][3] +
			a.mat[1][1] * b.mat[1][3] +
			a.mat[1][2] * b.mat[2][3] +
			a.mat[1][3] * b.mat[3][3];

		// row 2
		retVal.mat[2][0] =
			a.mat[2][0] * b.mat[0][0] +
			a.mat[2][1] * b.mat[1][0] +
			a.mat[2][2] * b.mat[2][0] +
			a.mat[2][3] * b.mat[3][0];

		retVal.mat[2][1] =
			a.mat[2][0] * b.mat[0][1] +
			a.mat[2][1] * b.mat[1][1] +
			a.mat[2][2] * b.mat[2][1] +
			a.mat[2][3] * b.mat[3][1];

		retVal.mat[2][2] =
			a.mat[2][0] * b.mat[0][2] +
			a.mat[2][1] * b.mat[1][2] +
			a.mat[2][2] * b.mat[2][2] +
			a.mat[2][3] * b.mat[3][2];

		retVal.mat[2][3] =
			a.mat[2][0] * b.mat[0][3] +
			a.mat[2][1] * b.mat[1][3] +
			a.mat[2][2] * b.mat[2][3] +
			a.mat[2][3] * b.mat[3][3];

		// row 3
		retVal.mat[3][0] =
			a.mat[3][0] * b.mat[0][0] +
			a.mat[3][1] * b.mat[1][0] +
			a.mat[3][2] * b.mat[2][0] +
			a.mat[3][3] * b.mat[3][0];

		retVal.mat[3][1] =
			a.mat[3][0] * b.mat[0][1] +
			a.mat[3][1] * b.mat[1][1] +
			a.mat[3][2] * b.mat[2][1] +
			a.mat[3][3] * b.mat[3][1];

		retVal.mat[3][2] =
			a.mat[3][0] * b.mat[0][2] +
			a.mat[3][1] * b.mat[1][2] +
			a.mat[3][2] * b.mat[2][2] +
			a.mat[3][3] * b.mat[3][2];

		retVal.mat[3][3] =
			a.mat[3][0] * b.mat[0][3] +
			a.mat[3][1] * b.mat[1][3] +
			a.mat[3][2] * b.mat[2][3] +
			a.mat[3][3] * b.mat[3][3];

		return retVal;
	}

	Matrix4 &operator*=(const Matrix4 &right)
	{
		*this = *this * right;
		return *this;
	}

	// Invert the matrix - super slow
	void Invert();

	// Get the translation component of the matrix
	[[nodiscard]] Vec3<float> GetTranslation() const
	{
		return Vec3<float>(mat[3][0], mat[3][1], mat[3][2]);
	}

	// Get the X axis of the matrix (forward)
	[[nodiscard]] Vec3<float> GetXAxis() const
	{
		return Vec3<float>::Normalize(Vec3<float>(mat[0][0], mat[0][1], mat[0][2]));
	}

	// Get the Y axis of the matrix (left)
	[[nodiscard]] Vec3<float> GetYAxis() const
	{
		return Vec3<float>::Normalize(Vec3<float>(mat[1][0], mat[1][1], mat[1][2]));
	}

	// Get the Z axis of the matrix (up)
	[[nodiscard]] Vec3<float> GetZAxis() const
	{
		return Vec3<float>::Normalize(Vec3<float>(mat[2][0], mat[2][1], mat[2][2]));
	}

	// Extract the scale component from the matrix
	[[nodiscard]] Vec3<float> GetScale() const
	{
		Vec3<float> retVal;
		retVal.x = Vec3<float>(mat[0][0], mat[0][1], mat[0][2]).Length();
		retVal.y = Vec3<float>(mat[1][0], mat[1][1], mat[1][2]).Length();
		retVal.z = Vec3<float>(mat[2][0], mat[2][1], mat[2][2]).Length();
		return retVal;
	}

	// Create a scale matrix with x, y, and z scales
	[[nodiscard]] static Matrix4 CreateScale(float xScale, float yScale, float zScale)
	{
		float temp[4][4] =
			{
				{xScale, 0.0f, 0.0f, 0.0f},
				{0.0f, yScale, 0.0f, 0.0f},
				{0.0f, 0.0f, zScale, 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f}};
		return Matrix4(temp);
	}

	[[nodiscard]] static Matrix4 CreateScale(const Vec3<float> &scaleVector)
	{
		return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
	}

	// Create a scale matrix with a uniform factor
	[[nodiscard]] static Matrix4 CreateScale(float scale)
	{
		return CreateScale(scale, scale, scale);
	}

	// Rotation about x-axis
	[[nodiscard]] static Matrix4 CreateRotationX(float theta)
	{
		float temp[4][4] =
			{
				{1.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, junebug::Cos(theta), junebug::Sin(theta), 0.0f},
				{0.0f, -junebug::Sin(theta), junebug::Cos(theta), 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f},
			};
		return Matrix4(temp);
	}

	// Rotation about y-axis
	[[nodiscard]] static Matrix4 CreateRotationY(float theta)
	{
		float temp[4][4] =
			{
				{junebug::Cos(theta), 0.0f, -junebug::Sin(theta), 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f},
				{junebug::Sin(theta), 0.0f, junebug::Cos(theta), 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f},
			};
		return Matrix4(temp);
	}

	// Rotation about z-axis
	[[nodiscard]] static Matrix4 CreateRotationZ(float theta)
	{
		float temp[4][4] =
			{
				{junebug::Cos(theta), junebug::Sin(theta), 0.0f, 0.0f},
				{-junebug::Sin(theta), junebug::Cos(theta), 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f},
			};
		return Matrix4(temp);
	}

	// Create a rotation matrix from a quaternion
	[[nodiscard]] static Matrix4 CreateFromQuaternion(const class Quaternion &q);

	[[nodiscard]] static Matrix4 CreateTranslation(const Vec3<float> &trans)
	{
		float temp[4][4] =
			{
				{1.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 0.0f},
				{trans.x, trans.y, trans.z, 1.0f}};
		return Matrix4(temp);
	}

	[[nodiscard]] static Matrix4 CreateLookAt(const Vec3<float> &eye, const Vec3<float> &target, const Vec3<float> &up)
	{
		Vec3<float> zaxis = Vec3<float>::Normalize(target - eye);
		Vec3<float> xaxis = Vec3<float>::Normalize(Vec3<float>::Cross(up, zaxis));
		Vec3<float> yaxis = Vec3<float>::Normalize(Vec3<float>::Cross(zaxis, xaxis));
		Vec3<float> trans;
		trans.x = -Vec3<float>::Dot(xaxis, eye);
		trans.y = -Vec3<float>::Dot(yaxis, eye);
		trans.z = -Vec3<float>::Dot(zaxis, eye);

		float temp[4][4] =
			{
				{xaxis.x, yaxis.x, zaxis.x, 0.0f},
				{xaxis.y, yaxis.y, zaxis.y, 0.0f},
				{xaxis.z, yaxis.z, zaxis.z, 0.0f},
				{trans.x, trans.y, trans.z, 1.0f}};
		return Matrix4(temp);
	}

	[[nodiscard]] static Matrix4 CreateOrtho(float width, float height, float near, float far)
	{
		float temp[4][4] =
			{
				{2.0f / width, 0.0f, 0.0f, 0.0f},
				{0.0f, 2.0f / height, 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f / (far - near), 0.0f},
				{0.0f, 0.0f, near / (near - far), 1.0f}};
		return Matrix4(temp);
	}

	[[nodiscard]] static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
	{
		float yScale = junebug::Cot(fovY / 2.0f);
		float xScale = yScale * height / width;
		float temp[4][4] =
			{
				{xScale, 0.0f, 0.0f, 0.0f},
				{0.0f, yScale, 0.0f, 0.0f},
				{0.0f, 0.0f, far / (far - near), 1.0f},
				{0.0f, 0.0f, -near * far / (far - near), 0.0f}};
		return Matrix4(temp);
	}

	// Create "Simple" View-Projection Matrix from Chapter 6
	[[nodiscard]] static Matrix4 CreateSimpleViewProj(float width, float height)
	{
		float temp[4][4] =
			{
				{2.0f / width, 0.0f, 0.0f, 0.0f},
				{0.0f, 2.0f / height, 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 1.0f}};
		return Matrix4(temp);
	}

	static const Matrix4 Identity;
};

// (Unit) Quaternion
class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion()
	{
		*this = Quaternion::Identity;
	}

	Quaternion(SDL_Rect r)
	{
		x = r.x;
		y = r.y;
		z = r.w;
		w = r.h;
	}

	// This directly sets the quaternion components --
	// don't use for axis/angle
	explicit Quaternion(float inX, float inY, float inZ, float inW)
	{
		Set(inX, inY, inZ, inW);
	}

	// Construct the quaternion from an axis and angle
	// It is assumed that axis is already normalized,
	// and the angle is in radians
	explicit Quaternion(const Vec3<float> &axis, float angle)
	{
		float scalar = junebug::Sin(angle / 2.0f);
		x = axis.x * scalar;
		y = axis.y * scalar;
		z = axis.z * scalar;
		w = junebug::Cos(angle / 2.0f);
	}

	// Directly set the internal components
	void Set(float inX, float inY, float inZ, float inW)
	{
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	void Conjugate()
	{
		x *= -1.0f;
		y *= -1.0f;
		z *= -1.0f;
	}

	[[nodiscard]] float LengthSq() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	[[nodiscard]] float Length() const
	{
		return junebug::Sqrt(LengthSq());
	}

	void Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	// Normalize the provided quaternion
	[[nodiscard]] static Quaternion Normalize(const Quaternion &q)
	{
		Quaternion retVal = q;
		retVal.Normalize();
		return retVal;
	}

	// Linear interpolation
	[[nodiscard]] static Quaternion Lerp(const Quaternion &a, const Quaternion &b, float f)
	{
		Quaternion retVal;
		retVal.x = junebug::Lerp(a.x, b.x, f);
		retVal.y = junebug::Lerp(a.y, b.y, f);
		retVal.z = junebug::Lerp(a.z, b.z, f);
		retVal.w = junebug::Lerp(a.w, b.w, f);
		retVal.Normalize();
		return retVal;
	}

	// Spherical Linear Interpolation
	[[nodiscard]] static Quaternion Slerp(const Quaternion &a, const Quaternion &b, float f)
	{
		float dot = (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
		// Can't compute this if the quaternions are collinear
		if (junebug::Abs(dot) > 0.9995f)
		{
			return Lerp(a, b, f);
		}

		dot = junebug::Clamp(dot, -1.0f, 1.0f);
		float halfTheta = junebug::Acos(dot);
		float sinHalfTheta = junebug::Sqrt(1.0f - dot * dot);

		float ratioA = junebug::Sin((1.0f - f) * halfTheta) / sinHalfTheta;
		float ratioB = junebug::Sin(f * halfTheta) / sinHalfTheta;

		Quaternion retVal;
		retVal.x = a.x * ratioA + b.x * ratioB;
		retVal.y = a.y * ratioA + b.y * ratioB;
		retVal.z = a.z * ratioA + b.z * ratioB;
		retVal.w = a.w * ratioA + b.w * ratioB;
		return retVal;
	}

	// Concatenate
	// Rotate by q FOLLOWED BY p
	[[nodiscard]] static Quaternion Concatenate(const Quaternion &q, const Quaternion &p)
	{
		Quaternion retVal;

		// Vector component is:
		// ps * qv + qs * pv + pv x qv
		Vec3<float> qv(q.x, q.y, q.z);
		Vec3<float> pv(p.x, p.y, p.z);
		Vec3<float> newVec = p.w * qv + q.w * pv + Vec3<float>::Cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		// Scalar component is:
		// ps * qs - pv . qv
		retVal.w = p.w * q.w - Vec3<float>::Dot(pv, qv);

		return retVal;
	}

	// Printable
	[[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const Quaternion &q)
	{
		os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
		return os;
	}

	static const Quaternion Identity;
};
