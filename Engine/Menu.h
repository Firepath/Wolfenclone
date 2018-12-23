#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Font.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Rect.h"
#include "Surface.h"
#include "Vec2.h"

class Editor;
class Graphics;

class SelectedCallBack
{
public:
	~SelectedCallBack() = default;

	virtual void Execute() const = 0;
};

class MenuItem
{
public:
	static constexpr Color DefaultBorderColour = Colors::White;
	static constexpr size_t DefaultBorderThickness = 1u;
	static constexpr Color DefaultBoxColour = Colors::DarkGray;
	static constexpr size_t DefaultBoxPadding = 8u;
	static constexpr Color DefaultHoverTextColour = Colors::LightBlue;
	static constexpr size_t DefaultMaxHeight = 30u;
	static constexpr size_t DefaultMaxWidth = 300u;
	static constexpr float DefaultOpacity = 95.0f;
	static constexpr Color DefaultTextColour = Colors::White;

	class Menu_ItemSelectedCallBack : public SelectedCallBack
	{
	public:
		Menu_ItemSelectedCallBack( MenuItem* const menuItem )
			:
			_MenuItem( menuItem )
		{
		}

		virtual void Execute() const override = 0;

	protected:
		MenuItem* GetMenuItem() const;

	private:
		MenuItem* const _MenuItem = nullptr;
	};

	class Menu_FileSaveItemSelectedCallBack : public Menu_ItemSelectedCallBack
	{
	public:
		Menu_FileSaveItemSelectedCallBack( MenuItem* const menuItem, Editor* const editor )
			:
			Menu_ItemSelectedCallBack( menuItem ),
			_Editor( editor )
		{
		}

		void Execute() const override;

	private:
		Editor* const _Editor = nullptr;
	};

	MenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, const MenuItem* const menu, const Font* const font, Graphics& gfx, const Color highlightColour = MenuItem::DefaultHoverTextColour );

	void AddMenuItem( std::unique_ptr<MenuItem> menuItem );
	void AddMenuItem( std::string text, std::unique_ptr<SelectedCallBack> callback, const Color highlightColour = MenuItem::DefaultHoverTextColour );
	void DoKeyboardEvents( Keyboard::Event& ke );
	virtual void DoMouseEvents( Mouse::Event& me );
	const size_t GetColumns() const;
	const Vei2 GetLocation() const;
	const Vei2 GetSize() const;
	const std::string GetText() const;
	const size_t GetWidth() const;
	void StackMenuItem( std::unique_ptr<MenuItem> menuItem, const size_t position = 0 );
	const bool IsOpen() const;
	const bool IsVisible() const;
	void Select();
	void SetColumns( const size_t columns );
	void SetLocation( const Vei2 screenLocation );
	void SetOpen( const bool open );
	void SetText( const std::string text );
	void SetVisible( const bool visible );
	void SetWidth( const size_t width );

	virtual void Show( const Vei2& location, std::unique_ptr<PixelEffect::Effect>& effect )
	{
		Location = location;
		Visible = true;

		RectI boxRect = RectI( location, (int)Width, (int)Height );
		std::unique_ptr<PixelEffect::Effect> fontEffect = std::make_unique<PixelEffect::Substitution>( TextColour, _Font->GetColour() );
		_Font->DrawString( GetText(), location + Vei2( (int)BoxPadding, (int)BoxPadding ), boxRect, fontEffect, _gfx );
	}

	virtual void ShowMenu();

protected:
	void DoHovering( const bool hovering );
	void DoSubMenuMouseEvents( Mouse::Event& me );
	const RectI GetSubMenuArea() const;
	virtual const Vei2 GetSubMenuLocation() const;
	const Vei2 GetSubMenuSize() const;
	const bool IsHovering();
	const bool IsHovering( const Vei2 mouseLocation, const bool onlyCheckThisMenuItem );
	void ResetSubMenuItems( const Vei2 mousePos );
	void SetHovering( bool hovering );
	void ShowMenu( const Vei2 location );
	void ShowSubMenu( const Vei2 location, std::unique_ptr<PixelEffect::Effect>& boxEffect, bool onlyHovering );

protected:
	Color BorderColour = MenuItem::DefaultBorderColour;
	size_t BorderThickness = MenuItem::DefaultBorderThickness;
	Color BoxColour = MenuItem::DefaultBoxColour;
	size_t BoxPadding = MenuItem::DefaultBoxPadding;
	Color HighlightColour = MenuItem::DefaultHoverTextColour;
	size_t MaximumWidth = MenuItem::DefaultMaxWidth;
	Color TextColour = MenuItem::DefaultTextColour;
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
	size_t Columns = 1;
	std::unique_ptr<SelectedCallBack> CallBack = nullptr;

private:
	bool Hovering = false;
};

class Menu : public MenuItem
{
public:
	Menu( std::string text, const Font* const font, Graphics& gfx, const Color highlightColour = MenuItem::DefaultHoverTextColour );

	void DoMouseEvents( Mouse::Event& me ) override;
	void ShowMenu() override;

protected:
	const Vei2 GetSubMenuLocation() const override;
};

class ImageMenuItem : public MenuItem
{
public:
	ImageMenuItem( const Surface* const image, const int height, const int width, std::unique_ptr<SelectedCallBack> callback, const MenuItem* const menu, Graphics& gfx, const Color highlightColour = MenuItem::DefaultHoverTextColour );

	void Show( const Vei2& location, std::unique_ptr<PixelEffect::Effect>& effect ) override;

private:
	const Surface* const Image;
};

class MenuBar
{
public:
	static constexpr Color DefaultBorderColour = Colors::White;
	static constexpr size_t DefaultBorderThickness = 1u;
	static constexpr Color DefaultBoxColour = Colors::DarkGray;
	static constexpr float DefaultOpacity = 95.0f;

	MenuBar( const Vei2 location, const Vei2 size, Graphics& gfx );

	void AddMenu( std::unique_ptr<MenuItem> menu );
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

	std::vector<std::unique_ptr<MenuItem>> Menus;
};