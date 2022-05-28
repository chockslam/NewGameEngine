/// CODE was written with help by OlympusMonsTutorial (https://www.youtube.com/c/OlympusMonsTutorials)
#pragma once

#include "../Common/EWin.h"
#include <string>

/// <summary>
/// Abstract Class that represents Object that can be either Splash Screen/Widnow/Loading Screen and etc.
/// </summary>
class SubObject {

public:
	SubObject(std::string className, std::string classTitle, HICON icon);
	~SubObject();

	
	
	
protected:
	// Needed for implementation of MessageHandler in the Class 
	static  LRESULT CALLBACK SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static  LRESULT			 AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT			 CommonMessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT			 MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;  // Abstract message handler
	virtual VOID RegisterNewClass();

	//static std::optional<int> ProcessMessages() ;
private:
	virtual VOID Initialize(UINT type) = 0;
protected:
	std::string m_Class;
	std::string m_Title;

	HICON m_Hicon;

	HWND m_Handle;

	HINSTANCE m_Inst;
public:
	HWND GetHandle() { return m_Handle; }
	HINSTANCE GetInstance() { return m_Inst; }

};