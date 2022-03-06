/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#define FULL_WINTARD
#include "EWin.h"
#include "GDIPlusManager.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>

ULONG_PTR GDIPlusManager::token = 0;
int GDIPlusManager::refCount = 0;

GDIPlusManager::GDIPlusManager()
{
	// Tracking number of instances.
	// Allowing to startup GDI+ only 1 time.
	if( refCount++ == 0 )
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup( &token,&input,nullptr );
	}
}

GDIPlusManager::~GDIPlusManager()
{
	// Shut down GDI+.
	if( --refCount == 0 )
	{
		Gdiplus::GdiplusShutdown( token );
	}
}
