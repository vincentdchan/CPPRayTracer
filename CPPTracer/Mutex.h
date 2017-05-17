#pragma once
#include <Windows.h>

namespace Util
{
	class Mutex final
	{
	public:
		class Proxy
		{
		private:
			Mutex* _mutex;
			Proxy(Mutex* mutex):
				_mutex(mutex)
			{}
		public:
			Proxy(): _mutex(nullptr)
			{}
			~Proxy()
			{
				if (_mutex)
					_mutex->unlock();
			}

			friend class Mutex;
		};

	private:
		HANDLE _mutex;
		bool _locked;

	public:
		Mutex():
			_locked(false)
		{
			_mutex = CreateMutex(
				NULL,              // default security attributes
				FALSE,             // initially not owned
				NULL);             // unnamed mutex
			if (_mutex == NULL)
				throw "fuck";
		}

		bool lock(Proxy& proxy)
		{
			DWORD dwResult = WaitForSingleObject(
				_mutex,    // handle to mutex
				INFINITE);
			switch (dwResult)
			{
			case WAIT_OBJECT_0:
				_locked = true;
				proxy._mutex = this;
				return true;
			case WAIT_ABANDONED:
				return false;
			}
			return true;
		}

		bool unlock()
		{
			if (ReleaseMutex(_mutex))
			{
				_locked = false;
				return true;
			}
			return false;
		}

		~Mutex()
		{
			if (_locked)
				unlock();
			CloseHandle(_mutex);
		}

	};
}
