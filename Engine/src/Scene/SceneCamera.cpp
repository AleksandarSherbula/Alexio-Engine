#include "aio_pch.h"
#include "SceneCamera.h"

namespace Alexio
{
	SceneCamera::SceneCamera()
	{
		mOrthographicSize = 10.0f;
		mAspectRatio = 0.0f;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size)
	{
		mOrthographicSize = size;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(float width, float height)
	{
		mAspectRatio = width / height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -mOrthographicSize * mAspectRatio;
		float orthoRight = mOrthographicSize * mAspectRatio;
		float orthoBottom = -mOrthographicSize;
		float orthoTop = mOrthographicSize;

		mProjection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop);
	}
}