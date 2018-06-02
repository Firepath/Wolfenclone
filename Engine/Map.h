#pragma once

#define NOMINMAX

#include <unordered_map>

#include "Colors.h"
#include "Graphics.h"
#include "Mouse.h"
#include "Vec2.h"

class Map
{
public:
	struct MouseInfo
	{
		// In Grid Coordinates
		Vei2 HoverGridLocation = Vei2( -1, -1 );
		Vei2 LMouseButtonGridLocation = Vei2( -1, -1 );

		// In Screen Coordinates
		Vei2 MMouseButtonLocation = Vei2( -1, -1 );
	};

	class Cell
	{
	public:
		Cell( const Vei2& location )
			:
			Location( location )
		{
		}

		void Clear()
		{
			Colour = Colors::Black;
		}

		void Click()
		{
			if ( Colour == Colors::Black )
			{
				Colour = Colors::White;
			}
			else
			{
				Colour.SetB( std::max( 0, (int)Colour.GetB() - 20 ) );
			}
		}

		void Draw( const Map& map, Graphics& gfx )
		{
			if ( Colour == Colors::Black )
			{
				return;
			}

			Vei2 topLeft( (int)std::ceil( map.Location.x ) + (int)std::ceil( (float)Location.x * map.CellSize ), (int)std::ceil( map.Location.y ) + (int)std::ceil( (float)Location.y * map.CellSize ) );
			Vei2 bottomRight( (int)std::ceil( map.Location.x ) + (int)std::ceil( (float)(Location.x + 1) * map.CellSize ) - 1, (int)std::ceil( map.Location.y ) + (int)std::ceil( (float)(Location.y + 1) * map.CellSize ) - 1 );
			gfx.DrawBox( topLeft, bottomRight, Colour );
		}

		const Vei2& GetLocation() const
		{
			return Location;
		}

	private:
		Color Colour = Colors::Black;
		Vei2 Location;
	};

public:
	Map( const int width, const int height, const Vec2& location );

	void Draw( Graphics& gfx );
	void DoMouseEvents( Mouse& mouse );
	void Move( const Vec2& delta );
	void Zoom( const Vec2& zoomLocation, const float zoomLevel );

private:
	static constexpr float MaximumZoomLevel = 10.0f;
	static constexpr float MinimumZoomLevel = 1.0f / MaximumZoomLevel;
	static constexpr float ZoomFactor = 1.25;
	static constexpr float ZoomFactorInverse = 1.0f / ZoomFactor;
	static constexpr Color GridBorderColour = Colors::Gray;
	static constexpr Color GridColour = Colors::DarkGray;
	static constexpr float DefaultCellSize = 8.0f;

	MouseInfo MouseInf;

	float ZoomLevel = 1.0f;
	float CellSize;
	const int Width;
	const int Height;
	Vec2 Location;

	std::unordered_map<Vei2, Cell, Vei2::Hasher> Cells;

	void Clear( const Vei2& screenLocation );
	void Click( const Vei2& screenLocation );
	bool IsOnGrid( const Vei2& gridLocation );
	const Vei2 ScreenToGrid( const Vei2& screenLocation );
};