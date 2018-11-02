// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#pragma once

// ===== Project includes =====
#include "feature.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Feature_ESP : public Feature
{
public:
	
	// 
	Feature_ESP( Game* game );
	~Feature_ESP( );

	// 
	virtual void		RegisterCVars( );
	virtual void		UpdateCVars( );
	
	// 
	virtual void		Render( );


private:

	// 
	vmCvar_t			cvar_esp_enabled_;
	vmCvar_t			cvar_esp_items_;
};

