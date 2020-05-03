#pragma once
#include "Mathematics/SVector2.h"
namespace Soul
{
	namespace Core
	{
		template<typename T>
		class SRect
		{
		public:
			SRect();
			SRect(T left, T top, T right, T bottom);
			T GetWidth() const;
			T GetHeight() const;
			T GetArea() const;
			bool IsPointInside(const SVector2& pos);
			bool IsRectCollided(const SRect<T>& other) const;
			SVector2 GetCenter() const;
			SRect<T> operator+(const SVector2 &delta) const;
		public:
			T mLeft;
			T mTop;
			T mRight;
			T mBottom;
		};
		
		template<typename T>
		inline SRect<T>::SRect()
			:
			mLeft(0),
			mTop(0),
			mRight(0),
			mBottom(0)
		{
		}
		template<typename T>
		inline SRect<T>::SRect(T left, T top, T right, T bottom)
			:
			mLeft(left),
			mTop(top),
			mRight(right),
			mBottom(bottom)
		{
		}
		template<typename T>
		inline T SRect<T>::GetWidth() const
		{
			return mRight - mLeft;
		}
		template<typename T>
		inline T SRect<T>::GetHeight() const
		{
			return mBottom - mTop;
		}
		template<typename T>
		inline T SRect<T>::GetArea() const
		{
			return GetWidth() * GetHeight();
		}
		template<typename T>
		inline bool SRect<T>::IsPointInside(const SVector2& pos)
		{
			return (mLeft <= pos.x &&
				mTop <= pos.y &&
				mRight >= pos.x &&
				mBottom >= pos.y);
		}
		template<typename T>
		inline bool SRect<T>::IsRectCollided(const SRect<T>& other) const
		{
			return (mBottom > other.mBottom &&
				mTop < other.mTop &&
				mRight > other.mRight &&
				mLeft < other.mLeft);
		}
		template<typename T>
		inline SVector2 SRect<T>::GetCenter() const
		{
			return SVector2(
				float(mLeft + mRight) / 2.0f,
				float(mTop + mBottom) / 2.0f);
		}
		template<typename T>
		inline SRect<T> SRect<T>::operator+(const SVector2 & delta) const
		{
			return SRect<T>(mLeft + (T)delta.x,
				mTop + (T)delta.y,
				mRight + (T)delta.x,
				mBottom + (T)delta.y);
		}
	}
}