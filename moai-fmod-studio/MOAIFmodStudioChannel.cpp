// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioChannel.h>
#include <moai-fmod-studio/MOAIFmodStudioSound.h>
#include <fmod.hpp>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getVolume
 @text	Returns the current volume of the channel.
 
 @in	MOAIFmodStudioChannel self
 @out	number volume				the volume currently set in this channel.
 */
int MOAIFmodStudioChannel::_getVolume ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )
	
	lua_pushnumber ( state, self->GetVolume ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPlaying
 @text	Returns true if channel is playing.
 
 @in	MOAIFmodStudioChannel self
 @out	boolean
 */
int MOAIFmodStudioChannel::_isPlaying ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodStudioChannel, "U" )
	
	bool isPlaying = self->mPlayState == PLAYING;

	if ( self->mSound ) {
		lua_pushboolean ( state, isPlaying );
		return 1; 
	}
	return 0;
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
	}
	return false;
}

//----------------------------------------------------------------//
float MOAIFmodStudioChannel::GetVolume () {
	return this->mVolume;
}

//----------------------------------------------------------------//
MOAIFmodStudioChannel::MOAIFmodStudioChannel () :
	mChannel ( 0 ),
	mVolume ( 1.0f ),
	mPaused ( false ) ,
	mLooping ( false ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIFmodStudioChannel::~MOAIFmodStudioChannel () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::Play ( MOAIFmodStudioSound* sound, int loopCount ) {

	this->Stop ();
	this->mSound = sound;
	if ( !sound ) return;
	if ( !sound->mSound ) return;
	
	FMOD::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	FMOD_RESULT result;
	FMOD::Channel* channel = 0;
	
	//printf ( "PLAY SOUND %s, @ %f\n", sound->GetFileName (), ZLDeviceTime::GetTimeInSeconds () );
	result = soundSys->playSound ( sound->mSound, 0, true, &channel );
	if ( result != FMOD_OK ) {
		printf (" FMOD ERROR: Sound did not play\n" );
		return;
	}
	
	this->mChannel = channel;
	this->mChannel->setMode ( FMOD_LOOP_NORMAL );

	if ( mLooping ) {
		this->mChannel->setLoopCount ( -1 );
	}
	else {
		this->mChannel->setLoopCount ( loopCount );
	}
	
	this->SetVolume ( this->mVolume );
	this->SetPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "ATTR_VOLUME", MOAIFmodStudioChannelAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getVolume",		_getVolume },
		{ "isPlaying",		_isPlaying },
		{ "moveVolume",		_moveVolume },
		{ "play",			_play },
		{ "seekVolume",		_seekVolume },
		{ "setPaused",		_setPaused },
		{ "setLooping",		_setLooping },
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
	this->mChannel->setPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::SetVolume ( float volume ) {

	this->mVolume = volume;
	if ( !this->mChannel ) return;
	this->mChannel->setVolume ( this->mVolume );
}

//----------------------------------------------------------------//
void MOAIFmodStudioChannel::Stop () {

	if ( !this->mChannel ) return;
	this->mChannel->stop();
}
