#include "Model.h"
#include <imgui.h>
#include "../Graphics/Surface.h"

namespace dx = DirectX;

// Mesh
Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bindable>> bindPtrs)
{
	AddBind(std::make_shared<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	

	for (auto& pb : bindPtrs)
	{
			AddBind(std::move(pb));
	}

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
}
void Mesh::Draw(Graphics& gfx, dx::FXMMATRIX accumulatedTransform) const noexcept
{
	dx::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::Draw(gfx);
}
dx::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return dx::XMLoadFloat4x4(&transform);
}


// Node
Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const dx::XMMATRIX& transform) noexcept
	:
id(id),
meshPtrs(std::move(meshPtrs)),
name(name)
{
	dx::XMStoreFloat4x4(&this->transform, transform);
	dx::XMStoreFloat4x4(&appliedTransform, dx::XMMatrixIdentity());
}
void Node::Draw(Graphics& gfx, dx::FXMMATRIX accumulatedTransform) const noexcept
{
	const auto built =
		dx::XMLoadFloat4x4(&appliedTransform) *
		dx::XMLoadFloat4x4(&transform) *
		accumulatedTransform;
	for (const auto pm : meshPtrs)
	{
		pm->Draw(gfx, built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->Draw(gfx, built);
	}
}
void Node::SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept
{
	dx::XMStoreFloat4x4(&appliedTransform, transform);
}
void Node::AddChild(std::unique_ptr<Node> pChild) noexcept
{
	assert(pChild);
	childPtrs.push_back(std::move(pChild));
}

void Node::ShowTree(std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcept
{
	// build up flags for current node
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((GetId() == selectedIndex.value_or(-1)) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
	// render this node
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)GetId(), node_flags, name.c_str()
	);
	// processing for selecting node
	if (ImGui::IsItemClicked())
	{
		selectedIndex = GetId();
		pSelectedNode = const_cast<Node*>(this);
	}
	// recursive rendering of open node's children
	if (expanded)
	{
		for (const auto& pChild : childPtrs)
		{
			pChild->ShowTree(selectedIndex, pSelectedNode);
		}
		ImGui::TreePop();
	}
}

int Node::GetId() const noexcept
{
	return id;
}



// Model
class ModelWindow // pImpl idiom, only defined in this .cpp
{
public:
	void Show(const char* windowName, const Node& root) noexcept
	{
		// window name defaults to "Model"
		windowName = windowName ? windowName : "Model";
		// need an ints to track node indices and selected node
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(selectedIndex, pSelectedNode);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				auto& transform = transforms[*selectedIndex];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);
			}
		}
		ImGui::End();
	}
	dx::XMMATRIX GetTransform() const noexcept
	{
		const auto& transform = transforms.at(*selectedIndex);
		return
			dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}
	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}
private:
	std::optional<int> selectedIndex;
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	std::unordered_map<int, TransformParameters> transforms;
};




Model::Model(Graphics& gfx, const std::string fileName)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);


	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
	}
	int nextId = 0;
	pRoot = ParseNode(nextId, *pScene->mRootNode);
}
void Model::Draw(Graphics& gfx) const
{
	if (auto node = pWindow->GetSelectedNode())
	{
		node->SetAppliedTransform(pWindow->GetTransform());
	}
	pRoot->Draw(gfx, dx::XMMatrixIdentity());
}
void Model::ShowWindow(const char* windowName) noexcept
{
	pWindow->Show(windowName, *pRoot);
}
Model::~Model() noexcept
{
}
void Model::SetRootTransform(DirectX::FXMMATRIX tf) noexcept
{
	pRoot->SetAppliedTransform(tf);
}
std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials)
{
	using hw3dexp::VertexLayout;

	hw3dexp::VertexBuffer vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)
		.Append(VertexLayout::Tangent)
		.Append(VertexLayout::Bitangent)
		.Append(VertexLayout::Texture2D)
	));

	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
			*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
			*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	std::vector<std::shared_ptr<Bindable>> bindablePtrs;


	using namespace std::string_literals;
	const auto base = "Models\\brick_wall\\"s;

	bool hasSpecularMap = false;
	float shininess = 3.5f;
	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString texFileName;

		material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
		bindablePtrs.push_back(Texture::Resolve(gfx, base + texFileName.C_Str()));
		bool here;
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS) //EXCEPTION- solved, needed to change output dir of shaders
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, base + texFileName.C_Str(), 1));
			hasSpecularMap = true;
		}
		else
		{
		    material.Get(AI_MATKEY_SHININESS, shininess);	
		}

		material.GetTexture(aiTextureType_NORMALS, 0, &texFileName);
		bindablePtrs.push_back(Texture::Resolve(gfx, base + texFileName.C_Str(), 2));

		bindablePtrs.push_back(Sampler::Resolve(gfx));
	}

	auto meshTag = base + "%" + mesh.mName.C_Str();
	bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));
	bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

	auto pvs = VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back(std::move(pvs));


	bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

	if (hasSpecularMap)
	{
		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSSpecNormalMap.cso"));
		struct PSMaterialConstant
		{
			BOOL  normalMapEnabled = TRUE;
			float padding[3];
		} pmc;

		bindablePtrs.push_back(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx,pmc,1u));
	}
	else
	{
		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSNormalMap.cso"));


		struct PSMaterialConstant
		{
			float specularIntensity = 0.18f;
			float specularPower;
			BOOL  normalMapEnabled = TRUE;
			float padding[1];
		} pmc;
		pmc.specularPower = shininess;


		//bindablePtrs.push_back(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));
		bindablePtrs.push_back(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));
	}

	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));

}
std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node)
{
	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId,*node.mChildren[i]));
	}

	return pNode;
}