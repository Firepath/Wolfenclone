#pragma once

#include "Colors.h"

namespace EditConstants
{
	class CellConstants
	{
	public:
		static constexpr Color CellEnclosedColour = Colors::Maroon;
		static constexpr Color CellEnclosedPlayableAreaColour = Colors::DarkBlue;
	};

	class CellEditing
	{
	public:
		static constexpr Color InactiveModeHoverColour = Colors::MediumGray;
		static constexpr Color InsertModeHoverColour = Colors::Green;
		static constexpr Color DeleteModeHoverColour = Colors::Red;
		static constexpr Color SelectModeHoverColour = Colors::Yellow;
		static constexpr Color MoveModeHoverColour = Colors::Cyan;
		static constexpr float CellHoverOpacity = 50.0f;
	};

	enum class MouseLClickMode
	{
		None,
		Select,
		Insert,
		Delete,
		Move,
		EnumOptionsCount // This is the size of the enum, don't add things after it
	};
}