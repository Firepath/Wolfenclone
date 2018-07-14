#pragma once

#include <string>
#include <vector>

class Menu
{
private:
	class MenuItem
	{
	};

public:
	template<typename SelectedCallBack>
	void AddMenuItem( std::string text, SelectedCallBack callback )
	{
		//TODO: Implement AddMenuItem
	}

private:
	std::vector<MenuItem> MenuItems;
};