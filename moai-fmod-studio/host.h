//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_FMOD_STUDIO_H
#define AKU_FMOD_STUDIO_H

#include <moai-core/host.h>

AKU_API void	MOAIFmodStudioAppFinalize				();
AKU_API void	MOAIFmodStudioAppInitialize				();
AKU_API void	MOAIFmodStudioContextInitialize			();

AKU_API void	MOAIFmodStudioUpdate					();
AKU_API void    MOAIFmodStudioPause                     ();
AKU_API void    MOAIFmodStudioResume                    ();

#endif
