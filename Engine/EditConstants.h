#pragma once

#include "Colors.h"

namespace EditConstants
{
	class CellSelection
	{
	public:
		static constexpr Color InactiveModeHoverColour = Colors::MediumGray;
		static constexpr Color InsertModeHoverColour = Colors::Green;
		static constexpr Color SelectModeHoverColour = Colors::Yellow;
		static constexpr float CellHoverOpacity = 50.0f;
	};

	enum class MouseLClickMode
	{
		None,
		Select,
		Insert,
		EnumOptionsCount // This is the size of the enum, don't add things after it
	};

	enum class SelectionMode
	{
		Rectangle,
		Single
	};
}