////////////////////////////////////////////////////////////////////////////////
// BaArchive

// Includes
#include <new>
#include "BtString.h"
#include "BtPrint.h"
#include "BtTypes.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "BaUserData.h"
#include "BaResource.h"
#include "BtMemory.h"
#include "BtCompressedFile.h"
#include "ApConfig.h"

#include "DyCollisionMesh.h"
#include "DyCollisionAnalytical.h"

// WinGL
#include "SdSoundWinGL.h"
#include "RsMaterialGLES.h"
#include "RsSpriteGLES.h"
#include "RsTextureGLES.h"
#include "RsFontGLES.h"
#include "RsShaderGLES.h"
#include "RsSceneGLES.h"
#include "RsVertexBufferGLES.h"
#include "RsIndexBufferGLES.h"
#include "RsFlashGLES.h"

#include "SgNodeGLES.h"
#include "SgAnimatorGLES.h"

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// GetInstanceSize

BtU32 BaArchive::GetInstanceSize(BaResourceHeader *pResourceHeader)
{
	BaResourceType resourceType = pResourceHeader->m_type;

	switch (resourceType)
	{
	case BaRT_CollisionMesh:
		return sizeof(DyCollisionMesh);
		break;

	case BaRT_CollisionAnalytical:
		return sizeof(DyCollisionAnalytical);
		break;

	case BaRT_VertexBuffer:
		return sizeof(RsVertexBufferWinGL);
		break;

	case BaRT_IndexBuffer:
		return sizeof(RsIndexBufferWinGL);
		break;

	case BaRT_Material:
		return sizeof(RsMaterialWinGL);
		break;

	case BaRT_UserData:
		return sizeof(BaUserData);
		break;

	case BaRT_Sprite:
		return sizeof(RsSpriteWinGL);
		break;

	case BaRT_Font:
		return sizeof(RsFontGLES);
		break;

	case BaRT_Texture:
		return sizeof(RsTextureWinGL);
		break;

	case BaRT_Sound:
		return sizeof(SdSoundWinGL);
		break;

	case BaRT_Scene:
		return sizeof(RsSceneWinGL);
		break;

	case BaRT_SgNode:
        {
            int instanceSize = SgNodeWinGL::GetInstanceSize(pResourceHeader);
        }
        break;

	case BaRT_Animator:
		return sizeof(SgAnimatorWinGL);
		break;

	case BaRT_Shader:
		return sizeof(RsShaderWinGL);
		break;

	case BaRT_Flash:
		return sizeof(RsFlashWinGL);
		break;

	default:
		return sizeof(BaResource);
		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// CreateResourceInstance

void BaArchive::CreateResourceInstance( BaResourceType resourceType, BtU8 *pMemory )
{
	// Cache the platform
	ApPlatform platform = ApConfig::GetPlatform();

	// Validate the platform
	BtAssert( platform != ApPlatform_Max );

	switch( resourceType )
	{
	case BaRT_CollisionMesh:
		{
			new(pMemory) DyCollisionMesh;
		}
		break;

	case BaRT_CollisionAnalytical:
		{
			new(pMemory) DyCollisionAnalytical;
		}
		break;

	case BaRT_VertexBuffer:
		new (pMemory) RsVertexBufferWinGL;
		break;

	case BaRT_IndexBuffer:
		new (pMemory) RsIndexBufferWinGL;
		break;

	case BaRT_Material:
		new( pMemory ) RsMaterialWinGL;
		break;

	case BaRT_UserData:
		new( pMemory ) BaUserData;
		break;

	case BaRT_Sprite:
		new( pMemory ) RsSpriteWinGL;
		break;

	case BaRT_Font:
		new( pMemory ) RsFontGLES;
		break;

	case BaRT_Texture:
		new( pMemory ) RsTextureWinGL;
		break;

	case BaRT_Sound:
		new( pMemory ) SdSoundWinGL;
		break;

	case BaRT_Scene:
		new (pMemory) RsSceneWinGL;
		break;

	case BaRT_SgNode:
		new( pMemory ) SgNodeWinGL;
		break;

	case BaRT_Animator:
		new( pMemory ) SgAnimatorWinGL;
		break;

	case BaRT_Shader:
		new( pMemory ) RsShaderWinGL;
		break;

	case BaRT_Flash:
		new( pMemory ) RsFlashWinGL;
		break;

	default:
		new( pMemory ) BaResource;
		break;
	}
}
