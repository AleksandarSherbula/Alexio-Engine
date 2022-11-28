#ifndef VECTOR2_H
#define VECTOR2_H

namespace Alexio
{
	template<typename T>
	class Vector2
	{
	public:
		union
		{
			struct { T x, y; };
			struct { T u, v; };
			struct { T width, height; };
		};
	public:
		inline Vector2() : x(0), y(0) {}
		inline Vector2(T value) : x(value), y(value) {}
		inline Vector2(T x, T y) : x(x), y(y) {}

		inline Vector2 operator+(T scaler) { return Vector2(this->x + scaler, this->y + scaler); }
		inline Vector2 operator-(T scaler) { return Vector2(this->x - scaler, this->y - scaler); }
		inline Vector2 operator*(T scaler) { return Vector2(this->x * scaler, this->y * scaler); }
		inline Vector2 operator/(T scaler) { return Vector2(this->x / scaler, this->y / scaler); }

		inline Vector2 operator+(const Vector2& other) { return Vector2(this->x + other.x, this->y + other.y); }
		inline Vector2 operator-(const Vector2& other) { return Vector2(this->x - other.x, this->y - other.y); }

		inline bool operator==(const Vector2& other) { return (x == other.x && y == other.y); }
		inline bool operator!=(const Vector2& other) { return (x != other.x || y != other.y); }

		inline Vector2& operator+=(T scaler) { this->x += scaler; this->y += scaler; return *this; }
		inline Vector2& operator-=(T scaler) { this->x -= scaler; this->y -= scaler; return *this; }
		inline Vector2& operator*=(T scaler) { this->x *= scaler; this->y *= scaler; return *this; }
		inline Vector2& operator/=(T scaler) { this->x /= scaler; this->y /= scaler; return *this; }

		inline Vector2& operator+=(const Vector2& other) { this->x += other.x; this->y += other.y; return *this; }
		inline Vector2& operator-=(const Vector2& other) { this->x -= other.x; this->y -= other.y; return *this; }

		inline T length() { return std::sqrt(x * x + y * y); }
		inline T lengthSquared() { return x * x + y * y; }
		inline T dot(const Vector2& other) { return this->x * other.x + this->y * other.y; }

		inline friend std::ostream& operator<< (std::ostream& os, const Vector2& vec2) { os << vec2.x << " : " << vec2.y; return os; }
	};


	typedef Vector2<int32_t> Vector2i;
	typedef Vector2<uint32_t> Vector2ui;
	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
}
#endif // !VECTOR2_H