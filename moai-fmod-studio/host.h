//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_FMOD_STUDIO_H
#define AKU_FMOD_STUDIO_H

#include <moai-core/host.h>

AKU_API void	AKUFmodStudioAppFinalize		();
AKU_API void	AKUFmodStudioAppInitialize		();
AKU_API void	AKUFmodStudioContextInitialize	();
AKU_API void	AKUFmodStudioUpdate             ();
AKU_API void    AKUFmodStudioPause              ();
AKU_API void    AKUFmodStudioResume             ();

#endif
