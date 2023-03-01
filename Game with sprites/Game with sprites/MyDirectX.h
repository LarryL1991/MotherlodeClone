#pragma once
//header files
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <d3d9.h>
#include <d3d10.h>
#include <dxgi.h>
#include <d3dx9.h>
#include <dinput.h>
#include <xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
//libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
//program values
extern const string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern bool gameover;
//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
//Direct3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y,
	LPDIRECT3DSURFACE9 source);
//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4];
//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void XInput_Vibrate(int contNum = 0, int amount = 65535);
bool XInput_Controller_Found();
//game functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();
//Sprite objects
D3DXVECTOR2 GetBitmapSize(string filename);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(1, 0, 0));
extern LPD3DXSPRITE spriteobj;
extern bool myWindowFocus;
LPD3DXFONT MakeFont(
	string name,
	int size,
	int weight
	);
void FontPrint(
	LPD3DXFONT font,
	int x,
	int y,
	string text,
	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255)
	);

int GetTextWidth(const char *szText, LPD3DXFONT pFont);
int GetTextHeight(const char *szText, LPD3DXFONT pFont);
POINT GetRectCenter(RECT rect);
int GetRectWidth(RECT rect);
int GetRectHeight(RECT rect);


const string minerSpriteFile = "Resources\\miner.tga";
const string dirtSpriteFile = "Resources\\dirt.tga";
const string dirtCopper = "Resources\\dirtcopper.bmp";
const string dirtTin = "Resources\\dirttin.bmp";
const string dirtEmerald = "Resources\\dirtemerald.bmp";
const string dirtRuby = "Resources\\dirtruby.bmp";
const string dirtMithril = "Resources\\dirtmithril.bmp";
const string dirtDarkium = "Resources\\dirtdarkium.bmp";
const string dirtCyanium = "Resources\\dirtcyanium.bmp";
const string dirtGold = "Resources\\dirtgold.bmp";
const string dirtPlatinum = "Resources\\dirtplatinum.bmp";
const string dirtDiamond = "Resources\\dirtdiamond.bmp";
const string refinerySpriteFile = "Resources\\refinery.tga";
const string gasStationSpriteFile = "Resources\\gasstation.tga";
const string upgradeStationSpriteFile = "Resources\\upgradestation.tga";


struct CAMERA
{
	float xOffset;
	float yOffset;

	void move(float xAmt, float yAmt)
	{
		xOffset += xAmt;
		yOffset += yAmt;
	}
	
	CAMERA(float x, float y)
	{
		xOffset = x;
		yOffset = y;
	}
};

struct MINER
{
	float x, y;
	float vx = 0;
	float vy = 0;
	float max_hv = 4;
	float max_vv = 4;
	int height, width;
	D3DXVECTOR3 pos, mineToPointRight, mineToPointLeft, mineToPointDown, midpt;
	float accel_h = 1;//horizontal acceleration
	float accel_v = 1; //vertical acceleration
	bool moving_h = false;
	bool moving_v = false;
	bool mining = false;
	bool atStore = false;
	bool wasAtStore = false;
	bool atRefinery = false;
	bool atGasStation = false;
	bool atUpgradeStation = false;
	bool cargoBayFull = false;
	float fuelTankSize = 5;
	float fuel = 2; //default 2
	int drillUpgrade = 0;
	int hullUpgrade = 0;
	int engineUpgrade = 0;
	int fuelTankUpgrade = 0;
	int radiatorUpgrade = 0;
	int cargoBayUpgrade = 0;
	float fuelUsageOnMove = .001;
	float fuelUsageOnMine = .002;
	float fuelUsageOnIdle = .00001;
	int hullMaxHitpoints = 100;
	int hitpoints = 100;
	int cargoBaySize = 6;
	int minecycle = 0;
	int mineAttempt = 0;
	int minedirection;
	bool leftClip, topClip, rightClip, bottomClip, onGroundMineTest, leftMineTest, rightMineTest;
	RECT rect, lcrect, tcrect, rcrect, bcrect, onGroundMineTestRect, rightMineTestRect, leftMineTestRect;
	LPDIRECT3DTEXTURE9 minerSprite = NULL;
	int copper, iron, emerald, ruby, mithril, darkium, cyanium, gold, platinum, diamond;
	int cargoAmount = 0;
	float money = 20;
	bool drawHitboxes = false;
	LPDIRECT3DSURFACE9 hitbox = NULL;

	bool init()
	{
		width = 50;
		height = 32;

		pos.x = (SCREENW / 2);
		pos.y = (SCREENH / 2);

		x = pos.x;
		y = pos.y;

		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };

		/*
		lcrect = { (long)pos.x + 1, (long)pos.y + 5, (long)pos.x + 8, (long)pos.y + height - 5 }; //left clip box
		tcrect = { (long)pos.x + 3, (long)pos.y + 3, (long)pos.x + width - 5, (long)pos.y + 8 }; //top clip box
		rcrect = { (long)pos.x + width - 5, (long)pos.y + 5, (long)pos.x + width - 3, (long)pos.y + height - 5 }; //right clip box
		bcrect = { (long)pos.x + 3, (long)pos.y + height - 5, (long)pos.x + width - 5, (long)pos.y + height - 3 }; //bottom clip box
		*/

		HRESULT result = d3ddev->CreateOffscreenPlainSurface(
			10, //width of the surface
			10, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&hitbox, //pointer to the surface
			NULL); //reserved (always NULL)

		lcrect = { (long)pos.x + 1, (long)pos.y + 13, (long)pos.x + 8, (long)pos.y + height - 13 }; //left clip box
		tcrect = { (long)pos.x + 20, (long)pos.y + 3, (long)pos.x + width - 20, (long)pos.y + 8 }; //top clip box
		rcrect = { (long)pos.x + width - 8, (long)pos.y + 13, (long)pos.x + width - 1, (long)pos.y + height - 13 }; //right clip box
		bcrect = { (long)pos.x + 20, (long)pos.y + height - 5, (long)pos.x + width - 20, (long)pos.y + height - 0 }; //bottom clip box

		onGroundMineTestRect = { (long)pos.x + 20, (long)pos.y + height - 0, (long)pos.x + width - 20, (long)pos.y + height + 10 };
		leftMineTestRect = { (long)pos.x - 1, (long)pos.y + 13, (long)pos.x + 8, (long)pos.y + height - 13 };
		rightMineTestRect = { (long)pos.x + width - 8, (long)pos.y + 13, (long)pos.x + width + 1, (long)pos.y + height - 13 };

		midpt.x = (rect.right + rect.left) / 2;
		midpt.y = (rect.bottom + rect.top) / 2;

		//load alpha transparent image
		minerSprite = LoadTexture(minerSpriteFile);

