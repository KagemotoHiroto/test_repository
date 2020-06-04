#pragma once

//3Dベクトル
class kdVec3 :public DirectX::XMFLOAT3//継承
{
public:

	kdVec3()//デフォルトコンストラクタは座標の０クリアを行う
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	kdVec3(float _x, float _y, float _z)//座標指定付きコンストラクタ
	{
		x = _x;
		y = _y;
		z = _z;
	}

	//XMVECTORから代入してきたとき
	kdVec3(const DirectX::XMVECTOR& v)
	{
		//変換して代入
		DirectX::XMStoreFloat3(this, v);
		//下記と同意だが、上記のSIMD命令を使用したほうが高速
	/*	x = v.m128_f32[0];
		y = v.m128_f32[1];
		z = v.m128_f32[2];*/
	}

	//XMVECROEへ変換
	operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat3(this); }

	//Math::Vector3と互換性を持つための関数
	operator Math::Vector3& () { return*(Math::Vector3*)this; }

	//算術演算子　乗算(*)
	kdVec3& operator*(float s)
	{
		*this = DirectX::XMVectorScale(*this, s);
		return *this;
	}

	//自分を正規化
	void Normalize()
	{
		*this = DirectX::XMVector3Normalize(*this);
	}

	//長さ
	float Length() const
	{
		return DirectX::XMVector3Length(*this).m128_f32[0];
	}

	//長さの2乗(高速なので判定用に使用することが多い)
	float LengthSquared() const
	{
		return DirectX::XMVector3LengthSq(*this).m128_f32[0];
	}
};

//4*4の行列
class kdMatrix :public DirectX::XMFLOAT4X4
{
public:

	kdMatrix()
	{
		//デフォルトコンストラクタは単位行列か
		*this = DirectX::XMMatrixIdentity();
	}

	kdMatrix(const DirectX::XMMATRIX& m)
	{
		DirectX::XMStoreFloat4x4(this, m);
	}

	//XMFLOAT4*4,Math::Matrixから代入してきた
	kdMatrix(const DirectX::XMFLOAT4X4& m)
	{
		memcpy_s(this, sizeof(float) * 16, &m, sizeof(XMFLOAT4X4));
	}

	//XMMATRIXへ変換
	operator DirectX::XMMATRIX() const
	{
		return DirectX::XMLoadFloat4x4(this);
	}

	//Math::Matrixと互換性を持つための関数
	operator Math::Matrix& () { return *(Math::Matrix*)this; }

	//代入演算子　乗算
	kdMatrix& operator*=(const kdMatrix& m)
	{
		*this = DirectX::XMMatrixMultiply(*this, m);
		return *this;
	}

	//作成===============================-==========

	//移動行列作成
	void CreateTranslation(float x, float y, float z)
	{
		*this = DirectX::XMMatrixTranslation(x, y, z);
	}

	//X回転行列作成
	void CreateRotationX(float angle)
	{
		*this = DirectX::XMMatrixRotationX(angle);
	}

	//操作=========================================

	//Z軸回転
	void RotateZ(float angle)
	{
		*this *= DirectX::XMMatrixRotationZ(angle);
	}

	// 逆行列にする
	void Inverse()
	{
		*this = DirectX::XMMatrixInverse(nullptr, *this);
	}

	//プロパティ====================================

	//Z軸取得
	kdVec3 GetAxisZ() const { return {_31, _32, _33 };}

    //Z軸セット
	void SetAxisZ(const kdVec3& v)
	{
		_31 = v.x;
		_32 = v.y;
		_32 = v.z;
	}

	// KdMatrix同士の合成
	inline KdMatrix operator* (const KdMatrix& M1, const KdMatrix& M2)
	{
		using namespace DirectX;
		return XMMatrixMultiply(M1, M2);
	}
};

