#pragma once

#include <string>
#include <vector>

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
	void Execute() const override
	{
		int i = 5;
	}
};

class Menu
{
private:
	static constexpr Color DefaultBorderColour = Colors::LightGray;
	static constexpr size_t DefaultBorderThickness = 3u;
	static constexpr Color DefaultBoxColour = Colors::Gray;
	static constexpr size_t DefaultBoxPadding = 4u;
	static constexpr size_t DefaultMaxHeight = 30u;
	static constexpr size_t DefaultMaxWidth = 100u;
	static constexpr float DefaultOpacity = 50.0f;

	class MenuItem
	{
	public:

		MenuItem( std::string text, const Menu& menu, std::unique_ptr<SelectedCallBack> callback )
			:
			Text( text ),
			_Menu( menu ),
			CallBack( std::move( callback ) )
		{
			Height = _Menu._Font.GetGlyphHeight() + 2 * _Menu.BoxPadding;
			Width = _Menu.MaximumWidth;
		}

		template<typename E>
		void Draw( const Vei2& location, E effect, Graphics& gfx )
		{
			const RectI borderRect( location, Width, Height );
			const RectI boxRect = borderRect.GetExpanded( -(int)_Menu.BorderThickness );
			gfx.DrawBorderedBox( boxRect, borderRect, _Menu.BoxColour, _Menu.BorderColour, effect );
			_Menu._Font.DrawString( GetText(), location + Vei2( (int)_Menu.BoxPadding, (int)_Menu.BoxPadding ), boxRect, gfx );
		}

		const Vei2 GetSize() const
		{
			return Vei2( (int)Width, (int)Height );
		}

		const std::string GetText() const
		{
			return Text;
		}

		void SetText( const std::string text )
		{
			Text = text;
		}

		void Select()
		{
			CallBack->Execute();
		}

	private:
		size_t Height = Menu::DefaultMaxHeight;
		size_t Width = Menu::DefaultMaxWidth;

		const Menu& _Menu;
		std::string Text;
		std::unique_ptr<SelectedCallBack> CallBack;
	};

public:
	Menu( const Font& font )
		:
		_Font( font )
	{
	}

	void AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback )
	{
		MenuItems.emplace_back(  text, *this, std::move( callback ) );
	}

	const Vei2 GetLocation()
	{
		return Location;
	}

	void SetLocation( const Vei2 screenLocation )
	{
		Location = screenLocation;
	}

	void Show( Graphics& gfx )
	{
		// TODO: Smarts to make sure menu shows on screen as best it can
		// TODO: Further smarts to allow scrolling menu that is off the screen

		PixelEffect::Transparency boxEffect( Opacity );

		Vei2 location = Location;
		for ( auto it = MenuItems.begin(); it != MenuItems.end(); it++ )
		{
			it->Draw( location, boxEffect, gfx );
			location.y += it->GetSize().y;
		}
	}

private:
	Color BorderColour = Menu::DefaultBorderColour;
	size_t BorderThickness = Menu::DefaultBorderThickness;
	Color BoxColour = Menu::DefaultBoxColour;
	size_t BoxPadding = Menu::DefaultBoxPadding;
	size_t MaximumWidth = Menu::DefaultMaxWidth;
	float Opacity = Menu::DefaultOpacity;

	Vei2 Location;
	const Font& _Font;

	std::vector<MenuItem> MenuItems;
};