		if (!minerSprite)
			return false;
		else
			return true;
	};

	void calcPos(CAMERA* cam) //That INSANE physics engine
	{
		float miningSpeed = .28 + (drillUpgrade * .05);
		//x = (x + vx) - cam->xOffset;
		if (!mining)
		{
			if (!moving_h)
			{
				vx = vx / 1.02; //simulates air resistance
				if (vx <= .02 && vx >= -.02)
				{
					vx = 0;
				}
			};

			if (bottomClip && vy >= 0) //stops the miner if the bottom clips?
			{
				if (vy == max_vv - .01)
				{
					hitpoints = hitpoints - (25 * vy);
				}
				else if (vy > max_vv / 1.75)
				{
					hitpoints = hitpoints - (20 * vy);
				}
				else if (vy > max_vv / 2.25)
				{
					hitpoints = hitpoints - (15 * vy);
				}
				vy = -.01; //ground pushes up a bit
			};

			if (leftClip && vx <= 0)
			{
				vx = .01;
			};

			if (rightClip && vx >= 0)
			{
				vx = -.01;
			};

			if (topClip && vy <= 0)
			{
				vy = .01;
			};




			if (vy < max_vv && !bottomClip)
			{
				vy = vy + (.03);
			}; //simulates gravity.

			if (!moving_v) //simulates air resistance (terminal velocity is 1.5 when max_vv = 10)
			{
				vy = vy / 1.01;
			};
		}
		else if(mining)
		{
			switch (minedirection)
			{
			case 0://left
			{
				fuel = fuel - fuelUsageOnMine;
					//mineToPointLeft
					if ((midpt.x) > (mineToPointLeft.x))
					{
						vx = -miningSpeed;
					}
					else
					{
						vx = miningSpeed;
					};
					if ((midpt.y) > (mineToPointLeft.y) + 8)
					{
						vy = -miningSpeed;
					}
					else
					{
						vy = miningSpeed;
					}
				break;
			}
			case 1://right
			{
				fuel = fuel - fuelUsageOnMine;
					//mineToPointRight
					if (midpt.x > mineToPointRight.x)
					{
						vx = -miningSpeed;
					}
					else
					{
						vx = miningSpeed;
					}
					if (midpt.y > mineToPointRight.y + 8)
					{
						vy = -miningSpeed;
					}
					else
					{
						vy = miningSpeed;
					}
				break;
			}
			case 3://down (can't mine up)
			{
				if (abs(midpt.x - mineToPointDown.x) < 30)
				{
					fuel = fuel - fuelUsageOnMine;
					//mineToPointDown
					if (midpt.x > (mineToPointDown.x))
					{
						vx = -miningSpeed;
					}
					else
					{
						vx = miningSpeed;
					}
					if (midpt.y > (mineToPointDown.y) + 8)
					{
						vy = -miningSpeed;
					}
					else
					{
						vy = miningSpeed;
					}
				}
				else
				{
					mining = false;
				}
				break;
			}
			}
		}

		//cam->move(vx, vy); //need to move the camera and x, y
		x = x + vx;
		y = y + vy;

		//pos.x = x;
		//pos.y = y;

		pos.x = (x - cam->xOffset); //+ vx;
		pos.y = (y - cam->yOffset); //+ vy;

		if (pos.y < 100)
		{
			cam->move(0, vy - .01);
			pos.y = pos.y + .01;
		}

		if (pos.y > SCREENH - 100)
		{
			cam->move(0, vy + .01);
			pos.y = pos.y - .01;
		}

		if (pos.x < 100)
		{
			cam->move(vx - .01, 0);
			pos.x = pos.x + .01;
		}

		if (pos.x > SCREENW - 100)
		{
			cam->move(vx + .01, 0);
			pos.x = pos.x - .01;
		}

		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
		lcrect = { (long)pos.x + 1, (long)pos.y + 13, (long)pos.x + 8, (long)pos.y + height - 13 }; //left clip box
		tcrect = { (long)pos.x + 20, (long)pos.y + 3, (long)pos.x + width - 20, (long)pos.y + 8 }; //top clip box
		rcrect = { (long)pos.x + width - 8, (long)pos.y + 13, (long)pos.x + width - 1, (long)pos.y + height - 13 }; //right clip box
		bcrect = { (long)pos.x + 20, (long)pos.y + height - 5, (long)pos.x + width - 20, (long)pos.y + height - 0 }; //bottom clip box
		onGroundMineTestRect = { (long)pos.x + 20, (long)pos.y + height - 0, (long)pos.x + width - 20, (long)pos.y + height + 10 };
		leftMineTestRect = { (long)pos.x - 1, (long)pos.y + 13, (long)pos.x + 8, (long)pos.y + height - 13 };
		rightMineTestRect = { (long)pos.x + width - 8, (long)pos.y + 13, (long)pos.x + width + 1, (long)pos.y + height - 13 };
		midpt.x = pos.x + (width/2);
		midpt.y = pos.y + (height/2);

	};

	void mine(int direction)
	{
		minecycle = 0;
		minedirection = direction;
		mining = true;
	}

	void move(int direction)
	{
		switch (direction)
		{
		case 0: //left
		{
			if (vx < .3 && vx > -.3 && vy < .3 && vy > -.3 && leftMineTest && onGroundMineTest && abs(midpt.x - mineToPointLeft.x) > 10)//only mine if you're on the ground.
			{
				if (mineAttempt >= 3) //3 frames of attempting to mine before you can mine
				{
					mineAttempt = 0;
					mine(direction);
				}
				else
				{
					mineAttempt += 1;
				}
			}
			else if (vx <= -(max_hv))
			{
				vx = -max_hv;
			}
			else
			{
				vx = vx - (.05 * accel_h);
				mineAttempt = 0;
				moving_h = true;
			};
			fuel = fuel - fuelUsageOnMove;
			break;
		}
		case 1: //right
		{
			if (vx < .3 && vx > -.3 && vy < .3 && vy > -.3 && rightMineTest && onGroundMineTest && abs(midpt.x - mineToPointRight.x) > 10)//only mine if you're on the ground.
			{
				if (mineAttempt >= 3) //3 frames of attempting to mine before you can mine
				{
					mineAttempt = 0;
					mine(direction);
				}
				else
				{
					mineAttempt += 1;
				}
			}
			else if (vx >= max_hv)
			{
				vx = max_hv;
			}
			else
			{
				vx = vx + (.05 * accel_h);
				mineAttempt = 0;
				moving_h = true;
			};
			fuel = fuel - fuelUsageOnMove;
			break;
		}
		case 2://up (can't mine upwards...)
		{
			mineAttempt = 0; //you're not trying to mine if you're pushing up.
			if (vy <= -(max_vv))
			{
				vy = -max_vv;
			}
			else
			{
				vy = vy - (.05 * accel_v);
				moving_v = true;
			};
			fuel = fuel - fuelUsageOnMove;
			break;
		}
		case 3://down
		{
			if (vx < .3 && vx > -.3 && vy < .3 && vy > -.3 && onGroundMineTest && abs(midpt.y - mineToPointDown.y) > 10)
			{
				if (mineAttempt >= 3) //3 frames of attempting to mine before you can mine
				{
					mineAttempt = 0;
					mine(direction);
				}
				else
				{
					//MessageBox(NULL, "", "", NULL);
					mineAttempt += 1;
				}
			}
			else if (vy >= max_vv)
			{
				vy = max_vv;
			}
			else
			{
				if (!onGroundMineTest)
				{
					vy = vy + (.05 * accel_v);
					mineAttempt = 0;
					moving_v = true;
				};
			};
			fuel = fuel - fuelUsageOnMove;
			break;
		};
		};
	};

	void addInv(int typeOfItem, int amount)
	{
		/*
		1 = NOT USED
		2 = Copper
		3 = Iron
		4 = Emerald
		5 = Ruby
		6 = Mithril
		7 = Darkium
		8 = Cyanium
		9 = Gold
		10 = Platinum
		11 = Diamond
		NOT IMPLEMENTED YET:
		12 = Reserve Fuel Tank
		13 = Nanobots (repair drones)
		14 = Dynamite
		15 = C4
		16 = Cheap Teleporter
		17 = Good Teleporter
		*/
		cargoAmount = copper + iron + emerald + ruby + mithril + darkium + cyanium + gold + platinum + diamond;
		if (cargoAmount >= cargoBaySize)
			cargoBayFull = true;
		else
			cargoBayFull = false;
		if (!cargoBayFull)
		{
			while (cargoBaySize < cargoAmount + amount)
			{
				amount = amount - 1; //fit it in to your cargo by dropping 1 at a time.
			}
			//add it to cargo
			switch (typeOfItem)
			{
			case 2:
			{
				copper += amount;
				break;
			};
			case 3:
			{
				iron += amount;
				break;
			};
			case 4:
			{
				emerald += amount;
				break;
			};
			case 5:
			{
				ruby += amount;
				break;
			};
			case 6:
			{
				mithril += amount;
				break;
			};
			case 7:
			{
				darkium += amount;
				break;
			};
			case 8:
			{
				cyanium += amount;
				break;
			};
			case 9:
			{
				gold += amount;
				break;
			};
			case 10:
			{
				platinum += amount;
				break;
			};
			case 11:
			{
				diamond += amount;
				break;
			};
			};
			cargoAmount = copper + iron + emerald + ruby + mithril + darkium + cyanium + gold + platinum + diamond;
			if (cargoBaySize <= cargoAmount)
				cargoBayFull = true;
		}
		else
		{
			//display message.
		};
	}

	void draw(CAMERA cam)
	{
		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//draw the sprite

		spriteobj->Draw(minerSprite, NULL, NULL, &pos,
			D3DCOLOR_XRGB(255, 255, 255));

		//stop drawing
		spriteobj->End();

		if (drawHitboxes)
		{
			d3ddev->ColorFill(hitbox, NULL, D3DCOLOR_XRGB(0, 0, 0));
			d3ddev->StretchRect(hitbox, NULL, backbuffer, &rect, D3DTEXF_NONE);

			d3ddev->ColorFill(hitbox, NULL, D3DCOLOR_XRGB(255, 255, 255));
			d3ddev->StretchRect(hitbox, NULL, backbuffer, &lcrect, D3DTEXF_NONE);

			d3ddev->ColorFill(hitbox, NULL, D3DCOLOR_XRGB(255, 255, 255));
			d3ddev->StretchRect(hitbox, NULL, backbuffer, &rcrect, D3DTEXF_NONE);

			d3ddev->ColorFill(hitbox, NULL, D3DCOLOR_XRGB(255, 255, 255));
			d3ddev->StretchRect(hitbox, NULL, backbuffer, &bcrect, D3DTEXF_NONE);

			d3ddev->ColorFill(hitbox, NULL, D3DCOLOR_XRGB(255, 255, 255));
			d3ddev->StretchRect(hitbox, NULL, backbuffer, &tcrect, D3DTEXF_NONE);
		}
	};

	void ApplyUpgrades()
	{
		fuelTankSize = 5 + (3 * fuelTankUpgrade);
		fuel = fuelTankSize; //free fuel on any upgrade...
		cargoBaySize = 6 + (4 * cargoBayUpgrade);

		max_hv = 4 + (engineUpgrade * .1);
		max_vv = 4 + (engineUpgrade * .1);

		accel_h = 1 + (engineUpgrade * .1);//horizontal acceleration
		accel_v = 1 + (engineUpgrade * .1); //vertical acceleration

		hullMaxHitpoints = 100 + ((hullUpgrade * 1.5) * 100);
		hitpoints = hullMaxHitpoints; //free hull repair on any upgrade...
	}

	void end()
	{
		minerSprite->Release();
	};
};

struct DIRT
{
	float x, y;
	D3DXVECTOR3 pos, midpt;
	LPDIRECT3DTEXTURE9 dirtSprite = NULL;
	int height, width;
	RECT rect;
	bool active;
	int myRarity = 1;

	bool init(int posx, int posy, int rarity)
	{
		width = 50;
		height = 50;
		x = posx;
		y = posy;

		rect.left = x;
		rect.top = y;
		rect.right = x + width;
		rect.bottom = y + height;

		midpt.x = (rect.right + rect.left) / 2;
		midpt.y = (rect.bottom + rect.top) / 2;

		myRarity = rarity;

		switch (rarity)
		{
		case 1:
		{
			dirtSprite = LoadTexture(dirtSpriteFile);
			if (!dirtSprite)
				return false;
			break;
		}
		case 2:
		{
			dirtSprite = LoadTexture(dirtCopper);
			if (!dirtSprite)
				return false;
			break;
		}
		case 3:
		{
			dirtSprite = LoadTexture(dirtTin);
			if (!dirtSprite)
				return false;
			break;
		};
		case 4:
		{
			dirtSprite = LoadTexture(dirtEmerald);
			if (!dirtSprite)
				return false;
			break;
		}
		case 5:
		{
			dirtSprite = LoadTexture(dirtRuby);
			if (!dirtSprite)
				return false;
			break;
		}
		case 6:
		{
			dirtSprite = LoadTexture(dirtMithril);
			if (!dirtSprite)
				return false;
			break;
		}
		case 7:
		{
			dirtSprite = LoadTexture(dirtDarkium);
			if (!dirtSprite)
				return false;
			break;
		}
		case 8:
		{
			dirtSprite = LoadTexture(dirtCyanium);
			if (!dirtSprite)
				return false;
			break;
		}
		case 9:
		{
			dirtSprite = LoadTexture(dirtGold);
			if (!dirtSprite)
				return false;
			break;
		}
		case 10:
		{
			dirtSprite = LoadTexture(dirtPlatinum);
			if (!dirtSprite)
				return false;
			break;
		}
		case 11:
		{
			dirtSprite = LoadTexture(dirtDiamond);
			if (!dirtSprite)
				return false;
			break;
		}
		}


		return true;
	};

	void end()
	{
		if (dirtSprite != NULL)
		{
			dirtSprite->Release();
		}
	}

	void draw(CAMERA cam)
	{
		if (active)
		{
			if((x > cam.xOffset - 60) && (x < (cam.xOffset + (SCREENW + 15)))) //only calculate location if you're near the screen in the x coord
			{
				if ((y < (cam.yOffset + (SCREENH + 15))) && (y > cam.yOffset - 60)) //only calculate location if you're near the screen in the y coord
				{
					pos.x = (x - cam.xOffset);
					pos.y = (y - cam.yOffset);
					rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
					midpt.x = ((rect.right + rect.left) / 2);
					midpt.y = ((rect.bottom + rect.top) / 2);
				}
			}
			

			if ((x > cam.xOffset - 55) && (x < (cam.xOffset + (SCREENW + 10)))) //((pos.x > 0 && pos.x < SCREENW - 55)) //check position on the screen in x coord to see if it should be drawn
			{
				if ((y < (cam.yOffset + (SCREENH + 10))) && (y > cam.yOffset - 55)) //(pos.y > (0 - 5) && pos.y < SCREENH - 55) //check position on the screen in y coord to see if it should be drawn
				{
				//start drawing
				spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

				//draw the sprite
				
				spriteobj->Draw(dirtSprite, NULL, NULL, &pos, D3DCOLOR_XRGB(255, 255, 255));

				//stop drawing
				spriteobj->End();
				};
			};
		}
		else
		{
			if (pos.x != -1)
			{
				pos.x = -1;
				pos.y = -1;
				rect.bottom = -1;
				rect.left = -1;
				rect.top = -1;
				rect.right = -1;
				end();
			};
		};
	};

