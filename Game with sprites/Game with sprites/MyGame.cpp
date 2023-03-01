#include "MyDirectX.h"
const string APPTITLE = "Transparent Sprite Demo";
const int SCREENW = 1024;
const int SCREENH = 768;
LPD3DXFONT fontArial = NULL;

const int dirtHeight = 100;
const int dirtWidth = 50;


MINER miner;
DIRT dirt[dirtWidth][dirtHeight];
CAMERA camera(0, 0);
REFINERY refinery;
POINT mousePos;
GASSTATION gasStation;
UPGRADE upgradeStation;
BOUNDARY leftBoundary;
BOUNDARY rightBoundary;

//Book's collision funtion
int Collision(RECT rect1, RECT rect2)
{
	RECT dest; //ignored
	return IntersectRect(&dest, &rect1, &rect2);
};

void GenerateMap()
{
	int rarity;
	int loadCount = 0;
	
	//spawn all dirt:
	for (int i = 1; i < dirtWidth; i++)
	{	
		for (int j = 0; j < dirtHeight; j++)
		{	
			rarity = 1; //dirt
			if ((j == 0) || (rand() % 25 != 0)) //5% chance to spawn nothing
			{
				if (j <= 10)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 2;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 3;
					}
				}
				if (j >= 11 && j <= 20)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 3;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 4;
					}
				}
				if (j >= 21 && j <= 30)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 4;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 5;
					}
				}
				if (j >= 31 && j <= 40)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 5;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 6;
					}
				}
				if (j >= 41 && j <= 50)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 6;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 7;
					}
				}
				if (j >= 51 && j <= 60)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 7;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 8;
					}

				}
				if (j >= 61 && j <= 70)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 8;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 9;
					}
				}
				if (j >= 71 && j <= 80)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 9;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 10;
					}
				}
				if (j >= 81 && j <= 90)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 10;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 11;
					}
				}
				if (j >= 91 && j <= 100)
				{
					if (rand() % 4 == 0)//25% chance to spawn copper
					{
						rarity = 10;
					}
					else if (rand() % 5 == 0)//20% chance to spawn tin
					{
						rarity = 11;
					};
				};
				dirt[i][j].init((i * 50) - 20, 425 + (j * 50), rarity);
				dirt[i][j].active = true;
			};
			if (i == 3 && j == 1)
			{
				//place the refinery
				refinery.init(i * 50 - 20, 325 + (j * 50));
			};

			if (i == 8 && j == 1)
			{
				//place the refinery
				gasStation.init(i * 50 - 20, 325 + (j * 50));
			};

			if (i == 12 & j == 1)
			{
				//place the upgrade station
				upgradeStation.init(i * 50 - 20, 325 + (j * 50));
			}
			loadCount += 1;
			//clear the backbuffer (to draw the loading screen)
			d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_XRGB(0, 0, 0));

			//start rendering
			if(loadCount % 500 == 0)
			if (d3ddev->BeginScene())
			{
				spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
				if (miner.mining || !miner.mining)
				{
					FontPrint(fontArial, SCREENW/2, SCREENH/2, "Loading", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontArial, SCREENW/2, (SCREENH/2) + 20, to_string(loadCount/1000) + "/" + to_string((dirtHeight * dirtWidth)/1000), D3DCOLOR_XRGB(255, 255, 255));

					spriteobj->End();
					//stop rendering
					d3ddev->EndScene();
					d3ddev->Present(NULL, NULL, NULL, NULL);
				}
		};
	};

	
	}
	leftBoundary.init(-20, 325);
	rightBoundary.init(dirtWidth * 50 - 20, 325 + (dirtHeight * 50));
		
	//place boss room/boss
	//place repair shop/item store:
};

