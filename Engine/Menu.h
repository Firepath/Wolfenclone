#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Font.h"
#include "Graphics.h"
#include "PixelEffect.h"
#include "Rect.h"
#include "Vec2.h"

class SelectedCallBack
{
public:
	~SelectedCallBack() = default;

	virtual void Execute() const = 0;
};

class TestCallBack : public SelectedCallBack
{
public:
	void Execute() const override;
};

class MenuItem
{
public:
	static constexpr Color DefaultBorderColour = Colors::White;
	static constexpr size_t DefaultBorderThickness = 1u;
	static constexpr Color DefaultBoxColour = Colors::DarkGray;
	static constexpr size_t DefaultBoxPadding = 4u;
	static constexpr size_t DefaultMaxHeight = 30u;
	static constexpr size_t DefaultMaxWidth = 300u;
	static constexpr float DefaultOpacity = 95.0f;

	MenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, const MenuItem* const menu, const Font* const font, Graphics& gfx );

	void AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback );
	const Vei2 GetLocation() const;
	const Vei2 GetSize() const;
	const std::string GetText() const;
	const bool GetVisible() const;
	const size_t GetWidth() const;
	void Select();
	void SetLocation( const Vei2 screenLocation );
	void SetText( const std::string text );
	void SetVisible( const bool visible );
	void SetWidth( const size_t width );

	template<typename E>
	void Show( const Vei2& location, E effect )
	{
		Location = location;
		Visible = true;

		const RectI borderRect = RectI( location, Width, Height ).GetExpanded( -1 );
		const RectI boxRect = borderRect.GetExpanded( -(int)BorderThickness );
		_gfx.DrawBorderedBox( boxRect, borderRect, BoxColour, BorderColour, effect );
		_Font->DrawString( GetText(), location + Vei2( (int)BoxPadding, (int)BoxPadding ), boxRect, _gfx );
	}

	virtual void ShowMenu();

protected:
	void ShowMenu( const Vei2 location );

protected:
	Color BorderColour = DefaultBorderColour;
	size_t BorderThickness = DefaultBorderThickness;
	Color BoxColour = DefaultBoxColour;
	size_t BoxPadding = DefaultBoxPadding;
	size_t MaximumWidth = DefaultMaxWidth;
	float Opacity = DefaultOpacity;

	size_t Height = DefaultMaxHeight;
	size_t Width = DefaultMaxWidth;

	Vei2 Location;
	bool Visible = false;

	const Font* const _Font;
	Graphics& _gfx;

	const MenuItem* const _Menu = nullptr;

	std::string Text;
	std::vector<MenuItem> MenuItems;
	std::unique_ptr<SelectedCallBack> CallBack = nullptr;
};

class Menu : public MenuItem
{
public:
	Menu( std::string text, const Font* const font, Graphics& gfx );

	void ShowMenu() override;
};

class MenuBar
{
public:
	MenuBar( const Vei2 location );

	void AddMenu( const std::string name, std::unique_ptr<Menu> menu );

private:
	const Vei2 Location;

	std::vector<std::unique_ptr<Menu>> Menus;
};