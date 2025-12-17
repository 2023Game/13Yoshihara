#include "CPhysicsDebugDraw.h"
#include "Primitive.h"
#include <GLFW/glfw3.h>
#include "btBulletDynamicsCommon.h"

CPhysicsDebugDraw::CPhysicsDebugDraw()
	: mDebugMode(0)
{
}

void CPhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	// 描画開始
	glBegin(GL_LINES);

	// 色を設定
	glColor3f(color.getX(), color.getY(), color.getZ());

	// 始点と終点の頂点を描画
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());

	// 描画終了
	glEnd();
}

void CPhysicsDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& coolor)
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	CColor color = CColor::red;
	glColor3f(color.R(),color.G(),color.B());
	glVertex3f(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	glEnd();
	glPointSize(1.0f);

	drawLine(PointOnB, PointOnB + normalOnB * 0.5f, btVector3(0.0f, 1.0f, 0.0f));
}

void CPhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
	printf("Bullet Warning: %s\n", warningString);
}

void CPhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}

void CPhysicsDebugDraw::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int CPhysicsDebugDraw::getDebugMode() const
{
	return mDebugMode;
}
