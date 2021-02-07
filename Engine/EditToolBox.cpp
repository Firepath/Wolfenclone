#include "EditTool.h"

EditTool_MouseButton* EditToolBox::GetMouseButtonTool( std::string name ) const
{
	return 	(EditTool_MouseButton*)&(ToolRepository->GetItem( EditTool_MouseButton::TypeName ).GetItem( name ));
}

void EditToolBox::SetupTools( Editor* const editor )
{
	ToolRepository = std::make_unique<StringKeyRepository<StringKeyRepository<EditTool>>>();
	std::unique_ptr<StringKeyRepository<EditTool>> mouseButtonTools = std::make_unique<StringKeyRepository<EditTool>>();

	std::unique_ptr<EditTool> mouseButtonToolNone = std::make_unique<EditTool_MouseButton_None>( editor );
	auto name = mouseButtonToolNone->GetTypeName();
	mouseButtonTools->AddItem( name, std::move( mouseButtonToolNone ) );

	std::unique_ptr<EditTool> mouseButtonToolInsert = std::make_unique<EditTool_MouseButton_Insert>( editor, nullptr );
	name = mouseButtonToolInsert->GetTypeName();
	mouseButtonTools->AddItem( name, std::move( mouseButtonToolInsert ) );

	std::unique_ptr<EditTool> mouseButtonToolDelete = std::make_unique<EditTool_MouseButton_Delete>( editor );
	name = mouseButtonToolDelete->GetTypeName();
	mouseButtonTools->AddItem( name, std::move( mouseButtonToolDelete ) );

	std::unique_ptr<EditTool> mouseButtonToolSelect = std::make_unique<EditTool_MouseButton_Select>( editor );
	name = mouseButtonToolSelect->GetTypeName();
	mouseButtonTools->AddItem( name, std::move( mouseButtonToolSelect ) );

	std::unique_ptr<EditTool> mouseButtonToolMove = std::make_unique<EditTool_MouseButton_Move>( editor );
	name = mouseButtonToolMove->GetTypeName();
	mouseButtonTools->AddItem( name, std::move( mouseButtonToolMove ) );

	ToolRepository->AddItem( EditTool_MouseButton::TypeName, std::move( mouseButtonTools ) );
}
