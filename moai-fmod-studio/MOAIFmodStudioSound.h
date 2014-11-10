// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSTUDIOSOUND_H
#define	MOAIFMODSTUDIOSOUND_H

#include <moai-sim/headers.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>

class MOAIDataBuffer;

//================================================================//
// MOAIFmodStudioSound
//================================================================//
/**	@name	MOAIFmodStudioSound
	@text	FMOD singleton.
*/
class MOAIFmodStudioSound :
	public virtual MOAILuaObject {
private:

	FMOD_SOUND* mSound;
	int mLoopCount;
    u32 mType;
	char mFileName[128];

	//----------------------------------------------------------------//
	static int	_load				( lua_State* L );
	static int	_loadBGM			( lua_State* L );
	static int	_loadSFX			( lua_State* L );
	static int	_release			( lua_State* L );

public:

    GET ( u32, Type, mType )
        
    enum {
        TYPE_UNSET,
        TYPE_BGM,
        TYPE_SFX,
    };

	friend class MOAIFmodStudioChannel;
	friend class MOAIFmodStudio;

	DECL_LUA_FACTORY ( MOAIFmodStudioSound )

	//----------------------------------------------------------------//
				MOAIFmodStudioSound		();
				~MOAIFmodStudioSound	();
	char		*GetFileName			() { return mFileName; }
	void		Load					( MOAIDataBuffer& data, bool streaming );
	void		Load					( cc8* filename, bool streaming, bool async );
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		ReleaseSound			();
	STLString	ToString				();
};

#endif
