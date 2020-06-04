#pragma once

class Aircraft
{
public:
	Aircraft(); //コンストラクタ
	~Aircraft();//デストラクタ

	void Deserialize();//初期化:オブジェクト作成用外部データの解釈
	void Update();     //更新
	void Draw();       //描画

	void SetCameraToShader();//追従カメラのビュー行列・射影行列をシェーダーへセット


	void UpdateMove();//移動更新処理
private:

	void Release();

	kdModel* m_pModel = nullptr;//3Dモデルデータ
	Math::Matrix m_mWorld;      //飛行機のワールド行列

	float m_speed = 0.2f;       //移動スピード

	//Math::Matrix m_mOffset;//追従カメラの相対座標行列
	kdMatrix m_mOffset;
	Math::Matrix m_mProj;  //追従カメラの射影行列
};