	void kill(MINER* miner)
	{
		miner->addInv(myRarity, 1);
		active = false;//kill dirt
	};

};

struct REFINERY
{
	RECT rect, storeRect, bgRect, closeRect, sellRect, storeWindowRect, headerRect, oreRect[11]; //storeRect is the collision that opens the store.
	D3DXVECTOR3 pos;
	LPDIRECT3DTEXTURE9 refinerySprite = NULL;
	LPDIRECT3DSURFACE9 background = NULL, xButton = NULL;
	int height, width;
	LPD3DXFONT fontStoreOre = NULL, fontStoreSellSm = NULL, fontStoreHeader = NULL;
	//bool atStore;
	float x, y;
	bool storeOpen = false;
	const int copperVal = 50;
	const int ironVal = 100;
	const int emeraldVal = 250;
	const int rubyVal = 500;
	const int mithrilVal = 1000;
	const int darkiumVal = 2000;
	const int cyaniumVal = 4000;
	const int goldVal = 8000;
	const int platinumVal = 15000;
	const int diamondVal = 30000;
	bool sellAll = false;
	
	bool init(int placex, int placey)
	{
		width = 100;
		height = 50;

		x = placex;
		y = placey;

		pos.x = x;
		pos.y = y;

		fontStoreOre = MakeFont("Arial", 20, 0);
		fontStoreHeader = MakeFont("Arial", 20, 600);
		
		fontStoreSellSm = MakeFont("Arial", 40, 1000);
		//fontStoreSellLg = MakeFont("Arial", 40, 800);

		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
		storeRect = { (long)pos.x + 11, (long)pos.y + 30, (long)pos.x + 19, (long)pos.y + 19 };

		//bgRect is the background of the menu.
		bgRect = { 100, 100, SCREENW - 100,  SCREENH - 100 };

		//closeRect is the close button rect
		closeRect = { SCREENW - 125, 105, SCREENW - 105, 125 };
		
		//sellRect is the sell all button rect
		sellRect = { SCREENW - 300, SCREENH - 200, SCREENW - 125, SCREENH - 125 };

		//storeWindowRect is the dark area of the store.
		storeWindowRect = { 150, 160, SCREENW - 150, SCREENH - 230 };

		//headerRect is the lighter-colored portion of the store window at the top
		headerRect = { storeWindowRect.left + 2, storeWindowRect.top + 2, storeWindowRect.right - 2, storeWindowRect.top + 21 };

		//oreRect[] contains ore locations in the menu
		for (int i = 1; i <= 10; i++)
		{
			oreRect[i] = {storeWindowRect.left + 1, headerRect.bottom + 4 + ((i - 1) * 22), storeWindowRect.right - 1, headerRect.bottom + (i * 22) + 4};
		}

		//load alpha transparent image
		refinerySprite = LoadTexture(refinerySpriteFile);

		HRESULT result = d3ddev->CreateOffscreenPlainSurface(
			&bgRect.right - &bgRect.left, //width of the surface
			&bgRect.bottom - &bgRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&background, //pointer to the surface
			NULL); //reserved (always NULL)
			
		result = d3ddev->CreateOffscreenPlainSurface(
			&closeRect.right - &closeRect.left, //width of the surface
			&closeRect.bottom - &closeRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&xButton, //pointer to the surface
			NULL); //reserved (always NULL)
		/*
		result = d3ddev->CreateOffscreenPlainSurface(
			&sellRect.right - &sellRect.left, //width of the surface
			&sellRect.bottom - &sellRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&sellButton, //pointer to the surface
			NULL); //reserved (always NULL)

		result = d3ddev->CreateOffscreenPlainSurface(
			&storeWindowRect.right - &storeWindowRect.left, //width of the surface
			&storeWindowRect.bottom - &storeWindowRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&storeWindow, //pointer to the surface
			NULL); //reserved (always NULL)*/

		if (!refinerySprite)
			return false;
		else
			return true;
	};

	bool MouseInRect(RECT* myrect, POINT* mousePos)
	{
		if(mousePos->x > myrect->left && mousePos->x < myrect->right && mousePos->y > myrect->top && mousePos->y < myrect->bottom)
			return true;
		return false;
	}

	void draw(CAMERA cam, MINER* aMiner, POINT* mousePos)
	{
		pos.x = (x - cam.xOffset);
		pos.y = (y - cam.yOffset);
		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
		storeRect = { (long)pos.x + 11, (long)pos.y + 30, (long)pos.x + 19 + 11, (long)pos.y + 19 + 30 };

		if (sellAll)
		{
			aMiner->money = aMiner->money + (aMiner->copper * copperVal);
			aMiner->copper = 0;

			aMiner->money = aMiner->money + (aMiner->iron * ironVal);
			aMiner->iron = 0;

			aMiner->money = aMiner->money + (aMiner->emerald * emeraldVal);
			aMiner->emerald = 0;

			aMiner->money = aMiner->money + (aMiner->ruby * rubyVal);
			aMiner->ruby = 0;

			aMiner->money = aMiner->money + (aMiner->mithril * mithrilVal);
			aMiner->mithril = 0;

			aMiner->money = aMiner->money + (aMiner->darkium * darkiumVal);
			aMiner->darkium = 0;

			aMiner->money = aMiner->money + (aMiner->cyanium * cyaniumVal);
			aMiner->cyanium = 0;

			aMiner->money = aMiner->money + (aMiner->gold * goldVal);
			aMiner->gold = 0;

			aMiner->money = aMiner->money + (aMiner->platinum * platinumVal);
			aMiner->platinum = 0;

			aMiner->money = aMiner->money + (aMiner->diamond * diamondVal);
			aMiner->diamond = 0;

			aMiner->cargoAmount = aMiner->copper + aMiner->iron + aMiner->emerald + aMiner->ruby + aMiner->mithril + aMiner->darkium + aMiner->cyanium + aMiner->gold + aMiner->platinum + aMiner->diamond;

			sellAll = false;
		}

		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//draw the sprite

		spriteobj->Draw(refinerySprite, NULL, NULL, &pos,
			D3DCOLOR_XRGB(255, 255, 255));


		//stop drawing
		spriteobj->End();

		if (storeOpen)
		{
			if (!d3ddev)
			{
				MessageBox(NULL, "d3dev bad", "", NULL);
			};
			
			//draw store button:
			d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(50, 50, 50));
			d3ddev->StretchRect(background, NULL, backbuffer, &bgRect, D3DTEXF_NONE);

			//draw close button:
			if(MouseInRect(&closeRect, mousePos))
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(255, 0, 0));
			}
			else
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(160, 0, 0));
			};
			d3ddev->StretchRect(xButton, NULL, backbuffer, &closeRect, D3DTEXF_NONE);
			


			//draw sell all button:
			if (MouseInRect(&sellRect, mousePos))
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(0, 205, 0));
			}
			else
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(0, 150, 0));
			}
			d3ddev->StretchRect(xButton, NULL, backbuffer, &sellRect, D3DTEXF_NONE);

			//draw store window:
			d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(25, 25, 25));
			d3ddev->StretchRect(xButton, NULL, backbuffer, &storeWindowRect, D3DTEXF_NONE);

			//draw header:
			d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(100, 100, 100));
			d3ddev->StretchRect(background, NULL, backbuffer, &headerRect, D3DTEXF_NONE);

			//list inventory contents:
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

			FontPrint(fontStoreOre, closeRect.left + 6, closeRect.top - 1, "x", D3DCOLOR_XRGB(0, 0, 0)); //should put an x down.
			
			//print text to screen:
			FontPrint(fontStoreHeader, storeWindowRect.left + 5, storeWindowRect.top + 2, "Ore Type:", D3DCOLOR_XRGB(255, 255, 255));
			FontPrint(fontStoreHeader, storeWindowRect.left + 200, storeWindowRect.top + 2, "Amount:", D3DCOLOR_XRGB(255, 255, 255));
			FontPrint(fontStoreHeader, storeWindowRect.left + 400, storeWindowRect.top + 2, "Value:", D3DCOLOR_XRGB(255, 255, 255));
			FontPrint(fontStoreHeader, storeWindowRect.left + 600, storeWindowRect.top + 2, "Total:", D3DCOLOR_XRGB(255, 255, 255));

			//sell button:
			//FontPrint(fontStoreSellLg, sellRect.left + 8, sellRect.top + 5, "SELL ALL", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontStoreSellSm, sellRect.left + 8, ((sellRect.top + sellRect.bottom) / 2) - (40/2), "SELL ALL", D3DCOLOR_XRGB(255, 255, 255));


			for (int i = 1; i <= 10; i++)
			{
				switch (i)
				{
				case 1:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(20, 20, 20));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Copper:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->copper), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(copperVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(copperVal * aMiner->copper), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 2:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(40, 40, 40));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Iron:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->iron), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(ironVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(ironVal * aMiner->iron), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 3:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(20, 20, 20));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Emerald:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->emerald), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(emeraldVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(emeraldVal * aMiner->emerald), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 4:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(40, 40, 40));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Ruby:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->ruby), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(rubyVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(rubyVal * aMiner->ruby), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 5:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(20, 20, 20));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Mithril:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->mithril), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(mithrilVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(mithrilVal * aMiner->mithril), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 6:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(40, 40, 40));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Darkium:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->darkium), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(darkiumVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(darkiumVal * aMiner->darkium), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 7:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(20, 20, 20));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Cyanium:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->cyanium), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(cyaniumVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(cyaniumVal * aMiner->cyanium), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 8:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(40, 40, 40));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Gold:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->gold), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(goldVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(goldVal * aMiner->gold), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 9:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(20, 20, 20));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Platinum:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->platinum), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(platinumVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(platinumVal * aMiner->platinum), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				case 10:
				{
					d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(40, 40, 40));
					d3ddev->StretchRect(background, NULL, backbuffer, &oreRect[i], D3DTEXF_NONE);
					FontPrint(fontStoreOre, storeWindowRect.left + 5, storeWindowRect.top + 5 + (22 * i), "Diamond:", D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 200, storeWindowRect.top + 5 + (22 * i), to_string(aMiner->diamond), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 400, storeWindowRect.top + 5 + (22 * i), to_string(diamondVal), D3DCOLOR_XRGB(255, 255, 255));
					FontPrint(fontStoreOre, storeWindowRect.left + 600, storeWindowRect.top + 5 + (22 * i), to_string(diamondVal * aMiner->diamond), D3DCOLOR_XRGB(255, 255, 255));
					break;
				}
				}
			}


			spriteobj->End();
		};

		spriteobj->End();
	}



	BOOL Input(POINT* mousePos)
	{
		if (MouseInRect(&closeRect, mousePos))
		{
			storeOpen = false;
			return false;
		}

		if (MouseInRect(&sellRect, mousePos))
		{
			sellAll = true;
			storeOpen = false;
			return false;
		}

		return true;
	}

};

