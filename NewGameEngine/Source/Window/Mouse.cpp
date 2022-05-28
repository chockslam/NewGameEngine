/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle)


#include "../Common/EWin.h"
#include "Mouse.h"
#include <string>

std::pair<int, int> Mouse::GetPos() const 
{
	return { x,y };
}

int Mouse::GetPosX() const 
{
	return x;
}

int Mouse::GetPosY() const 
{
	return y;
}

void Mouse::EnableRaw() 
{
	rawEnabled = true;
}

void Mouse::DisableRaw() 
{
	rawEnabled = false;
}

bool Mouse::RawEnabled() const 
{
	return rawEnabled;
}


std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() 
{
	// if delta not registered, return nullopt 
	if (rawDeltaBuffer.empty())
	{
		return std::nullopt;
	}
	// return RawDelta struct instance from the buffer
	const RawDelta d = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return d;
}


bool Mouse::IsInWindow() const 
{
	return isInWindow;
}

bool Mouse::LeftIsPressed() const 
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const 
{
	return rightIsPressed;
}

std::optional<Mouse::Event> Mouse::Read() 
{
	// Read buffer if it has members
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return {};
}

void Mouse::Flush() 
{
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newx, int newy) 
{
	
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseMoveRaw(int xR, int yR) 
{
	rawDeltaBuffer.push({ xR,yR });
	TrimRawInputBuffer();
}

void Mouse::OnMouseLeave() 
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() 
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) 
{
	leftIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) 
{
	leftIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) 
{
	rightIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) 
{
	rightIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) 
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) 
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() 
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void Mouse::TrimRawInputBuffer() 
{
	while (rawDeltaBuffer.size() > bufferSize)
	{
		rawDeltaBuffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta) 
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
