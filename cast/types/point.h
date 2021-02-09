#pragma once

#ifndef __CAST_TYPES_POINT_H__
#define __CAST_TYPES_POINT_H__

#include <string>

namespace xir {
	namespace cast {
		template <typename TYPE> class TPoint
		{
		public:
			TPoint() :
				x(0),
				y(0) {}

			TPoint(const TYPE& x, const TYPE& y) :
				x(x),
				y(y) {}

			~TPoint() {}

			void	Set(const TYPE& newX, const TYPE& newY) {
				this->x = newX;
				this->y = newY;
			}

			TYPE	GetX(void) const {
				return this->x;
			}

			TYPE	GetY(void) const {
				return this->y;
			}

			void	SetX(const TYPE& newX) {
				this->x = newX;
			}

			void	SetY(const TYPE& newY) {
				this->y = newY;
			}

			void	Offset(const TYPE& deltaX, const TYPE& deltaY) {
				this->x += deltaX;
				this->y += deltaY;
			}

			TPoint<TYPE> Scale(double scaleFactor) const {
				return TPoint<TYPE>((TYPE)(this->x*scaleFactor), (TYPE)(this->y*scaleFactor));
			}

			bool	operator== (const TPoint<TYPE>& op) const {
				return this->x == op.x&&this->y == op.y;
			}

			bool	operator!= (const TPoint<TYPE>& op) const {
				return this->x != op.x || this->y != op.y;
			}

			std::string __str__() {
				return "(" + std::to_string(this->x) + "," + std::to_string(this->y) + ")";
			}

		protected:
			TYPE x;
			TYPE y;
		};

		typedef TPoint<int64_t>		Point;
		typedef TPoint<double>		FPoint;
	}
}

#endif // !__CAST_TYPES_POINT_H__
