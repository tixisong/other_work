
#ifndef MYLOG_H__
#define MYLOG_H__

#include<iomanip>
#include <fstream>
#include <string>
#include <Windows.h>


using namespace std;

class locker
{
public:
	inline locker()
	{
		InitializeCriticalSection(&m_cirtical_sec);
		
	}
	inline ~locker()
	{
		DeleteCriticalSection(&m_cirtical_sec);
	
	}
public:
	inline void lock()
	{
		EnterCriticalSection(&m_cirtical_sec);
	
	}
	inline void unlock()
	{
		LeaveCriticalSection(&m_cirtical_sec);
	
	}
private:
	CRITICAL_SECTION m_cirtical_sec;
};



static locker ll;


#define LOG(p)	do {\
	ll.lock();\
	ofstream & filelogo = getfile();\
	filelogo << GetNowTime() << " ";\
	filelogo << __FILE__ << " " << __LINE__ << " " << p << endl; \
	ll.unlock();\
}while(0);

//#ifdef _DEBUG
//#define LOG(p)	do {\
//	ll.lock(); \
//	ofstream & filelogo = getfile(); \
//	filelogo << GetNowTime() << " "; \
//	filelogo << __FILE__ << " " << __LINE__ << " " << p << endl; \
//	filelogo << __FILE__ << " " << __LINE__ << " " << p << endl; \
//	ll.unlock(); \
//}while (0);
//#endif
//
////
////#ifdef NDEBUG
////#define LOG(p)	do {\
////	ll.lock(); \
////	ofstream & filelogo = getfile(); \
////	filelogo << GetNowTime() << " "; \
////	filelogo << __FILE__ << " " << __LINE__ << " " << p << endl; \
////	filelogo << __FILE__ << " " << __LINE__ << " " << p << endl; \
////	ll.unlock(); \
////}while (0);
//
//#endif
//

ofstream & getfile();
string  GetNowTime();




#endif // !MYLOG_H__
