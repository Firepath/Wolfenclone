#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Font.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
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
	static constexpr Color DefaultHoverTextColour = Colors::LightBlue;
	static constexpr size_t DefaultMaxHeight = 30u;
	static constexpr size_t DefaultMaxWidth = 300u;
	static constexpr float DefaultOpacity = 95.0f;
	static constexpr Color DefaultTextColour = Colors::White;

	MenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, const MenuItem* const menu, const Font* const font, Graphics& gfx, const Color textHighlightColour = MenuItem::DefaultHoverTextColour );

	void AddMenuItem( std::unique_ptr<MenuItem> menuItem );
	void AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, const Color textHighlightColour );
	void DoHovering( const bool hovering, const bool hoveringOnChild );
	void DoKeyboardEvents( Keyboard::Event& ke );
	void DoMouseEvents( Mouse::Event& me );
	const Vei2 GetLocation() const;
	const Vei2 GetSize() const;
	const std::string GetText() const;
	const size_t GetWidth() const;
	const bool IsOpen() const;
	const bool IsVisible() const;
	void Select();
	void SetLocation( const Vei2 screenLocation );
	void SetOpen( const bool open );
	void SetText( const std::string text );
	void SetVisible( const bool visible );
	void SetWidth( const size_t width );

	template<typename E>
	void Show( const Vei2& location, E effect )
	{
		Location = location;
		Visible = true;

		const RectI boxRect = RectI( location, (int)Width, (int)Height );
		_gfx.DrawBox( boxRect, BoxColour, effect );
		PixelEffect::Sustitution fontEffect( TextColour, _Font->GetColour() );
		_Font->DrawString( GetText(), location + Vei2( (int)BoxPadding, (int)BoxPadding ), boxRect, fontEffect, _gfx );

		if ( IsOpen() )
		{
			ShowMenu();
		}
	}

	virtual void ShowMenu();

protected:
	const bool IsHovering( const Vei2 mouseLocation, const bool onlyCheckThisMenuItem ) const;
	void ShowMenu( const Vei2 location );

protected:
	Color BorderColour = MenuItem::DefaultBorderColour;
	size_t BorderThickness = MenuItem::DefaultBorderThickness;
	Color BoxColour = MenuItem::DefaultBoxColour;
	size_t BoxPadding = MenuItem::DefaultBoxPadding;
	Color TextColour = MenuItem::DefaultTextColour;
	Color TextHighlightColour = MenuItem::DefaultHoverTextColour;
	size_t MaximumWidth = MenuItem::DefaultMaxWidth;
	float Opacity = MenuItem::DefaultOpacity;

	size_t Height = MenuItem::DefaultMaxHeight;
	size_t Width = MenuItem::DefaultMaxWidth;

	Vei2 Location;
	bool Open = false;
	bool Visible = false;

	const Font* const _Font;
	Graphics& _gfx;

	const MenuItem* const _Menu = nullptr;

	std::string Text;
	std::vector<std::unique_ptr<MenuItem>> MenuItems;
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
	static constexpr Color DefaultBorderColour = Colors::White;
	static constexpr size_t DefaultBorderThickness = 1u;
	static constexpr Color DefaultBoxColour = Colors::DarkGray;
	static constexpr float DefaultOpacity = 95.0f;

	MenuBar( const Vei2 location, const Vei2 size, Graphics& gfx );

	void AddMenu( std::unique_ptr<Menu> menu );
	void DoKeyboardEvents( Keyboard::Event& ke );
	void DoMouseEvents( Mouse::Event& me );
	void Draw() const;

private:
	void CancelMenus();

private:
	Color BorderColour = MenuBar::DefaultBorderColour;
	size_t BorderThickness = MenuBar::DefaultBorderThickness;
	Color BoxColour = MenuBar::DefaultBoxColour;
	float Opacity = MenuBar::DefaultOpacity;

	const Vei2 Location;
	Graphics& _gfx;
	Vei2 Size;

	std::vector<std::unique_ptr<Menu>> Menus;
};