#include <conio.h>
#include "dglib2.h"

#define _USE_MATH_DEFINES //謎のクソ仕様
#include <math.h>
#include <stdint.h>
#include <memory.h>

#define swap(x,y) x^=y^=x^=y

typedef _Bool bool;
typedef COLORREF Color;

const float M_RADIAN = M_PI / 180.0;
const unsigned long M_FPS = 1000 / 8;
const float M_FOV = 30.0 * M_PI_2 / 180.0;
const int M_WINDOWSIZE_X = 640;
const int M_WINDOWSIZE_Y = 480;

const float M_RADIAN_SP = M_PI / 360.0;

typedef struct {
	int x, y;
} Point; //template使いたい

typedef struct {
	float x, y;
} Vector2;

typedef struct {
	float x, y, z;
} Vector3;

void Vector3Reverse(Vector3* a) {
	a->x = -a->x;
	a->y = -a->y;
	a->z = -a->z;
};

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

void QuaternionMultiple(Quaternion* a, Quaternion* b, Quaternion* o) {
	float dot = Vector3Dot(&a->v, &b->v);
	o->t = a->t * b->t - dot;

	Vector3Cross(&a->v, &b->v, &o->v);
	Vector3 av, bv;
	Vector3Mul(&a->v, b->t, &av);
	Vector3Mul(&b->v, a->t, &bv);
	Vector3Add(&o->v, &a->v, &o->v);
	Vector3Add(&o->v, &b->v, &o->v);
};

void QuaternionConjugate(Quaternion* q) {
	q->v.x = -q->v.x;
	q->v.y = -q->v.y;
	q->v.z = -q->v.z;
};

void QuaternionRotate(float radian, Vector3* axis, Quaternion* out) {
	out->t = cosf(radian * 0.5);
	float msin = sinf(radian * 0.5);
	Vector3Mul(axis, msin, &out->v);
}; //axisは正規化する必要あり

void QuaternionApply(Quaternion* q, Vector3* vec) {
	Quaternion p = { 0.0, vec->x, vec->y, vec->z };
	Quaternion tmp;
	QuaternionConjugate(q);
	QuaternionMultiple(q, &p, &tmp);
	QuaternionConjugate(q);
	QuaternionMultiple(&tmp, q, &p);
	*vec = p.v;
}; //クォータニオンの回転をベクトルに反映させます



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

void Matrix4Scale(Matrix4* mat, Vector3* scale) {
	Matrix4 a = *mat;
	Matrix4 b = MATRIX4IDENTITY;
	b.v11 = scale->x;
	b.v22 = scale->y;
	b.v33 = scale->z;
	Matrix4Multiple(&b, &a, mat);
};

void Matrix4Translate(Matrix4* mat, Vector3* trans) {
	Matrix4 a = *mat;
	Matrix4 b = MATRIX4IDENTITY;
	b.v14 = trans->x;
	b.v24 = trans->y;
	b.v34 = trans->z;
	Matrix4Multiple(&b, &a, mat);
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
	Matrix4Multiple(&b, &a, mat);
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
	Matrix4Multiple(&b, &a, mat);
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
	Matrix4Multiple(&b, &a, mat);
};

void Matrix4RotateXYZ(Matrix4* mat, Vector3* rotate) {
	Matrix4RotateX(mat, rotate->x);
	Matrix4RotateY(mat, rotate->y);
	Matrix4RotateZ(mat, rotate->z);
};

void Matrix4Quaternion(Matrix4* mat, Quaternion* q) {
	Matrix4 m = *mat;
	Matrix4 quo = MATRIX4IDENTITY;
	
	float xx = 2.0 * q->v.x * q->v.x;
	float yy = 2.0 * q->v.y * q->v.y;
	float zz = 2.0 * q->v.z * q->v.z;

	float xy = 2.0 * q->v.x * q->v.y;
	float yz = 2.0 * q->v.y * q->v.z;
	float zx = 2.0 * q->v.z * q->v.x;

	float wx = 2.0 * q->t * q->v.x;
	float wy = 2.0 * q->t * q->v.y;
	float wz = 2.0 * q->t * q->v.z;

	quo.v11 -= yy + zz;
	quo.v12 += xy - wz;
	quo.v13 += zx + wz;

	quo.v21 += xy + wz;
	quo.v22 -= zz + xx;
	quo.v23 += yz - wx;

	quo.v31 += zx - wy;
	quo.v32 += yz + wx;
	quo.v33 -= xx + yy;

	Matrix4Multiple(&m, &quo, mat);
};

