include Audio

init: extern(AEAudioInit) func(numberOfChannels: Char)
quit: extern(AEAudioQuit) func

modeIsMusic: extern(AEAudioMusicMode) Int

load: extern(AEAudioLoad) func(filename: const String) ->Pointer
play: extern(AEAudioPlay) func(sound: Pointer, loop: Int) ->Int
pause: extern(AEAudioPause) func(channel: Int) ->Int
resume: extern(AEAudioResume) func(channel: Int) ->Int
volume: extern(AEAudioVolume) func(channel: Int, level: Float) ->Int
delete: extern(AEAudioDelete) func(sound: Pointer)
