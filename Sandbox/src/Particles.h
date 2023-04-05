#pragma once
#include <glm/glm.hpp>



class ParticleSystem
{
public:
	ParticleSystem();

	void Update(float deltaTime);

	
public:
	static uint32_t Index;
};