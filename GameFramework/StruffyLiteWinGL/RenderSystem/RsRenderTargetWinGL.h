////////////////////////////////////////////////////////////////////////////////
// RsRenderTargetWin32GL.h

#pragma once
#include "BtArray.h"
#include "BtTypes.h"
#include "BtLinkedList.h"
#include "RsRenderTarget.h"
#include "RsTextureWinGL.h"
#include "RsShader.h"
#include "RsMaterialWinGL.h"
#include "RsPrimitive.h"

struct RsVertex3;
class RsFontWin32GL;
class RsSpriteWinGL;
class SgNodeWinGL;
class SgMeshWinGL;
class RsMaterialWinGL;
class RsRenderTarget;
class RsImplWinGL;

#include "RsRenderTarget.h"

// Class declaration
class RsRenderTargetWinGL : public RsRenderTarget
{
public:

	// Public functions
	void									Reset();
	void									Add( BtU32 sortOrder, RsFontRenderable *pFontRenderable );
	void									Add( BtU32 sortOrder, RsTextureRenderable *pTextureRenderable );
	void									Add( BtU32 sortOrder, RsMaterialRenderable *pMaterialRenderable );
	void									Add( BtU32 sortOrder, RsSpriteRenderable *pSpriteRenderable );
	void									Add( BtU32 sortOrder, SgMeshRenderable *pNodeRenderable );
	void									Add( BtU32 sortOrder, SgSkinRenderable *pNodeRenderable );
	void									Render();

	static void							    SetFrameBufferId(BtU32 index) { m_frameBufferId = index; }

protected:

	BtLinkedList<RsFontRenderable>			m_fonts[MaxSortOrders];
	BtLinkedList<RsTextureRenderable>		m_textures[MaxSortOrders];
	BtLinkedList<RsMaterialRenderable>		m_materials[MaxSortOrders];
	BtLinkedList<SgMeshRenderable>			m_nodes[MaxSortOrders];
	BtLinkedList<SgSkinRenderable>			m_skinNodes[MaxSortOrders];
	BtLinkedList<RsSpriteRenderable>		m_sprites[MaxSortOrders];
	static BtU32							m_frameBufferId;
};
