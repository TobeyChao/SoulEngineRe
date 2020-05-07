#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <map>
#include <queue>
#include <set>
#include <fstream>
#include <sstream>
#include "Utils\UtilsPCH.h"

namespace Soul
{
	class Launcher;
	// Res
	class IResourceExtraData;
	class IResourceLoader;
	class Resource;
	class ResourceGroupManager;
	class ResourceManager;
	class TextureManager;
	class WaveResourceLoader;
	// Audio
	class Audio;
	class AudioBuffer;
	class IAudio;
	class IAudioBuffer;
	class SoundResourceExtraData;
	class SoundResourceManager;

	class GPUBufferManager;
	class ShaderManager;
	// Scene
	class BoundingBox;
	class Frustum;
	class GameObject;
	class GeometryGenerator;
	class ParticleEmitter;
	class SubMesh;
	class ParticalList;
	class Light;
	class SceneNodeRenderable;
	class SceneNodeLight;
	class SceneNodeCamera;
	class SceneNode;
	class SceneManager;
	enum class LIGHT_TYPE;
	enum class SIMPLE_GAMEOBJECT;
	enum class E_SCENENODE_TYPES;

	class FrameEventReceiver;
	class GPUBuffer;
	class ITexture;
	class RenderParameter;
	class Shader;
	class RenderSystem;
	class RenderSystem2D;
	class RenderTarget;
	class RenderWindow;
	class Viewport;
	enum class GPU_BUFFER_TYPE;

	class Module;

	class SColorf;
}