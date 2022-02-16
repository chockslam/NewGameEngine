#pragma once

#include "SubObject.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>
#include <memory>
#include "../Graphics/Graphics.h"
#include <imgui_impl_win32.h>
#include "AudioIO.h"

class Window : public SubObject {

public:
	Window(std::string WindowName,HICON icon , INT width, INT height);
	~Window();

	virtual VOID Initialize(UINT type) override;

	virtual LRESULT			 MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	Graphics& Gfx();
	static std::optional<int> ProcessMessages() noexcept;
	void EnableCursor();
	void DisableCursor();
private:
	void SetupRawInput();
	void HideCursor();
	void ShowCursor();
	void DisableImGuiMouse();
	void EnableImGuiMouse();
	void ConfineCursor();
	void FreeCursor();
public:
	Keyboard kbd;
	Mouse mouse;
	bool CursorEnabled;
protected:
	INT	m_Width;
	INT	m_Height;
	std::unique_ptr<Graphics> pGfx;
};