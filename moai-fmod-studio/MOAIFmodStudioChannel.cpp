// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioChannel.h>
#include <moai-fmod-studio/MOAIFmodStudioSound.h>
#include <fmod.hpp>
#include <fmod_errors.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getPan
	@text	Returns the current pan of the channel.

	@in		MOAIFmodStudioChannel 	self
	@out	number pan				the pan currently set in this channel.
*/
int MOAIFmodStudioChannel::_getPan ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )
	
	state.Push ( state, self->GetPan ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPitch
	@text	Returns the current pitch of the channel.

	@in		MOAIFmodStudioChannel 	self
	@out	number pitch			the pitch currently set in this channel.
 */
int MOAIFmodStudioChannel::_getPitch ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )
	
	state.Push ( state, self->GetPitch ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getVolume
	@text	Returns the current volume of the channel.

	@in		MOAIFmodStudioChannel 	self
	@out	number volume			the volume currently set in this channel.
*/
int MOAIFmodStudioChannel::_getVolume ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )
	
	state.Push ( state, self->GetVolume ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPlaying
    @text	Returns true if channel is playing.
 
    @in     MOAIFmodStudioChannel self
    @out	boolean
 */
int MOAIFmodStudioChannel::_isPlaying ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )
	
	bool isPlaying = self->IsPlaying ();
    
    state.Push ( isPlaying );
    return 1;
}

//----------------------------------------------------------------//
/**	@name	moveVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.

	@in		MOAIFmodStudioChannel self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
*/
int MOAIFmodStudioChannel::_moveVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	float delta		= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIFmodStudioChannelAttr::Pack ( ATTR_VOLUME ), delta, mode );

	action->SetSpan ( length );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	play
	@text	Plays the specified sound, looping it if desired.

	@in		MOAIFmodStudioChannel self
	@in		MOAIFmodStudioSound sound		The sound to play.
	@in		number loopCount			Number of loops.
	@out	nil
*/
int MOAIFmodStudioChannel::_play ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UU" )

	self->mPlayState = PLAYING;
	MOAIFmodStudioSound* sound = state.GetLuaObject < MOAIFmodStudioSound >( 2, true );
	if ( !sound ) return 0;

	int loopCount = state.GetValue < int >( 3, 0 );

	self->Play ( sound, loopCount );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.

	@in		MOAIFmodStudioChannel self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
*/
int MOAIFmodStudioChannel::_seekVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	float target	= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIFmodStudioChannelAttr::Pack ( ATTR_VOLUME ), target - self->mVolume, mode );

	action->SetSpan ( length );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setPan
	@text	Immediately sets the pan for this channel.

	@in		MOAIFmodStudioChannel 	self
	@in		number pan				The pan of this channel.
	@out	nil
*/
int MOAIFmodStudioChannel::_setPan ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UN" )

	float pan = state.GetValue < float >( 2, 0.0f );
	self->SetPan ( pan );

	return 0;
}


//----------------------------------------------------------------//
/**	@name	setPitch
	@text	Immediately sets the pitch of this channel.

	@in		MOAIFmodStudioChannel 	self
	@in		number pitch			The pitch of this channel.
	@out	nil
*/
int MOAIFmodStudioChannel::_setPitch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UN" )

	float pitch = state.GetValue < float >( 2, 1.0f );
	self->SetPitch ( pitch );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setReuse
    @text	If set to true then channel won't stop previous playing sound and
            will use a new fmod channel every time play is called.

    @in		MOAIFmodStudioChannel self
    @out	nil
*/
int MOAIFmodStudioChannel::_setReusable ( lua_State *L ) {
    MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UB" )
    
    bool reuse = state.GetValue < bool >( 2, true );
    self->mReusable = reuse;
    
    return 0;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Immediately sets the volume of this channel.

	@in		MOAIFmodStudioChannel self
	@in		number volume			The volume of this channel.
	@out	nil
*/
int MOAIFmodStudioChannel::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UN" )

	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPaused
	@text	Sets whether this channel is paused and hence does not play any sounds.

	@in		MOAIFmodStudioChannel self
	@in		boolean paused			Whether this channel is paused.
	@out	nil
*/
int MOAIFmodStudioChannel::_setPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "UB" )

	self->mPlayState = PAUSED;
	bool paused = state.GetValue < bool >( 2, false );
	self->SetPaused ( paused );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLooping
	@text	Immediately sets looping for this channel.

	@in		MOAIFmodStudioChannel self
	@in		boolean looping	  True if channel should loop.
	@out	nil
*/
int MOAIFmodStudioChannel::_setLooping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )

	bool looping = state.GetValue < bool >( 2, false );
	self->mLooping = looping;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stops playing the sound on this channel.

	@in		MOAIFmodStudioChannel self
	@out	nil
*/
int MOAIFmodStudioChannel::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )
	
	self->mPlayState = STOPPED;
	self->Stop ();

	return 0;
}

//================================================================//
// MOAIFmodStudioChannel
//================================================================//

