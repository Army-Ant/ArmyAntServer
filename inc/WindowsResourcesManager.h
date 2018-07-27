#ifndef WINDOWS_RESOURCES_MANAGER_H_20180724
#define WINDOWS_RESOURCES_MANAGER_H_20180724

#include <stdafx.h>
#include <AADefine.h>
#include <functional>

class WindowsResourcesManager{
public:
	static WindowsResourcesManager*getInstance();
	static void releaseInstance();

public:
	enum class Window{
		Default
	};
	enum class Control{
		Window_Self
	};
	enum class EventType{
		Create
	};
	typedef std::function<bool(Control, EventType)> EventCallback;

public:
	bool attach(HINSTANCE handle);
	void dettach();

	bool registerEvent(Window win, Control control, EventType event, EventCallback cb);
	bool unregisterEvent(Window win, Control control, EventType event);

	bool openWindow(Window win);
	bool closeWindow(Window win);

private:
	HINSTANCE sysHandle;

private:
	static WindowsResourcesManager* instance;

private:
	WindowsResourcesManager();
	~WindowsResourcesManager();

	AA_FORBID_COPY_CTOR(WindowsResourcesManager);
	AA_FORBID_ASSGN_OPR(WindowsResourcesManager);
};

#endif // WINDOWS_RESOURCES_MANAGER_H_20180724
