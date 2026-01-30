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

	void QuaternionScreenPrintf(int x, int y, const Quaternion & quaternion, const char* label);
	void MatrixScreenPrintf(int x, int y, const Matrix4x4 & matrix, const char label[]);
	void VectorScreenPrintf(int x, int y, const Vector3 & vector, const char label[]);
	//Matrix4x4 MakeRotateAxisAngle(const Vector3 & axis, float angle);
	Vector3 Normalize(Vector3 v1);
	Vector3 Transform(const Vector3 & vector, const Matrix4x4 & matrix);

	//Quaternionの積
	//Quaternion Multiply(const Quaternion & lhs, const Quaternion & rhs);
	//単位Quaternionを返す
	//Quaternion IdentityQuaternion();
	//共役Quaternionを返す
	//Quaternion Conjugate(const Quaternion & quaternion);
	//Quaternionのnormを返す
	//float Norm(const Quaternion & quaternion);
	//正規化したQuaternionを返す
	//Quaternion Normalize(const Quaternion & quaternion);
	//逆Quaternionを返す
	//Quaternion Inverse(const Quaternion & quaternion);

	//任意軸回転を表すQuaternionの生成
	Quaternion MakeRotateAxisAngleQuaternion(const Vector3 & axis, float angle);
	//ベクトルをQuaternionで回転させた結果のベクトルを求める
	Vector3 RotateVector(const Vector3 & vector, const Quaternion & quaternion);
	//Quaternionから回転行列を求める
	Matrix4x4 MakeRotateMatrix(const Quaternion & quaternion);

	Quaternion rotation = MakeRotateAxisAngleQuaternion(
		Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

	Quaternion Slerp(const Quaternion & q0, const Quaternion & q1, float t);

	Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f,0.0f }, 0.3f);
	Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f,0.71f }, 3.141592f);

	Quaternion interporate0 = Slerp(rotation0, rotation1, 0.0f);
	Quaternion interporate1 = Slerp(rotation0, rotation1, 0.3f);
	Quaternion interporate2 = Slerp(rotation0, rotation1, 0.5f);
	Quaternion interporate3 = Slerp(rotation0, rotation1, 0.7f);
	Quaternion interporate4 = Slerp(rotation0, rotation1, 1.0f);

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

		QuaternionScreenPrintf(0, 0, interporate0, "interporate0");
		QuaternionScreenPrintf(0, 20, interporate1, "interporate1");
		QuaternionScreenPrintf(0, 40, interporate2, "interporate2");
		QuaternionScreenPrintf(0, 60, interporate3, "interporate3");
		QuaternionScreenPrintf(0, 80, interporate4, "interporate4");
		/*MatrixScreenPrintf(0, 20, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, 120, rotateByQuaternion, "rotateByQuatrenion");
		VectorScreenPrintf(0, 210, rotateByMatrix, "rotateByMatrix");*/

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

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];


	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

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

//Quaternion Normalize(const Quaternion& quaternion) {
//	// ノルム（長さ）を求める
//	float length = std::sqrt(
//		quaternion.x * quaternion.x +
//		quaternion.y * quaternion.y +
//		quaternion.z * quaternion.z +
//		quaternion.w * quaternion.w
//	);
//
//	// 長さがゼロならそのまま返す（ゼロ除算防止）
//	if (length == 0.0f) {
//		return Quaternion{ 0.0f, 0.0f, 0.0f, 0.0f };
//	}
//
//	float invLength = 1.0f / length;
//
//	// 各成分を割って単位化
//	return Quaternion{
//		quaternion.x * invLength,
//		quaternion.y * invLength,
//		quaternion.z * invLength,
//		quaternion.w * invLength
//	};
//}

float Norm(const Quaternion& quaternion) {
	return std::sqrt(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {

	Vector3 n = Normalize(axis);

	float half = angle * 0.5f;
	float s = std::sin(half);

	Quaternion q{};
	q.x = n.x * s;
	q.y = n.y * s;
	q.z = n.z * s;
	q.w = std::cos(half);
	return q;
}

Vector3 RotateVector(const Vector3& v, const Quaternion& q) {

	// q の成分を取り出し
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;

	// t = 2 * cross(q.xyz, v)
	Vector3 t{
		2.0f * (y * v.z - z * v.y),
		2.0f * (z * v.x - x * v.z),
		2.0f * (x * v.y - y * v.x)
	};

	// v' = v + w * t + cross(q.xyz, t)
	Vector3 result{
		v.x + w * t.x + (y * t.z - z * t.y),
		v.y + w * t.y + (z * t.x - x * t.z),
		v.z + w * t.z + (x * t.y - y * t.x)
	};

	return result;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
	Matrix4x4 result{};

	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;

	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	result.m[0][0] = 1.0f - 2.0f * (yy + zz);
	result.m[0][1] = 2.0f * (xy + wz);
	result.m[0][2] = 2.0f * (xz - wy);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (xy - wz);
	result.m[1][1] = 1.0f - 2.0f * (xx + zz);
	result.m[1][2] = 2.0f * (yz + wx);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (xz + wy);
	result.m[2][1] = 2.0f * (yz - wx);
	result.m[2][2] = 1.0f - 2.0f * (xx + yy);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char label[]) {
	// ラベル表示
	Novice::ScreenPrintf(x, y, "%s", label);

	// 各成分を縦に並べて表示
	Novice::ScreenPrintf(x, y + 20, "x: %6.02f", vector.x);
	Novice::ScreenPrintf(x, y + 40, "y: %6.02f", vector.y);
	Novice::ScreenPrintf(x, y + 60, "z: %6.02f", vector.z);
}

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

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
	// 内積（cosθ）
	float dot =
		q0.x * q1.x +
		q0.y * q1.y +
		q0.z * q1.z +
		q0.w * q1.w;

	Quaternion q1Copy = q1;

	// 逆方向を向いていたら反転（最短経路で補間）
	if (dot < 0.0f) {
		dot = -dot;
		q1Copy.x = -q1Copy.x;
		q1Copy.y = -q1Copy.y;
		q1Copy.z = -q1Copy.z;
		q1Copy.w = -q1Copy.w;
	}

	// ほぼ同じ向きなら Lerp（数値安定性対策）
	const float DOT_THRESHOLD = 0.9995f;
	if (dot > DOT_THRESHOLD) {
		Quaternion result;
		result.x = q0.x + t * (q1Copy.x - q0.x);
		result.y = q0.y + t * (q1Copy.y - q0.y);
		result.z = q0.z + t * (q1Copy.z - q0.z);
		result.w = q0.w + t * (q1Copy.w - q0.w);

		// 正規化
		float len = std::sqrt(
			result.x * result.x +
			result.y * result.y +
			result.z * result.z +
			result.w * result.w
		);
		result.x /= len;
		result.y /= len;
		result.z /= len;
		result.w /= len;

		return result;
	}

	// θ = acos(dot)
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	float w0 = std::sin((1.0f - t) * theta) / sinTheta;
	float w1 = std::sin(t * theta) / sinTheta;

	Quaternion result;
	result.x = w0 * q0.x + w1 * q1Copy.x;
	result.y = w0 * q0.y + w1 * q1Copy.y;
	result.z = w0 * q0.z + w1 * q1Copy.z;
	result.w = w0 * q0.w + w1 * q1Copy.w;

	return result;
}
