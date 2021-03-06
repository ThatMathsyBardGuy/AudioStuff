#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")

#include "AudioEngine.h"
#include <chrono>

#define SOUNDDIR "E:/Users/Ethan/Documents/Untitled Games/AudioStuff/AudioStuff/resources/sounds/"

void main() {
	float startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	audiostuff::AudioEngine SoundEngine;
	audiostuff::AudioEngine::Init();
	
	SoundEngine.LoadSound(SOUNDDIR"sheep.wav");
	SoundEngine.LoadSound(SOUNDDIR"boar.wav");
	int sheepChannelId = SoundEngine.PlaySound(SOUNDDIR"sheep.wav");
	int boarChannelId = SoundEngine.PlaySound(SOUNDDIR"boar.wav");
	int engineChannelId = SoundEngine.PlaySound(SOUNDDIR"engine.wav");
	SoundEngine.SetChannel3dPosition(sheepChannelId, utilStuff::Vector3({ 10, 0, 0 }));
	SoundEngine.SetChannel3dPosition(boarChannelId, utilStuff::Vector3({ -10, 0, 0 }));
	SoundEngine.SetChannel3dPosition(engineChannelId, utilStuff::Vector3({ 0, 10, 0 }));
	
	float currentTime = startTime;
	float lastTime = startTime;
	float dt;
	utilStuff::Vector3 enginePos;
	bool running = true;
	while (running)
	{
		lastTime = currentTime;
		currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		dt = currentTime - lastTime;
		
		SoundEngine.Update();
	};
}