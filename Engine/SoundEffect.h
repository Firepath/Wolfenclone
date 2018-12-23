#pragma once

#include <random>

#include "Sound.h"

class SoundEffect
{
public:
	virtual void Play( Sound& sound )
	{
		sound.Play();
	}
};

class FrequencyVariationSoundEffect : public SoundEffect
{
public:
	FrequencyVariationSoundEffect( float stdDev, unsigned int seed )
		:
		rng( seed ),
		freqDist( 1.0f, stdDev )
	{}
	void Play( Sound& sound ) override
	{
		sound.Play( freqDist( rng ) );
	}
private:
	std::mt19937 rng;
	std::normal_distribution<float> freqDist;
};