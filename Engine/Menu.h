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

		MenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback )
			:
			Text( text ),
			CallBack( std::move( callback ) )
		{
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
		std::string Text;
		std::unique_ptr<SelectedCallBack> CallBack;
	};

public:
	Menu( const Font& font )
		:
		_Font( font )
	{
		BoxHeight = _Font.GetGlyphHeight() + 2 * BoxPadding;
	}

	void AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback )
	{
		MenuItems.emplace_back(  text, std::move( callback ) );
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
			const RectI borderRect( location, BoxWidth, BoxHeight );
			const RectI boxRect = borderRect.GetExpanded( -(int)BorderThickness );
			gfx.DrawBorderedBox( boxRect, borderRect, BoxColour, BorderColour, boxEffect );
			_Font.DrawString( it->GetText(), location + Vei2( (int)BoxPadding, (int)BoxPadding ), boxRect, gfx );
			location.y += BoxHeight;
		}
	}

private:
	size_t BoxHeight = Menu::DefaultMaxHeight;
	size_t BoxWidth = Menu::DefaultMaxWidth;

	Color BorderColour = Menu::DefaultBorderColour;
	size_t BorderThickness = Menu::DefaultBorderThickness;
	Color BoxColour = Menu::DefaultBoxColour;
	size_t BoxPadding = Menu::DefaultBoxPadding;
	float Opacity = Menu::DefaultOpacity;

	Vei2 Location;
	const Font& _Font;

	std::vector<MenuItem> MenuItems;
};