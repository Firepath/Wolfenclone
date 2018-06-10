#pragma once

namespace EditMode
{
	enum class MouseLClick
	{
		None,
		Select,
		Insert,
		EnumOptionsCount // This is the size of the enum, don't add things after it
	};

	enum class Selection
	{
		Rectangle,
		Single
	};
}