#include "camera.h"

Camera::Camera()
{
	this->fov = 45;
	this->aspect = 1;
	this->near_plane = 0.01;
	this->far_plane = 10000;
	
	eye = Vector3(0, 10, 20);
	center = Vector3(0, 10, 0);
	up = Vector3(0, 1, 0);

	//here I gave you two matrices so you can develop them independently
	//pay attention that they are column-major order [column][row]
	view_matrix.setIdentity();
	projection_matrix.setIdentity();
	viewprojection_matrix.setIdentity();

	updateViewMatrix();
	updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
	//IMPLEMENT THIS using eye, center and up, store in this->view_matrix
	//Careful with the order of matrix multiplications, and be sure to use normalized vectors

	//ADD YOUR MATRIX PRECOMPUTATIONS HERE
	Vector3 S, F, T;
	F = (this->center - this->eye).normalize();
	S = (F.cross(this->up)).normalize();
	T = S.cross(F);

	//CHANGE THE MATRIX VALUES USING YOUR OWN
	//example view:   eye(0,10,20)  center(0,10,0)  up(0,1,0)
	view_matrix.setIdentity();
	view_matrix.M[0][0] = S.x; 	view_matrix.M[0][1] = T.x; view_matrix.M[0][2] = -F.x;  view_matrix.M[0][3] = 0.0;
	view_matrix.M[1][0] = S.y; 	view_matrix.M[1][1] = T.y; view_matrix.M[1][2] = -F.y;  view_matrix.M[1][3] = 0.0;
	view_matrix.M[2][0] = S.z; 	view_matrix.M[2][1] = T.z; view_matrix.M[2][2] = -F.z;  view_matrix.M[2][3] = 0.0;
	view_matrix.M[3][0] = 0.0; 	view_matrix.M[3][1] = 0.0; view_matrix.M[3][2] = 0.0;  view_matrix.M[3][3] = 1.0;

	//ADD ANY MATRIX POSTCOMPUTATIONS IF NECESSARY
	view_matrix.traslateLocal(-eye.x, -eye.y, -eye.z);

	//update the viewprojection_matrix
	viewprojection_matrix = view_matrix * projection_matrix;
}

void Camera::updateProjectionMatrix()
{
	//IMPLEMENT THIS using fov, aspect, near_plane and far_plane, store in this->projection_matrix
	//Careful with using degrees in trigonometric functions, must be radians, and use float types in divisions

	//ADD YOUR MATRIX PRECOMPUTATIONS HERE
	float rad_fov = this->fov * DEG2RAD;
	float f = 1 / tan(rad_fov / 2);


	//CHANGE THE MATRIX VALUES USING YOUR OWN
	//example projection:   fov: 60deg, aspect: 1.33333337, near_plane: 0.1, far_plane: 10000
	projection_matrix.setIdentity();
	projection_matrix.M[0][0] = f / aspect; projection_matrix.M[0][1] = 0.0; projection_matrix.M[0][2] = 0.0;  projection_matrix.M[0][3] = 0.0;
	projection_matrix.M[1][0] = 0.0; 	projection_matrix.M[1][1] = f; projection_matrix.M[1][2] = 0.0;  projection_matrix.M[1][3] = 0.0;
	projection_matrix.M[2][0] = 0.0; 	projection_matrix.M[2][1] = 0.0; projection_matrix.M[2][2] = (far_plane + near_plane) / (float)(near_plane - far_plane); projection_matrix.M[2][3] = -1.0;
	projection_matrix.M[3][0] = 0.0; 	projection_matrix.M[3][1] = 0.0; projection_matrix.M[3][2] = 2 * ((far_plane * near_plane) / (float)(near_plane - far_plane));  projection_matrix.M[3][3] = 0.0;

	//update the viewprojection_matrix
	viewprojection_matrix = view_matrix * projection_matrix;
}

Vector3 Camera::projectVector( Vector3 pos )
{
	Vector4 pos4 = Vector4(pos.x, pos.y, pos.z, 1.0);
	Vector4 result = viewprojection_matrix * pos4;
	return result.getVector3() / result.w;
}

void Camera::lookAt( Vector3 eye, Vector3 center, Vector3 up )
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	this->updateViewMatrix();
}

void Camera::perspective( float fov, float aspect, float near_plane, float far_plane )
{
	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	this->updateProjectionMatrix();
}

Matrix44 Camera::getViewProjectionMatrix()
{
	viewprojection_matrix = view_matrix * projection_matrix;
	return viewprojection_matrix ;
}

