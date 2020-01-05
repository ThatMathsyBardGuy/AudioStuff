#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")

#include "AudioEngine.h"

#define SOUNDDIR nullptr

void main() {
	audiostuff::AudioEngine SoundEngine;
	audiostuff::AudioEngine::Init();
	
	SoundEngine.LoadSound("E:/Users/Ethan/Documents/Untitled Games/AudioStuff/AudioStuff/resources/sounds/sheep.wav");
	SoundEngine.PlaySound("E:/Users/Ethan/Documents/Untitled Games/AudioStuff/AudioStuff/resources/sounds/sheep.wav");

	while (true)
	{

	};
}