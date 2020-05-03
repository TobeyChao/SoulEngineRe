#pragma once
#include <cassert>
namespace Soul
{
	template <typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			assert(!mSingleton);
			mSingleton = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
			assert(mSingleton);
			mSingleton = nullptr;
		}

		static T& GetInstance()
		{
			assert(mSingleton);
			return (*mSingleton);
		}

		static T* GetInstancePtr()
		{
			return mSingleton;
		}
	private:
		Singleton(const Singleton<T>&) = delete;
		Singleton& operator=(const Singleton<T>&) = delete;

	protected:
		static T* mSingleton;
	};
}