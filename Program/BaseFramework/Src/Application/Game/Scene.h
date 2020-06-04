#pragma once

#include"EditerCamera.h"

//前方宣言
class StageObject;//ヘッダーの中で使うクラスはできるだけ前方宣言して使う(毎回インクルードしていると重くなるため)
class Aircraft;

class Scene
{
public:
	Scene();       //コンストラクタ
	~Scene();      //デストラクタ

	void Init();   //初期化
	void Release();//解放
	void Update(); //更新
	void Draw();   //描画

	//デバッグライン描画
	void AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2, const Math::Color& color = { 1,1,1,1 });
	
	//デバッグスフィア描画
	void AppDebugSphereLine(const Math::Vector3& pos, float radius, const Math::Color& color = { 1,1,1,1 });

private:
	kdModel m_sky;//スカイスフィア
	EditerCamera m_camera;
	StageObject* m_pGround = nullptr; //ステージの地面
	Aircraft* m_pAircraft = nullptr; //飛行機

	//デバッグライン描画用の頂点配列
	std::vector<KdEffectShader::Vertex>m_debugLines;
};