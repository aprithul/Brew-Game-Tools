#ifndef MATH_UTIL_HPP
#define MATH_UTIL_HPP

//#define ENABLE_SIMD

#ifdef ENABLE_SIMD
#include <xmmintrin.h>
#endif

#define DEF_STATIC

#define DEG_TO_RAD PI/180.f
#define PI 3.14159265
#define EPS 0.000001	// very small value

#include "Types.hpp"

///////////////////////////////////

struct Vec2f;
struct Vec3f;
struct Mat3x3;

struct Vec2i
{
	Int_32 x,y;
};

struct Vec2f
{
	Float_32 x, y;

	//const, dest, copy
	Vec2f();
	~Vec2f() = default;
	Vec2f(Float_32, Float_32);
	Vec2f(const Vec2f& other) = default;
	Vec2f& operator=(const Vec2f& other) = default;
	Vec2f(Vec2f&& other) = default;
	Vec2f& operator=(Vec2f&& other) = default;

	//operations
	Vec2f operator+(const Vec2f& other) const;
	Vec2f operator-(const Vec2f& other) const;
	Vec2f operator-() const;
	Vec2f& operator+=(const Vec2f& other);
	Vec2f& operator-=(const Vec2f& other);
	Vec2f operator*(const Vec2f& other) const; // componenet wise
	Vec2f& operator*=(const Vec2f& other); // 
	Vec2f operator*(Float_32 s) const; // componenet wise
	Vec2f& operator*=(Float_32 s); // 
	Vec2f operator/(Float_32 s) const;
	Vec2f& operator/=(Float_32 s);
	Vec2f GetNormalized() const;
	Vec2f& Normalize();
	Float_32 GetMagnitude() const;
	Float_32 GetSqrdMagnitude() const;
	operator Vec3f() const;
};

struct Vec3f
{
	Float_32 x, y, z;

	//const, dest, copy
	Vec3f();
	~Vec3f() = default;
	Vec3f(Float_32 _x, Float_32 _y, Float_32 _z);
	Vec3f(const Vec3f& other) = default;
	Vec3f& operator=(const Vec3f& other) = default;
	Vec3f(Vec3f&& other) = default;
	Vec3f& operator=(Vec3f&& other) = default;

	//operations
	Vec3f operator+(const Vec3f& other) const;
	Vec3f operator-(const Vec3f& other) const;
	Vec3f operator-() const;
	Vec3f& operator+=(const Vec3f& other);
	Vec3f& operator-=(const Vec3f& other);
	Vec3f operator*(const Vec3f& other) const; // componenet wise
	Vec3f& operator*=(const Vec3f& other); // 
	Vec3f operator*(Float_32 s) const; // componenet wise
	Vec3f& operator*=(Float_32 s); // 
	Vec3f operator/(Float_32 s) const;
	Vec3f& operator/=(Float_32 s);
	Vec3f GetNormalized() const;
	Vec3f& Normalize();
	Float_32 GetMagnitude() const;
	Float_32 GetSqrdMagnitude() const;
	operator Vec2f() const;

};


struct Mat3x3
{
	Float_32 data[3 * 3];
	Mat3x3();
	Mat3x3(Vec3f _c0, Vec3f _c1, Vec3f _c2);
	Mat3x3(Float_32 v00, Float_32 v01, Float_32 v02,
		Float_32 v10, Float_32 v11, Float_32 v12,
		Float_32 v20, Float_32 v21, Float_32 v22);
	~Mat3x3() = default;

	Mat3x3 operator+(const Mat3x3& m) const;
	Mat3x3 operator-(const Mat3x3& m) const;
	Mat3x3 operator*(const Mat3x3& m) const;
	Vec3f operator*(const Vec3f& vec4) const;
	Vec3f operator*(const Vec2f& v) const;
	Mat3x3 operator/(Float_32 v) const;
	Mat3x3 operator*(Float_32 v) const;
	Mat3x3 GetInverse() const;
	Mat3x3 GetTranspose() const;
	const Vec3f& operator[](Uint_32 col) const;
	Float_32 operator()(Uint_32 row, Uint_32 col) const;
	Float_32& operator()(Uint_32 row, Uint_32 col);
	static Mat3x3 Identity();
};

DEF_STATIC Float_32 Dot(const Vec3f& a, const Vec3f& b);
DEF_STATIC Float_32 Dot(const Vec2f& a, const Vec2f& b);
DEF_STATIC Vec3f Cross(const Vec3f& a, const Vec3f& b);
DEF_STATIC Float_32 Cross(const Vec2f& a, const Vec2f& b);
DEF_STATIC Vec2f Cross(Float_32 s, const Vec2f& v);

