#include "EditTool.h"

EditTool_MouseButton * EditToolBox::GetMouseButtonTool( std::string name ) const
{
	return 	(EditTool_MouseButton*)&(ToolRepository->GetItem( EditTool_MouseButton::TypeName ).GetItem( name ));
}

void EditToolBox::SetupTools( Editor* const editor )
{
	ToolRepository = std::make_unique<StringKeyRepository<StringKeyRepository<EditTool>>>();
	std::unique_ptr<StringKeyRepository<EditTool>> leftMouseButtonTools = std::make_unique<StringKeyRepository<EditTool>>();

	std::unique_ptr<EditTool> leftMouseButtonToolNone = std::make_unique<EditTool_MouseButton_None>( editor );
	leftMouseButtonTools->AddItem( leftMouseButtonToolNone->GetTypeName(), std::move( leftMouseButtonToolNone ) );
	std::unique_ptr<EditTool> leftMouseButtonToolInsert = std::make_unique<EditTool_MouseButton_Insert>( editor, nullptr );
	leftMouseButtonTools->AddItem( leftMouseButtonToolInsert->GetTypeName(), std::move( leftMouseButtonToolInsert ) );
	std::unique_ptr<EditTool> leftMouseButtonToolSelect = std::make_unique<EditTool_MouseButton_Select>( editor );
	leftMouseButtonTools->AddItem( leftMouseButtonToolSelect->GetTypeName(), std::move( leftMouseButtonToolSelect ) );
	std::unique_ptr<EditTool> leftMouseButtonToolMove = std::make_unique<EditTool_MouseButton_Move>( editor );
	leftMouseButtonTools->AddItem( leftMouseButtonToolMove->GetTypeName(), std::move( leftMouseButtonToolMove ) );

	ToolRepository->AddItem( EditTool_MouseButton::TypeName, std::move( leftMouseButtonTools ) );
}
