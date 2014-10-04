----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAIFileSystem.mountVirtualDirectory ( 'test', 'virtual.zip' )
MOAIFileSystem.checkFileExists ( 'test/virtual/mono16.wav' )

MOAIFmodStudio.init()
channel = MOAIFmodStudioChannel.new()

sound = MOAIFmodStudioSound.new()
sound:loadSFX('test/virtual/mono16.wav')

channel:setLooping(true)
channel:play(sound)
channel:seekAttr(MOAIFmodStudioChannel.ATTR_PAN, -1, 2):setMode(MOAITimer.PING_PONG)

-- MOAIUntzSystem.initialize ()

-- sound = MOAIUntzSound.new ()
-- sound:load ( 'mono16.wav' )
-- sound:setVolume ( 1 )
-- sound:setLooping ( false )
-- sound:play ()
