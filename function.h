#include <cstddef>

struct Vector2D
{
	std::size_t x, y;

	Vector2D& operator+=(const Vector2D& rhs){
	    this->x += rhs.x;
	    this->y += rhs.y;
	    return *this;
	}
};

////////////////////////////////////////////////////////////
Vector2D getPointBranchFree(std::size_t index)
{
	std::size_t firstBit = index & 1;
	std::size_t secondBit = (index >> 1);
	firstBit = firstBit^secondBit;

	return Vector2D{firstBit, secondBit};
}

////////////////////////////////////////////////////////////
Vector2D getPointBranch(std::size_t index)
{
	switch (index)
    {
        default:
        case 0: return Vector2D{0, 0};;
        case 1: return Vector2D{1, 0};
        case 2: return Vector2D{1, 1};
        case 3: return Vector2D{0, 1};
    }
}