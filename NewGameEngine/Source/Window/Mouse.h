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
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		Type GetType() const noexcept
		{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return{ x,y };
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		bool LeftIsPressed() const noexcept
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const noexcept
		{
			return rightIsPressed;
		}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	// Get position of the mouse in the windows
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	// Get Delta position
	std::optional<RawDelta> ReadRawDelta() noexcept;
	// Check whether the mouse in the window
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	// Read event and delete it from the queue
	std::optional<Mouse::Event> Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}
	bool IsRawEmpty() const noexcept
	{
		return rawDeltaBuffer.empty();
	}
	void Flush() noexcept;
	// Enable/Disable delta mouse
	void EnableRaw() noexcept;
	void DisableRaw() noexcept;
	bool RawEnabled() const noexcept;
private:
	// Actions on mouse eventss
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseMoveRaw(int xR, int yR) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	// If buffer overflow happens, trim it.
	void TrimRawInputBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
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