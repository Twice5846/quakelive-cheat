// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#pragma once

// ===== Platform Includes
#include <Windows.h>
#include <vector>

// ===== Project includes =====
#include "../game.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Feature
{
public:

	Feature( Game* game );
	~Feature( );

	virtual void			RegisterCVars( )	{ }
	virtual void			UpdateCVars( )		{ }
	virtual void			Render( )			{ }
	virtual void			ClientCommand( )	{ }

protected:

	Game*					game_;
};

