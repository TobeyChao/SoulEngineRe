#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <sstream>

#include "../SoulEngineRe/SoulMain/Audio/IAudio.h"
#include "../SoulEngineRe/SoulMain/Audio/IAudioBuffer.h"
#include "../SoulEngineRe/SoulMain/Audio/SoundResourceManager.h"
#include "../SoulEngineRe/Platform/Win/Timer.h"
#include "../SoulEngineRe/App/Application.h"
#include "../SoulEngineRe/Input/DXInput/Input.h"
#include "../SoulEngineRe/SoulMain/Core/Mathematics/MathConsts.h"
#include "../SoulEngineRe/SoulMain/Scene/ParticleEmitter.h"
#include "../SoulEngineRe/SoulMain/Scene/SceneManager.h"
#include "../SoulEngineRe/SoulMain/Scene/GameObject.h"
#include "../SoulEngineRe/SoulMain/Scene/SubMesh.h"
#include "../SoulEngineRe/SoulMain/Scene/Light.h"
#include "../SoulEngineRe/SoulMain/Scene/Ray.h"
#include "../SoulEngineRe/SoulMain/Scene/SceneNodeRenderable.h"
#include "../SoulEngineRe/SoulMain/Scene/SceneNodeLight.h"
#include "../SoulEngineRe/SoulMain/Scene/SceneNodeCamera.h"
#include "../SoulEngineRe/SoulMain/Resource/TextureManager.h"
#include "../SoulEngineRe/SoulMain/RenderSystem/RenderWindow.h"
#include "../SoulEngineRe/SoulMain/RenderSystem/RenderSystem2D.h"

#ifdef _DEBUG
#pragma comment(lib, "../bin/SoulEngineRe/Debug/SoulEngineRe_d.lib")
#else
#pragma comment(lib, "../bin/SoulEngineRe/Release/SoulEngineRe.lib")
#endif

using namespace std;
using namespace Soul;