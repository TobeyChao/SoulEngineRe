#pragma once
namespace Soul
{
	class FrameEventReceiver
	{
	public:
		FrameEventReceiver() {};
		virtual ~FrameEventReceiver() {}
		// 当帧事件处理完成后返回true则继续传递消息，返回false则抛弃消息
		virtual bool FrameStarted() { return true; }
		virtual bool FrameUpdated() { return true; }
		virtual bool FrameEnd() { return true; }
	};
}