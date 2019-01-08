#include "camera.h"
#include <QtMath>

Camera::Camera(float x, float y, float z, float pitch, float yaw)
    : position(QVector3D(x, y, z)), pitch(pitch), yaw(yaw)
{
    mouseSensitivity = .1f;
    updateVectors();
}

float Camera::getX()
{
    return position.x();
}

void Camera::setY(float val)
{
    position.setY(val);
}


float Camera::getY()
{
    return position.y();
}

void Camera::setZ(float val)
{
    position.setZ(val);
}

float Camera::getZ()
{
    return position.z();
}

QVector3D Camera::getPosition(){
    return position;
}

void Camera::ApplyGravity(float gravityForce)
{
    position.setZ(position.z() - gravityForce);
}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(position, position + front, up);
    return viewMatrix;
}

void Camera::processMovement(Direction dir, float dist)
{
    switch (dir)
    {
    case FORWARD:
        position += front * dist;
        break;
    case BACKWARD:
        position -= front * dist;
        break;
    case LEFT:
        position -= right * dist;
        break;
    case RIGHT:
        position += right * dist;
        break;
    case UP:
        position += up * dist;
        break;
    case DOWN:
        position -= up * dist;
        break;
    }
}

void Camera::processMouseMovement(float offset_x, float offset_z)
{
    offset_x *= mouseSensitivity;
    offset_z *= mouseSensitivity;
    pitch += offset_z;
    yaw += offset_x;
    if (pitch < -89.f) pitch = -89.f;
    if (pitch > 89.f) pitch = 89.f;
    updateVectors();
}

void Camera::updateVectors()
{
    front.setX(qCos(qDegreesToRadians(pitch)) * qCos((qDegreesToRadians(yaw))));
    front.setZ(qSin(qDegreesToRadians(pitch)));
    front.setY(qCos(qDegreesToRadians(pitch)) * qSin((qDegreesToRadians(yaw))));

    right = QVector3D::crossProduct(front, QVector3D(0,0,1)).normalized();
    up = QVector3D::crossProduct(right, front).normalized();
}
