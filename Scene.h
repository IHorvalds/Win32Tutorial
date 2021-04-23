#pragma once

#include "GraphicsScene.h"

class Scene : public GraphicsScene
{
	CComPtr<ID2D1SolidColorBrush> m_pFill;
	CComPtr<ID2D1SolidColorBrush> m_pStroke;

	D2D1_ELLIPSE		m_ellipse;
	D2D_POINT_2F		m_Ticks[24];

	HRESULT CreateDeviceIndependentResources() { return S_OK; }
	void	DiscardDeviceIndependentResources() { }
	HRESULT CreateDeviceDependentResources();
	void	DiscardDeviceDependentResources();
	void	CalculateLayout();
	void	RenderScene();

	void	DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
};

