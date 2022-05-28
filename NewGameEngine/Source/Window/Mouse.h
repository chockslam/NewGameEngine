/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle)
#pragma once
#include <queue>
#include <optional>

class Mouse
{
	friend class Window; // window can access its private member
public:
	/// <summary>
	/// Struct that stores x,y of delta mouse input.
	/// </summary>
	struct RawDelta
	{
		int x, y;
	};
	/// <summary>
	/// Class that represents event
	/// </summary>
	class Event
	{
	public:
		/// <summary>
		/// Type of Mouse event
		/// </summary>
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			MoveRaw,
			Enter,
			Leave,
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event(Type type, const Mouse& parent) 
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		Type GetType() const 
		{
			return type;
		}
		std::pair<int, int> GetPos() const 
		{
			return{ x,y };
		}
		int GetPosX() const 
		{
			return x;
		}
		int GetPosY() const 
		{
			return y;
		}
		bool LeftIsPressed() const 
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const 
		{
			return rightIsPressed;
		}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	// Get position of the mouse in the windows
	std::pair<int, int> GetPos() const ;
	int GetPosX() const ;
	int GetPosY() const ;
	// Get Delta position
	std::optional<RawDelta> ReadRawDelta() ;
	// Check whether the mouse in the window
	bool IsInWindow() const ;
	bool LeftIsPressed() const ;
	bool RightIsPressed() const ;
	// Read event and delete it from the queue
	std::optional<Mouse::Event> Read() ;
	bool IsEmpty() const 
	{
		return buffer.empty();
	}
	bool IsRawEmpty() const 
	{
		return rawDeltaBuffer.empty();
	}
	void Flush() ;
	// Enable/Disable delta mouse
	void EnableRaw() ;
	void DisableRaw() ;
	bool RawEnabled() const ;
private:
	// Actions on mouse eventss
	void OnMouseMove(int x, int y) ;
	void OnMouseMoveRaw(int xR, int yR) ;
	void OnMouseLeave() ;
	void OnMouseEnter() ;
	void OnLeftPressed(int x, int y) ;
	void OnLeftReleased(int x, int y) ;
	void OnRightPressed(int x, int y) ;
	void OnRightReleased(int x, int y) ;
	void OnWheelUp(int x, int y) ;
	void OnWheelDown(int x, int y) ;
	void TrimBuffer() ;
	// If buffer overflow happens, trim it.
	void TrimRawInputBuffer() ;
	void OnWheelDelta(int x, int y, int delta) ;
private:
	static constexpr unsigned int bufferSize = 32u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	bool rawEnabled = false;
	std::queue<Event> buffer;
	std::queue<RawDelta> rawDeltaBuffer;	
};