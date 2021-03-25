#include <iostream>

extern "C" {
#include "cmnclib.h"
}

#include "cmn-tools/Command.hpp"
#include "cmn-tools/CommandException.hpp"
#include "cmn-tools/commands/Win32SkeletonCommand.hpp"
#include "cmn-tools/win32/Skeleton.hpp"

namespace cmnclib {
namespace tools {
namespace command {

	int Win32SkeletonCommand::execute(std::vector<std::string>& args) {

		HINSTANCE hInstance = GetModuleHandle(NULL);
		win32::Skeleton *wnd = new win32::Skeleton(hInstance);

		if (wnd->InitApp() == FALSE)
			return FALSE;
		if (wnd->InitInstance(SW_SHOW) == FALSE)
			return FALSE;

		// Message Loop
		WPARAM ret = wnd->Run();
		delete wnd;

		return ret;
	}

}
}
}