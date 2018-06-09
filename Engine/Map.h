#pragma once

#define NOMINMAX

#include <unordered_map>
#include <vector>

#include "Colors.h"
#include "Graphics.h"
#include "Mouse.h"
#include "PixelEffect.h"
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
		static constexpr Color CellHoverHighlightColour = Colors::MediumGray;
		static constexpr float CellHoverOpacity = 50.0f;

		static constexpr Color CellEnclosedColour = Colors::Maroon;

		Cell( const Vei2& location );

		void Clear();
		void Draw( const Map& map, Graphics& gfx ) const;
		const bool Fill( const Color colour );
		const Vei2& GetLocation() const;
		void Hover( const Map& map, Graphics& gfx ) const;
		const bool IsEmpty() const;
		const bool IsEnclosed() const;
		void SetEnclosed( bool enclosed );

	private:
		const bool NothingToDraw() const;

		Color Colour = Colors::Black;
		Vei2 Location;
		bool Enclosed = false;
	};

public:
	Map( const int width, const int height, const Vec2& location );

	void Draw( Graphics& gfx );
	void DoMouseEvents( Mouse& mouse );
	void Move( const Vec2& delta );
	void Zoom( const Vec2& zoomLocation, const bool zoomingIn );

private:
	static constexpr float MaximumZoomLevel = 10.0f;
	static constexpr float MinimumZoomLevel = 1.0f / MaximumZoomLevel;
	static constexpr float ZoomFactor = 1.25;
	static constexpr float ZoomFactorInverse = 1.0f / ZoomFactor;
	static constexpr Color GridBorderColour = Colors::Gray;
	static constexpr Color GridColour = Colors::DarkGray;
	static constexpr float DefaultCellSize = 8.0f;
	static constexpr float MinimumCellSize = 2.0f;

	MouseInfo MouseInf;

	float ZoomLevel = 1.0f;
	float CellSize;
	const int Width;
	const int Height;
	Vec2 Location;

	std::unique_ptr<std::unordered_map<Vei2, Cell, Vei2::Hasher>> Cells;

	void Clear( const Vei2& screenLocation );
	void ClearEnclosedCells( const Vei2& gridLocation );
	void Click( const Vei2& screenLocation );
	const bool FillClosedArea( const Vei2& gridLocation );
	const bool FindWall( const Vei2& gridLocation, const int xDirection, const int yDirection ) const;
	const bool IsCellEnclosed( const Vei2& gridLocation ) const;
	bool IsJointFormed( const Vei2& gridLocation ) const;
	bool IsOnGrid( const Vei2& gridLocation ) const;
	const Vei2 ScreenLocation() const;
	const Vei2 ScreenToGrid( const Vei2& screenLocation ) const;
};