bool Game_Init(HWND window)
{
	Direct3D_Init(window, SCREENW - 16, SCREENH - 38, false); //-16, -38 are -- hacks to get the screen to fit properly.
	DirectInput_Init(window);
	//get the back buffer surface
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	//position the bomb
	srand((unsigned int)time(NULL));

	if (!miner.init())
		return false;

	fontArial = MakeFont("Arial", 20, 0);

	GenerateMap();

	return true;
}
void Game_Run(HWND window)
{

	DirectInput_Update();
	//make sure the Direct3D device is valid
	if (!d3ddev)
	{
		return;
	};

	GetCursorPos(&mousePos);

	ScreenToClient(window, &mousePos);//translates mousePos to client

	miner.leftClip = false;
	miner.topClip = false;
	miner.rightClip = false;
	miner.bottomClip = false;
	miner.onGroundMineTest = false;
	miner.leftMineTest = false;
	miner.rightMineTest = false;

	for (int i = 0; i < dirtWidth; i++)
		for (int j = 0; j < dirtHeight; j++)
		{
			if (Collision(miner.onGroundMineTestRect, dirt[i][j].rect))
			{
				miner.onGroundMineTest = true;
			};

			if (Collision(miner.leftMineTestRect, dirt[i][j].rect))
			{
				miner.leftMineTest = true;
			}

			if (Collision(miner.rightMineTestRect, dirt[i][j].rect))
			{
				miner.rightMineTest = true;
			}

			if (Collision(miner.bcrect, dirt[i][j].rect))
			{
				miner.bottomClip = true;
				miner.mineToPointDown = dirt[i][j].midpt;
			};

			if (Collision(miner.tcrect, dirt[i][j].rect))
			{
				miner.topClip = true;
			};

			if (Collision(miner.lcrect, dirt[i][j].rect))
			{
				miner.leftClip = true;
				miner.mineToPointLeft = dirt[i][j].midpt;
			};

			if (Collision(miner.rcrect, dirt[i][j].rect))
			{
				miner.rightClip = true;
				miner.mineToPointRight = dirt[i][j].midpt;
			};
		};

	if (Collision(miner.lcrect, leftBoundary.rect))
	{
		miner.leftClip = true;
	};

	if (Collision(miner.rcrect, rightBoundary.rect))
	{
		miner.rightClip = true;
	};



	//check for miner collision with store rects:
	if (Collision(refinery.storeRect, miner.rect))//Miner is at the refinery
	{
		if (!miner.wasAtStore)
		{
			miner.atStore = true;
			miner.atRefinery = true;
			miner.vx = 0;
			miner.vy = 0;//get rid of these soon!!
		}
		miner.wasAtStore = true;
	}
	else if (Collision(gasStation.storeRect, miner.rect))
	{
		if (!miner.wasAtStore)
		{
			//MessageBox(NULL, "", "", NULL);
			miner.atStore = true;
			miner.atGasStation = true;
			miner.vx = 0;
			miner.vy = 0;//get rid of these soon!!
		}
		miner.wasAtStore = true;
	}
	else if (Collision(upgradeStation.storeRect, miner.rect))
	{
		if (!miner.wasAtStore)
		{
			miner.atStore = true;
			miner.atUpgradeStation = true;
			miner.vx = 0;
			miner.vy = 0;//get rid of these soon!!
		}
		miner.wasAtStore = true;
	}
	else
	{
		miner.wasAtStore = false;
	};

	
	//get input from the store that's open:
	if (miner.atRefinery)
	{
		refinery.storeOpen = true;
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!refinery.Input(&mousePos))
			{
				miner.atStore = false;
				miner.atRefinery = false;
			};
		};
	}
	else if (miner.atGasStation)
	{
		gasStation.storeOpen = true;
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!gasStation.Input(&mousePos))
			{
				miner.atStore = false;
				miner.atGasStation = false;
			};
		};
	}
	else if (miner.atUpgradeStation)
	{
		upgradeStation.storeOpen = true;
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!upgradeStation.Input(&mousePos, &miner)) //pass miner here because upgrade station is very complex.
			{
				miner.atStore = false;
				miner.atUpgradeStation = false;
			};
		};
	};

	if (!miner.mining && !miner.atStore) //check if the miner is mining and if the miner is at the refinery
	{
		//clipping used to be here.
		
		miner.fuel = miner.fuel - miner.fuelUsageOnIdle; //need fo find a better way

		//move the miner with the keyboard (consider packing this into calcPos())
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			miner.move(0); //0 = left
		}
		else if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			miner.move(1); //1 = right
		}
		else
		{
			miner.moving_h = false;
		};

		if (GetKeyState(VK_UP) & 0x8000)//(Key_Down(DIK_UP)))
		{
			miner.move(2); //2 = up
		}
		else if (GetKeyState(VK_DOWN) & 0x8000)
		{
			miner.move(3); //3 = down
		}
		else
		{
			miner.moving_v = false;
		};

	}
	else if(miner.mining)//miner is mining
	{
		miner.minecycle += 1;

		miner.fuel = miner.fuel - miner.fuelUsageOnIdle;//need to find a better way.
		
		if (miner.minecycle >= (180/ (1 + (miner.drillUpgrade * .18))))//180 = 3 seconds
		{
			for (int i = 0; i < dirtWidth; i++)
				for (int j = 0; j < dirtHeight; j++)
				{
					if (Collision({ miner.rect.left + 20, miner.rect.top + 10, miner.rect.right - 20, miner.rect.bottom - 10 }, dirt[i][j].rect))
					{
						dirt[i][j].kill(&miner);
					}
				}
			miner.mining = false;
		}
	}
	miner.calcPos(&camera);
	
	
	//clear the backbuffer
	d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_XRGB(51, 204, 204));

	//camera.move(1, 1);

	//start rendering
	if (d3ddev->BeginScene())
	{
		

		for (int i = 0; i < dirtWidth; i++)
			for (int j = 0; j < dirtHeight; j++)
			{
				dirt[i][j].draw(camera);
			};
		

		//check for open windows and draw the miner/stores appropriately.
		if (refinery.storeOpen)
		{
			miner.draw(camera); //draw miner in background
			gasStation.draw(camera, &miner, &mousePos);
			upgradeStation.draw(camera, &miner, &mousePos);
			refinery.draw(camera, &miner, &mousePos);
			
		}
		else if(gasStation.storeOpen)
		{
			miner.draw(camera);
			refinery.draw(camera, &miner, &mousePos);
			upgradeStation.draw(camera, &miner, &mousePos);
			gasStation.draw(camera, &miner, &mousePos); //draw this last because it's open.
		}
		else if (upgradeStation.storeOpen)
		{
			miner.draw(camera);
			refinery.draw(camera, &miner, &mousePos);
			gasStation.draw(camera, &miner, &mousePos);
			upgradeStation.draw(camera, &miner, &mousePos);//draw this last because it's open.
		}
		else
		{
			refinery.draw(camera, &miner, &mousePos);
			gasStation.draw(camera, &miner, &mousePos);
			upgradeStation.draw(camera, &miner, &mousePos);
			miner.draw(camera);
		}
		
		//print debug info.
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
		if (miner.mining || !miner.mining)
		{
			FontPrint(fontArial, 60, 50, "Miner:", D3DCOLOR_XRGB(0, 0, 0));
			//camera pos:
			FontPrint(fontArial, 60, 70, "Coords:", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 60 + GetTextWidth("Coords: ", fontArial), 70, "(" + to_string((int)miner.x) + ", " + to_string((int)miner.y) + ")", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 60, 90, "Speed:", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 60 + GetTextWidth("Speed: ", fontArial), 90, "(" + to_string(miner.vx) + ", " + to_string(miner.vy) + ")", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 60, 110, "INV:", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 95, 110, to_string(miner.cargoAmount), D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 60, 130, "Money:", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 115, 130, to_string(miner.money), D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 60, 150, "Fuel:", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 105, 150, to_string(miner.fuel), D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, 60, 170, "Hitpoints:", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontArial, GetTextWidth("Hitpoints: ", fontArial) + 60, 170, to_string(miner.hitpoints), D3DCOLOR_XRGB(0, 0, 0));
			

		}
		spriteobj->End();

		leftBoundary.draw(camera, miner);
		rightBoundary.draw(camera, miner);
		//stop rendering
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
	//escape key exits
	if (GetKeyState(VK_ESCAPE) & 0x8000)
		gameover = true;

	if (miner.fuel <= 0)
	{
		MessageBox(NULL, "You ran out of fuel.", "YOU LOSE", NULL);
		gameover = true;
	}

	if (miner.hitpoints <= 0)
	{
		MessageBox(NULL, "You took too much damage.", "YOU LOSE", NULL);
		gameover = true;
	}
}
void Game_End()
{
	for (int i = 0; i < dirtWidth; i++)
		for (int j = 0; j < dirtHeight; j++)
		{
			dirt[i][j].end();
		};
	miner.end();
	
	if (fontArial) fontArial->Release();

	DirectInput_Shutdown();
	Direct3D_Shutdown();
}