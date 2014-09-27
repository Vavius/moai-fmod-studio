// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/host.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioChannel.h>
#include <moai-fmod-studio/MOAIFmodStudioSound.h>

#ifdef MOAI_OS_IPHONE
	// #include <fmodiphone.h>
#endif

//================================================================//
// MOAIFmodStudio
//================================================================//

//----------------------------------------------------------------//
void MOAIFmodStudioAppFinalize () {
}

//----------------------------------------------------------------//
void MOAIFmodStudioAppInitialize () {
}

//----------------------------------------------------------------//
void MOAIFmodStudioContextInitialize () {

	MOAIFmodStudio::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAIFmodStudio )
	REGISTER_LUA_CLASS ( MOAIFmodStudioChannel )
	REGISTER_LUA_CLASS ( MOAIFmodStudioSound )
}

//----------------------------------------------------------------//
void MOAIFmodStudioUpdate () {

	MOAIFmodStudio::Get ().Update ();
}