struct GASSTATION
{
	RECT rect, storeRect, bgRect, closeRect, storeWindowRect, oneGalRect, twoGalRect, fiveGalRect, fillRect, acceptRect; //storeRect is the collision that opens the store.
	D3DXVECTOR3 pos;
	LPDIRECT3DTEXTURE9 gasStationSprite = NULL;
	LPDIRECT3DSURFACE9 background = NULL, xButton = NULL;
	int height, width;
	LPD3DXFONT fontStoreGas = NULL, fontStoreHeader = NULL, fontStoreAccept = NULL;
	float x, y;
	bool storeOpen = false;
	bool oneGalClicked, twoGalClicked, fiveGalClicked, fillClicked, accept;
	float pricePerGallon = 2.95;

	bool init(int placex, int placey)
	{
		width = 100;
		height = 50;

		x = placex;
		y = placey;

		pos.x = x;
		pos.y = y;

		fontStoreGas = MakeFont("Arial", 20, 0);
		fontStoreHeader = MakeFont("Arial", 40, 1600);
		fontStoreAccept = MakeFont("Arial", 40, 1000);

		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
		storeRect = { (long)pos.x + 11, (long)pos.y + 30, (long)pos.x + 19, (long)pos.y + 19 };

		//bgRect is the background of the menu.
		bgRect = { 100, 100, SCREENW - 100,  SCREENH - 100 };

		//closeRect is the close button rect
		closeRect = { SCREENW - 125, 105, SCREENW - 105, 125 };

		//storeWindowRect is the dark area of the store.
		storeWindowRect = { 150, 160, SCREENW - 150, SCREENH - 230 };

		//oneGalRect is for 1 gallon of gas
		oneGalRect = {storeWindowRect.left + 10, storeWindowRect.top + 10, ((storeWindowRect.left + storeWindowRect.right)/2) - 5, ((storeWindowRect.top + storeWindowRect.bottom)/2) - 5};

		//twoGalRect is for 2 gallons of gas
		twoGalRect = { ((storeWindowRect.left + storeWindowRect.right) / 2) + 5, storeWindowRect.top + 10, storeWindowRect.right - 10, ((storeWindowRect.top + storeWindowRect.bottom) / 2) - 5 };

		//fiveGalRect is for 5 gallons of gas
		fiveGalRect = { storeWindowRect.left + 10, ((storeWindowRect.top + storeWindowRect.bottom) / 2) + 5, ((storeWindowRect.left + storeWindowRect.right) / 2) - 5, storeWindowRect.bottom - 10 };

		//fillRect is to fill tank
		fillRect = { ((storeWindowRect.left + storeWindowRect.right) / 2) + 5, ((storeWindowRect.top + storeWindowRect.bottom) / 2) + 5, storeWindowRect.right - 10, storeWindowRect.bottom - 10 };

		//acceptRect is the sell all button rect
		acceptRect = { SCREENW - 300, SCREENH - 200, SCREENW - 125, SCREENH - 125 }; //see "REFINERY sellRect"

		//load alpha transparent image
		gasStationSprite = LoadTexture(gasStationSpriteFile);

		HRESULT result = d3ddev->CreateOffscreenPlainSurface(
			&bgRect.right - &bgRect.left, //width of the surface
			&bgRect.bottom - &bgRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&background, //pointer to the surface
			NULL); //reserved (always NULL)

		result = d3ddev->CreateOffscreenPlainSurface(
			&closeRect.right - &closeRect.left, //width of the surface
			&closeRect.bottom - &closeRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&xButton, //pointer to the surface
			NULL); //reserved (always NULL)
		

		if (!gasStationSprite)
			return false;
		else
			return true;
	};

	bool MouseInRect(RECT* myrect, POINT* mousePos)
	{
		if (mousePos->x > myrect->left && mousePos->x < myrect->right && mousePos->y > myrect->top && mousePos->y < myrect->bottom)
			return true;
		return false;
	}

	void draw(CAMERA cam, MINER* aMiner, POINT* mousePos)
	{
		pos.x = (x - cam.xOffset);
		pos.y = (y - cam.yOffset);
		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
		storeRect = { (long)pos.x + 45, (long)pos.y + 30, (long)pos.x + 55, (long)pos.y + 19 + 30 };

		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//draw the sprite

		spriteobj->Draw(gasStationSprite, NULL, NULL, &pos,
			D3DCOLOR_XRGB(255, 255, 255));

		//stop drawing
		spriteobj->End();

		if (accept)
		{
			if (oneGalClicked)
			{
				if (aMiner->fuel + 1 < aMiner->fuelTankSize)
				{
					aMiner->money = aMiner->money - (1 * pricePerGallon);
					aMiner->fuel = aMiner->fuel + 1;
				}
				else
					fillClicked = true;
			}
			else if (twoGalClicked)
			{
				if (aMiner->fuel + 2 < aMiner->fuelTankSize)
				{
					aMiner->money = aMiner->money - (2 * pricePerGallon);
					aMiner->fuel = aMiner->fuel + 2;
				}
				else
					fillClicked = true;
			}
			else if (fiveGalClicked)
			{
				if (aMiner->fuel + 5 < aMiner->fuelTankSize)
				{
					aMiner->money = aMiner->money - (5 * pricePerGallon);
					aMiner->fuel = aMiner->fuel + 5;
				}
				else
					fillClicked = true;
			} 
			
			if (fillClicked)
			{
				aMiner->money = aMiner->money - ((aMiner->fuelTankSize - aMiner->fuel) * pricePerGallon);
				aMiner->fuel = aMiner->fuelTankSize;
			};

			accept = false;
			oneGalClicked = false;
			twoGalClicked = false;
			fiveGalClicked = false;
			fillClicked = false;
		}

		if (storeOpen)
		{
			if (!d3ddev)
			{
				MessageBox(NULL, "d3dev bad", "", NULL);
			};

			//draw store button:
			d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(50, 50, 50));
			d3ddev->StretchRect(background, NULL, backbuffer, &bgRect, D3DTEXF_NONE);

			//draw close button:
			if (MouseInRect(&closeRect, mousePos))
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(255, 0, 0));
			}
			else
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(160, 0, 0));
			};
			d3ddev->StretchRect(xButton, NULL, backbuffer, &closeRect, D3DTEXF_NONE);

			//draw store window:
			d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(25, 25, 25));
			d3ddev->StretchRect(xButton, NULL, backbuffer, &storeWindowRect, D3DTEXF_NONE);

			//draw accept button:
			if (MouseInRect(&acceptRect, mousePos))
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(0, 205, 0));
			}
			else
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(0, 150, 0));
			}
			d3ddev->StretchRect(xButton, NULL, backbuffer, &acceptRect, D3DTEXF_NONE);

			//oneGalRect
			if (oneGalClicked)
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(50, 180, 50));
			}
			else if (MouseInRect(&oneGalRect, mousePos))
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(150, 150, 150));
			}
			else
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(120, 120, 120));
			}
			d3ddev->StretchRect(background, NULL, backbuffer, &oneGalRect, D3DTEXF_NONE);

			//twoGalRect
			if (twoGalClicked)
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(50, 180, 50));
			}
			else if (MouseInRect(&twoGalRect, mousePos))
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(150, 150, 150));
			}
			else
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(120, 120, 120));
			}
			d3ddev->StretchRect(background, NULL, backbuffer, &twoGalRect, D3DTEXF_NONE);

			//fiveGalRect
			if (fiveGalClicked)
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(50, 180, 50));
			}
			else if (MouseInRect(&fiveGalRect, mousePos))
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(150, 150, 150));
			}
			else
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(120, 120, 120));
			}
			d3ddev->StretchRect(background, NULL, backbuffer, &fiveGalRect, D3DTEXF_NONE);

			//fillRect
			if (fillClicked)
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(50, 180, 50));
			}
			else if (MouseInRect(&fillRect, mousePos))
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(150, 150, 150));
			}
			else
			{
				d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(120, 120, 120));
			}
			d3ddev->StretchRect(background, NULL, backbuffer, &fillRect, D3DTEXF_NONE);

			//list inventory contents:
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

			FontPrint(fontStoreAccept, acceptRect.left + 17, ((acceptRect.top + acceptRect.bottom) / 2) - (40 / 2), "ACCEPT", D3DCOLOR_XRGB(255, 255, 255));

			FontPrint(fontStoreGas, closeRect.left + 6, closeRect.top - 1, "x", D3DCOLOR_XRGB(0, 0, 0)); //should put an x down.

			FontPrint(fontStoreHeader, ((bgRect.left + bgRect.right)/2) - 100 , bgRect.top + 10, "GAS Station", D3DCOLOR_XRGB(50, 210, 50));

			FontPrint(fontStoreHeader, ((oneGalRect.left + oneGalRect.right) / 2) - 120, ((oneGalRect.top + oneGalRect.bottom)/2) - 20, "Buy one gallon", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontStoreHeader, ((twoGalRect.left + twoGalRect.right) / 2) - 130, ((twoGalRect.top + twoGalRect.bottom) / 2) - 20, "Buy two gallons", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontStoreHeader, ((fiveGalRect.left + fiveGalRect.right) / 2) - 130, ((fiveGalRect.top + fiveGalRect.bottom) / 2) - 20, "Buy five gallons", D3DCOLOR_XRGB(0, 0, 0));
			FontPrint(fontStoreHeader, ((fillRect.left + fillRect.right) / 2) - 60, ((fillRect.top + fillRect.bottom) / 2) - 20, "Fill tank", D3DCOLOR_XRGB(0, 0, 0));

			spriteobj->End();
		};

	}



	BOOL Input(POINT* mousePos)
	{
		if (MouseInRect(&closeRect, mousePos))
		{
			storeOpen = false;
			return false;
		}

		if (MouseInRect(&oneGalRect, mousePos))
		{
			oneGalClicked = true;
			twoGalClicked = false;
			fiveGalClicked = false;
			fillClicked = false;
		}

		if (MouseInRect(&twoGalRect, mousePos))
		{
			oneGalClicked = false;
			twoGalClicked = true;
			fiveGalClicked = false;
			fillClicked = false;
		}

		if (MouseInRect(&fiveGalRect, mousePos))
		{
			oneGalClicked = false;
			twoGalClicked = false;
			fiveGalClicked = true;
			fillClicked = false;
		}

		if (MouseInRect(&fillRect, mousePos))
		{
			oneGalClicked = false;
			twoGalClicked = false;
			fiveGalClicked = false;
			fillClicked = true;
		}

		if (MouseInRect(&acceptRect, mousePos))
		{
			accept = true;
			storeOpen = false;
			return false;
		}

		return true;
	}
};

