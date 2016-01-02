#include "GSTest3.h"
#include "../../../Game.h"
#include "../../Objects/Test3/Cube.h"
#include "../../Objects/Test3/VoxelGrid.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;

std::shared_ptr<Cube> testCube = std::shared_ptr<Cube>(new Cube());
std::shared_ptr<Cube> testCube1 = std::shared_ptr<Cube>(new Cube());
std::shared_ptr<VoxelGrid> testGrid = std::shared_ptr<VoxelGrid>(new VoxelGrid(Vector3I(50, 50, 10)));

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
	testGrid->SetNormalTexture(Bitz::Content::ContentManager::Load<Texture>("texture_n.Gdat"));
	testGrid->SetSpecularTexture(Bitz::Content::ContentManager::Load<Texture>("texture_s.Gdat"));
	testGrid->SetSize(Vector3F(0.4f));


	VoxelGrid::Voxel vox;
	vox.Type = 1;
	for (int x = 0;x < 50;x++)
	{
		if (x % 2 == 0)continue;
		for (int y = 0;y < 50;y++)
		{
			if (y % 2 == 0)continue;
			for (int z = 0;z < 10;z++)
			{
				if (y % 4 == 1)
				{
					vox.Colour = Vector4B(128, 128, 255, 255);
				}
				else
				{
					vox.Colour = Vector4B(255, 128, 128, 255);
				}
				if (x % 4 == 1)vox.Colour.Y = 0;
				testGrid->SetVoxel(Vector3I(x, y, z), vox);
			}
		}
	}
	vox.Type = -1;
	testGrid->SetVoxel(Vector3I(0, 0, 9), vox);

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
	rotation += 0.00025f*ms;
	if (rotation > M_PI * 2)
	{
		rotation -= M_PI * 2;
	}

	testGrid->SetRotation(Vector3F(M_PI*0.5 + (sinf(rotation)*0.1f), 0, rotation));
}

void GSTest3::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState3d);

	//	GraphicsManager::Render(testCube);
	//	GraphicsManager::Render(testCube1);
	GraphicsManager::Render(testGrid);
	GraphicsManager::EndRender();
}