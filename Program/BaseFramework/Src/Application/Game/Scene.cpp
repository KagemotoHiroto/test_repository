#include"Scene.h"

#include"Shooting/StageObject.h"
#include"Shooting/Aircraft.h"

//コンストラクタ
Scene::Scene()
{
}

//デストラクタ
Scene::~Scene()
{
}

//初期化
void Scene::Init()
{
	m_sky.Load("Data/Sky/Sky.gltf");

	m_pGround = new StageObject();
	if (m_pGround)
	{
		m_pGround->Deserialize();
	}

	m_pAircraft = new Aircraft();
	if (m_pAircraft)
	{
		m_pAircraft->Deserialize();
	}

	/*OutputDebugStringA("初期化\n");*/
}

//解放
void Scene::Release()
{
	if (m_pGround)
	{
		delete m_pGround;
		m_pGround = nullptr;
	}

	if (m_pAircraft)
	{
		delete m_pAircraft;
		m_pAircraft = nullptr;
	}
}

//更新
void Scene::Update()
{
	if (m_pAircraft)
	{
		m_pAircraft->Update();
	}

	m_camera.Update();


	if (m_pAircraft)
	{
		m_pAircraft->Update();
	}

	/*OutputDebugStringA("更新\n");*/
}

//描画
void Scene::Draw()
{
	//エディターカメラをシェーダーにセット
	/*m_camera.SetToShader();*/

	if (m_pAircraft)
	{
		m_pAircraft->SetCameraToShader();
	}

	//カメラ情報(ビュー行列、射影行列)を、核シェーダの定数バッファにセット
	SHADER.m_cb7_Camera.Write();

	//ライトの情報をセット
	SHADER.m_cb8_Light.Write();

	//エフェクトシェーダーを描画デバイスにセット
	SHADER.m_effectShader.SetToDevice();

	Math::Matrix skyScale = DirectX::XMMatrixScaling(100.0f, 100.0f, 100.0f);

	SHADER.m_effectShader.SetWorldMatrix(skyScale);

	//モデルの描画(メッシュ情報とマテリアルの情報を渡す)
	SHADER.m_effectShader.DrawMesh(m_sky.GetMesh(), m_sky.GetMaterials());

	//不透明物描画
	SHADER.m_standardShader.SetToDevice();

	if (m_pGround)
	{
		m_pGround->Draw();
	}

	if (m_pAircraft)
	{
		m_pAircraft->Draw();
	}

	//デバッグライン描画
	SHADER.m_effectShader.SetToDevice();
	SHADER.m_effectShader.SetTexture(D3D.GetWhiteTex()->GetSRView());
	{
		/*std::vector<KdEffectShader::Vertex>debugLines;*/

		////ラインの開始頂点
		//KdEffectShader::Vertex ver1;
		//ver1.Color = { 1.0f,1.0f,1.0f,1.0f };
		//ver1.UV = { 0.0f,0.0f };
		//ver1.Pos = { 0.0f,0.0f,0.0f };

		////ラインの終端頂点
		//KdEffectShader::Vertex ver2;
		//ver2.Color = { 1.0f,1.0f,1.0f,1.0f };
		//ver2.UV = { 0.0f,0.0f };
		//ver2.Pos = { 0.0f,10.0f,0.0f };

		//debugLines.push_back(ver1);//push_back 動的配列の中で増やす
		//debugLines.push_back(ver2);

		AddDebugLine(Math::Vector3(), Math::Vector3(0.0f, 10.0f, 0.0f));

		AppDebugSphereLine(Math::Vector3(5.0f, 5.0f, 0.0f), 2.0f);

		//Zバッファ使用ON・書き込みON
		D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZEnable_ZWriteEnable, 0);

		if (m_debugLines.size() >= 1)
		{
			SHADER.m_effectShader.SetWorldMatrix(Math::Matrix());

			SHADER.m_effectShader.DrawVertices(m_debugLines, D3D_PRIMITIVE_TOPOLOGY_LINELIST);

			m_debugLines.clear();
		}

		//Zバッファ使用
		D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZDisable_ZWriteDisable, 0);
	}

	/*OutputDebugStringA("描画\n");*/
}

//デバッグライン描画
void Scene::AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2, const Math::Color& color)
{
	//ラインの開始頂点
	KdEffectShader::Vertex ver;
	ver.Color = color;
	ver.UV = { 0.0f,0.0f };
	ver.Pos = p1;
	m_debugLines.push_back(ver);

	//ラインの終端頂点
	ver.Pos = p2;
	m_debugLines.push_back(ver);
}

void Scene::AppDebugSphereLine(const Math::Vector3& pos, float radius, const Math::Color& color)
{
	KdEffectShader::Vertex ver;
	ver.Color = color;
	ver.UV = { 0.0f,0.0f };

	static constexpr int kDetail = 32;
	for (UINT i = 0; i < kDetail + 1; ++i)
	{
		//XZ平面
		ver.Pos = pos;
		ver.Pos.x += cos((float)i * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)i * (360 / kDetail) * KdToRadians) * radius;
		m_debugLines.push_back(ver);

		ver.Pos = pos;
		ver.Pos.x += cos((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		m_debugLines.push_back(ver);

		//XY平面
		ver.Pos = pos;
		ver.Pos.x += cos((float)i * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.y += sin((float)i * (360 / kDetail) * KdToRadians) * radius;
		m_debugLines.push_back(ver);

		ver.Pos = pos;
		ver.Pos.x += cos((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.y += sin((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		m_debugLines.push_back(ver);

		//YZ平面
		ver.Pos = pos;
		ver.Pos.y += cos((float)i * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)i * (360 / kDetail) * KdToRadians) * radius;
		m_debugLines.push_back(ver);

		ver.Pos = pos;
		ver.Pos.y += cos((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		ver.Pos.z += sin((float)(i + 1) * (360 / kDetail) * KdToRadians) * radius;
		m_debugLines.push_back(ver);

	}
}