struct UPGRADE
{
	RECT rect, storeRect, bgRect, closeRect, storeWindowRect, drillTabRect, hullTabRect, engineTabRect, 
		fuelTankTabRect, radiatorTabRect, cargoBayTabRect, tierOneRect, tierTwoRect, tierThreeRect,
		tierFourRect, tierFiveRect, tierSixRect, leftPaneRect, confirmUpgradeRect; //storeRect is the collision that opens the store.
	D3DXVECTOR3 pos;
	LPDIRECT3DTEXTURE9 upgradeStationSprite = NULL;
	LPDIRECT3DSURFACE9 background = NULL, xButton = NULL;
	int height, width;
	LPD3DXFONT fontStoreUpgrade = NULL, fontStoreHeader = NULL, fontStoreAccept = NULL, fontLeftPane = NULL;
	float x, y;
	bool storeOpen = false;
	bool confirm = false;
	int tabSelection = 0;
	int tierSelection = 0;
	D3DXCOLOR card = D3DCOLOR_XRGB(160, 70, 70);
	D3DXCOLOR cardHover = D3DCOLOR_XRGB(180, 90, 90);
	D3DXCOLOR cardSelected = D3DCOLOR_XRGB(70, 160, 70);
	D3DXCOLOR cardObsolete = D3DCOLOR_XRGB(100, 50, 50);
	string upgradeText[7][7];
	int upgradePrice[7];
	bool enoughMoney = true; //only used for displaying a message.
	bool saleMade = false; //only used for displaying a message.
	int frames;


	bool init(int placex, int placey)
	{
		//initialize drill upgrade texts:
		upgradeText[1][0] = "Wood Drill"; //[upgradetype][upgradelevel]
		upgradeText[1][1] = "Steel Drill";
		upgradeText[1][2] = "Emerald Drill";
		upgradeText[1][3] = "Mithril Drill";
		upgradeText[1][4] = "Darkium Drill";
		upgradeText[1][5] = "Cyanium Drill";
		upgradeText[1][6] = "Diamond Drill";

		//initialize hull upgrade texts:
		upgradeText[2][0] = "Wood Hull";
		upgradeText[2][1] = "Steel Hull";
		upgradeText[2][2] = "Emerald Hull";
		upgradeText[2][3] = "Mithril Hull";
		upgradeText[2][4] = "Darkium Hull";
		upgradeText[2][5] = "Cyanium Hull";
		upgradeText[2][6] = "Diamond Hull";

		//initialize engine upgrade texts:
		upgradeText[3][0] = "2-Cylinder\n  Engine";
		upgradeText[3][1] = "4-Cylinder\n   Engine";
		upgradeText[3][2] = "6-Cylinder\n   Engine";
		upgradeText[3][3] = "8-Cylinder\n  Engine";
		upgradeText[3][4] = "10-Cylinder\n    Engine";
		upgradeText[3][5] = "12-Cylinder\n    Engine";
		upgradeText[3][6] = "Jet Engine";

		//initialize fuel tank upgrade texts:
		upgradeText[4][0] = "Tin Can Tank";
		upgradeText[4][1] = "Tiny Tank";
		upgradeText[4][2] = "Tupperware Tank";
		upgradeText[4][3] = "Thermos Tank";
		upgradeText[4][4] = "Trough Tank";
		upgradeText[4][5] = "Thirsty Tank";
		upgradeText[4][6] = "Tremendous Tank";

		//initialize radiator upgrade texts:
		upgradeText[5][0] = "No Radiator";
		upgradeText[5][1] = "Small Radiator";
		upgradeText[5][2] = "Medium Radiator";
		upgradeText[5][3] = "Large Radiator";
		upgradeText[5][4] = "     Small\nWater cooler";
		upgradeText[5][5] = "     Large\nWater cooler";
		upgradeText[5][6] = "Liquid Nitrogen\n      cooler";

		//initialize cargo bay upgrade texts:
		upgradeText[6][0] = "Baby-sized Bay";
		upgradeText[6][1] = "Bulky Bay";
		upgradeText[6][2] = "Big Bay";
		upgradeText[6][3] = "Bovine Bay";
		upgradeText[6][4] = "Bunyanesque\n        Bay";
		upgradeText[6][5] = "Bounteous Bay";
		upgradeText[6][6] = "Brobdingnagian\n         Bay";

		//initialize upgrade prices
		upgradePrice[1] = 5000;
		upgradePrice[2] = 20000;
		upgradePrice[3] = 50000;
		upgradePrice[4] = 100000;
		upgradePrice[5] = 250000;
		upgradePrice[6] = 500000;

		frames = 0;
		width = 100;
		height = 50;

		x = placex;
		y = placey;

		pos.x = x;
		pos.y = y;

		fontStoreUpgrade = MakeFont("Arial", 20, 0);
		fontStoreHeader = MakeFont("Arial", 40, 1600);
		fontStoreAccept = MakeFont("Arial", 40, 1000);
		fontLeftPane = MakeFont("Arial", 30, 1000);

		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
		storeRect = { (long)pos.x + 11, (long)pos.y + 30, (long)pos.x + 19, (long)pos.y + 19 };

		//bgRect is the background of the menu.
		bgRect = { 100, 100, SCREENW - 100,  SCREENH - 100 };

		//closeRect is the close button rect
		closeRect = { SCREENW - 125, 105, SCREENW - 105, 125 };

		//storeWindowRect is the dark area of the store. (width: 724 height: 388)
		storeWindowRect = { 150, 160, SCREENW - 150, SCREENH - 230 };

		//19 tall. 724 total px/6 = 120.6667
		//drillTabRect
		drillTabRect = {storeWindowRect.left + 2, storeWindowRect.top + 2, storeWindowRect.left + 120, storeWindowRect.top + 21};

		//hullTabRect
		hullTabRect = {drillTabRect.right + 2, storeWindowRect.top + 2, drillTabRect.right + 120, storeWindowRect.top + 21};

		//engineTabRect
		engineTabRect = {hullTabRect.right + 2, storeWindowRect.top + 2, hullTabRect.right + 120, storeWindowRect.top + 21};

		//fuelTankTabRect
		fuelTankTabRect = { engineTabRect.right + 2, storeWindowRect.top + 2, engineTabRect.right + 120, storeWindowRect.top + 21 };

		//radiatorTabRect
		radiatorTabRect = {fuelTankTabRect.right + 2, storeWindowRect.top + 2, fuelTankTabRect.right + 120, storeWindowRect.top + 21};

		//cargoBayTabRect
		cargoBayTabRect = {radiatorTabRect.right + 2, storeWindowRect.top + 2, storeWindowRect.right - 2, storeWindowRect.top + 21};

		//tierOneRect x1 = 762 (262/3 = 87.3)... y1 = 363 / 2 = 181.5
		tierOneRect = { GetRectCenter(storeWindowRect).x - 80, drillTabRect.bottom + 2, GetRectCenter(storeWindowRect).x + ((GetRectWidth(storeWindowRect)/2)/3) - 55, GetRectCenter(storeWindowRect).y }; // sets the spacing for all tier rects

		//tierTwoRect
		tierTwoRect = { tierOneRect.right + 2, tierOneRect.top, tierOneRect.right + GetRectWidth(tierOneRect) + 2, tierOneRect.bottom };

		//tierThreeRect
		tierThreeRect = { tierTwoRect.right + 2, tierOneRect.top, storeWindowRect.right - 3, GetRectCenter(storeWindowRect).y }; //-3 to make the sizes the same....

		//tierFourRect
		tierFourRect = { tierOneRect.left, tierOneRect.bottom + 2, tierOneRect.right, storeWindowRect.bottom - 2 };

		//tierFiveRect
		tierFiveRect = { tierTwoRect.left, tierTwoRect.bottom + 2, tierTwoRect.right, tierFourRect.bottom};

		//tierSixRect
		tierSixRect = { tierThreeRect.left, tierThreeRect.bottom + 2, tierThreeRect.right, tierFourRect.bottom };

		//leftPaneRect
		leftPaneRect = { storeWindowRect.left + 2, drillTabRect.bottom + 2, tierOneRect.left - 2, storeWindowRect.bottom - 2};

		//see other acceptRect/sellAllRect etc...
		confirmUpgradeRect = { SCREENW - 300, SCREENH - 200, SCREENW - 125, SCREENH - 125 }; //see "REFINERY sellRect"


		//load alpha transparent image
		upgradeStationSprite = LoadTexture(upgradeStationSpriteFile);

		HRESULT result = d3ddev->CreateOffscreenPlainSurface(
			&bgRect.right - &bgRect.left, //width of the surface
			&bgRect.bottom - &bgRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&background, //pointer to the surface
			NULL); //reserved (always NULL)

		result = d3ddev->CreateOffscreenPlainSurface(
			&closeRect.right - &closeRect.left, //width of the surface
			&closeRect.bottom - &closeRect.top, //height of the surface
			D3DFMT_X8R8G8B8, //surface format
			D3DPOOL_DEFAULT, //memory pool to use
			&xButton, //pointer to the surface
			NULL); //reserved (always NULL)


		if (!upgradeStationSprite)
			return false;
		else
			return true;
	};

	bool MouseInRect(RECT* myrect, POINT* mousePos)
	{
		if (mousePos->x > myrect->left && mousePos->x < myrect->right && mousePos->y > myrect->top && mousePos->y < myrect->bottom)
			return true;
		return false;
	}

	void DrawTab(LPDIRECT3DSURFACE9 surface, RECT* rect, D3DCOLOR color)
	{
		d3ddev->ColorFill(xButton, NULL, color);
		d3ddev->StretchRect(xButton, NULL, backbuffer, rect, D3DTEXF_NONE);
	}

