#include <Novice.h>
#include <cmath>
#include <cassert>
#include <imgui.h>
#include <algorithm>
#include <string>

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

const char kWindowTitle[] = "LE2C_26_ミヤシタツナグ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//Vector3 Normalize(Vector3 v1);
	void QuaternionScreenPrintf(int x, int y, const Quaternion & quaternion, const char* label);
	void MatrixScreenPrintf(int x, int y, const Matrix4x4 & matrix, const char label[]);
	//Matrix4x4 MakeRotateAxisAngle(const Vector3 & axis, float angle);

	//Quaternionの積
	Quaternion Multiply(const Quaternion & lhs, const Quaternion & rhs);
	//単位Quaternionを返す
	Quaternion IdentityQuaternion();
	//共役Quaternionを返す
	Quaternion Conjugate(const Quaternion & quaternion);
	//Quaternionのnormを返す
	float Norm(const Quaternion & quaternion);
	//正規化したQuaternionを返す
	Quaternion Normalize(const Quaternion & quaternion);
	//逆Quaternionを返す
	Quaternion Inverse(const Quaternion & quaternion);

	Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
	Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };
	Quaternion qIdentity = IdentityQuaternion();
	Quaternion qConj = Conjugate(q1);
	Quaternion qInv = Inverse(q1);
	Quaternion qNormalize = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float qNorm = Norm(q1);

	/*Vector3 axis = Normalize({ 1.0f, 1.0f, 1.0f });
	float angle = 0.44f;
	Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);*/

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

		QuaternionScreenPrintf(0, 0, qIdentity, "Identity");
		QuaternionScreenPrintf(0, 20, qConj, "Conjugate");
		QuaternionScreenPrintf(0, 40, qInv, "Inverse");
		QuaternionScreenPrintf(0, 60, qNormalize, "Normalize");
		QuaternionScreenPrintf(0, 80, mul1, "Multiply(q1,q2)");
		QuaternionScreenPrintf(0, 100, mul2, "Multiply(q2,q1)");
		Novice::ScreenPrintf(0, 120, "%.2f                      Norm", qNorm);

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

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	// 軸の正規化
	Vector3 n = axis;
	float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
	if (len != 0.0f) {
		n.x /= len;
		n.y /= len;
		n.z /= len;
	}

	float c = std::cos(angle);
	float s = std::sin(angle);
	float oneMinusC = 1.0f - c;

	// S = I * cosθ
	Matrix4x4 S = {};
	S.m[0][0] = c;
	S.m[1][1] = c;
	S.m[2][2] = c;
	S.m[3][3] = 0.0f;

	// P = (1 - cosθ) * a a^T
	Matrix4x4 P = {};
	P.m[0][0] = n.x * n.x * oneMinusC;
	P.m[0][1] = n.x * n.y * oneMinusC;
	P.m[0][2] = n.x * n.z * oneMinusC;

	P.m[1][0] = n.y * n.x * oneMinusC;
	P.m[1][1] = n.y * n.y * oneMinusC;
	P.m[1][2] = n.y * n.z * oneMinusC;

	P.m[2][0] = n.z * n.x * oneMinusC;
	P.m[2][1] = n.z * n.y * oneMinusC;
	P.m[2][2] = n.z * n.z * oneMinusC;

	P.m[3][3] = 0.0f;

	// C = [a]_× sinθ
	Matrix4x4 C = {};
	C.m[0][0] = 0.0f;
	C.m[0][1] = n.z * s;    // ← 符号を反転
	C.m[0][2] = -n.y * s;

	C.m[1][0] = -n.z * s;
	C.m[1][1] = 0.0f;
	C.m[1][2] = n.x * s;

	C.m[2][0] = n.y * s;
	C.m[2][1] = -n.x * s;
	C.m[2][2] = 0.0f;

	C.m[3][3] = 1.0f;

	// R = S + P + C
	Matrix4x4 R = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			R.m[i][j] = S.m[i][j] + P.m[i][j] + C.m[i][j];
		}
	}

	return R;

}

float Dot(const Vector3& v1, const Vector3& v2) {
	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;
}

Vector3 Normalize(Vector3 v1) {
	Vector3 result;

	result.x = v1.x / std::sqrtf(Dot(v1, v1));
	result.y = v1.y / std::sqrtf(Dot(v1, v1));
	result.z = v1.z / std::sqrtf(Dot(v1, v1));

	return result;
}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	Quaternion result;

	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;

	return result;
}

Quaternion IdentityQuaternion() {
	return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion Conjugate(const Quaternion& quaternion) {
	return Quaternion{
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		 quaternion.w
	};
}

Quaternion Inverse(const Quaternion& quaternion) {
	// ノルム（長さの二乗）を求める
	float norm = quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w;

	// 共役を取る
	Quaternion conjugate = Conjugate(quaternion);

	// 正規化されていない場合に対応（norm = 1 なら単位クォータニオンなので共役と同じ）
	if (norm == 0.0f) {
		return Quaternion{ 0.0f, 0.0f, 0.0f, 0.0f }; // エラー回避（ゼロ除算防止）
	}

	float invNorm = 1.0f / norm;

	return Quaternion{
		conjugate.x * invNorm,
		conjugate.y * invNorm,
		conjugate.z * invNorm,
		conjugate.w * invNorm
	};
}

Quaternion Normalize(const Quaternion& quaternion) {
	// ノルム（長さ）を求める
	float length = std::sqrt(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);

	// 長さがゼロならそのまま返す（ゼロ除算防止）
	if (length == 0.0f) {
		return Quaternion{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	float invLength = 1.0f / length;

	// 各成分を割って単位化
	return Quaternion{
		quaternion.x * invLength,
		quaternion.y * invLength,
		quaternion.z * invLength,
		quaternion.w * invLength
	};
}

float Norm(const Quaternion& quaternion) {
	return std::sqrt(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
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

void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label) {

	Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", quaternion.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", quaternion.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.02f", quaternion.w);
	Novice::ScreenPrintf(x + kColumnWidth * 4, y, "%s", label);
}
