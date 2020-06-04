#pragma once

class kdModel 
{
public:

	kdModel();
	~kdModel();

	bool Load(const std::string& filename);

	//�A�N�Z�T
	const KdMesh* GetMesh()const
	{
		return m_pMesh;
	}
	const std::vector<KdMaterial>& GetMaterials()const
	{
		return m_materials;
	}

private:

	KdMesh* m_pMesh=nullptr;

	//�}�e���A���z��
	std::vector<KdMaterial>m_materials;
};