	void DrawCards(int currentUpgrade, LPDIRECT3DSURFACE9 surface, POINT* mousePos)
	{
		switch (currentUpgrade)
		{
		case 0:
		{
			if (tierSelection == 1)
			{
				DrawTab(surface, &tierOneRect, cardSelected);
			}
			else if (MouseInRect(&tierOneRect, mousePos))
			{
				DrawTab(surface, &tierOneRect, cardHover);
			} 
			else
			{
				DrawTab(surface, &tierOneRect, card);
			};

			if (tierSelection == 2)
			{
				DrawTab(surface, &tierTwoRect, cardSelected);
			}
			else if (MouseInRect(&tierTwoRect, mousePos))
			{
				DrawTab(surface, &tierTwoRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierTwoRect, card);
			};

			if (tierSelection == 3)
			{
				DrawTab(surface, &tierThreeRect, cardSelected);
			}
			else if (MouseInRect(&tierThreeRect, mousePos))
			{
				DrawTab(surface, &tierThreeRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierThreeRect, card);
			};

			if (tierSelection == 4)
			{
				DrawTab(surface, &tierFourRect, cardSelected);
			}
			else if (MouseInRect(&tierFourRect, mousePos))
			{
				DrawTab(surface, &tierFourRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFourRect, card);
			};
			
			if (tierSelection == 5)
			{
				DrawTab(surface, &tierFiveRect, cardSelected);
			}
			else if (MouseInRect(&tierFiveRect, mousePos))
			{
				DrawTab(surface, &tierFiveRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFiveRect, card);
			};

			if (tierSelection == 6)
			{
				DrawTab(surface, &tierSixRect, cardSelected);
			}
			else if (MouseInRect(&tierSixRect, mousePos))
			{
				DrawTab(surface, &tierSixRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierSixRect, card);
			};
			
			break;
		}
		case 1:
		{
			DrawTab(surface, &tierOneRect, cardObsolete);

			if (tierSelection == 2)
			{
				DrawTab(surface, &tierTwoRect, cardSelected);
			}
			else if (MouseInRect(&tierTwoRect, mousePos))
			{
				DrawTab(surface, &tierTwoRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierTwoRect, card);
			};

			if (tierSelection == 3)
			{
				DrawTab(surface, &tierThreeRect, cardSelected);
			}
			else if (MouseInRect(&tierThreeRect, mousePos))
			{
				DrawTab(surface, &tierThreeRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierThreeRect, card);
			};

			if (tierSelection == 4)
			{
				DrawTab(surface, &tierFourRect, cardSelected);
			}
			else if (MouseInRect(&tierFourRect, mousePos))
			{
				DrawTab(surface, &tierFourRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFourRect, card);
			};

			if (tierSelection == 5)
			{
				DrawTab(surface, &tierFiveRect, cardSelected);
			}
			else if (MouseInRect(&tierFiveRect, mousePos))
			{
				DrawTab(surface, &tierFiveRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFiveRect, card);
			};

			if (tierSelection == 6)
			{
				DrawTab(surface, &tierSixRect, cardSelected);
			}
			else if (MouseInRect(&tierSixRect, mousePos))
			{
				DrawTab(surface, &tierSixRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierSixRect, card);
			};
			break;
		}
		case 2:
		{
			DrawTab(surface, &tierOneRect, cardObsolete);
			DrawTab(surface, &tierTwoRect, cardObsolete);

			if (tierSelection == 3)
			{
				DrawTab(surface, &tierThreeRect, cardSelected);
			}
			else if (MouseInRect(&tierThreeRect, mousePos))
			{
				DrawTab(surface, &tierThreeRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierThreeRect, card);
			};

			if (tierSelection == 4)
			{
				DrawTab(surface, &tierFourRect, cardSelected);
			}
			else if (MouseInRect(&tierFourRect, mousePos))
			{
				DrawTab(surface, &tierFourRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFourRect, card);
			};

			if (tierSelection == 5)
			{
				DrawTab(surface, &tierFiveRect, cardSelected);
			}
			else if (MouseInRect(&tierFiveRect, mousePos))
			{
				DrawTab(surface, &tierFiveRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFiveRect, card);
			};

			if (tierSelection == 6)
			{
				DrawTab(surface, &tierSixRect, cardSelected);
			}
			else if (MouseInRect(&tierSixRect, mousePos))
			{
				DrawTab(surface, &tierSixRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierSixRect, card);
			};
			break;
		}
		case 3:
		{
			DrawTab(surface, &tierOneRect, cardObsolete);
			DrawTab(surface, &tierTwoRect, cardObsolete);
			DrawTab(surface, &tierThreeRect, cardObsolete);

			if (tierSelection == 4)
			{
				DrawTab(surface, &tierFourRect, cardSelected);
			}
			else if (MouseInRect(&tierFourRect, mousePos))
			{
				DrawTab(surface, &tierFourRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFourRect, card);
			};

			if (tierSelection == 5)
			{
				DrawTab(surface, &tierFiveRect, cardSelected);
			}
			else if (MouseInRect(&tierFiveRect, mousePos))
			{
				DrawTab(surface, &tierFiveRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFiveRect, card);
			};

			if (tierSelection == 6)
			{
				DrawTab(surface, &tierSixRect, cardSelected);
			}
			else if (MouseInRect(&tierSixRect, mousePos))
			{
				DrawTab(surface, &tierSixRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierSixRect, card);
			};
			break;
		}
		case 4:
		{
			DrawTab(surface, &tierOneRect, cardObsolete);
			DrawTab(surface, &tierTwoRect, cardObsolete);
			DrawTab(surface, &tierThreeRect, cardObsolete);
			DrawTab(surface, &tierFourRect, cardObsolete);
			
			if (tierSelection == 5)
			{
				DrawTab(surface, &tierFiveRect, cardSelected);
			}
			else if (MouseInRect(&tierFiveRect, mousePos))
			{
				DrawTab(surface, &tierFiveRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierFiveRect, card);
			};

			if (tierSelection == 6)
			{
				DrawTab(surface, &tierSixRect, cardSelected);
			}
			else if (MouseInRect(&tierSixRect, mousePos))
			{
				DrawTab(surface, &tierSixRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierSixRect, card);
			};
			break;
		}
		case 5:
		{
			DrawTab(surface, &tierOneRect, cardObsolete);
			DrawTab(surface, &tierTwoRect, cardObsolete);
			DrawTab(surface, &tierThreeRect, cardObsolete);
			DrawTab(surface, &tierFourRect, cardObsolete);
			DrawTab(surface, &tierFiveRect, cardObsolete);
			
			if (tierSelection == 6)
			{
				DrawTab(surface, &tierSixRect, cardSelected);
			}
			else if (MouseInRect(&tierSixRect, mousePos))
			{
				DrawTab(surface, &tierSixRect, cardHover);
			}
			else
			{
				DrawTab(surface, &tierSixRect, card);
			};
			break;
		}
		case 6:
		{
			DrawTab(surface, &tierOneRect, cardObsolete);
			DrawTab(surface, &tierTwoRect, cardObsolete);
			DrawTab(surface, &tierThreeRect, cardObsolete);
			DrawTab(surface, &tierFourRect, cardObsolete);
			DrawTab(surface, &tierFiveRect, cardObsolete);
			DrawTab(surface, &tierSixRect, cardObsolete);
			break;
		}
		}
	}

	//1 for centered vertically, 0 for centered at the top, 2 for centered at the bottom
	void PrintTextCenter(LPD3DXFONT font, RECT* rect, string text, D3DCOLOR color, int vertical)
	{
		if (vertical == 1)
		{
			FontPrint(font, ((rect->left + rect->right) / 2) - (GetTextWidth(text.c_str(), font) / 2), ((rect->top + rect->bottom) / 2) - (GetTextHeight(text.c_str(), font) / 2), text, color); //minus text height at some point...
		}
		else if (vertical == 0)
		{
			FontPrint(font, ((rect->left + rect->right) / 2) - (GetTextWidth(text.c_str(), font) / 2), rect->top, text, color);
		}
		else if (vertical == 2)
		{
			FontPrint(font, ((rect->left + rect->right) / 2) - (GetTextWidth(text.c_str(), font) / 2), rect->bottom - GetTextHeight(text.c_str(), font), text, color);
		}
	}

