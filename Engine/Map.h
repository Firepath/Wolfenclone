#pragma once

#define NOMINMAX

#include <unordered_map>
#include <vector>

#include "Colors.h"
#include "EditMode.h"
#include "Graphics.h"
#include "Mouse.h"
#include "PixelEffect.h"
#include "Surface.h"
#include "Vec2.h"

class Map
{
public:
	class Cell
	{
	public:
		static constexpr Color CellEnclosedColour = Colors::Maroon;

		Cell( const Vei2& location );

		void Clear();
		void Draw( const Map& map, Graphics& gfx ) const;
		const bool Fill( const Color colour );
		const bool Fill( Surface* const surface );
		const Vei2& GetLocation() const;
		const bool IsEmpty() const;
		const bool IsEnclosed() const;
		void SetEnclosed( bool enclosed );

	private:
		const bool NothingToDraw() const;

		Color Colour = Colors::Black;
		Surface* Surf = nullptr;
		Vei2 Location;
		bool Enclosed = false;
	};

public:
	Map( const int width, const int height, const Vec2& location );

	void ClearCell( const Vei2& gridLocation );
	void Draw( Graphics& gfx );
	void Fill( const Vei2& gridLocation, const Color colour );
	void Fill( const Vei2& gridLocation, Surface* const surface );
	Cell& GetCell( const Vei2& gridLocation ) const;
	const int GetCellBorderThickness() const;
	const float GetCellSize() const;
	const Vei2 GetScreenLocation() const;
	const Vei2 GetSize() const;
	void HighlightCell( const Vei2& gridLocation, const Color highlightColour, const float highlightOpacity, const bool drawBorder, Graphics& gfx ) const;
	bool IsOnGrid( const Vei2& gridLocation ) const;
	void Move( const Vec2& delta );
	const Vei2 ScreenToGrid( const Vei2& screenLocation ) const;
	void ToggleGridDrawing();
	void Zoom( const Vec2& zoomLocation, const bool zoomingIn );

private:
	static constexpr float MaximumZoomLevel = 40.0f;
	static constexpr float MinimumZoomLevel = 1.0f / MaximumZoomLevel;
	static constexpr float ZoomFactor = 1.25f;
	static constexpr float ZoomFactorInverse = 1.0f / ZoomFactor;
	static constexpr Color GridBorderColour = Colors::Gray;
	static constexpr Color GridColour = Colors::DarkGray;
	static constexpr float DefaultCellSize = 8.0f;
	static constexpr float MinimumCellSize = 2.0f;

	std::unique_ptr<std::unordered_map<Vei2, Cell, Vei2::Hasher>> Cells;

	void ClearEnclosedCells( const Vei2& gridLocation );
	void DrawCells( const Vei2 screenLocation, Graphics& gfx ) const;
	void DrawGrid( const Vei2 screenLocation, Graphics& gfx ) const;
	void CheckForClosingArea( const Vei2& gridLocation, const bool wasEnclosed );
	const bool FillClosedArea( const Vei2& gridLocation );
	const bool FindWall( const Vei2& gridLocation, const int xDirection, const int yDirection ) const;
	const bool IsCellEnclosed( const Vei2& gridLocation ) const;
	bool IsJointFormed( const Vei2& gridLocation ) const;

	float ZoomLevel = 1.0f;
	float CellSize;
	bool DrawGridOverCells = false;
	const int Width;
	const int Height;
	Vec2 Location;
};