#pragma once
namespace Soul
{
	namespace Core
	{
		class SColorf
		{
		public:
			SColorf()
				:
				mColor()
			{
				for (size_t i = 0; i < 4; i++)
				{
					mColor[i] = 0.f;
				}
			}
			SColorf(float r, float g, float b, float a = 1.0f)
				:
				mColor()
			{
				mColor[0] = r;
				mColor[1] = g;
				mColor[2] = b;
				mColor[3] = a;
			}
			const float* Get() const
			{
				return mColor;
			}

			float GetR() const
			{
				return mColor[0];
			}

			float GetG() const
			{
				return mColor[1];
			}

			float GetB() const
			{
				return mColor[2];
			}

			float GetA() const
			{
				return mColor[3];
			}

			void SetR(float r)
			{
				mColor[0] = r;
			}

			void SetG(float g)
			{
				mColor[1] = g;
			}

			void SetB(float b)
			{
				mColor[2] = b;
			}

			void SetA(float a)
			{
				mColor[3] = a;
			}
		private:
			float mColor[4];
		};
	}
}