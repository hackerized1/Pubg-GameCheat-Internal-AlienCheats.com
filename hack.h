#pragma once
#include "Renders.h"
#include "sdk.h"
#include "Settings.h"

class Esp
{
public:
	static void DrawSkeleton(AActor* pEntity)
	{
		auto pMesh = pEntity->GetMesh();
		auto IsVisible = UnrealEngine4::IsVisible(pEntity);
		if (pMesh)
		{
			Vector3 neckpos = pMesh->GetBoneWithRotation(EBoneIndex::neck_01);
			Vector3 pelvispos = pMesh->GetBoneWithRotation(EBoneIndex::pelvis);
			Vector3 previous(0, 0, 0);
			Vector3 current, p1, c1;

			for (auto a : skeleton)
			{
				previous = Vector3(0, 0, 0);

				for (int bone : a)
				{
					current = bone == EBoneIndex::neck_01 ? neckpos : (bone == EBoneIndex::pelvis ? pelvispos : pMesh->GetBoneWithRotation(bone));
					if (previous.x == 0.f)
					{
						previous = current;
						continue;
					}
					p1 = UnrealEngine4::WorldToScreen(previous);
					c1 = UnrealEngine4::WorldToScreen(current);
					DrawLine(p1.x + 1, p1.y, c1.y + 1, c1.y, BLACK, 1.3f);
					DrawLine(p1.x, p1.y, c1.x, c1.y, IsVisible ? GREEN : RED, 1.3f);
					DrawLine(p1.x - 1, p1.y, c1.x - 1, c1.y, BLACK, 1.3f);
					previous = current;
				}
			}
		}
	}
};

class Hack
{
public:
	static void MainLoop()
	{
		if (UnrealEngine4::IsInGame()) {
			auto pLocal = UnrealEngine4::GetLocalEntity();
			auto pCamera = UnrealEngine4::GetCameraManager();
			if (pLocal)
			{
				// pre loop hacks
				// todo: anticolllision
			}
			auto EntityList = UnrealEngine4::GetEntityList();
			if (EntityList.IsValid()) {
				auto HighestEntityIndex = EntityList.Count();
				for (int EntityIndex = 0; EntityIndex < HighestEntityIndex; EntityIndex++)
				{
					auto pEntity = EntityList[EntityIndex];
					if (!pEntity)
						continue;
					if (pEntity == pLocal)
						continue;
					auto GlobalName = UnrealEngine4::GetGNameByID(pEntity->GetObjectID());
					if (UnrealEngine4::IsPlayer(GlobalName))
					{
						if (Settings::espskeleton_enable)
							Esp::DrawSkeleton(pEntity);
						// todo: esp, aim
					}
				}
			}
			if (pLocal)
			{
				// post loop hacks/restore
			}
		}
	}
};