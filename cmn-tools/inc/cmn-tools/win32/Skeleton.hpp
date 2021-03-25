#ifndef _WIN32_SKELETON_HPP
#define _WIN32_SKELETON_HPP

#include <windows.h>

namespace cmnclib {
namespace tools {
namespace win32 {

class Skeleton {

public:
private:
	HINSTANCE m_hInstance;

public:
	Skeleton(HINSTANCE hInstance);
	~Skeleton();
	WPARAM Run();
	BOOL InitApp();
	BOOL InitInstance(int mode);

};

}
}
}

#endif
