#pragma once

extern ID2D1HwndRenderTarget* pRT;
extern ID2D1SolidColorBrush* pBrush;
extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;
class Renderer
{
public:


	float camWidth = 10;
	vec2 camPos;

	void drawBox(vec2 position, vec2 scale, float angle, RGBA color)
	{
		pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
		float modifiedScale = 0.5f * (float)SCREEN_WIDTH / camWidth;

		pRT->SetTransform(D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
			D2D1::Matrix3x2F::Rotation(angle) *
			D2D1::Matrix3x2F::Translation(0.5f * (FLOAT)SCREEN_WIDTH + (position.x - camPos.x) * modifiedScale, 0.5f * (FLOAT)SCREEN_HEIGHT - (position.y - camPos.y) * modifiedScale));

		pRT->FillRectangle(D2D1::RectF(-modifiedScale, -modifiedScale, modifiedScale, modifiedScale), pBrush);
	}

	void drawBox(vec2 position, vec2 scale, float angle, bool fill, RGBA color)
	{
		pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
		float modifiedScale = 0.5f * (float)SCREEN_WIDTH / camWidth;

		pRT->SetTransform(D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
			D2D1::Matrix3x2F::Rotation(angle) *
			D2D1::Matrix3x2F::Translation(0.5f * (FLOAT)SCREEN_WIDTH + (position.x - camPos.x) * modifiedScale, 0.5f * (FLOAT)SCREEN_HEIGHT - (position.y - camPos.y) * modifiedScale));

		if (fill)
			pRT->FillRectangle(D2D1::RectF(-modifiedScale, -modifiedScale, modifiedScale, modifiedScale), pBrush);
		else
			pRT->DrawRectangle(D2D1::RectF(-modifiedScale, -modifiedScale, modifiedScale, modifiedScale), pBrush);
	}

	void drawLine(vec2 position1, vec2 position2, float yScale, bool fill, RGBA color)
	{
		vec2 dp = position2 - position1;
		float len = dp.mag() * 0.5f;
		dp.normalize();

		vec2 mid = (position2 + position1) * 0.5f;
		float theta = atanf(-dp.y / dp.x) * 57.29577f;
		drawBox(mid, { len, yScale }, theta, color);

	}


	void drawCircle(vec2 pos, float radius, float angle, RGBA color, bool doDrawLine)
	{
		pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
		float modifiedScale = radius / camWidth;
		pRT->SetTransform(D2D1::Matrix3x2F::Scale(modifiedScale, modifiedScale) *
			D2D1::Matrix3x2F::Rotation(angle) *
			D2D1::Matrix3x2F::Translation(0.5f * (FLOAT)SCREEN_WIDTH + (pos.x - camPos.x) * (0.5f / camWidth) * SCREEN_WIDTH, 0.5f * (FLOAT)SCREEN_HEIGHT - (pos.y - camPos.y) * (0.5f / camWidth) * SCREEN_WIDTH));

		pRT->FillEllipse(D2D1::Ellipse({ 0.0F, 0.0F }, SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5F), pBrush);

		if (doDrawLine)
		{
			vec2 dest = pos;
			pos += {radius* cosf(0.01745 * angle), radius* sinf(-0.01745 * angle)};
			drawLine(pos, dest, 0.025f, true, { 0.0f,1.0f,0.0f,1.0f });
		}

	}

	void drawCapsule(vec2 pos, float radius, float height, float angle, RGBA color)
	{
		angle = -angle;
		vec2 p1 = pos;
		vec2 p2 = pos;
		p1 += {sinf(angle)* height, cosf(angle)* height};
		p2 += {-sinf(angle)* height, -cosf(angle)* height};
		drawCircle(p1, radius, angle, color, false);
		drawCircle(p2, radius, -angle, color, false);
		drawLine(p1, p2, radius, true, color);
		vec2 dest = p1;
		p1 += {radius* cosf(angle), radius* sinf(-angle)};
		drawLine(p1, dest, 0.025f, true, { 0.0f,1.0f,0.0f,1.0f });
	}

	void drawBoxCollider(const BoxCollider* bc, RGBA color)
	{
		pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
		drawBox(bc->position, bc->halfExtents, -57.2957795f * bc->angle, color);
	}

	void drawCircleCollider(const CircleCollider* cc, RGBA color)
	{
		pBrush->SetColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
		drawCircle(cc->position, cc->radius, -57.2957795f * cc->angle, color, true);
	}

	void drawUnitDirs(const vec2& position, const float& angle)
	{
		vec2 right = { cosf(angle), sinf(angle) };
		vec2 up = { -sinf(angle), cosf(angle) };
		drawLine(position, position + right, 0.025f, true, { 0,0,1,1 });
		drawLine(position, position + up, 0.025f, true, { 1,0,0,1 });
	}


};