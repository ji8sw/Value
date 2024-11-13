#include "Value.h"
#include <iostream>
#include <thread>

int main()
{
	Value::Value New("GameVersion:1.2.3");

	Value::Value New2("HowMuchWoodCouldAWoodChuckChuckIfAWoodChuckCouldChuckWood:329254");

	Value::Set NewSet;
	NewSet.Add(New);
	NewSet.Add(New2);

	NewSet.SaveTo("Configuration.Set");

	std::cout << "Saved to Configuration.Set!\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	system("cls");
	std::this_thread::sleep_for(std::chrono::seconds(1));

	Value::Set LoadedSet("Configuration.Set");
	std::cout << "Loaded Set from Configuration.Set:\n";
	std::cout << LoadedSet.ToString() << '\n';
}