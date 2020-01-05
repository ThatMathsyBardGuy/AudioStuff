#include "AudioEngine.h"

namespace audioStuff {
	void AudioEngine::Init()
	{
		s_Implementation = new FMODImplementation();
	}

	void AudioEngine::Update()
	{
		s_Implementation->Update();
	}

	void AudioEngine::LoadSound(const std::string soundName, bool threeD, bool looping, bool stream)
	{
		std::map<std::string, FMOD::Sound*>::iterator foundIt = s_Implementation->SoundMap.find(soundName);
		if (foundIt != s_Implementation->SoundMap.end()) return;

		FMOD_MODE mode = FMOD_DEFAULT;
		mode |= threeD ? FMOD_3D : FMOD_2D;
		mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

		FMOD::Sound* sound = nullptr;
		AudioEngine::ErrorCheck(s_Implementation->System->createSound(soundName.c_str(), mode, nullptr, &sound));
		if (sound)
		{
			s_Implementation->SoundMap[soundName] = sound;
		}
	}

	void AudioEngine::UnloadSound(const std::string soundName)
	{
		std::map<std::string, FMOD::Sound*>::iterator foundIt = s_Implementation->SoundMap.find(soundName);
		if (foundIt == s_Implementation->SoundMap.end()) return;

		AudioEngine::ErrorCheck(foundIt->second->release());
		s_Implementation->SoundMap.erase(foundIt);
	}

	int AudioEngine::PlaySound(const std::string soundName, const utilStuff::Vector3 position, float volumedB)
	{
		int channelId = s_Implementation->NextChannelId++;
		std::map<std::string, FMOD::Sound*>::iterator foundIt = s_Implementation->SoundMap.find(soundName);
		if (foundIt == s_Implementation->SoundMap.end())
		{
			LoadSound(soundName);
			foundIt = s_Implementation->SoundMap.find(soundName);
			if (foundIt == s_Implementation->SoundMap.end()) return channelId;
		}
		FMOD::Channel* channel = nullptr;
		AudioEngine::ErrorCheck(s_Implementation->System->playSound(foundIt->second, nullptr, true, &channel));
		if (channel)
		{
			FMOD_MODE mode;
			foundIt->second->getMode(&mode);
			if(mode & FMOD_3D)
			{
				FMOD_VECTOR fmodPosition = VectorToFmod(position);
				AudioEngine::ErrorCheck(channel->set3DAttributes(&fmodPosition, nullptr));
			}
			AudioEngine::ErrorCheck(channel->setVolume(DBToVolume(volumedB)));
			AudioEngine::ErrorCheck(channel->setPaused(false));
			s_Implementation->ChannelMap[channelId] = channel;
		}
		return channelId;
	}

	void AudioEngine::SetChannel3dPosition(int channelId, const utilStuff::Vector3 position)
	{
		std::map<int, FMOD::Channel*>::iterator foundIt = s_Implementation->ChannelMap.find(channelId);
		if (foundIt == s_Implementation->ChannelMap.end()) return;

		FMOD_VECTOR fmodPosition = VectorToFmod(position);
		AudioEngine::ErrorCheck(foundIt->second->set3DAttributes(&fmodPosition, NULL));
	}

	void AudioEngine::SetChannelVolume(int channelId, float volumedB)
	{
		std::map<int, FMOD::Channel*>::iterator foundIt = s_Implementation->ChannelMap.find(channelId);
		if (foundIt == s_Implementation->ChannelMap.end()) return;

		AudioEngine::ErrorCheck(foundIt->second->setVolume(DBToVolume(volumedB)));
	}

	void AudioEngine::LoadBank(const std::string bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
		std::map<std::string, FMOD::Studio::Bank*>::iterator foundIt = s_Implementation->BankMap.find(bankName);
		if (foundIt != s_Implementation->BankMap.end()) return;

		FMOD::Studio::Bank* bank = nullptr;
		// TODO is &bank really correct?
		AudioEngine::ErrorCheck(s_Implementation->StudioSystem->loadBankFile(bankName.c_str(), flags, &bank));
		if(bank)
		{
			s_Implementation->BankMap[bankName] = bank;
		}
	}

