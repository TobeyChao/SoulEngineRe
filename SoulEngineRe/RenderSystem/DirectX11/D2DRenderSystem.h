#pragma once
#include "../../SoulMain/RenderSystem/RenderSystem2D.h"
#include "D3D11RenderWindow.h"
#include "D2DPCH.h"

namespace Soul
{
	class D2DRenderSystem : public RenderSystem2D
	{
	public:
		D2DRenderSystem(const D3D11RenderWindow& mD3D11RenderWindow);
		~D2DRenderSystem();
		bool Initialize() override;
		void DrawTextW(const std::wstring& text, const Core::SVector2& pos) override;
	private:
		const D3D11RenderWindow& mD3D11RenderWindow;

		Microsoft::WRL::ComPtr<ID2D1Factory> mFactory;
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> mRenderTarget;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mBrush;

		Microsoft::WRL::ComPtr<IDWriteFactory> mWriteFactory;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> mTextFormat;
	};
}