/// <summary>
/// Vector 3 class
/// your name here or no marks
/// </summary>
#include "MyVector3.h"
#define PI           3.14159265358979323846

/// <summary>
/// Defautl constructor set all 3 values to zero
/// </summary>
MyVector3::MyVector3() :
	x{ 0.0 },
	y{ 0.0 },
	z{ 0.0 }
{
}

/// <summary>
/// constructo from 3 doubles
/// </summary>
/// <param name="t_x">x componenet</param>
/// <param name="t_y">y componenet</param>
/// <param name="t_z">z componenet</param>
MyVector3::MyVector3(double t_x, double t_y, double t_z) :
	x{ t_x },
	y{ t_y },
	z{ t_z }
{
}

/// <summary>
/// construct from a SF vector3f
/// </summary>
/// <param name="t_sfVector">sf vector3f to match</param>
MyVector3::MyVector3(sf::Vector3f t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ static_cast<double>(t_sfVector.z) }
{
}

/// <summary>
/// construct from a SF vector3i
/// </summary>
/// <param name="t_sfVector">sf vector3i to match</param>
MyVector3::MyVector3(sf::Vector3i t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ static_cast<double>(t_sfVector.z) }
{
}
/// <summary>
/// construct from a 2d sf vector2i
/// set z component to zero
/// </summary>
/// <param name="t_sfVector">sf vector2i to match</param>
MyVector3::MyVector3(sf::Vector2i t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ 0.0 }
{
}
/// <summary>
/// construct from a sf vector2u
/// set z to sero
/// </summary>
/// <param name="t_sfVector">sf vector2u to match from</param>
MyVector3::MyVector3(sf::Vector2u t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ 0.0 }
{
}
/// <summary>
/// construct from a sf vector 2f
/// set z to 0
/// </summary>
/// <param name="t_sfVector">sf vector to match</param>
MyVector3::MyVector3(sf::Vector2f t_sfVector) :
	x{ static_cast<double>(t_sfVector.x) },
	y{ static_cast<double>(t_sfVector.y) },
	z{ 0.0 }
{
}
/// <summary>
/// default destructor
/// nothing to release because we only have
/// primitive type data
/// </summary>
MyVector3::~MyVector3()
{
}

/// <summary>
/// 
/// </summary>
/// <returns>the vector as a string surrounded by square brackets and seperated by commas</returns>
std::string MyVector3::toString()
{
	const std::string output = "[" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "]";
	return output;
}



/// <summary>
/// overload for plus (addition opperator)
/// </summary>
/// <param name="t_right">the vector to add to current vector</param>
/// <returns>sum of both vectors</returns>
MyVector3 MyVector3::operator+(const MyVector3 t_right) const
{
	return MyVector3{ x + t_right.x, y + t_right.y, z + t_right.z };
}

/// <summary>
/// operator overload for minus (subtract operator)
/// </summary>
/// <param name="t_right">ther vector to subtract from the current</param>
/// <returns>the difference between the vectors</returns>
MyVector3 MyVector3::operator-(const MyVector3 t_right) const
{
	return MyVector3{ x - t_right.x, y - t_right.y, z - t_right.z };
}
/// <summary>
/// multiply by a scalar (double) change the vector magnitude by the scaling factor
/// </summary>
/// <param name="t_scalar">value to multiply by</param>
/// <returns>re-sized vector</returns>
MyVector3 MyVector3::operator*(const double t_scalar) const
{
	return MyVector3{ x * t_scalar, y * t_scalar, z * t_scalar };
}
/// <summary>
/// operator overload for divide
/// </summary>
/// <param name="t_divisor">scalar to affect magnitude by</param>
/// <returns>re-sized vector</returns>
MyVector3 MyVector3::operator/(const double t_divisor) const
{
	if (t_divisor != 0) // never divide by zero 
	{
		return MyVector3{ x / t_divisor, y / t_divisor, z / t_divisor };
	}
	else
	{
		return MyVector3{}; // dividing by zero has no effect according to Pete ??
	}
}

/// <summary>
/// operator overload for plus equals
/// thios will change the value of the current vector
/// </summary>
/// <param name="t_right">the vector to add</param>
/// <returns>sum of both vectors</returns>
MyVector3 MyVector3::operator+=(const MyVector3 t_right)
{
	x += t_right.x;
	y += t_right.y;
	z += t_right.z;
	return MyVector3{ x, y, z };
}

/// <summary>
/// operator overload for minus equals
/// this will change the current vector
/// </summary>
/// <param name="t_right">vector to subtract</param>
/// <returns>the difference between the vectors</returns>
MyVector3 MyVector3::operator-=(const MyVector3 t_right)
{
	x -= t_right.x;
	y -= t_right.y;
	z -= t_right.z;
	return MyVector3{ x, y, z };
}

