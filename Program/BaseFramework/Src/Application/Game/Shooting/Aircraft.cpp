#include "Aircraft.h"

Aircraft::Aircraft()
{
}

Aircraft::~Aircraft()
{
	Release();
}

void Aircraft::Deserialize()
{
	m_pModel = new kdModel();
	if (m_pModel == nullptr)
	{
		return;
	}

	if (m_pModel->Load("Data/Aircraft/Aircraft_body.gltf") == false)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	//初期配置座標を地面から少し浮いた位置にする
	m_mWorld = m_mWorld.CreateTranslation(0.0f, 5.0f, 0.0f);

	//カメラが飛行機からどれだけ離れているか
	m_mOffset = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(15));
	m_mOffset *= Math::Matrix::CreateTranslation(0.0f, 2.0f, -10.0f);

	//カメラの描写設定(描写する距離、角度)
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60),
		D3D.GetBackBuffer()->GetAspectRatio(), 0.01f, 5000.0f);
}

void Aircraft::Update()
{
	UpdateMove();
}

void Aircraft::Draw()
{
	if (m_pModel == nullptr)
	{
		return;
	}

	SHADER.m_standardShader.SetWorldMatrix(m_mWorld);
	SHADER.m_standardShader.DrawMesh(m_pModel->GetMesh(), m_pModel->GetMaterials());
}

void Aircraft::SetCameraToShader()
{
	//追従カメラ行列の作成:追従カメラからの相対座標行列に、ゲーム上の飛行機の絶対座標行列を合成
	kdMatrix mCam = m_mOffset * m_mWorld;

	//追従カメラをシェーダーにセット
	SHADER.m_cb7_Camera.Work().CamPos = mCam.GetTranslation();

	//追従カメラのビュー行列をシェーダーにセット
	mCam.Inverse();
	SHADER.m_cb7_Camera.Work().mV = mCam;

	//追従カメラの射影行列をシェーダーにセット
	SHADER.m_cb7_Camera.Work().mP = m_mProj;

	//カメラ情報(ビュー行列、射影行列)をシェーダーの定数バッファにセット
	SHADER.m_cb7_Camera.Write();
}

void Aircraft::UpdateMove()
{
	//移動ベクトル作成
	Math::Vector3 move{ 0.0f,0.0f,0.0f };

	/*kdVec3 move;*/

	if (GetAsyncKeyState(VK_UP) & 0x8000) { move.z = 1.0f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { move.z = -1.0f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { move.x = 1.0f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { move.x = -1.0f; }

	move.Normalize();

	//移動速度補正
	move *= m_speed;

	/*m_mworld._41 += move.x;
	m_mworld._42 += move.y;
	m_mworld._43 += move.z;*/

	//移動行列作成
	//Math::Matrix moveMat = DirectX::XMMatrixTranslation(move.x, move.y, move.z);
	kdMatrix moveMat;
	moveMat.CreateTranslation(move.x, move.y, move.z);

	//ワールド行列に合成
	/*m_mWorld = DirectX::XMMatrixMultiply(moveMat, m_mWorld);*/
	m_mWorld = moveMat*m_mWorld;

	//回転ベクトルの作成
	/*Math::Vector3 rotate = { 0.0f,0.0f,0.0f };*/
	kdVec3 rotate;

	if (GetAsyncKeyState('W') & 0x8000) { rotate.x = 1.0f; }
	if (GetAsyncKeyState('A') & 0x8000) { rotate.z = -1.0f; }
	if (GetAsyncKeyState('S') & 0x8000) { rotate.x = -1.0f; }
	if (GetAsyncKeyState('D') & 0x8000) { rotate.z = 1.0f; }

	//回転行列作成
	/*Math::Matrix rotateMat = DirectX::XMMatrixRotationX(rotate.x * KdToRadians);
	rotateMat = DirectX::XMMatrixMultiply(rotateMat, DirectX::XMMatrixRotationZ(rotate.z * KdToRadians));*/
	kdMatrix rotateMat;
	rotateMat.CreateRotationX(rotate.x * KdToRadians);
	rotateMat.RotateZ(rotate.z * KdToRadians);

	//ワールド行列に合成
	/*m_mWorld = DirectX::XMMatrixMultiply(rotateMat, m_mWorld);*/
	m_mWorld = rotateMat * m_mWorld;
}

void Aircraft::Release()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}
