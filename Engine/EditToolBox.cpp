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
	mouseButtonTools->AddItem( mouseButtonToolNone->GetTypeName(), std::move( mouseButtonToolNone ) );
	std::unique_ptr<EditTool> mouseButtonToolInsert = std::make_unique<EditTool_MouseButton_Insert>( editor, nullptr );
	mouseButtonTools->AddItem( mouseButtonToolInsert->GetTypeName(), std::move( mouseButtonToolInsert ) );
	std::unique_ptr<EditTool> mouseButtonToolDelete = std::make_unique<EditTool_MouseButton_Delete>( editor );
	mouseButtonTools->AddItem( mouseButtonToolDelete->GetTypeName(), std::move( mouseButtonToolDelete ) );
	std::unique_ptr<EditTool> mouseButtonToolSelect = std::make_unique<EditTool_MouseButton_Select>( editor );
	mouseButtonTools->AddItem( mouseButtonToolSelect->GetTypeName(), std::move( mouseButtonToolSelect ) );
	std::unique_ptr<EditTool> mouseButtonToolMove = std::make_unique<EditTool_MouseButton_Move>( editor );
	mouseButtonTools->AddItem( mouseButtonToolMove->GetTypeName(), std::move( mouseButtonToolMove ) );

	ToolRepository->AddItem( EditTool_MouseButton::TypeName, std::move( mouseButtonTools ) );
}