void Matrix4Apply(Matrix4* mat, Vector3* vec) {
	Vector3 v = *vec;
	vec->x = v.x * mat->v11 + v.y * mat->v12 + v.z * mat->v13 + mat->v14;
	vec->y = v.x * mat->v21 + v.y * mat->v22 + v.z * mat->v23 + mat->v24;
	vec->z = v.x * mat->v31 + v.y * mat->v32 + v.z * mat->v33 + mat->v34;
}

void Matrix4Zero(Matrix4* mat) {
	mat->v11 = 0.0; mat->v12 = 0.0;	mat->v13 = 0.0;	mat->v14 = 0.0;
	mat->v21 = 0.0; mat->v22 = 0.0;	mat->v23 = 0.0;	mat->v24 = 0.0;
	mat->v31 = 0.0; mat->v32 = 0.0;	mat->v33 = 0.0;	mat->v34 = 0.0;
	mat->v41 = 0.0; mat->v42 = 0.0;	mat->v43 = 0.0;	mat->v44 = 1.0;
};

void Projection(Vector3* pos, Point* out) {
	float fov = 1.0 / tan(M_FOV);
	out->x = (pos->x / pos->z) * fov * M_WINDOWSIZE_Y * 0.5;
	out->y = (pos->y / pos->z) * fov * M_WINDOWSIZE_Y * 0.5;
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

int SURFACE[6][4] = {
	{ 0, 1, 3, 2 },
	{ 1, 0, 4, 5 },
	{ 0, 2, 6, 4 },
	{ 4, 6, 7, 5 },
	{ 2, 3, 7, 6 },
	{ 1, 5, 7, 3 }
};

int SIDE[12][2] = {
	{ 0, 1 },
	{ 0, 2 },
	{ 0, 4 },
	{ 1, 3 },
	{ 1, 5 },
	{ 2, 3 },
	{ 2, 6 },
	{ 3, 7 },
	{ 4, 5 },
	{ 4, 6 },
	{ 5, 7 },
	{ 6, 7 }
};

bool SideJudge(Vector3* ab, Vector3* bc, Vector3* v) {
	Vector3 cross;
	Vector3Cross(ab, bc, &cross);
	float dot = Vector3Dot(&cross, v);
	return dot > 0.0;

}; //ポリゴンの裏表判定 表ならtrue



unsigned long frame; //総フレーム数
char_far gvram; //1st buffer
char_far dvram; //2nd buffer
unsigned long starttime; //game start time

void DGLine(Point* a, Point* b, Color color) {
	dg_line(dvram, a->x + M_WINDOWSIZE_X / 2, a->y + M_WINDOWSIZE_Y / 2, b->x + M_WINDOWSIZE_X / 2, b->y + M_WINDOWSIZE_Y / 2, color);
}

//グローバル変数使うの糞コードな気しかしない
//名前空間がないクソ言語
Vector3 camorg = { 0.0, 0.0, 10.0 };
Vector3 camdir = { 0.0, 0.0, 0.0 };
Vector2 diceloc = { 0.0, 0.0 };
Quaternion dicerot = { 0.0, 0.0, 0.0, 0.0 };

void RenderingDice() {
	int i, j, k;


	Matrix4 matrix = MATRIX4IDENTITY;
	Matrix4Quaternion(&matrix, &dicerot);

	//Matrix4RotateY(&matrix, frame * -0.01);
	Matrix4Translate(&matrix, &diceloc);

	Vector3 model[8];
	memcpy(model, CUBE, sizeof(Vector3) * 8);

	for (i = 0; i < 8; i++) { //行列によるワールド座標への変換
		Matrix4Apply(&matrix, &model[i]);
	}

	

	uint8_t surfaceflag = 0; //bool[6]

	for (i = 0; i < 6; i++) {
		Vector3* a = &model[SURFACE[i][0]];
		Vector3* b = &model[SURFACE[i][1]];
		Vector3* c = &model[SURFACE[i][2]];
		Vector3* d = &model[SURFACE[i][3]];

		Vector3 ab, bc, ray;
		Vector3Sub(b, a, &ab);
		Vector3Sub(c, b, &bc);
		Vector3Sub(d, &camorg, &ray);

		if (SideJudge(&ab, &bc, &ray)) {
			surfaceflag |= (1 << i);
		}
	}
	dg_printf(0, 0, RGB(00, 255, 0), "%d", surfaceflag);
	//surfaceflag = (frame / 60) % 127;

	uint16_t sideflag = 0; //bool[12]

	for (i = 0; i < 6; i++) {
		if (surfaceflag & (1 << i)) {
			int ss[4];
			memcpy(ss, &SURFACE[i], sizeof(int) * 4);
			for (j = 0; j < 4; j++) {
				int a = ss[j];
				int b = ss[(j + 1) % 4];
				if(a > b) swap(a, b);
				for (k = 0; k < 12; k++) {
					if (SIDE[k][0] == a && SIDE[k][1] == b) {
						sideflag |= 1 << k;
						break;
					}
				}
			}
		}
	}

	matrix = MATRIX4IDENTITY;


	Vector3Reverse(&camorg);
	Matrix4Translate(&matrix, &camorg);
	Vector3Reverse(&camorg);
	Matrix4RotateXYZ(&matrix, &camdir);

	for (i = 0; i < 8; i++) { //行列によるビュー座標への変換
		Matrix4Apply(&matrix, &model[i]);
	}

	Point pts[8];

	for (i = 0; i < 8; i++) {
		Projection(&model[i], &pts[i]);
	}

	for (i = 0; i < 12; i++) {
		if (sideflag & 1 << i) {
			DGLine(&pts[SIDE[i][0]], &pts[SIDE[i][1]], RGB(255, 255, 255));

		}
	}
};

void RenderingAxis() {

}

void Rendering() {
	RenderingDice();
};

void Init() {
	starttime = timeGetTime();
	dg_setscreen(0, 0, M_WINDOWSIZE_X, M_WINDOWSIZE_Y, 0, "Dice Simulator");
	dvram = dg_createbmp(M_WINDOWSIZE_X, M_WINDOWSIZE_Y);
};

void Input() {
	Matrix4 matrix = MATRIX4IDENTITY;

	if (getch3(VK_DOWN))  camdir.x += M_RADIAN_SP;
	if (getch3(VK_UP))    camdir.x -= M_RADIAN_SP;
	if (getch3(VK_RIGHT)) camdir.y += M_RADIAN_SP;
	if (getch3(VK_LEFT))  camdir.y -= M_RADIAN_SP;

	dg_printf(0, 40, RGB(0, 255, 0), "%f %f %f", camorg.x, camorg.y, camorg.z);

	dg_printf(0, 60, RGB(0, 255, 0), "%f %f", camdir.x, camdir.y);

	Matrix4RotateY(&matrix, -camdir.y);
	Matrix4RotateX(&matrix, -camdir.x);

	camorg.x = 0.0;
	camorg.y = 0.0;
	camorg.z = -10.0;
	
	Matrix4Apply(&matrix, &camorg);
}

int main() {
	Init();
	do {
		ML();
		if (timeGetTime() - starttime < M_FPS) continue;
		dg_gcls(dvram, RGB(0, 0, 0));
		Input();
		Rendering();
		
		dg_drawbmp(gvram, 0, 0, M_WINDOWSIZE_X, M_WINDOWSIZE_Y, dvram); //write double buffer
		frame++;
	} while (!getch3(0x1b)); //esc close
};

#pragma warning ( disable : 4716 ) //C99 5.1.2.2.3 Program termination
