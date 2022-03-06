/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)


#pragma once
#include "EWin.h"

/// <summary>
/// Class handles GDI+ API, which helps loading pictures as Textures.
/// </summary>
class GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();
private:
	static ULONG_PTR token;
	static int refCount;
};