	void DrawUpgradeText(LPD3DXFONT font)//tab selection
	{
		switch(tabSelection)
		{
		case 1:
		{
			PrintTextCenter(font, &tierOneRect, upgradeText[tabSelection][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierTwoRect, upgradeText[tabSelection][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierThreeRect, upgradeText[tabSelection][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFourRect, upgradeText[tabSelection][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFiveRect, upgradeText[tabSelection][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierSixRect, upgradeText[tabSelection][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 2:
		{
			PrintTextCenter(font, &tierOneRect, upgradeText[tabSelection][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierTwoRect, upgradeText[tabSelection][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierThreeRect, upgradeText[tabSelection][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFourRect, upgradeText[tabSelection][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFiveRect, upgradeText[tabSelection][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierSixRect, upgradeText[tabSelection][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 3:
		{
			PrintTextCenter(font, &tierOneRect, upgradeText[tabSelection][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierTwoRect, upgradeText[tabSelection][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierThreeRect, upgradeText[tabSelection][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFourRect, upgradeText[tabSelection][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFiveRect, upgradeText[tabSelection][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierSixRect, upgradeText[tabSelection][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 4:
		{
			PrintTextCenter(font, &tierOneRect, upgradeText[tabSelection][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierTwoRect, upgradeText[tabSelection][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierThreeRect, upgradeText[tabSelection][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFourRect, upgradeText[tabSelection][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFiveRect, upgradeText[tabSelection][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierSixRect, upgradeText[tabSelection][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 5:
		{
			PrintTextCenter(font, &tierOneRect, upgradeText[tabSelection][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierTwoRect, upgradeText[tabSelection][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierThreeRect, upgradeText[tabSelection][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFourRect, upgradeText[tabSelection][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFiveRect, upgradeText[tabSelection][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierSixRect, upgradeText[tabSelection][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 6:
		{
			PrintTextCenter(font, &tierOneRect, upgradeText[tabSelection][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierTwoRect, upgradeText[tabSelection][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierThreeRect, upgradeText[tabSelection][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFourRect, upgradeText[tabSelection][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierFiveRect, upgradeText[tabSelection][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(font, &tierSixRect, upgradeText[tabSelection][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		}
	}

	void DrawLeftPaneInfoDrill(LPD3DXFONT font, int upgradeLevel)
	{
		//draw drill info here.
		switch (tierSelection)
		{
		case 0:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CURRENT DRILL", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[1][upgradeLevel], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 1:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "DRILL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[1][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 2:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "DRILL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[1][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 3:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "DRILL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[1][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 4:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "DRILL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[1][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 5:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "DRILL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[1][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 6:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "DRILL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[1][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		}
	}

	void DrawLeftPaneInfoHull(LPD3DXFONT font, int upgradeLevel)
	{
		//draw hull info here.
		switch (tierSelection)
		{
		case 0:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CURRENT HULL", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[2][upgradeLevel], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 1:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "HULL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[2][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 2:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "HULL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[2][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 3:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "HULL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[2][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 4:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "HULL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[2][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 5:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "HULL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[2][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 6:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "HULL UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[2][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		}
	}

	void DrawLeftPaneInfoEngine(LPD3DXFONT font, int upgradeLevel)
	{
		//draw ENGINE info here.
		switch (tierSelection)
		{
		case 0:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CURRENT ENGINE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[3][upgradeLevel], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 1:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "ENGINE UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[3][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 2:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "ENGINE UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[3][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 3:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "ENGINE UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[3][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 4:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "ENGINE UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[3][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 5:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "ENGINE UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[3][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 6:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "ENGINE UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[3][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		}
	}

	void DrawLeftPaneInfoFuelTank(LPD3DXFONT font, int upgradeLevel)
	{
		//draw FUEL TANK info here.
		switch (tierSelection)
		{
		case 0:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CURRENT FUEL TANK", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[4][upgradeLevel], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 1:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "FUEL TANK UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[4][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 2:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "FUEL TANK UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[4][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 3:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "FUEL TANK UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[4][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 4:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "FUEL TANK UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[4][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 5:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "FUEL TANK UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[4][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 6:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "FUEL TANK UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[4][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		}
	}

	void DrawLeftPaneInfoRadiator(LPD3DXFONT font, int upgradeLevel)
	{
		//draw RADIATOR info here.
		switch (tierSelection)
		{
		case 0:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CURRENT RADIATOR", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[5][upgradeLevel], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 1:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "RADIATOR UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[5][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 2:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "RADIATOR UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[5][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 3:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "RADIATOR UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[5][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 4:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "RADIATOR UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[5][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 5:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "RADIATOR UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[5][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 6:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "RADIATOR UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[5][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		}
	}

	void DrawLeftPaneInfoCargoBay(LPD3DXFONT font, int upgradeLevel)
	{
		//draw CARGO BAY info here.
		switch (tierSelection)
		{
		case 0:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CURRENT CARGO BAY", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[6][upgradeLevel], D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 1:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CARGO BAY UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[6][1], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 2:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CARGO BAY UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[6][2], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 3:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CARGO BAY UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[6][3], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 4:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CARGO BAY UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[6][4], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 5:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CARGO BAY UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[6][5], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		case 6:
		{
			PrintTextCenter(fontLeftPane, &leftPaneRect, "CARGO BAY UPGRADE", D3DCOLOR_XRGB(255, 255, 255), 0);
			PrintTextCenter(fontLeftPane, &leftPaneRect, upgradeText[6][6], D3DCOLOR_XRGB(255, 255, 255), 1);
			PrintTextCenter(fontLeftPane, &leftPaneRect, "$" + to_string(upgradePrice[tierSelection]), D3DCOLOR_XRGB(0, 200, 0), 2);
			break;
		}
		}
	}

	void DrawLeftPaneInfo(LPD3DXFONT font, MINER* aMiner)
	{
		//print current upgrade if nothing is selected
		//print selected upgrade if something is selected based on which tab is open.
		switch (tabSelection)
		{
		case 0:
		{
			//if tab not selected print store splash screen
			//print current upgrade info (break down by tabs)
			PrintTextCenter(fontLeftPane, &storeWindowRect, "Welcome to the upgrade shop,\n     what can we do for you?", D3DCOLOR_XRGB(255, 255, 255), 1);
			break;
		}
		case 1:
		{
			DrawLeftPaneInfoDrill(font, aMiner->drillUpgrade);
			//print tab 1 upgrade info (break down by tabs)
			break;
		}
		case 2:
		{
			DrawLeftPaneInfoHull(font, aMiner->hullUpgrade);
			break;
		}
		case 3:
		{
			DrawLeftPaneInfoEngine(font, aMiner->engineUpgrade);
			break;
		}
		case 4:
		{
			DrawLeftPaneInfoFuelTank(font, aMiner->fuelTankUpgrade);
			break;
		}
		case 5:
		{
			DrawLeftPaneInfoRadiator(font, aMiner->radiatorUpgrade);
			break;
		}
		case 6:
		{
			DrawLeftPaneInfoCargoBay(font, aMiner->cargoBayUpgrade);
			break;
		}
		}
	}

	void HandleConfirm(MINER* aMiner)
	{
		switch (tabSelection)
		{
		case 1://drills
		{
			
			if (aMiner->drillUpgrade < tierSelection) //check if the miner has a better upgrade than what's selected
			{
				//charge the miner money...
				if (aMiner->money >= upgradePrice[tierSelection])
				{
					aMiner->drillUpgrade = tierSelection;
					aMiner->money = aMiner->money - upgradePrice[tierSelection];
					saleMade = true;
					enoughMoney = true;
				}
				else
				{
					enoughMoney = false;
					saleMade = false;
				};
			};
			break;
		}
		case 2://hulls
		{
			if (aMiner->hullUpgrade < tierSelection) //check if the miner has a better upgrade than what's selected
			{
				//charge the miner money...
				if (aMiner->money >= upgradePrice[tierSelection])
				{
					aMiner->hullUpgrade = tierSelection;
					aMiner->money = aMiner->money - upgradePrice[tierSelection];
					saleMade = true;
					enoughMoney = true;
				}
				else
				{
					enoughMoney = false;
					saleMade = false;
				};
			};
			break;
		}
		case 3://engines
		{
			if (aMiner->engineUpgrade < tierSelection) //check if the miner has a better upgrade than what's selected
			{
				//charge the miner money...
				if (aMiner->money >= upgradePrice[tierSelection])
				{
					aMiner->engineUpgrade = tierSelection;
					aMiner->money = aMiner->money - upgradePrice[tierSelection];
					saleMade = true;
					enoughMoney = true;
				}
				else
				{
					enoughMoney = false;
					saleMade = false;
				};
			};
			break;
		}
		case 4://fuel tanks
		{

			if (aMiner->fuelTankUpgrade < tierSelection) //check if the miner has a better upgrade than what's selected
			{
				//charge the miner money...
				if (aMiner->money >= upgradePrice[tierSelection])
				{
					aMiner->fuelTankUpgrade = tierSelection;
					aMiner->money = aMiner->money - upgradePrice[tierSelection];
					saleMade = true;
					enoughMoney = true;
				}
				else
				{
					enoughMoney = false;
					saleMade = false;
				};
			};
			break;
		}
		case 5://radiators
		{
			if (aMiner->radiatorUpgrade < tierSelection) //check if the miner has a better upgrade than what's selected
			{
				//charge the miner money...
				if (aMiner->money >= upgradePrice[tierSelection])
				{
					aMiner->radiatorUpgrade = tierSelection;
					aMiner->money = aMiner->money - upgradePrice[tierSelection];
					saleMade = true;
					enoughMoney = true;
				}
				else
				{
					enoughMoney = false;
					saleMade = false;
				};
			};
			break;
		}
		case 6://cargo bays
		{
			if (aMiner->cargoBayUpgrade < tierSelection) //check if the miner has a better upgrade than what's selected
			{
				//charge the miner money...
				if (aMiner->money >= upgradePrice[tierSelection])
				{
					aMiner->cargoBayUpgrade = tierSelection;
					aMiner->money = aMiner->money - upgradePrice[tierSelection];
					saleMade = true;
					enoughMoney = true;
				}
				else
				{
					enoughMoney = false;
					saleMade = false;
				};
			};
			break;
		}
		};

		//handle can't afford text somewhere
		//maybe display purchased animation or something.
		//tabSelection = 0;
		if (enoughMoney)
		{
			tierSelection = 0;
			aMiner->ApplyUpgrades();
		};
		confirm = false;
	};

	void draw(CAMERA cam, MINER* aMiner, POINT* mousePos)
	{
		pos.x = (x - cam.xOffset);
		pos.y = (y - cam.yOffset);
		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };
		storeRect = { (long)pos.x + 45, (long)pos.y + 30, (long)pos.x + 55, (long)pos.y + 19 + 30 };

		//start drawing
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		//draw the sprite

		spriteobj->Draw(upgradeStationSprite, NULL, NULL, &pos,
			D3DCOLOR_XRGB(255, 255, 255));

		//stop drawing (so that it doesn't draw over my menu, we start drawing again later for text)
		spriteobj->End();

		

		if (storeOpen)
		{
			if (!d3ddev)
			{
				MessageBox(NULL, "d3dev bad", "", NULL);
			};

			if (confirm)
			{
				HandleConfirm(aMiner);
			};

			//draw store button:
			d3ddev->ColorFill(background, NULL, D3DCOLOR_XRGB(50, 50, 50));
			d3ddev->StretchRect(background, NULL, backbuffer, &bgRect, D3DTEXF_NONE);

			//draw close button:
			if (MouseInRect(&closeRect, mousePos))
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(255, 0, 0));
			}
			else
			{
				d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(160, 0, 0));
			};
			d3ddev->StretchRect(xButton, NULL, backbuffer, &closeRect, D3DTEXF_NONE);

			//draw store window:
			d3ddev->ColorFill(xButton, NULL, D3DCOLOR_XRGB(25, 25, 25));
			d3ddev->StretchRect(xButton, NULL, backbuffer, &storeWindowRect, D3DTEXF_NONE);

			//draw Confirm Upgrade button:
			if (MouseInRect(&confirmUpgradeRect, mousePos))
			{
				DrawTab(xButton, &confirmUpgradeRect, D3DCOLOR_XRGB(0, 205, 0));
			}
			else
			{
				DrawTab(xButton, &confirmUpgradeRect, D3DCOLOR_XRGB(0, 150, 0));
			};

			//drills
			if (MouseInRect(&drillTabRect, mousePos) && tabSelection != 1) //if not selected do this:
			{
				DrawTab(xButton, &drillTabRect, D3DCOLOR_XRGB(180, 70, 0));
			}
			else if(tabSelection == 1)
			{
				DrawTab(xButton, &drillTabRect, D3DCOLOR_XRGB(70, 160, 70));
			}
			else
			{
				DrawTab(xButton, &drillTabRect, D3DCOLOR_XRGB(160, 50, 0));
			};

			//hulls
			if (MouseInRect(&hullTabRect, mousePos) && tabSelection != 2) //if not selected do this:
			{
				DrawTab(xButton, &hullTabRect, D3DCOLOR_XRGB(180, 70, 0));
			}
			else if (tabSelection == 2)
			{
				DrawTab(xButton, &hullTabRect, D3DCOLOR_XRGB(70, 160, 70));
			}
			else
			{
				DrawTab(xButton, &hullTabRect, D3DCOLOR_XRGB(160, 50, 0));
			};

			//engines
			if (MouseInRect(&engineTabRect, mousePos) && tabSelection != 3) //if not selected do this:
			{
				DrawTab(xButton, &engineTabRect, D3DCOLOR_XRGB(180, 70, 0));
			}
			else if (tabSelection == 3)
			{
				DrawTab(xButton, &engineTabRect, D3DCOLOR_XRGB(70, 160, 70));
			}
			else
			{
				DrawTab(xButton, &engineTabRect, D3DCOLOR_XRGB(160, 50, 0));
			};

			//fuel tanks
			if (MouseInRect(&fuelTankTabRect, mousePos) && tabSelection != 4) //if not selected do this:
			{
				DrawTab(xButton, &fuelTankTabRect, D3DCOLOR_XRGB(180, 70, 0));
			}
			else if (tabSelection == 4)
			{
				DrawTab(xButton, &fuelTankTabRect, D3DCOLOR_XRGB(70, 160, 70));
			}
			else
			{
				DrawTab(xButton, &fuelTankTabRect, D3DCOLOR_XRGB(160, 50, 0));
			};
			
			//radiators
			if (MouseInRect(&radiatorTabRect, mousePos) && tabSelection != 5) //if not selected do this:
			{
				DrawTab(xButton, &radiatorTabRect, D3DCOLOR_XRGB(180, 70, 0));
			}
			else if (tabSelection == 5)
			{
				DrawTab(xButton, &radiatorTabRect, D3DCOLOR_XRGB(70, 160, 70));
			}
			else
			{
				DrawTab(xButton, &radiatorTabRect, D3DCOLOR_XRGB(160, 50, 0));
			};
			
			//cargo bays
			if (MouseInRect(&cargoBayTabRect, mousePos) && tabSelection != 6) //if not selected do this:
			{
				DrawTab(xButton, &cargoBayTabRect, D3DCOLOR_XRGB(180, 70, 0));
			}
			else if (tabSelection == 6)
			{
				DrawTab(xButton, &cargoBayTabRect, D3DCOLOR_XRGB(70, 160, 70));
			}
			else
			{
				DrawTab(xButton, &cargoBayTabRect, D3DCOLOR_XRGB(160, 50, 0));
			};


			//set up tier card colors
			switch (tabSelection)
			{
			case 1:
			{
				//card 1 selected, show details on the left about attributes and highlight selected card.
				DrawCards(aMiner->drillUpgrade, xButton, mousePos);
				break;
			}
			case 2:
			{
				//card 2
				DrawCards(aMiner->hullUpgrade, xButton, mousePos);
				break;
			}
			case 3:
			{
				//card 3
				DrawCards(aMiner->engineUpgrade, xButton, mousePos);
				break;
			}
			case 4:
			{
				//card 4
				DrawCards(aMiner->fuelTankUpgrade, xButton, mousePos);
				break;
			}
			case 5:
			{
				//card 5
				DrawCards(aMiner->radiatorUpgrade, xButton, mousePos);
				break;
			}
			case 6:
			{
				//card 6
				DrawCards(aMiner->cargoBayUpgrade, xButton, mousePos);
				break;
			}
			};

			//begin text output:
			spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

			//DrawTab(xButton, &leftPaneRect, D3DCOLOR_XRGB(255, 255, 255)); //checking to see if leftPaneRect is positioned properly.
			//DrawUpgradeText(tabSelection, tierOneRect);
			DrawUpgradeText(fontStoreUpgrade);

			DrawLeftPaneInfo(fontLeftPane, aMiner);


			FontPrint(fontStoreUpgrade, closeRect.left + 6, closeRect.top - 1, "x", D3DCOLOR_XRGB(0, 0, 0)); //should put an x down.

			FontPrint(fontStoreHeader, ((bgRect.left + bgRect.right) / 2) - 130, bgRect.top + 10, "Upgrade Station", D3DCOLOR_XRGB(190, 10, 10)); //title

			PrintTextCenter(fontLeftPane, &confirmUpgradeRect, "Confirm\nUpgrade", D3DCOLOR_XRGB(255,255,255), 1);

			FontPrint(fontLeftPane, storeWindowRect.left, storeWindowRect.bottom + 60, "Funds: $" + to_string((signed long)aMiner->money), D3DCOLOR_XRGB(0, 200, 0));

			if (!enoughMoney)
			{
				FontPrint(fontLeftPane, storeWindowRect.left, storeWindowRect.bottom + 30, "Not enough money!", D3DCOLOR_XRGB(200, 0, 0));
				frames = frames + 1;
				if (frames >= 120) //120 is roughly 2 seconds of animation
				{
					frames = 0;
					enoughMoney = true;
				}
			}
			else if (saleMade)
			{
				FontPrint(fontLeftPane, storeWindowRect.left, storeWindowRect.bottom + 30, "Upgrade Purchased!", D3DCOLOR_XRGB(20, 20, 150));
				frames = frames + 1;
				if (frames >= 120) //120 is roughly 2 seconds of animation
				{
					frames = 0;
					saleMade = false;
				}
			}


			//drillTab
			FontPrint(fontStoreUpgrade, ((drillTabRect.left + drillTabRect.right) / 2) - (GetTextWidth("Drills", fontStoreUpgrade) / 2), drillTabRect.top, "Drills", D3DCOLOR_XRGB(255, 255, 255));

			//hullTab
			FontPrint(fontStoreUpgrade, ((hullTabRect.left + hullTabRect.right) / 2) - (GetTextWidth("Hulls", fontStoreUpgrade) / 2), hullTabRect.top, "Hulls", D3DCOLOR_XRGB(255, 255, 255));

			//engineTab
			FontPrint(fontStoreUpgrade, ((engineTabRect.left + engineTabRect.right) / 2) - (GetTextWidth("Engines", fontStoreUpgrade) / 2), engineTabRect.top, "Engines", D3DCOLOR_XRGB(255, 255, 255));

			//fuelTankTab
			FontPrint(fontStoreUpgrade, ((fuelTankTabRect.left + fuelTankTabRect.right) / 2) - (GetTextWidth("Fuel Tanks", fontStoreUpgrade) / 2), fuelTankTabRect.top, "Fuel Tanks", D3DCOLOR_XRGB(255, 255, 255));

			//radiatorTab
			FontPrint(fontStoreUpgrade, ((radiatorTabRect.left + radiatorTabRect.right) / 2) - (GetTextWidth("Radiators", fontStoreUpgrade) / 2), radiatorTabRect.top, "Radiators", D3DCOLOR_XRGB(255, 255, 255));

			//cargoBayTab (finally discovered the ability to get the center point with a function, wow man, just wow)
			FontPrint(fontStoreUpgrade, GetRectCenter(cargoBayTabRect).x - (GetTextWidth("Cargo Bays", fontStoreUpgrade) / 2), cargoBayTabRect.top, "Cargo Bays", D3DCOLOR_XRGB(255, 255, 255));


			spriteobj->End();
		};

	}

	BOOL UpgradeCheck(int tab, int tier, MINER* aMiner)
	{
		//if miner upgrade is less than selected tier card for each upgrade type return true
		switch (tab)
		{
		case 0:
			break;
		case 1:
		{
			//drill tab selected
			if (tier > aMiner->drillUpgrade)
				return true;
			break;
		}

		case 2:
		{
			//hull tab selected
			if (tier > aMiner->hullUpgrade)
				return true;
			break;
		}

		case 3:
		{
			//hull tab selected
			if (tier > aMiner->engineUpgrade)
				return true;
			break;
		}

		case 4:
		{
			//hull tab selected
			if (tier > aMiner->fuelTankUpgrade)
				return true;
			break;
		}

		case 5:
		{
			//hull tab selected
			if (tier > aMiner->radiatorUpgrade)
				return true;
			break;
		}

		case 6:
		{
			//hull tab selected
			if (tier > aMiner->cargoBayUpgrade)
				return true;
			break;
		}
		}
		return false;
	}

	BOOL Input(POINT* mousePos, MINER* aMiner)
	{
		if (MouseInRect(&closeRect, mousePos))
		{
			storeOpen = false;
			tabSelection = 0;
			tierSelection = 0;
			enoughMoney = true;
			saleMade = false;
			return false;
		}

		//Upgrade tabs:
		if (MouseInRect(&drillTabRect, mousePos))
		{
			tabSelection = 1;
			tierSelection = 0;
		};

		if (MouseInRect(&hullTabRect, mousePos))
		{
			tabSelection = 2;
			tierSelection = 0;
		};

		if (MouseInRect(&engineTabRect, mousePos))
		{
			tabSelection = 3;
			tierSelection = 0;
		};

		if (MouseInRect(&fuelTankTabRect, mousePos))
		{
			tabSelection = 4;
			tierSelection = 0;
		};

		if (MouseInRect(&radiatorTabRect, mousePos))
		{
			tabSelection = 5;
			tierSelection = 0;
		};

		if (MouseInRect(&cargoBayTabRect, mousePos))
		{
			tabSelection = 6;
			tierSelection = 0;
		};

		if (!MouseInRect(&storeWindowRect, mousePos) && !MouseInRect(&confirmUpgradeRect, mousePos) && MouseInRect(&bgRect, mousePos))
		{
			tabSelection = 0;
			tierSelection = 0;
			enoughMoney = true;
		}

		//tier cards:
		if (MouseInRect(&tierOneRect, mousePos) && UpgradeCheck(tabSelection, 1, aMiner))
		{
			tierSelection = 1;
		};

		if (MouseInRect(&tierTwoRect, mousePos) && UpgradeCheck(tabSelection, 2, aMiner))
		{
			tierSelection = 2;
		};

		if (MouseInRect(&tierThreeRect, mousePos) && UpgradeCheck(tabSelection, 3, aMiner))
		{
			tierSelection = 3;
		};

		if (MouseInRect(&tierFourRect, mousePos) && UpgradeCheck(tabSelection, 4, aMiner))
		{
			tierSelection = 4;
		};

		if (MouseInRect(&tierFiveRect, mousePos) && UpgradeCheck(tabSelection, 5, aMiner))
		{
			tierSelection = 5;
		};

		if (MouseInRect(&tierSixRect, mousePos) && UpgradeCheck(tabSelection, 6, aMiner))
		{
			tierSelection = 6;
		};

		if (MouseInRect(&leftPaneRect, mousePos))
		{
			tierSelection = 0;
		}

		if (MouseInRect(&confirmUpgradeRect, mousePos))
		{
			confirm = true;
		}
		

		return true;
	}
};

struct BOUNDARY
{
	RECT rect, rect2;
	D3DXVECTOR3 pos;
	int height, width;
	float x, y;

	bool init(int posx, int posy)
	{
		width = 50;
		height = 100;
		x = posx;
		y = posy;

		rect.left = x;
		rect.top = y;
		rect.right = x + width;
		rect.bottom = y + height;

		return true;
	};

	void draw(CAMERA cam, MINER aMiner)
	{
		pos.x = (x - cam.xOffset);
		pos.y = (aMiner.pos.y + (aMiner.height/2)) - (height/2);
		rect = { (long)pos.x, (long)pos.y, (long)pos.x + width, (long)pos.y + height };

		//d3ddev->ColorFill(backbuffer, &rect, D3DCOLOR_XRGB(255, 0, 0));
		
	}
};