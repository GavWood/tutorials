////////////////////////////////////////////////////////////////////////////////
// ScMain.cpp

// Includes
#include <stdio.h>
#include "ApConfig.h"
#include "RsUtil.h"
#include "RsShader.h"
#include "HlMaterial.h"
#include "HlFont.h"
#include "HlDraw.h"
#include "HlMouse.h"
#include "UiKeyboard.h"
#include "RsMaterial.h"
#include "ShHMD.h"
#include "UiKeyboard.h"
#include "HlKeyboard.h"
#include "ShTouch.h"
#include "HlDebug.h"
#include "SgNode.h"
#include "HlModel.h"

#include "ScMain.h"
#include "ScCamera.h"
#include "ShCamera.h"

static BtBool isFrustumStill = BtFalse;

////////////////////////////////////////////////////////////////////////////////
// Init

void ScMain::Init()
{
	m_isClosing = BtFalse;
	m_isClosed  = BtFalse;

	MtVector2 v2Dimension;

	ApConfig::Init();
	ApConfig::SetTitle( "3DScene" );
	//ApConfig::SetDebug( BtTrue );
	ApConfig::SetDebug( BtFalse );
	RdRandom::SetRandomSeed();
    
    BtBool isSimulator = ApConfig::IsSimulator();
    (void)isSimulator;
    
	if( ApConfig::IsPhone() && !isSimulator )
	{
		ApConfig::SetAR(BtTrue);
	}
	else
	{
		ApConfig::SetAR(BtFalse);
	}

	// Load the game archive
#ifdef WIN32
    ApConfig::SetResourcePath( "..\\3DScene\\release\\" );
    ApConfig::CheckResourcePath( "game" );
    RsUtil::SetDimension(MtVector2(800.0f, 600.0f));
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Create

void ScMain::Create()
{
	m_gameArchive.Load( "game" );
	m_animArchive.Load("anim");
	m_utilityArchive.Load("utility");
    
	m_pWhite2 = m_utilityArchive.GetMaterial( "white2" );
	m_pWhite3 = m_utilityArchive.GetMaterial( "white3" );
	m_pShader = m_gameArchive.GetShader( "shader" );
	 
	m_skybox.Setup( &m_gameArchive );
	m_model.Setup( &m_gameArchive, &m_animArchive );
	m_bubbles.Setup(&m_gameArchive);
	m_frustum.Setup( &m_utilityArchive);
	
	RsMaterial *pMaterial2 = m_utilityArchive.GetMaterial("white2");
	RsMaterial *pMaterial3 = m_utilityArchive.GetMaterial("white3");
	RsMaterial *pMaterial3notest = m_utilityArchive.GetMaterial("white3noztest");
    
	HlFont::Setup( &m_utilityArchive, "vera" );
	HlMouse::Setup( &m_utilityArchive );
	HlDraw::Setup( pMaterial2, pMaterial3, pMaterial3notest );
    
	Reset();
    
    ShCamera::Create( &m_gameArchive );
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void ScMain::Reset()
{
    ApConfig::SetPaused( BtFalse );
    
	MtVector2 v2Dimension = RsUtil::GetDimension();

	if (ShHMD::IsHMD())
	{
		v2Dimension = ShHMD::GetDimension();
	}

	m_camera.Init( v2Dimension );
	m_camera.SetSpeed(10.0f);

	m_bubbles.Reset();
	m_bubbles.SetEmitter(BtTrue);
	m_bubbles.SetPosition(MtVector3(0, 0, 0.6f));
}

////////////////////////////////////////////////////////////////////////////////
// UpdateTest

void ScMain::UpdateTest()
{
	HlDebug::Reset();

	m_camera.Update();
	m_skybox.Update( m_camera.GetCamera().GetPosition() );
	m_model.Update( m_camera.GetCamera() );
	m_bubbles.Update();

	HlDebug::Update();

	if( !isFrustumStill )
	{
		RsCamera camera = m_camera.GetCamera();
		m_frustum.Update(camera.GetRotation(), camera.GetPosition(), camera.GetFieldOfView(), camera.GetAspect());
	}
	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_F))
	{
		isFrustumStill = !isFrustumStill;
	}
	if (UiKeyboard::pInstance()->IsPressed(PauseKey))
	{
		ApConfig::SetPaused(!ApConfig::IsPaused());
	}
	if(UiKeyboard::pInstance()->IsPressed(DebugKey))
	{
		ApConfig::SetDebug(!ApConfig::IsDebug());
	}
	if (UiKeyboard::pInstance()->IsPressed(CloseGameKey))
	{
		m_isClosing = BtTrue;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ScMain::Update()
{
	// Are we closing
	if( m_isClosing == BtTrue )
	{
		// Unload the anim archive
		m_animArchive.Unload();

		// Unload the archive
		m_gameArchive.Unload();
	
		// Unload the archive
		m_utilityArchive.Unload();
        
		// Read to close
		m_isClosed = BtTrue;
	}
	else
	{
		UpdateTest();
	}
}

//////////////////////////////////////////////////////////////////
// SetupRender

void ScMain::SetupRender()
{
	// Apply the shader
	m_pShader->Apply();

	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the camera
	pRenderTarget->SetCamera( m_camera.GetCamera() );

	// Clear the render target
    pRenderTarget->SetCleared( BtTrue );
    
	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	//pRenderTarget->SetClearColour( RsColour( 0.5f, 0.6f, 0.7f, 1.0f ) );
	pRenderTarget->SetClearColour( RsColour::NoColour() );

	// Apply this render target
	pRenderTarget->Apply();
}

//////////////////////////////////////////////////////////////////
// SetupRenderToTexture

void ScMain::SetupRenderToTexture( RsTexture *pTexture, RsCamera camera )
{
	// Apply the shader
	m_pShader->Apply();

	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the texture
	pRenderTarget->SetTexture( pTexture );

	// Set the camera
	pRenderTarget->SetCamera( camera );

	// Clear the render target
	pRenderTarget->SetCleared( BtTrue );

	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	pRenderTarget->SetClearColour( RsColour::NoColour() );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// RestoreRenderTarget

void ScMain::RestoreRenderTarget()
{
	// Make a new render target
	RsRenderTarget *pRenderTarget = RsUtil::GetNewRenderTarget();

	// Set the camera
	pRenderTarget->SetCamera( m_camera.GetCamera() );
    
    // Clear the render target
    pRenderTarget->SetCleared( BtTrue );
    
	// Clear the z buffer
	pRenderTarget->SetZCleared( BtTrue );

	// Set a good clear colour
	pRenderTarget->SetClearColour( RsColour( 0.95f, 0.92f, 1.0f ) );

	// Apply this render target
	pRenderTarget->Apply();
}

////////////////////////////////////////////////////////////////////////////////
// Render2DScene

void ScMain::Render2DScene()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render3DScene

void ScMain::Render3DScene()
{
    if( ApConfig::IsAR() )
    {
		ShCamera::Render();
    }
    else
    {
		m_skybox.Render();
    }
    
	m_bubbles.Render();
	m_model.Render( m_camera.GetCamera() );

	if(isFrustumStill)
	{
		m_frustum.Render();
	}
}

////////////////////////////////////////////////////////////////////////////////
// RenderTests

void ScMain::RenderTests()
{
	m_camera.Render();

	// Apply the shader
	m_pShader->Apply();

	// Empty the render targets at the start
	RsUtil::EmptyRenderTargets();
    
	RestoreRenderTarget();
	Render3DScene();

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void ScMain::Render()
{
	if( m_isClosing == BtTrue )
	{
		return;
	}

	// Render the tests
	RenderTests();

	// Render the mouse
	//HlMouse::Render();

	HlDebug::Render();
}

////////////////////////////////////////////////////////////////////////////////
// PreRender

void ScMain::PreRender()
{
}

////////////////////////////////////////////////////////////////////////////////
// PostRender

void ScMain::PostRender()
{
	// Render the mouse
	//m_renderMouse.Render();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void ScMain::Destroy()
{
}
