#pragma once

#include <string>
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

class Menu
{
private:
	static constexpr Color DefaultBorderColour = Colors::White;
	static constexpr size_t DefaultBorderThickness = 1u;
	static constexpr Color DefaultBoxColour = Colors::DarkGray;
	static constexpr size_t DefaultBoxPadding = 4u;
	static constexpr size_t DefaultMaxHeight = 30u;
	static constexpr size_t DefaultMaxWidth = 1400u;
	static constexpr float DefaultOpacity = 95.0f;

	class MenuItem
	{
	public:

		MenuItem( std::string text, const Menu& menu, std::unique_ptr<SelectedCallBack> callback );

		template<typename E>
		void Draw( const Vei2& location, E effect, Graphics& gfx )
		{

			const RectI borderRect = RectI( location, Width, Height ).GetExpanded( -1 );
			const RectI boxRect = borderRect.GetExpanded( -(int)_Menu.BorderThickness );
			gfx.DrawBorderedBox( boxRect, borderRect, _Menu.BoxColour, _Menu.BorderColour, effect );
			_Menu._Font.DrawString( GetText(), location + Vei2( (int)_Menu.BoxPadding, (int)_Menu.BoxPadding ), boxRect, gfx );
		}

		const Vei2 GetSize() const;
		const std::string GetText() const;
		void SetText( const std::string text );
		void Select();

	private:
		size_t Height = Menu::DefaultMaxHeight;
		size_t Width = Menu::DefaultMaxWidth;

		const Menu& _Menu;
		std::string Text;
		std::unique_ptr<SelectedCallBack> CallBack;
	};

public:
	Menu( const Font& font );

	void AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback );
	const Vei2 GetLocation();
	void SetLocation( const Vei2 screenLocation );
	void Show( Graphics& gfx );

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