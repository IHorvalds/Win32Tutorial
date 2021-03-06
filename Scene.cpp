#include "Scene.h"

HRESULT Scene::CreateDeviceDependentResources()
{
	HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0), &m_pFill);

	if (SUCCEEDED(hr))
	{
		hr = m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0, 0, 0),
			D2D1::BrushProperties(),
			&m_pStroke
		);
	}

	return hr;
}

void Scene::DrawClockHand(float fHandLength, float fAngle, float fStrokeWith)
{
	m_pRenderTarget->SetTransform(
		D2D1::Matrix3x2F::Rotation(fAngle, m_ellipse.point)
	);

	D2D_POINT_2F endPoint = D2D1::Point2F(
		m_ellipse.point.x,
		m_ellipse.point.y - (m_ellipse.radiusY * fHandLength)
	);

	m_pRenderTarget->DrawLine(m_ellipse.point, endPoint, m_pStroke, fStrokeWith);
}

void Scene::RenderScene()
{
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue));

	m_pRenderTarget->FillEllipse(m_ellipse, m_pFill);
	m_pRenderTarget->DrawEllipse(m_ellipse, m_pStroke);

	for (DWORD i = 0; i < 12; ++i)
	{
		m_pRenderTarget->DrawLine(m_Ticks[i * 2], m_Ticks[i * 2 + 1], m_pStroke, 2.0f);
	}

	SYSTEMTIME time;
	GetLocalTime(&time);

	const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
	const float fMinuteAngle = (360.0f / 60) * (time.wMinute);

	const float fSecondAngle = (360.0f / 60) * (time.wSecond) + (360.0f / 60000) * (time.wMilliseconds);

	DrawClockHand(0.6f, fHourAngle,	  6);
	DrawClockHand(0.6f, fMinuteAngle, 4);
	DrawClockHand(0.6f, fSecondAngle, 1);

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Scene::CalculateLayout()
{
	D2D1_SIZE_F fSize = m_pRenderTarget->GetSize();

	const float x = fSize.width / 2.0f;
	const float y = fSize.height / 2.0f;
	const float radius = min(x, y);

	m_ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

	D2D1_POINT_2F pt1 = D2D1::Point2F(
		m_ellipse.point.x,
		m_ellipse.point.y - (m_ellipse.radiusY * 0.9f)
	);

	D2D1_POINT_2F pt2 = D2D1::Point2F(
		m_ellipse.point.x,
		m_ellipse.point.y - m_ellipse.radiusY
	);

	for (DWORD i = 0; i < 12; ++i)
	{
		D2D1::Matrix3x2F mat = D2D1::Matrix3x2F::Rotation((360.0f / 12) * i, m_ellipse.point);

		m_Ticks[i * 2] = mat.TransformPoint(pt1);
		m_Ticks[i * 2 + 1] = mat.TransformPoint(pt2);
	}
}

void Scene::DiscardDeviceDependentResources()
{
	m_pFill.Release();
	m_pStroke.Release();
}