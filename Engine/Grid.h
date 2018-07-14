#pragma once

#include <assert.h>
#include <unordered_map>
#include <vector>

#include "Colors.h"
#include "EditConstants.h"
#include "Graphics.h"
#include "MapFixture.h"
#include "Mouse.h"
#include "PixelEffect.h"
#include "Surface.h"
#include "Vec2.h"

class Grid
{
public:
	class Cell
	{
	public:
		Cell( const Vei2& location, Surface* const surface );

		void Draw( const Grid& map, Graphics& gfx ) const;
		const Vei2& GetLocation() const;

	private:
		Surface* Surf = nullptr;
		Vei2 Location;
		bool Enclosed = false;
	};

public:
	Grid( const int width, const int height, const Vec2& location );

	void ClearSelectedCells();
	void DeleteCell( const Vei2& gridLocation );
	void DeleteSelectedCells();
	void Draw( Graphics& gfx );
	void Fill( const Vei2& gridLocation, Surface* const surface );
	Cell& GetCell( const Vei2& gridLocation ) const;
	void HighlightCell( const Vei2& gridLocation, const Color highlightColour, const float highlightOpacity, const bool drawBorder, Graphics& gfx ) const;
	bool IsOnGrid( const Vei2& gridLocation ) const;
	void Move( const Vec2& delta );
	const Vei2 ScreenToGrid( const Vei2& screenLocation ) const;
	void SetTemporarySelectedToSelected();
	void TemporarySelectCell( const Vei2& gridLocation );
	void TemporarySelectCellsInRectangle( const RectI& rect );
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

	const bool CheckIfCellIsEnclosed( const Vei2& gridLocation ) const;
	void ClearEnclosedCells( const Vei2& gridLocation );
	void DrawCells( const Vei2& screenLocation, Graphics& gfx ) const;
	void DrawEnclosedCells( const Vei2& screenLocation, Graphics& gfx ) const;
	void DrawEnclosedCell( const Vei2& screenLocation, const Vei2& gridLocation, Graphics& gfx ) const;
	void DrawGrid( const Vei2 screenLocation, Graphics& gfx ) const;
	void EraseCell( const Vei2& gridLocation );
	void EraseEnclosedCell( const Vei2& gridLocation );
	void CheckForClosingArea( const Vei2& gridLocation, const bool wasEnclosed );
	const bool FillClosedArea( const Vei2& gridLocation );
	const bool FindWall( const Vei2& gridLocation, const int xDirection, const int yDirection ) const;
	const int GetCellBorderThickness() const;
	const Vei2 GetScreenLocation() const;
	const Vei2 GetSize() const;
	void HighlightSelectedCells( Graphics& gfx ) const;
	const bool IsCellAlreadyEnclosed( const Vei2& gridLocation ) const;
	const bool IsCellEnclosed( const Vei2& gridLocation ) const;
	const bool IsCellOccupied( const Vei2& gridLocation ) const;
	bool IsJointFormed( const Vei2& gridLocation ) const;

	float ZoomLevel = 1.0f;
	float CellSize;
	bool DrawGridOverCells = false;
	const int Width;
	const int Height;
	Vec2 Location;

	std::unique_ptr<std::unordered_map<Vei2, Cell, Vei2::Hasher>> Cells;
	std::unique_ptr<std::unordered_map<Vei2, bool, Vei2::Hasher>> EnclosedCells;
	std::vector<Vei2> SelectedCells;
	std::vector<Vei2> TemporarySelectedCells;
};