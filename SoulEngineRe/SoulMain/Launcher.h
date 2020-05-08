#pragma once
#include "SoulPCH.h"
namespace Soul
{
	class Launcher : public Singleton<Launcher>
	{
	public:
		explicit Launcher(const std::string& moduleConfig);
		~Launcher();
		virtual RenderWindow* Initialize(const std::string& initConfig);

		void AddFrameEventReceiver(FrameEventReceiver* frameEvtReceiver);
		void RemoveFrameEventReceiver(FrameEventReceiver* frameEvtReceiver);
		// 运行，由main函数调用
		void Run();
		// 渲染帧，内部游戏循环每帧调用
		bool RenderFrame();
		// 通知帧开始
		bool FireFrameStarted();
		// 更新帧，内部游戏循环每帧调用
		bool UpdateFrame();
		// 通知更新完毕
		bool FireFrameUpdated();
		// 通知帧渲染完成
		bool FireFrameEnded();
		// 创建渲染窗口
		RenderWindow* CreateRenderWindow(const json& config);
		// 创建场景管理器
		SceneManager* CreateSceneManager();
		/***********AudioSystem Start************/
		// 增加音频系统
		void AddAudioSystem(IAudio* audioSystem);
		// 获取当前的音频系统
		IAudio* GetActiveAudioSystem() const;
		// 获取音频系统通过名字
		IAudio* GetAudioSystemByName(std::string audioSystemName);
		/************AudioSystem End*************/
		/***********RenderSystem Start************/
		void AddRenderSystem(RenderSystem* renderSystem);
		RenderSystem* GetActiveRenderSystem() const;
		RenderSystem* GetRenderSystemByName(std::string name);
		/***********RenderSystem End**************/
	private:
		/***********Module Start**************/
		void LoadModules(const std::string& moduleConfig);
		// 在析构函数中调用
		void UnLoadModules();
		void AddModule(Module* module);
		void RemoveModule(Module* module);
		/************Module End***************/
		// 同步帧事件接收者
		void SyncFrameEventReceiver();

	private:
		// 加载到的所有的模组
		std::vector<Module*> mModules;
		// 渲染系统列表，在initialize函数中填充，由module_config.json文件指定
		std::vector<RenderSystem*> mRenderSystems;
		// 当前激活的渲染系统，在Initialize函数中赋值，由boot_config.json文件指定
		RenderSystem* mActiveRenderSystem;
		// 帧事件接收者
		std::set<FrameEventReceiver*> mFrameEventReceivers;
		std::set<FrameEventReceiver*> mRemovedFrameEventReceivers;
		std::set<FrameEventReceiver*> mAddedFrameEventReceiver;
		// 音频系统列表
		std::vector<IAudio*> mAudioSystems;
		// 当前激活的音频系统
		IAudio* mActiveAudioSystems;
		// 音频资源管理器
		SoundResourceManager* mSoundResourceManager;
		// 资源组管理器
		ResourceGroupManager* mResourceGroupManager;
		// 模组设置
		std::string mModuleConfig;
		// 控制游戏循环
		bool mQuit;
	};
}
