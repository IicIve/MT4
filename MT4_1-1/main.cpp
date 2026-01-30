#include <Novice.h>
#include <cmath>
#include <cassert>
#include <imgui.h>
#include <algorithm>
#include <string>
#include <numbers>

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;
};

const char kWindowTitle[] = "LE2C_26";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 Normalize(Vector3 v1);
	void MatrixScreenPrintf(int x, int y, const Matrix4x4 & matrix, const char label[]);
	//Matrix4x4 MakeRotateAxisAngle(const Vector3 & axis, float angle);
	Matrix4x4 DirectionToDirection(const Vector3 & from, const Vector3 & to);

	Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f, 0.5f });
	Vector3 to0 = Vector3{ -from0.x, -from0.y, -from0.z };
	Vector3 from1 = Normalize(Vector3{ -0.6f, 0.9f, 0.2f });
	Vector3 to1 = Normalize(Vector3{ 0.4f, 0.7f, -0.5f });
	Matrix4x4 rotateMatrix0 = DirectionToDirection(
		Normalize(Vector3{ 1.0f,0.0f,0.0f }), Normalize(Vector3{ -1.0f,0.0f, 0.0f })
	);
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

	

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, 100, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, 200, rotateMatrix2, "rotateMatrix2");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	// 左手系クロス（右手系とは Y の符号が逆になる）
	return {
		v1.y * v2.z - v1.z * v2.y,
		-(v1.x * v2.z - v1.z * v2.x),
		v1.x * v2.y - v1.y * v2.x
	};
}

float Length(const Vector3& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(Vector3 v1) {
	Vector3 result;

	result.x = v1.x / std::sqrtf(Dot(v1, v1));
	result.y = v1.y / std::sqrtf(Dot(v1, v1));
	result.z = v1.z / std::sqrtf(Dot(v1, v1));

	return result;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result = {};

	for (int i = 0; i < 4; ++i) {
		result.m[i][i] = 1.0f;
	}

	return result;
}



Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {

	Vector3 n = Normalize(axis);

	float c = cosf(angle);
	float s = sinf(angle); // ← 左手系では -s
	float oneMinusC = 1.0f - c;

	Matrix4x4 R{};

	R.m[0][0] = c + n.x * n.x * oneMinusC;
	R.m[0][1] = n.x * n.y * oneMinusC - n.z * s;     // -n.z*s
	R.m[0][2] = n.x * n.z * oneMinusC + n.y * s;     // +n.y*s
	R.m[0][3] = 0;

	R.m[1][0] = n.y * n.x * oneMinusC + n.z * s;     // +n.z*s
	R.m[1][1] = c + n.y * n.y * oneMinusC;
	R.m[1][2] = n.y * n.z * oneMinusC - n.x * s;     // -n.x*s
	R.m[1][3] = 0;

	R.m[2][0] = n.z * n.x * oneMinusC - n.y * s;     // -n.y*s
	R.m[2][1] = n.z * n.y * oneMinusC + n.x * s;     // +n.x*s
	R.m[2][2] = c + n.z * n.z * oneMinusC;
	R.m[2][3] = 0;

	R.m[3][0] = R.m[3][1] = R.m[3][2] = 0;
	R.m[3][3] = 1;

	return R;
}



Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	//// 右手系！軸は Cross(f, t)
	Vector3 f = Normalize(from);
	Vector3 t = Normalize(to);
	//Vector3 axis = Normalize(Cross(f, t));

	const float EPS = 1e-6f;
	const float PI = std::numbers::pi_v<float>;

	float dotValue = std::clamp(Dot(f, t), -1.0f, 1.0f);
	//float angle = std::acos(dotValue);

	// 同方向
	if (dotValue > 1.0f - EPS) {
		return MakeIdentity4x4();
	}


	// 反対方向（180度）
	if (dotValue < -1.0f + EPS) {
		// f と直交する安定した軸を選ぶ
		Vector3 axis;
		if (fabsf(f.x) < fabsf(f.z)) {
			axis = Normalize(Cross(f, { 1,0,0 }));
		} else {
			axis = Normalize(Cross(f, { 0,0,1 }));
		}

		return MakeRotateAxisAngle(axis, PI);
	}

	// 通常ケース
	Vector3 axis = Normalize(Cross(f, t)); // 左手系
	float angle = std::acos(dotValue);
	return MakeRotateAxisAngle(axis, angle);

	
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char label[]) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + 20 + row * kRowHeight, "%6.03f", matrix.m[row][column]
			);
		}
	}

}
