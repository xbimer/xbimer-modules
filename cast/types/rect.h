#pragma once


#ifndef __CAST_TYPES_RECT_H__
#define __CAST_TYPES_RECT_H__

#include "./point.h"

#define XIR_MAX(a,b) a>b?a:b
#define XIR_MIN(a,b) a<b?a:b

namespace xir {
	namespace cast {
		template<typename TYPE,typename PTYPE> class TRect {
		protected:
			TYPE left;
			TYPE top;
			TYPE right;
			TYPE bottom;

			void Normalize() {
				if (this->top > this->bottom) {
					TYPE t = this->top;
					this->top = this->bottom;
					this->bottom = t;
				}
				
				if(this->left>this->right){
					TYPE t = this->left;
					this->left = this->right;
					this->right = t;
				}
			}

		public:
			virtual ~TRect() {}
			TRect():left(0),top(0), right(0),bottom(0){}

			TRect(const PTYPE& pt1, const PTYPE& pt2) :
				left(pt1.GetX()),
				top(pt1.GetY()),
				right(pt2.GetX()),
				bottom(pt2.GetY()) {
				this->Normalize();
			}

			TRect(const PTYPE& pt, const TYPE& width, const TYPE& height) :
				left(pt.GetX()),
				top(pt.GetY()),
				right(left + width),
				bottom(right + height) {
				this->Normalize();
			}

			TRect(const TYPE& left, const TYPE& top, const TYPE& right, const TYPE& bottom) :
				left(left),
				top(top),
				right(right),
				bottom(bottom) {
				this->Normalize();
			}

			void Reset() {
				this->left = TYPE(0);
				this->top = TYPE(0);
				this->right = TYPE(0);
				this->bottom = TYPE(0);
			}

			void Set(const TRect<TYPE, PTYPE> &rect) {
				this->left = rect.left;
				this->top = rect.top;
				this->right = rect.right;
				this->bottom = rect.bottom;
				this->Normalize();
			}

			void Set(const PTYPE& pt1, const PTYPE& pt2) {
				this->left = pt1.GetX();
				this->top = pt1.GetY();
				this->right = pt2.GetX();
				this->bottom = pt2.GetY();
				this->Normalize();
			}

			void Set(const PTYPE& pt, const TYPE& width, const TYPE& height) {
				this->left = pt.GetX();
				this->top = pt.GetY();
				this->right = left + width;
				this->bottom = top + height;
				this->Normalize();
			}

			void Set(const TYPE& lv, const TYPE& tv, const TYPE& rv, const TYPE& bv) {
				this->left = lv;
				this->top = tv;
				this->right = rv;
				this->bottom = bv;
				this->Normalize();
			}

			void SetWithSize(const TYPE& lv, const TYPE& tv, const TYPE& wv, const TYPE& hv) {
				this->left = lv;
				this->top = tv;
				this->right = left + wv;
				this->bottom = top + hv;
				this->Normalize();
			}

			void SetLeft(const TYPE& v) {
				this->left = v;
				this->Normalize();
			}

			TYPE GetLeft()const {
				return this->left;
			}

			void SetTop(const TYPE& v) {
				this->top = v;
				this->Normalize();
			}

			TYPE GetTop() const {
				return this->top;
			}

			void SetRight(const TYPE& v) {
				this->right = v;
				this->Normalize();
			}

			TYPE GetRight() const {
				return this->right;
			}

			void SetBottom(const TYPE& v) {
				this->bottom = v;
				this->Normalize();
			}

			TYPE GetBottom() const {
				return this->bottom;
			}

			void SetLeftTop(const PTYPE& pt) {
				this->left = pt.GetX();
				this->top = pt.GetY();
				this->Normalize();
			}

			PTYPE GetLeftTop() const {
				return PTYPE(this->left, this->top);
			}

			void SetLeftBottom(const PTYPE& pt) {
				this->left = pt.GetX();
				this->bottom = pt.GetY();
				this->Normalize();
			}

			PTYPE GetLeftBottom() const {
				return PTYPE(this->left, this->bottom);
			}

			void SetRightTop(const PTYPE& pt) {
				this->right = pt.GetX();
				this->top = pt.GetY();
				this->Normalize();
			}

			PTYPE GetRightTop() const {
				return PTYPE(this->right, this->top);
			}

			void SetRightBottom(const PTYPE& pt) {
				this->right = pt.GetX();
				this->bottom = pt.GetY();
				this->Normalize();
			}

			PTYPE GetRightBottom() const {
				return PTYPE(this->right, this->bottom);
			}

			void SetCenter(const PTYPE& pt) {
				TYPE width = this->right - this->left;
				this->left = pt.GetX() - width / 2;
				this->right = this->left + width;

				TYPE height = this->bottom - this->top;
				this->top = pt.GetY() - height / 2;
				this->bottom = this->top + height;

				this->Normalize();
			}

			PTYPE GetCenter() const {
				return PTYPE((left + right) / 2, (top + bottom) / 2);
			}

			void SetWidth(const TYPE& width) {
				this->right = this->left + width;
				this->Normalize();
			}

			TYPE GetWidth() const {
				return this->right - this->left;
			}

			void SetHeight(const TYPE& height) {
				this->bottom = this->top + height;
				this->Normalize();
			}

			TYPE GetHeight() const {
				return this->bottom - this->top;
			}

			void SetSize(const TYPE& width, const TYPE& height) {
				this->right = this->left + width;
				this->bottom = this->top + height;
				this->Normalize();
			}

			void Offset(const TYPE& dx, const TYPE& dy) {
				this->left += dx;
				this->top += dy;
				this->right += dx;
				this->bottom += dy;
			}

			void Resize(const TYPE& dx, const TYPE& dy) {
				this->right += dx;
				this->bottom += dy;
				this->Normalize();
			}

			void Inflate(const TYPE& dx, const TYPE& dy) {
				this->left -= dx;
				this->top -= dy;
				this->right += dx;
				this->bottom += dy;
				this->Normalize();
			}

			void Inset(const TYPE& dx, const TYPE& dy) {
				this->left += dx;
				this->top += dy;
				this->right -= dx;
				this->bottom -= dy;
				this->Normalize();
			}

			bool IsEmpty() const {
				return this->left == this->right || this->top == this->bottom;
			}

			bool IsIntersecting(const TRect<TYPE, PTYPE>& other) const
			{
				bool horizontalExcluded = left > other.right || right < other.left;
				bool verticalExcluded = top > other.bottom || bottom < other.top;

				return !horizontalExcluded && !verticalExcluded;
			}

			bool Contains(const PTYPE& pt) 
			{
				return left <= pt.GetX() && right >= pt.GetX() && top <= pt.GetY() && bottom >= pt.GetY();
			}

			bool Contains(const TYPE& x, const TYPE& y) 
			{
				return left <= x && right >= x && top <= y && bottom >= y;
			}

			bool Contains(const TRect<TYPE, PTYPE>& in) 
			{
				return (in.left >= left && in.right <= right &&
					in.top >= top && in.bottom <= bottom);
			}

			TRect<TYPE, PTYPE> Union(const TRect<TYPE, PTYPE>& other)
			{
				TRect<TYPE, PTYPE> t(other.GetLeftTop(), other.GetRightBottom());

				if (left != 0 || top != 0 || right != 0 || bottom != 0) {
					t.left = XIR_MIN(left, t.left);
					t.top = XIR_MIN(top, t.top);
					t.right = XIR_MAX(right, t.right);
					t.bottom = XIR_MAX(bottom, t.bottom);
				}

				return t;
			}

			TRect<TYPE, PTYPE> Intersect(TRect<TYPE, PTYPE>& other) const
			{
				TRect<TYPE, PTYPE> intersection;
				if (!IsIntersecting(other)) {
					return intersection;
				}

				TYPE interLeft = XIR_MAX(left, other.left);
				TYPE interRight = XIR_MIN(right, other.right);
				TYPE interTop = XIR_MAX(top, other.top);
				TYPE interBottom = XIR_MIN(bottom, other.bottom);

				intersection.Set(interLeft, interTop, interRight, interBottom);

				return intersection;
			}

			TRect<TYPE, PTYPE> Scale(double factor) {
				return TRect<TYPE, PTYPE>(this->GetLeftTop(), (TYPE)(this->GetWidth()*factor), (TYPE)(this->GetHeight()*factor));
			}

			bool operator== (const TRect<TYPE, PTYPE>& toCompare) const
			{
				return (toCompare.left == left && toCompare.top == top &&
					toCompare.right == right && toCompare.bottom == bottom);
			}

			bool operator!= (const TRect<TYPE, PTYPE>& toCompare) const
			{
				return (toCompare.left != left || toCompare.top != top ||
					toCompare.right != right || toCompare.bottom != bottom);
			}

			std::string __str__() {
				return "[(" + std::to_string(this->left) + "," + std::to_string(this->top) + "),(" + std::to_string(this->right) + "," + std::to_string(this->bottom) + ")]";
			}
		};

		typedef TRect<int64_t,Point>	Rect;
		typedef TRect<double,FPoint>	FRect;
	}
}
#endif // !__CAST_TYPES_RECT_H__