	void AudioEngine::LoadEvent(const std::string eventName) {
		std::map<std::string, FMOD::Studio::EventInstance*>::iterator foundIt = s_Implementation->EventMap.find(eventName);
		if (foundIt != s_Implementation->EventMap.end()) return;

		FMOD::Studio::EventDescription* eventDescription = nullptr;
		AudioEngine::ErrorCheck(s_Implementation->StudioSystem->getEvent(eventName.c_str(), &eventDescription));
		if (eventDescription)
		{
			FMOD::Studio::EventInstance* eventInstance = nullptr;
			AudioEngine::ErrorCheck(eventDescription->createInstance(&eventInstance));
			if (eventInstance)
			{
				s_Implementation->EventMap[eventName] = eventInstance;
			}
		}
	}

	void AudioEngine::PlayEvent(const std::string eventName) {
		std::map<std::string, FMOD::Studio::EventInstance*>::iterator foundIt = s_Implementation->EventMap.find(eventName);
		if (foundIt == s_Implementation->EventMap.end())
		{
			LoadEvent(eventName);
			foundIt = s_Implementation->EventMap.find(eventName);
			if (foundIt == s_Implementation->EventMap.end()) return;
		}
		foundIt->second->start();
	}

	void AudioEngine::StopEvent(const std::string eventName, bool immediate) {
		std::map<std::string, FMOD::Studio::EventInstance*>::iterator foundIt = s_Implementation->EventMap.find(eventName);
		if (foundIt == s_Implementation->EventMap.end()) return;

		FMOD_STUDIO_STOP_MODE mode;
		mode = immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;

		AudioEngine::ErrorCheck(foundIt->second->stop(mode));
	}

	bool AudioEngine::IsEventPlaying(const std::string eventName) const {
		std::map<std::string, FMOD::Studio::EventInstance*>::iterator foundIt = s_Implementation->EventMap.find(eventName);
		if (foundIt == s_Implementation->EventMap.end()) return false;

		FMOD_STUDIO_PLAYBACK_STATE* state = nullptr;
		if (foundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) return true;
		return false;
	}

	// TODO change this to a return type?
	void AudioEngine::GetEventParameterValue(const std::string eventName, const std::string parameterName, float* parameter) {
		std::map<std::string, FMOD::Studio::EventInstance*>::iterator foundIt = s_Implementation->EventMap.find(eventName);
		if (foundIt == s_Implementation->EventMap.end()) return;

		float* parameterValue = nullptr;
		// CHECK Does this work?
		AudioEngine::ErrorCheck(foundIt->second->getParameterByName(parameterName.c_str(), parameterValue));
		// CHECK Does this work?
		parameter = parameterValue;
	}

	void AudioEngine::SetEventParameterValue(const std::string eventName, const std::string parameterName, float parameterValue) {
		std::map<std::string, FMOD::Studio::EventInstance*>::iterator foundIt = s_Implementation->EventMap.find(eventName);
		if (foundIt == s_Implementation->EventMap.end()) return;

		float* parameter = nullptr;
		AudioEngine::ErrorCheck(foundIt->second->getParameterByName(parameterName.c_str(), parameter));
	}

	FMOD_VECTOR AudioEngine::VectorToFmod(const utilStuff::Vector3 position) {
		FMOD_VECTOR fmodVector;
		fmodVector.x = position.x;
		fmodVector.y = position.y;
		fmodVector.z = position.z;
		return fmodVector;
	}

	float AudioEngine::DBToVolume(float db) {
		return powf(10.0f, 0.05f * db);
	}

	float AudioEngine::VolumeTodB(float volume) {
		return 20.0f * log10f(volume);
	}

	int AudioEngine::ErrorCheck(FMOD_RESULT result) {
		if (result != FMOD_OK)
		{
			std::cout << "FMOD ERROR " << result << std::endl;
			return 1;
		}
		return 0;
	}

	void AudioEngine::Shutdown() {
		delete s_Implementation;
	}


}