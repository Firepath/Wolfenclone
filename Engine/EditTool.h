#pragma once

#include <string>

#include "Colors.h"
#include "StringKeyRepository.h"

class Editor;
class MapFixture;

class EditTool
{
public:
	static constexpr char TypeName[] = "EditTool_MouseButton";

	EditTool( Editor* const editor )
		:
		_Editor( editor )
	{
	}

	virtual const std::string GetTypeName() const
	{
		return EditTool::TypeName;
	}

protected:
	Editor& GetEditor() const
	{
		return *_Editor;
	}

private:
	Editor* _Editor = nullptr;
};

class EditTool_MouseButton : public EditTool
{
public:
	static constexpr char TypeName[] = "EditTool_MouseButton";

	EditTool_MouseButton( Editor* const editor )
		:
		EditTool( editor )
	{
	}

	virtual void ButtonPressed();
	virtual void ButtonReleased();

	virtual const std::string GetTypeName() const override
	{
		return EditTool_MouseButton::TypeName;
	}
	
	virtual Color GetToolColour() const = 0;
	virtual void MouseMoved() = 0;

protected:
	bool ButtonIsPressed = false;
};

class EditTool_MouseButton_None : public EditTool_MouseButton
{
public:
	static constexpr char TypeName[] = "EditTool_MouseButton_None";

	EditTool_MouseButton_None( Editor* const editor )
		:
		EditTool_MouseButton( editor )
	{
	}

	void ButtonPressed() override;
	void ButtonReleased() override;
	const std::string GetTypeName() const
	{
		return EditTool_MouseButton_None::TypeName;
	}

	Color GetToolColour() const override;
	void MouseMoved() override;
};

class EditTool_MouseButton_Insert : public EditTool_MouseButton
{
public:
	static constexpr char TypeName[] = "EditTool_MouseButton_Insert";

	EditTool_MouseButton_Insert( Editor* const editor, MapFixture* const fixture )
		:
		EditTool_MouseButton( editor ),
		Fixture( fixture )
	{
	}

	void ButtonPressed() override;
	void ButtonReleased() override;
	const std::string GetTypeName() const
	{
		return EditTool_MouseButton_Insert::TypeName;
	}

	Color GetToolColour() const override;
	void MouseMoved() override;
	void SetFixture( const MapFixture* fixture );

private:
	const MapFixture* Fixture = nullptr;
};

class EditTool_MouseButton_Delete : public EditTool_MouseButton
{
public:
	static constexpr char TypeName[] = "EditTool_MouseButton_Delete";

	EditTool_MouseButton_Delete( Editor* const editor )
		:
		EditTool_MouseButton( editor )
	{
	}

	void ButtonPressed() override;
	void ButtonReleased() override;
	const std::string GetTypeName() const
	{
		return EditTool_MouseButton_Delete::TypeName;
	}

	Color GetToolColour() const override;
	void MouseMoved() override;
};

class EditTool_MouseButton_Select : public EditTool_MouseButton
{
public:
	static constexpr char TypeName[] = "EditTool_MouseButton_Select";

	EditTool_MouseButton_Select( Editor* const editor )
		:
		EditTool_MouseButton( editor )
	{
	}

	void ButtonPressed() override;
	void ButtonReleased() override;
	const std::string GetTypeName() const
	{
		return EditTool_MouseButton_Select::TypeName;
	}

	Color GetToolColour() const override;
	void MouseMoved() override;

private:
	enum class SelectionMode
	{
		Rectangle,
		Single
	};

private:
	const bool AppendSelection() const;
	const SelectionMode GetSelectionMode() const;
};

class EditTool_MouseButton_Move : public EditTool_MouseButton
{
public:
	static constexpr char TypeName[] = "EditTool_MouseButton_Move";

	EditTool_MouseButton_Move( Editor* const editor )
		:
		EditTool_MouseButton( editor )
	{
	}

	void ButtonPressed() override;
	void ButtonReleased() override;
	const std::string GetTypeName() const
	{
		return EditTool_MouseButton_Move::TypeName;
	}

	Color GetToolColour() const override;
	void MouseMoved() override;
};

class EditToolBox
{
public:
	EditTool_MouseButton* GetMouseButtonTool( std::string name ) const;
	void SetupTools( Editor* const editor );

private:
	std::unique_ptr<StringKeyRepository<StringKeyRepository<EditTool>>> ToolRepository = nullptr;
};