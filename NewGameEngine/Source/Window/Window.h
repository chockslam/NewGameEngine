#pragma once

#include "SubObject.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>
#include <memory>
#include "../Graphics/Graphics.h"

class Window : public SubObject {

public:
	Window(std::wstring WindowName,HICON icon , INT width, INT height);
	~Window();

	virtual VOID Initialize(UINT type) override;

	virtual LRESULT			 MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	Graphics& Gfx();
	static std::optional<int> ProcessMessages() noexcept;

public:
	Keyboard kbd;
	Mouse mouse;
protected:

	INT	m_Width;
	INT	m_Height;
	std::unique_ptr<Graphics> pGfx;

};