#pragma once

#include "../Common/EWin.h"
#include <string>


class SubObject {

public:
	SubObject(std::string className, std::string classTitle, HICON icon);
	~SubObject();

	
	
	
protected:
	static  LRESULT CALLBACK SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static  LRESULT			 AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT			 CommonMessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual LRESULT			 MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual VOID RegisterNewClass();

	//static std::optional<int> ProcessMessages() noexcept;
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

};