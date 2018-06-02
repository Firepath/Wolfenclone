#pragma once

#include <unordered_map>

#include "Colors.h"
#include "Graphics.h"
#include "Vec2.h"

class Map
{
public:
	class Cell
	{
	public:
		Cell( const Vei2& location )
			:
			Location( location )
		{
		}

		void Click()
		{
			if ( Colour == Colors::Black )
			{
				Colour = Colors::White;
			}
			else
			{
				Colour = Colors::Yellow;
			}
		}

		void Draw( const Map& map, Graphics& gfx )
		{
			if ( Colour == Colors::Black )
			{
				return;
			}

			Vei2 location = (Vei2)map.Location + (Vei2)((Vec2)Location * map.CellSize);
			gfx.DrawBox( location, location + Vei2( (int)map.CellSize, (int)map.CellSize ), Colour );
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

	void Click( const Vei2& clickLocation );
	void Draw( Graphics& gfx );
	void Move( const Vec2& delta );
	void Zoom( const Vec2& zoomLocation, const float zoomLevel );

private:
	static constexpr Color GridColour = Colors::DarkGray;
	static constexpr float DefaultCellSize = 40.0f;

	float ZoomLevel = 1.0f;
	float CellSize;
	const int Width;
	const int Height;
	Vec2 Location;

	std::unordered_map<Vei2, Cell, Vei2::Hasher> Cells;
};