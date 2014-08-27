#include <conio.h>
#include "dglib2.h"

#define _USE_MATH_DEFINES //謎のクソ仕様
#include <math.h>
#include <stdint.h>

const float M_RADIAN = M_PI / 180.0;
const unsigned long M_FPS = 1000 / 15;
const float M_FOV = 60.0 * M_PI_2 / 180.0;
const int M_WINDOWSIZE_X = 640;
const int M_WINDOWSIZE_Y = 480;

typedef struct {
	int x, y;
} Point; //template使いたい

typedef struct {
	float x, y;
} Vector2;

typedef struct {
	float x, y, z;
} Vector3;

void Vector3Add(Vector3* a, Vector3* b, Vector3* o) {
	o->x = a->x + b->x;
	o->y = a->y + b->y;
	o->z = a->z + b->z;
};

void Vector3Sub(Vector3* a, Vector3* b, Vector3* o) {
	o->x = a->x - b->x;
	o->y = a->y - b->y;
	o->z = a->z - b->z;
};

void Vector3Mul(Vector3* a, float b, Vector3* o) {
	o->x = a->x * b;
	o->y = a->y * b;
	o->z = a->z * b;
};

void Vector3Cross(Vector3* a, Vector3* b, Vector3* o) {
	o->x = a->y * b->z - a->z * b->y;
	o->y = a->z * b->x - a->x * b->z;
	o->z = a->x * b->y - a->y * b->x;
};

float Vector3Dot(Vector3* a, Vector3* b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
};

float Vector3NormGet(Vector3* vec) {
	return sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
};

void Vector3Normalize(Vector3* vec) {
	float f = Vector3NormGet(vec);
	vec->x *= f;
	vec->y *= f;
	vec->z *= f;
};



typedef struct {
	float t;
	Vector3 v;
} Quaternion;

void QuaternionMultipe(Quaternion* a, Quaternion* b, Quaternion* o) {
	float dot = Vector3Dot(&a->v, &b->v);
	o->t = a->t * b->t - dot;

	Vector3Cross(&a->v, &b->v, &o->v);
	Vector3 av, bv;
	Vector3Mul(&a->v, b->t, &av);
	Vector3Mul(&b->v, a->t, &bv);
	Vector3Add(&o->v, &a->v, &o->v);
	Vector3Add(&o->v, &b->v, &o->v);
};



typedef struct {
	float v11, v12, v13, v14;
	float v21, v22, v23, v24;
	float v31, v32, v33, v34;
	float v41, v42, v43, v44;
} Matrix4;

const Matrix4 MATRIX4IDENTITY = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

void Matrix4Identity(Matrix4* mat) {
	*mat = MATRIX4IDENTITY;
};

void Matrix4Multiple(Matrix4* a, Matrix4* b, Matrix4* o) {
	o->v11 = a->v11 * b->v11 + a->v12 * b->v21 + a->v13 * b->v31 + a->v14 * b->v41;
	o->v12 = a->v11 * b->v12 + a->v12 * b->v22 + a->v13 * b->v32 + a->v14 * b->v42;
	o->v13 = a->v11 * b->v13 + a->v12 * b->v23 + a->v13 * b->v33 + a->v14 * b->v43;
	o->v14 = a->v11 * b->v14 + a->v12 * b->v24 + a->v13 * b->v34 + a->v14 * b->v44;
	o->v21 = a->v21 * b->v11 + a->v22 * b->v21 + a->v23 * b->v31 + a->v24 * b->v41;
	o->v22 = a->v21 * b->v12 + a->v22 * b->v22 + a->v23 * b->v32 + a->v24 * b->v42;
	o->v23 = a->v21 * b->v13 + a->v22 * b->v23 + a->v23 * b->v33 + a->v24 * b->v43;
	o->v24 = a->v21 * b->v14 + a->v22 * b->v24 + a->v23 * b->v34 + a->v24 * b->v44;
	o->v31 = a->v31 * b->v11 + a->v32 * b->v21 + a->v33 * b->v31 + a->v34 * b->v41;
	o->v32 = a->v31 * b->v12 + a->v32 * b->v22 + a->v33 * b->v32 + a->v34 * b->v42;
	o->v33 = a->v31 * b->v13 + a->v32 * b->v23 + a->v33 * b->v33 + a->v34 * b->v43;
	o->v34 = a->v31 * b->v14 + a->v32 * b->v24 + a->v33 * b->v34 + a->v34 * b->v44;
	o->v41 = a->v41 * b->v11 + a->v42 * b->v21 + a->v43 * b->v31 + a->v44 * b->v41;
	o->v42 = a->v41 * b->v12 + a->v42 * b->v22 + a->v43 * b->v32 + a->v44 * b->v42;
	o->v43 = a->v41 * b->v13 + a->v42 * b->v23 + a->v43 * b->v33 + a->v44 * b->v43;
	o->v44 = a->v41 * b->v14 + a->v42 * b->v24 + a->v43 * b->v34 + a->v44 * b->v44;
	//車輪の再発明ーーーーーーーーーぐるぐるーーーーーーーーーー
};