//----------------------------------------------------------------//
bool MOAIFmodStudioChannel::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIFmodStudioChannelAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );

		if ( attrID == ATTR_VOLUME ) {
			this->mVolume = attrOp.Apply ( this->mVolume, op, MOAIAttrOp::ATTR_READ_WRITE );
			this->SetVolume ( this->mVolume );
			return true;
		}

		if ( attrID == ATTR_PITCH ) {
			this->mPitch = attrOp.Apply ( this->mPitch, op, MOAIAttrOp::ATTR_READ_WRITE );
			this->SetPitch ( this->mPitch );
			return true;
		}

		if ( attrID == ATTR_PAN ) {
			this->mPan = attrOp.Apply ( this->mPan, op, MOAIAttrOp::ATTR_READ_WRITE );
			this->SetPan ( this->mPan );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
float MOAIFmodStudioChannel::GetPan () {
	return this->mPan;
}

//----------------------------------------------------------------//
float MOAIFmodStudioChannel::GetPitch () {
	return this->mPitch;
}

//----------------------------------------------------------------//
float MOAIFmodStudioChannel::GetVolume () {
	return this->mVolume;
}

//----------------------------------------------------------------//
bool MOAIFmodStudioChannel::IsPlaying () {
    
    if ( this->mChannel ) {
        
        FMOD_BOOL playing = false;
        FMOD_RESULT result = FMOD_Channel_IsPlaying ( this->mChannel, &playing );
        if ( MOAIFmodCheckError ( result )) {
            return ( bool ) playing;
        }
    }
    return false;
}

//----------------------------------------------------------------//
MOAIFmodStudioChannel::MOAIFmodStudioChannel () :
	mChannel ( 0 ),
	mPitch ( 1.0f ),
	mPan ( 0.0f ),
	mVolume ( 1.0f ),
	mPaused ( false ) ,
	mLooping ( false ),
	mReusable ( false ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIFmodStudioChannel::~MOAIFmodStudioChannel () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::Play ( MOAIFmodStudioSound* sound, int loopCount ) {

	if ( !this->mReusable ) {
        this->Stop ();
    }

	this->mSound = sound;
	if ( !sound ) return;
	if ( !sound->mSound ) return;
	
	MOAIFmodStudio& system = MOAIFmodStudio::Get ();
    
	FMOD_CHANNELGROUP* group = 0;
	switch ( sound->GetType ()) {
        case MOAIFmodStudioSound::TYPE_BGM:
            group = system.GetBGMChannelGroup ();
            break;
            
        case MOAIFmodStudioSound::TYPE_SFX:
            group = system.GetSFXChannelGroup ();
            break;
            
        default:
            group = system.GetMainChannelGroup ();
            break;
    }
    
    FMOD_SYSTEM* soundSys = system.GetSoundSys ();
	if ( !soundSys ) return;
	
	FMOD_RESULT result;
	FMOD_CHANNEL* channel = 0;
    
    result = FMOD_System_PlaySound ( soundSys, sound->mSound, group, true, &channel );
	if ( !MOAIFmodCheckError ( result )) return;
	
	this->mChannel = channel;
	result = FMOD_Channel_SetMode ( this->mChannel, FMOD_LOOP_NORMAL );
    MOAIFmodCheckError ( result );
    
	if ( mLooping ) {
		result = FMOD_Channel_SetLoopCount ( this->mChannel, -1 );
        MOAIFmodCheckError ( result );
	}
	else {
		result = FMOD_Channel_SetLoopCount ( this->mChannel, loopCount );
        MOAIFmodCheckError ( result );
	}

	
    this->SetPan ( this->mPan );
    this->SetPitch ( this->mPitch );
    this->SetVolume ( this->mVolume );
	this->SetPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::RegisterLuaClass ( MOAILuaState& state ) {
	
    MOAINode::RegisterLuaClass ( state );
    
	state.SetField ( -1, "ATTR_VOLUME", MOAIFmodStudioChannelAttr::Pack ( ATTR_VOLUME ));
	state.SetField ( -1, "ATTR_PITCH", MOAIFmodStudioChannelAttr::Pack ( ATTR_PITCH ));
	state.SetField ( -1, "ATTR_PAN", MOAIFmodStudioChannelAttr::Pack ( ATTR_PAN ));
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::RegisterLuaFuncs ( MOAILuaState& state ) {

    MOAINode::RegisterLuaFuncs ( state );
    
	luaL_Reg regTable [] = {
		{ "getPan", 		_getPan },
		{ "getPitch", 		_getPitch },
		{ "getVolume",		_getVolume },
		{ "isPlaying",		_isPlaying },
		{ "moveVolume",		_moveVolume },
		{ "play",			_play },
		{ "seekVolume",		_seekVolume },
		{ "setPaused",		_setPaused },
		{ "setLooping",		_setLooping },
		{ "setPan", 		_setPan },
		{ "setPitch", 		_setPitch },
        { "setReusable",    _setReusable },
		{ "setVolume",		_setVolume },
		{ "stop",			_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::SetPaused ( bool paused ) {

	this->mPaused = paused;
	if ( !this->mChannel ) return;
	FMOD_RESULT result = FMOD_Channel_SetPaused ( this->mChannel, this->mPaused );
    MOAIFmodCheckError ( result );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::SetPan ( float pan ) {

	this->mPan = pan;
	if ( !this->mChannel ) return;
	FMOD_RESULT result = FMOD_Channel_SetPan ( this->mChannel, this->mPan );
    MOAIFmodCheckError ( result );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::SetPitch ( float pitch ) {

	this->mPitch = pitch;
	if ( !this->mChannel ) return;
	FMOD_RESULT result = FMOD_Channel_SetPitch ( this->mChannel, this->mPitch );
    MOAIFmodCheckError ( result );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::SetVolume ( float volume ) {

	this->mVolume = volume;
	if ( !this->mChannel ) return;
	FMOD_RESULT result = FMOD_Channel_SetVolume ( this->mChannel, this->mVolume );
    MOAIFmodCheckError ( result );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::Stop () {
	if ( !this->mChannel ) return;
	FMOD_RESULT result = FMOD_Channel_Stop ( this->mChannel );
    this->mChannel = 0;
    MOAIFmodCheckError ( result );
}