#endif

#ifdef MATH_UTIL_IMPLEMENTATION
#undef MATH_UTIL_IMPLEMENTATION

#include <math.h>

////////////////////////////////////////////////
// Vec2f
////////////////////////////////////////////////
#pragma region Vec2f
Vec2f::Vec2f()
{
	x = 0;
	y = 0;
}

Vec2f::Vec2f(Float_32 _x, Float_32 _y)
{
	x = _x;
	y = _y;
}

Vec2f Vec2f::operator+(const Vec2f& other) const
{
	return Vec2f{ x + other.x, y + other.y };
}
Vec2f Vec2f::operator-(const Vec2f& other) const
{
	return Vec2f{ x - other.x, y - other.y };
}

Vec2f Vec2f::operator-() const
{
	return Vec2f{ -x,-y };
}


Vec2f& Vec2f::operator+=(const Vec2f& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vec2f& Vec2f::operator-=(const Vec2f& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vec2f Vec2f::operator*(const Vec2f& other) const
{
	return Vec2f{ x*other.x, y*other.y };
}

Vec2f& Vec2f::operator*=(const Vec2f& other)
{
	x *= other.x;
	y *= other.y;
	return *this;
}

Vec2f Vec2f::operator*(Float_32 s) const
{
	return Vec2f{ x*s, y*s };
}

Vec2f& Vec2f::operator*=(Float_32 s)
{
	x *= s;
	y *= s;
	return *this;
}

Vec2f Vec2f::operator/(Float_32 s) const
{
	return Vec2f{ x / s, y / s };
}

Vec2f& Vec2f::operator/=(Float_32 s)
{
	x /= s;
	y /= s;
	return *this;
}

Vec2f Vec2f::GetNormalized() const
{
	Float_32 _magnitude = GetMagnitude();
	if (_magnitude < EPS)
		return Vec2f();

	return Vec2f{ x / _magnitude, y / _magnitude };
}


Vec2f& Vec2f::Normalize()
{
	Float_32 _magnitude = GetMagnitude();
	if (_magnitude < EPS)
	{
		x = 0.f;
		y = 0.f;
	}
	else
	{
		x /= _magnitude;
		y /= _magnitude;
	}

	return *this;
}

Float_32 Vec2f::GetMagnitude() const
{
	return sqrtf(x * x + y * y);
}

Float_32 Vec2f::GetSqrdMagnitude() const
{
	return (x * x + y * y);
}

Vec2f::operator Vec3f() const
{
	return Vec3f{ x, y, 0 };
}

#pragma endregion

////////////////////////////////////////////////
// Vec3f
////////////////////////////////////////////////

#pragma region Vec3f

Vec3f::Vec3f()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3f::Vec3f(Float_32 _x, Float_32 _y, Float_32 _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vec3f Vec3f::operator+(const Vec3f & other) const
{
	return Vec3f{ x + other.x, y + other.y, z + other.z };
}

Vec3f Vec3f::operator-(const Vec3f & other) const
{
	return Vec3f{ x - other.x, y - other.y, z - other.z };
}

Vec3f Vec3f::operator-() const
{
	return Vec3f{ -x,-y,-z };
}


Vec3f& Vec3f::operator+=(const Vec3f & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vec3f& Vec3f::operator-=(const Vec3f & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;

}

Vec3f Vec3f::operator*(const Vec3f & other) const
{
	return Vec3f{ x * other.x, y * other.y, z * other.z };

}

Vec3f & Vec3f::operator*=(const Vec3f & other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

Vec3f Vec3f::operator*(Float_32 s) const
{
	return Vec3f{ x*s, y*s, z*s };
}

Vec3f& Vec3f::operator*=(Float_32 s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vec3f Vec3f::operator/(Float_32 s) const
{
	return Vec3f{ x / s, y / s, z / s };
}

Vec3f& Vec3f::operator/=(Float_32 s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vec3f Vec3f::GetNormalized() const
{
	Float_32 _magnitude = GetMagnitude();
	if (_magnitude < EPS)
		return Vec3f();

	return Vec3f{ x / _magnitude, y / _magnitude, z / _magnitude };
}

Vec3f& Vec3f::Normalize()
{
	Float_32 _magnitude = GetMagnitude();
	if (_magnitude < EPS)
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	else
	{
		x /= _magnitude;
		y /= _magnitude;
		z /= _magnitude;
	}

	return *this;
}

Float_32 Vec3f::GetMagnitude() const
{
	return sqrtf(x * x + y * y + z * z);
}

Float_32 Vec3f::GetSqrdMagnitude() const
{
	return (x * x + y * y + z * z);
}

Vec3f::operator Vec2f() const
{
	//return *((Vec2f*)this);
	return Vec2f{ x, y };
}

////////////////////////////////////////////////
// Mat3x3
////////////////////////////////////////////////

Mat3x3::Mat3x3()
{
	Mat3x3& _m = (*this);
	_m(0, 0) = 0; _m(0, 1) = 0; _m(0, 2) = 0;
	_m(1, 0) = 0; _m(1, 1) = 0; _m(1, 2) = 0;
	_m(2, 0) = 0; _m(2, 1) = 0; _m(2, 2) = 0;
}

Mat3x3::Mat3x3(Vec3f _c0, Vec3f _c1, Vec3f _c2)
{
	Mat3x3& _m = (*this);
	_m(0, 0) = _c0.x; _m(0, 1) = _c1.x; _m(0, 2) = _c2.x;
	_m(1, 0) = _c0.y; _m(1, 1) = _c1.y; _m(1, 2) = _c2.y;
	_m(2, 0) = _c0.z; _m(2, 1) = _c1.z; _m(2, 2) = _c2.z;
}

Mat3x3::Mat3x3(Float_32 v00, Float_32 v01, Float_32 v02,
	Float_32 v10, Float_32 v11, Float_32 v12,
	Float_32 v20, Float_32 v21, Float_32 v22)
{
	Mat3x3& _m = (*this);
	_m(0, 0) = v00; _m(0, 1) = v01; _m(0, 2) = v02;
	_m(1, 0) = v10; _m(1, 1) = v11; _m(1, 2) = v12;
	_m(2, 0) = v20; _m(2, 1) = v21; _m(2, 2) = v22;
}


Mat3x3 Mat3x3::operator+(const Mat3x3& m) const
{
	const Mat3x3& _m = (*this);
	return Mat3x3{
		_m(0,0) + m(0,0), _m(0,1) + m(0,1), _m(0,2) + m(0,2),
		_m(1,0) + m(1,0), _m(1,1) + m(1,1), _m(1,2) + m(1,2),
		_m(2,0) + m(2,0), _m(2,1) + m(2,1), _m(2,2) + m(2,2)
	};
}

Mat3x3 Mat3x3::operator-(const Mat3x3& m) const
{
	const Mat3x3& _m = (*this);
	return Mat3x3{
		_m(0,0) - m(0,0), _m(0,1) - m(0,1), _m(0,2) - m(0,2),
		_m(1,0) - m(1,0), _m(1,1) - m(1,1), _m(1,2) - m(1,2),
		_m(2,0) - m(2,0), _m(2,1) - m(2,1), _m(2,2) - m(2,2)
	};
}

Mat3x3 Mat3x3::operator*(const Mat3x3& m) const
{
	const Mat3x3& _m = (*this);

	return Mat3x3(
		_m(0, 0)*m(0, 0) + _m(0, 1)*m(1, 0) + _m(0, 2)*m(2, 0),
		_m(0, 0)*m(0, 1) + _m(0, 1)*m(1, 1) + _m(0, 2)*m(2, 1),
		_m(0, 0)*m(0, 2) + _m(0, 1)*m(1, 2) + _m(0, 2)*m(2, 2),

		_m(1, 0)*m(0, 0) + _m(1, 1)*m(1, 0) + _m(1, 2)*m(2, 0),
		_m(1, 0)*m(0, 1) + _m(1, 1)*m(1, 1) + _m(1, 2)*m(2, 1),
		_m(1, 0)*m(0, 2) + _m(1, 1)*m(1, 2) + _m(1, 2)*m(2, 2),

		_m(2, 0)*m(0, 0) + _m(2, 1)*m(1, 0) + _m(2, 2)*m(2, 0),
		_m(2, 0)*m(0, 1) + _m(2, 1)*m(1, 1) + _m(2, 2)*m(2, 1),
		_m(2, 0)*m(0, 2) + _m(2, 1)*m(1, 2) + _m(2, 2)*m(2, 2)
	);
}


Vec3f Mat3x3::operator*(const Vec3f& vec3) const
{
	const Mat3x3& _m = (*this);
	Vec3f r;
	r.x = (_m(0, 0) * vec3.x) + (_m(0, 1) * vec3.y) + (_m(0, 2) * vec3.z);
	r.y = (_m(1, 0) * vec3.x) + (_m(1, 1) * vec3.y) + (_m(1, 2) * vec3.z);
	r.z = (_m(2, 0) * vec3.x) + (_m(2, 1) * vec3.y) + (_m(2, 2) * vec3.z);
	return r;
}

Vec3f Mat3x3::operator*(const Vec2f& vec2) const
{
	const Mat3x3& _m = (*this);
	Vec3f r;
	r.x = (_m(0, 0) * vec2.x) + (_m(0, 1) * vec2.y) + _m(0, 2);
	r.y = (_m(1, 0) * vec2.x) + (_m(1, 1) * vec2.y) + _m(1, 2);
	r.z = (_m(2, 0) * vec2.x) + (_m(2, 1) * vec2.y) + _m(2, 2);
	return r;
}

Mat3x3 Mat3x3::operator/(Float_32 s) const
{
	const Mat3x3& _m = (*this);
	return Mat3x3{
		_m(0, 0) / s, _m(0, 1) / s, _m(0, 2) / s,
		_m(1, 0) / s, _m(1, 1) / s, _m(1, 2) / s,
		_m(2, 0) / s, _m(2, 1) / s, _m(2, 2) / s
	};
}

Mat3x3 Mat3x3::operator*(Float_32 s) const
{
	const Mat3x3& _m = (*this);
	return Mat3x3{
		_m(0, 0) * s, _m(0, 1) * s, _m(0, 2) * s,
		_m(1, 0) * s, _m(1, 1) * s, _m(1, 2) * s,
		_m(2, 0) * s, _m(2, 1) * s, _m(2, 2) * s
	};
}

// every 4 elements is a column
Float_32 Mat3x3::operator()(Uint_32 row, Uint_32 col) const
{
	return data[col * 3 + row];
}

Float_32& Mat3x3::operator()(Uint_32 row, Uint_32 col)
{
	return data[col * 3 + row];

}

const Vec3f& Mat3x3::operator[](Uint_32 col) const
{
	return *(reinterpret_cast<const Vec3f*>(&data[col * 3]));
}

Mat3x3 Mat3x3::GetInverse() const
{
	const Mat3x3& _m = (*this);
	const Vec3f& a = reinterpret_cast<const Vec3f&>(_m[0]);
	const Vec3f& b = reinterpret_cast<const Vec3f&>(_m[1]);
	const Vec3f& c = reinterpret_cast<const Vec3f&>(_m[2]);


	Vec3f r0 = Cross(b, c);
	Vec3f r1 = Cross(c, a);
	Vec3f r2 = Cross(a, b);

	Float_32 inv_det = (1.0 / Dot(r2, c));

	return (Mat3x3{
		r0.x * inv_det, r0.y * inv_det, r0.z * inv_det,
		r1.x * inv_det, r1.y * inv_det, r1.z * inv_det,
		r2.x * inv_det, r2.y * inv_det, r2.z * inv_det
		});
}

// inline void swap_f(Float_32& _a, Float_32& _b)
// {
// 	Float_32 _temp = _a;
// 	_a = _b;
// 	_b = _temp;
// }

// 036
// 147
// 258

Mat3x3 Mat3x3::GetTranspose() const
{
	// layout in memory
	// 036
	// 147
	// 258

	auto _md = this->data;
	return Mat3x3(_md[0], _md[1], _md[2],
					_md[3], _md[4], _md[5],
						_md[6], _md[7], _md[8]);
}


Mat3x3 Mat3x3::Identity()
{
	return Mat3x3{ 1,0,0,
					0,1,0,
					0,0,1 };
}


////////////////////////////////////////////////
// Static functions
////////////////////////////////////////////////
#pragma region Statics
	// static functions
Float_32 Dot(const Vec3f& a, const Vec3f& b)
{
	return (a.x*b.x + a.y * b.y + a.z * b.z);
}
Float_32 Dot(const Vec2f& a, const Vec2f& b)
{
	return (a.x*b.x + a.y * b.y);
}
Vec3f Cross(const Vec3f& a, const Vec3f& b)
{
	return Vec3f{
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	};
}

Float_32 Cross(const Vec2f& a, const Vec2f& b)
{
	return a.x*b.y - a.y*b.x;
}

Vec2f Cross(Float_32 s, const Vec2f& v)
{
	return { -s * v.y, s*v.x };
}




#pragma endregion

#endif