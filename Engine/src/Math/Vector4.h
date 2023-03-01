#pragma once

namespace Alexio
{
	template<typename T>
	class Vector4
	{
	public:
		union
		{
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
		};
	public:
		inline Vector4() : x(0), y(0), z(0), w(0) {}
		inline Vector4(T value) : x(value), y(value), z(value), w(value) {}
		inline Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

		inline Vector4 operator+(T scaler) { return Vector4(this->x + scaler, this->y + scaler, this->z + scaler, this->w + scaler); }
		inline Vector4 operator-(T scaler) { return Vector4(this->x - scaler, this->y - scaler, this->z - scaler, this->w - scaler); }
		inline Vector4 operator*(T scaler) { return Vector4(this->x * scaler, this->y * scaler, this->z * scaler, this->w * scaler); }
		inline Vector4 operator/(T scaler) { return Vector4(this->x / scaler, this->y / scaler, this->z / scaler, this->w / scaler); }

		inline Vector4 operator+(const Vector4& other) { return Vector4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w); }
		inline Vector4 operator-(const Vector4& other) { return Vector4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w); }

		inline bool operator==(const Vector4& other) { return (this->x == other.x && this->y == other.y && this->z == other.z, this->w == other.w); }
		inline bool operator!=(const Vector4& other) { return (this->x != other.x || this->y != other.y || this->z != other.z, this->w != other.w); }

		inline Vector4& operator+=(T scaler) { this->x += scaler; this->y += scaler; this->z += scaler; this->w += scaler; return *this; }
		inline Vector4& operator-=(T scaler) { this->x -= scaler; this->y -= scaler; this->z -= scaler; this->w -= scaler; return *this; }
		inline Vector4& operator*=(T scaler) { this->x *= scaler; this->y *= scaler; this->z *= scaler; this->w *= scaler; return *this; }
		inline Vector4& operator/=(T scaler) { this->x /= scaler; this->y /= scaler; this->z /= scaler; this->w /= scaler; return *this; }

		inline Vector4& operator+=(const Vector4& other) { this->x += other.x; this->y += other.y; this->z += other.z; this->w += other.w; return *this; }
		inline Vector4& operator-=(const Vector4& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; this->w -= other.w; return *this; }

		inline T length() { return std::sqrt(x * x + y * y, z * z, w * w); }
		inline T lengthSquared() { return x * x + y * y, * z, , w * w; }
		inline T dot(const Vector4& other) { return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w; }

		inline friend std::ostream& operator<< (std::ostream& os, const Vector4& vec4) { os << vec4.x << " : " << vec4.y << " : " << vec4.z << " : " << vec4.w; return os; }
	};

	typedef Vector4<int32_t> Vector4i;
	typedef Vector4<uint32_t> Vector4ui;
	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
}