void Matrix4Scale(Matrix4* mat, Vector3 scale) {
	Matrix4 a = *mat;
	Matrix4 b = MATRIX4IDENTITY;
	b.v11 = scale.x;
	b.v22 = scale.y;
	b.v33 = scale.z;
	Matrix4Multiple(&a, &b, mat);
};

void Matrix4Translate(Matrix4* mat, Vector3 trans) {
	Matrix4 a = *mat;
	Matrix4 b = MATRIX4IDENTITY;
	b.v41 = trans.x;
	b.v42 = trans.y;
	b.v43 = trans.z;
	Matrix4Multiple(&a, &b, mat);
};

void Matrix4RotateX(Matrix4* mat, float radian) {
	float msin = sinf(radian);
	float mcos = cosf(radian);

	Matrix4 a = *mat;
	Matrix4 b = MATRIX4IDENTITY;
	b.v22 = +mcos;
	b.v23 = -msin;
	b.v32 = +msin;
	b.v33 = +mcos;
	Matrix4Multiple(&a, &b, mat);
};

void Matrix4RotateY(Matrix4* mat, float radian) {
	float msin = sinf(radian);
	float mcos = cosf(radian);

	Matrix4 a = *mat;
	Matrix4 b = MATRIX4IDENTITY;
	b.v11 = +mcos;
	b.v13 = +msin;
	b.v31 = -msin;
	b.v33 = +mcos;
	Matrix4Multiple(&a, &b, mat);
};

void Matrix4RotateZ(Matrix4* mat, float radian) {
	float msin = sinf(radian);
	float mcos = cosf(radian);

	Matrix4 a = *mat;
	Matrix4 b = MATRIX4IDENTITY;
	b.v11 = +mcos;
	b.v12 = -msin;
	b.v21 = +msin;
	b.v22 = +mcos;
	Matrix4Multiple(&a, &b, mat);
};

void Matrix4RotateXYZ(Matrix4* mat, Vector3* rotate) {
	Matrix4RotateX(mat, rotate->x);
	Matrix4RotateY(mat, rotate->y);
	Matrix4RotateZ(mat, rotate->z);
};

void Matrix4Zero(Matrix4* mat) {
	mat->v11 = 0.0; mat->v12 = 0.0;	mat->v13 = 0.0;	mat->v14 = 0.0;
	mat->v21 = 0.0; mat->v22 = 0.0;	mat->v23 = 0.0;	mat->v24 = 0.0;
	mat->v31 = 0.0; mat->v32 = 0.0;	mat->v33 = 0.0;	mat->v34 = 0.0;
	mat->v41 = 0.0; mat->v42 = 0.0;	mat->v43 = 0.0;	mat->v44 = 1.0;
};

void Projection(Vector3* pos, Point* out) {
	out->x = pos->x / pos->z * M_FOV * M_WINDOWSIZE_X * 0.5;
	out->y = pos->y / pos->z * M_FOV * M_WINDOWSIZE_Y * 0.5;
};

Vector3 CUBE[8] = {
	{ +1.0, +1.0, +1.0 },
	{ +1.0, +1.0, -1.0 },
	{ +1.0, -1.0, +1.0 },
	{ +1.0, -1.0, -1.0 },
	{ -1.0, +1.0, +1.0 },
	{ -1.0, +1.0, -1.0 },
	{ -1.0, -1.0, +1.0 },
	{ -1.0, -1.0, -1.0 }
};



unsigned long frame; //総フレーム数
char_far gvram; //1st buffer
char_far dvram; //2nd buffer
unsigned long starttime; //game start time

//グローバル変数使うの糞コードな気しかしない
//名前空間がないクソ言語
Vector3 camorg = { 0.0, 0.0, -5.0 };
Vector3 camdir = { 0.0, 0.0, +1.0 };
Matrix4 dicelocation;

void RenderingDice() {
	Matrix4 matrix = MATRIX4IDENTITY;

	uint8_t surfaceflag = 0; //bool[6]

};

void Rendering() {
	camorg = 
	RenderingDice();
};

void Init() {
	starttime = timeGetTime();
	dg_setscreen(0, 0, M_WINDOWSIZE_X, M_WINDOWSIZE_Y, 0, "Dice Simulator");
	dvram = dg_createbmp(M_WINDOWSIZE_X, M_WINDOWSIZE_Y);
	Matrix4Identity(&dicelocation);
};

int main() {
	Init();
	do {
		ML();
		if (timeGetTime() - starttime < M_FPS) continue;
		dg_gcls(dvram, RGB(0, 0, 0));

		Rendering();
		Matrix4RotateY(&dicelocation, 0.01); //debug
		dg_drawbmp(gvram, 0, 0, M_WINDOWSIZE_X, M_WINDOWSIZE_Y, dvram); //write double buffer
		frame++;
	} while (!getch3(0x1b)); //esc close
};

#pragma warning ( disable : 4716 ) //C99 5.1.2.2.3 Program termination
