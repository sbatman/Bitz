#include "GSTest3.h"
#include "../../../Game.h"
#include "../../Objects/Test3/Cube.h"
#include "../../Objects/Test3/VoxelGrid.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;

std::shared_ptr<Cube> testCube = std::shared_ptr<Cube>(new Cube());
std::shared_ptr<Cube> testCube1 = std::shared_ptr<Cube>(new Cube());
std::shared_ptr<VoxelGrid> testGrid = std::shared_ptr<VoxelGrid>(new VoxelGrid(Vector3I(10, 10, 10)));

float rotation = 0;

GSTest3::GSTest3() : Bitz::GameLogic::GameState("Main")
{
	testCube->SetVisible(true);
	testCube->SetPosition(Vector3F(0, -1, 0));
	testCube->SetTexture(Bitz::Content::ContentManager::Load<Texture>("box.Gdat"));

	testCube1->SetVisible(true);
	testCube1->SetPosition(Vector3F(-1, -1, 0));
	testCube1->SetTexture(Bitz::Content::ContentManager::Load<Texture>("box.Gdat"));

	testGrid->SetVisible(true);
	testGrid->SetPosition(Vector3F(0, 0, 0));
	testGrid->SetTexture(Bitz::Content::ContentManager::Load<Texture>("texture.Gdat"));
	testGrid->SetSize(Vector3F(0.3f));

	VoxelGrid::Voxel vox;
	vox.Type = 1;
	for (int x = 0;x < 10;x++)
	{
		if (x > 4 && x < 8)continue;
		for (int y = 0;y < 10;y++)
		{
			if (y > 4 && y < 8)continue;
			for (int z = 0;z < 10;z++)
			{
				testGrid->SetVoxel(Vector3I(x, y, z), vox);
			}
		}
	}

}

GSTest3::~GSTest3()
{
}

void GSTest3::OnEnter(const Bitz::GameLogic::GameState_Ptr)
{

}

void GSTest3::OnExit()
{
}

void GSTest3::OnUpdate(double ms)
{
	rotation += 0.0005f*ms;
	if (rotation > M_PI * 2)
	{
		rotation -= M_PI *2;
	}

	testGrid->SetRotation(Vector3F(rotation, 0, 0));
}

void GSTest3::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState3d);

	//	GraphicsManager::Render(testCube);
	//	GraphicsManager::Render(testCube1);
	GraphicsManager::Render(testGrid);
	GraphicsManager::EndRender();
}