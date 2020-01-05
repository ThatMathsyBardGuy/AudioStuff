#pragma once

#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include "UtilStuff.h"

#include "fmod_studio.hpp"
#include "fmod.hpp"

#include <string>
#include <map>
#include <vector>
#include <iostream>

//#include <math.h>

namespace audiostuff {
	struct FMODImplementation {
		FMODImplementation();
		~FMODImplementation();

		void Update();

		FMOD::Studio::System* StudioSystem;
		FMOD::System* System;

		int NextChannelId;

		std::map<std::string, FMOD::Sound*> SoundMap;
		std::map<int, FMOD::Channel*> ChannelMap;
		std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
		std::map<std::string, FMOD::Studio::Bank*> BankMap;

	};

	class AudioEngine {
	public:
		static void Init();
		static void Update();
		static void Shutdown();
		static uint8_t ErrorCheck(FMOD_RESULT result);

		void LoadBank(const std::string bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void LoadEvent(const std::string eventName);
		void LoadSound(const std::string soundName, bool threeD = true, bool looping = false, bool stream = false);
		void UnloadSound(const std::string soundName);
		//void Set3dListenerAndOrientation(const utilStuff::Vector3 pos = utilStuff::Vector3{0, 0, 0}, float volumedB = 0.0f);
		int PlaySound(const std::string soundName, const utilStuff::Vector3 pos = utilStuff::Vector3{0, 0, 0}, float volumedB = 0.0f);
		void PlayEvent(const std::string eventName);
		//void StopChannel(int channelId);
		void StopEvent(const std::string eventName, bool immediate = false);
		void GetEventParameterValue(const std::string eventName, const std::string eventParameter, float* parameter);
		void SetEventParameterValue(const std::string eventName, const std::string parameterName, float value);
		//void StopAllChannels();
		void SetChannel3dPosition(int channelId, const utilStuff::Vector3 position);
		void SetChannelVolume(int channelId, float volumedB);
		//bool IsPlaying(int channelId) const;
		bool IsEventPlaying(const std::string eventName) const;
		float DBToVolume(float db);
		float VolumeTodB(float volume);
		FMOD_VECTOR VectorToFmod(const utilStuff::Vector3 position);
	};
	
}

#endif
