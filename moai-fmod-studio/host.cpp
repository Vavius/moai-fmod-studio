// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/host.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioChannel.h>
#include <moai-fmod-studio/MOAIFmodStudioSound.h>

#ifdef MOAI_OS_IPHONE
	#include <fmod_ios.h>
    #include <AudioToolbox/AudioToolbox.h>
    #include <AudioToolbox/AudioServices.h>
#endif

//================================================================//
// MOAIFmodStudio
//================================================================//

#ifdef MOAI_OS_IPHONE
void setAudioSessionCategory () {

    UInt32 isPlaying;
    UInt32 propertySize = sizeof ( isPlaying );
    OSStatus status;
    
    // check to see if their iPod music is playing
    status = AudioSessionGetProperty ( kAudioSessionProperty_OtherAudioIsPlaying, &propertySize, &isPlaying );
    
    // set the session category accordingly
    if ( !isPlaying ) {
        UInt32 sessionCategory = kAudioSessionCategory_SoloAmbientSound;
        AudioSessionSetProperty ( kAudioSessionProperty_AudioCategory, sizeof ( sessionCategory ), &sessionCategory );
    }
    else {
        UInt32 sessionCategory = kAudioSessionCategory_AmbientSound;
        AudioSessionSetProperty ( kAudioSessionProperty_AudioCategory, sizeof ( sessionCategory ), &sessionCategory );
    }
    AudioSessionSetActive ( true );
}

void interruptionListenerCallback ( void *inUserData, UInt32 interruptionState ) {
    
    if (interruptionState == kAudioSessionBeginInterruption) {
        
    }
    else if (interruptionState == kAudioSessionEndInterruption) {
        setAudioSessionCategory ();
    }
}
#endif

//----------------------------------------------------------------//
void AKUFmodStudioAppFinalize () {
}

//----------------------------------------------------------------//
void AKUFmodStudioAppInitialize () {

#ifdef MOAI_OS_IPHONE
    AudioSessionInitialize ( NULL, NULL, interruptionListenerCallback, NULL );
    setAudioSessionCategory ();
#endif
}

//----------------------------------------------------------------//
void AKUFmodStudioContextInitialize () {
    
	MOAIFmodStudio::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAIFmodStudio )
	REGISTER_LUA_CLASS ( MOAIFmodStudioChannel )
	REGISTER_LUA_CLASS ( MOAIFmodStudioSound )
}

//----------------------------------------------------------------//
void AKUFmodStudioUpdate () {

	MOAIFmodStudio::Get ().Update ();
}

//----------------------------------------------------------------//
void AKUFmodStudioPause() {
	MOAIFmodStudio::Get ().Suspend ();
}

//----------------------------------------------------------------//
void AKUFmodStudioResume() {
	MOAIFmodStudio::Get ().Resume ();
}
