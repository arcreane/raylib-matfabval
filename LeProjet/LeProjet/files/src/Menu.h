#pragma once
#include "Level.h"
#include <raylib.h>

class Menu : public Level
{
private:
	int world;
	int currentLevel;
	int totalLevel;
	int unlockLevel;
	int envItemsLengthMAPmonde1;

	Texture2D youAreHereTexture;

public:
	Menu(LevelManager& levelManager);
	void InitLevel() override;
	void UpdateLevel() override;
	void DrawLevel() override;
	Color returnColorToPrint(int i, int unlockLevel, int actuelLevel);


	// Getter - Setter 
	int GetWorld();
	void SetWorld(int c_world);
	const int GetCurrentLevel() const;
	void SetCurrentLevel(int c_level);
	int GetTotalLevel();
	void SetTotalLevel(int c_totalLevel);
	int GetUnlockLevel();
	void SetUnlockLevel(int c_unlocklevel);
};