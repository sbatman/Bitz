#include "GSTest3.h"
#include "../../../Game.h"

#define ORBCOUNT 1000

using namespace Bitz::Math;
using namespace Bitz::GFX;

std::shared_ptr<Drawables::VoxelGrid> testGrid = std::make_shared<Drawables::VoxelGrid>(Vector3I(30, 30, 10));

float rotation = 0;

GSTest3::GSTest3() : GameState("Main")
{
	testGrid->SetVisible(true);
	testGrid->SetPosition(Vector3F(0, 0, 0));
	testGrid->SetTexture(Bitz::Content::ContentManager::Load<Texture>("texture.Gdat"));
	testGrid->SetNormalTexture(Bitz::Content::ContentManager::Load<Texture>("texture_n.Gdat"));
	testGrid->SetSpecularTexture(Bitz::Content::ContentManager::Load<Texture>("texture_s.Gdat"));
	testGrid->SetSize(Vector3F(0.4f));
	
	Drawables::VoxelGrid::Voxel vox;
	vox.Type = 1;
	for (int x = 0;x < 30;x++)
	{
		if (x % 2 == 0)continue;
		for (int y = 0;y < 30;y++)
		{
			if (y % 2 == 0)continue;
			for (int z = 0;z < 10;z++)
			{
				if (y % 4 == 1)
				{
					vox.Colour = Vector4B(180, 180, 255, 255);
				}
				else
				{
					vox.Colour = Vector4B(255, 180, 180, 255);
				}
				if (x % 4 == 1)vox.Colour.Y = 128;
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
	rotation += 0.00015f*ms;
	if (rotation > M_PI * 2)
	{
		rotation -= M_PI * 2;
	}

	testGrid->SetRotation(Vector3F(M_PI*0.5 + (sinf(rotation)*0.1f), 0, rotation));
}

void GSTest3::OnDraw()
{
	GraphicsManager::BeginRender(Game::RenderState3d);

	GraphicsManager::Render(testGrid);
	GraphicsManager::EndRender();
}