/// <summary>
/// operator overload for equality check
/// </summary>
/// <param name="right">vector to compare with</param>
/// <returns>true if the same</returns>
bool MyVector3::operator==(const MyVector3 t_right) const
{
	if (x == t_right.x && y == t_right.y && z == t_right.z)
	{
		return true;
	}
	return false;
}
/// <summary>
/// operator overload for inequality
/// </summary>
/// <param name="t_right">vector to compare with</param>
/// <returns>true is not equal</returns>
bool MyVector3::operator!=(const MyVector3 t_right) const
{
	return !(this->operator==(t_right)); //equal and not equal are complimentry sets whose union is the domain
}
/// <summary>
/// operator overload for uniary negative
/// </summary>
/// <returns>the negative of the vector</returns>
MyVector3 MyVector3::operator-()
{
	return MyVector3{ -x, -y, -z };
}
/// <summary>
/// negate the x component for use when hitting the left & right cushions
/// </summary>
void MyVector3::reverseX()
{
	x = -x;
}
/// <summary>
/// negate the y component for use when hitting the top and bottom cushions
/// </summary>
void MyVector3::reverseY()
{
	y = -y;
}
/// <summary>
/// calculate the lenght of the vector 
/// </summary>
/// <returns>lenght</returns>
double MyVector3::length() const
{
	const double result = std::sqrt(x * x + y * y + z * z);
	return result;
}
/// <summary>
/// the lenght of the vector squared (not square rooted)
/// </summary>
/// <returns>lenght squared</returns>
double MyVector3::lengthSquared() const
{
	const double result = std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2);
	return result;
}
/// <summary>
/// dot product of vectors
/// </summary>
/// <param name="t_other">seconf vector</param>
/// <returns>scalar dot product</returns>
double MyVector3::dot(const MyVector3 t_other) const
{
	const double result = x * t_other.x + y * t_other.y + z * t_other.z;
	return result;
}
/// <summary>
/// cross product of two vectors
/// </summary>
/// <param name="t_other">other vector</param>
/// <returns>cross product vector</returns>
MyVector3 MyVector3::crossProduct(const MyVector3 t_other) const
{
	const double newX = y * t_other.z - z * t_other.y;
	const double newY = z * t_other.x - x * t_other.z;
	const double newZ = x * t_other.y - y * t_other.x;
	return MyVector3{ newX, newY, newZ };
}

/// <summary>
/// get the angle between two vectors
/// </summary>
/// <param name="t_other">other vector</param>
/// <returns>angle in degrees</returns>
double MyVector3::angleBetween(const MyVector3 t_other) const
{
	// formula
	// angle = arccos (a dot b / |a|*|b|)

	double top = dot(t_other);
	double under = length() * t_other.length();
	double angleRadians;
	if (under != 0) // never divide by zero
	{
		double answer = top / under;
		if (answer > 1) answer = 1;
		if (answer < -1) answer = -1;
		angleRadians = std::acos(answer);
		const double angleDegrees = angleRadians * 180.0 / PI;
		return angleDegrees;
	}
	return 0.0; // angle with null vector is zero according to Pete ?
}

/// <summary>
/// return a unit vector along the current vector
/// </summary>
/// <returns>unit vector</returns>
MyVector3 MyVector3::unit() const
{
	const double unitX = x / length();
	const double unitY = y / length();
	const double unitZ = z / length();
	return MyVector3{ unitX, unitY, unitZ };
}

/// <summary>
/// re-size current vector to magnitude one
/// </summary>
void MyVector3::normalise()
{
	const double orignalLenght = length();
	x /= orignalLenght;
	y /= orignalLenght;
	z /= orignalLenght;
}

/// <summary>
///  projection of one vector onto the current vector
/// </summary>
/// <param name="t_other">vector to project</param>
/// <returns>projection parralell to current</returns>
MyVector3 MyVector3::projection(const MyVector3 t_other) const
{
	double lenghtSq = lengthSquared();
	double dotProduct = dot(t_other);
	double scale{ 1.0 }; // default for null vector involvement
	if (lenghtSq != 0)
	{
		scale = dotProduct / lenghtSq;
	}

	return this->operator*(scale);
}

/// <summary>
/// perpendicular component of vector relative to current vector
/// defined as orignal - projection
/// </summary>
/// <param name="t_other">vector to project/reject</param>
/// <returns>rejection perpendiucular to current</returns>
MyVector3 MyVector3::rejection(const MyVector3 t_other) const
{
	return t_other - projection(t_other);
}

MyVector3::operator sf::Vector2u()
{
	double newx{ x };
	double newy{ y };
	if (newx < 0)
	{
		newx = -newx;
	}
	if (newy < 0)
	{
		newy = -newy;
	}
	return sf::Vector2u{ static_cast<unsigned>(newx),static_cast<unsigned>(newy) };
}