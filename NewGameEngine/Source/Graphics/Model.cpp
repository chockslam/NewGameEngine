#include "Model.h"
#include <imgui.h>
#include "../Graphics/Surface.h"
#include <sstream>
#include <implot.h>

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
template <class T> bool Node::ControlMeDaddy(Graphics& gfx, T& c)
{
	if (meshPtrs.empty())
	{
		return false;
	}
	if constexpr (std::is_same<T, PSMaterialConstantFullmonte>::value)
	{
		if (auto pcb = meshPtrs.front()->QueryBindable<PixelConstantBuffer<PSMaterialConstantFullmonte>>())
		{


			ImGui::Text("Material");

			bool normalMapEnabled = (bool)c.normalMapEnabled;
			ImGui::Checkbox("Norm Map", &normalMapEnabled);
			c.normalMapEnabled = normalMapEnabled ? TRUE : FALSE;

			bool specularMapEnabled = (bool)c.specularMapEnabled;
			ImGui::Checkbox("Spec Map", &specularMapEnabled);
			c.specularMapEnabled = specularMapEnabled ? TRUE : FALSE;

			bool hasGlossMap = (bool)c.hasGlossMap;
			ImGui::Checkbox("Gloss Alpha", &hasGlossMap);
			c.hasGlossMap = hasGlossMap ? TRUE : FALSE;

			ImGui::SliderFloat("Spec Weight", &c.specularMapWeight, 0.0f, 2.0f);

			ImGui::SliderFloat("Spec Pow", &c.specularPower, 0.0f, 10.0f);

			ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&c.specularColor));

			pcb->Update(gfx, c);
			return true;
		}
	}
	else if constexpr (std::is_same<T, PSMaterialConstantNotex>::value) 
	{
		if (auto pcb = meshPtrs.front()->QueryBindable<PixelConstantBuffer<T>>())
		{
			ImGui::Text("Material");

			ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&c.specularColor));

			ImGui::SliderFloat("Spec Pow", &c.specularPower, 0.0f, 1000.0f);

			ImGui::ColorPicker3("Diff Color", reinterpret_cast<float*>(&c.materialColor));

			pcb->Update(gfx, c);
			return true;
		}
	}
	return false;
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
	void Show(Graphics& gfx, const char* windowName, const Node& root) noexcept
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
				
				if (!pSelectedNode->ControlMeDaddy(gfx, skinMaterial))
				{
					pSelectedNode->ControlMeDaddy(gfx, ringMaterial);
				}
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
	Node::PSMaterialConstantFullmonte skinMaterial;
	Node::PSMaterialConstantNotex ringMaterial;
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

	if (audio->OpenFile("wavSamples\\swth.wav")) {
		audio->PlayAudio();
	}
	else {
		MessageBox(0, "Failed to Load Audio", 0, 0);
	}
}
void Model::Draw(Graphics& gfx) const
{
	if (auto node = pWindow->GetSelectedNode())
	{
		node->SetAppliedTransform(pWindow->GetTransform());
	}
	pRoot->Draw(gfx, dx::XMMatrixIdentity());
}
void Model::ShowWindow(Graphics& gfx, const char* windowName) noexcept
{

	ImGui::Begin("Plot Mid"); 
	if (ImPlot::BeginPlot("My Plot Mid", "Frequency", "Magnitude")) {
		//ImPlot::PlotBars("Bars", audio->audio->freq, audio->audio->magn, 5000, 10);
		ImPlot::PlotLine("My Line Plot Mid", audio->audio->freqM, audio->audio->magnM, 172);
		ImPlot::EndPlot();
	}
	ImGui::End();

	ImGui::Begin("Plot Bass");
	if (ImPlot::BeginPlot("My Plot Bass", "Frequency", "Magnitude")) {
		//ImPlot::PlotBars("Bars", audio->audio->freq, audio->audio->magn, 5000, 10);
		ImPlot::PlotLine("My Line Plot Bass", audio->audio->freqB, audio->audio->magnB, 13);
		ImPlot::EndPlot();
	}
	ImGui::End();


	ImGui::Begin("Plot Treble");
	if (ImPlot::BeginPlot("My Plot Treble", "Frequency", "Magnitude")) {
		//ImPlot::PlotBars("Bars", audio->audio->freq, audio->audio->magn, 5000, 10);
		ImPlot::PlotLine("My Line Plot Treble", audio->audio->freqT, audio->audio->magnT, 838);
		ImPlot::EndPlot();
	}
	ImGui::End();





	pWindow->Show(gfx, windowName, *pRoot);
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
	using namespace std::string_literals;
	using hw3dexp::VertexLayout;

	std::vector<std::shared_ptr<Bindable>> bindablePtrs;

	const auto base = "Models\\gobber\\"s;

	bool hasSpecularMap = false;
	bool hasNormalMap = false;
	bool hasAlphaGloss = false;
	bool hasDiffuseMap = false;
	float shininess = 2.0f;

	dx::XMFLOAT4 specularColor = { 0.18f,0.18f, 0.18f, 1.0f };
	dx::XMFLOAT4 diffuseColor = { 0.45f, 0.45f, 0.85f, 1.0f };

	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString texFileName;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			bindablePtrs.push_back(Texture::Resolve(gfx, base + texFileName.C_Str()));
			hasDiffuseMap = true;
		}
		else {
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, base + texFileName.C_Str(), 1);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasSpecularMap = true;
		}
		else 
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor));
		}
		if(!hasAlphaGloss)
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, base + texFileName.C_Str(), 2);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasNormalMap = true;
		}

		if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
		{
			bindablePtrs.push_back(Sampler::Resolve(gfx));
		}
	}

	const auto meshTag = base + "%" + mesh.mName.C_Str();

	const float scale = 6.0f;

	if( hasDiffuseMap && hasNormalMap && hasSpecularMap )
	{
		hw3dexp::VertexBuffer vbuf( std::move(
			VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
			.Append( VertexLayout::Tangent )
			.Append( VertexLayout::Bitangent )
			.Append( VertexLayout::Texture2D )
		) );

		for( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3( mesh.mVertices[i].x * scale,mesh.mVertices[i].y * scale,mesh.mVertices[i].z * scale ),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( gfx,meshTag,vbuf ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( gfx,meshTag,indices ) );

		auto pvs = VertexShader::Resolve( gfx,"PhongVSNormalMap.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( gfx,"PhongPSSpecNormalMap.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( gfx,vbuf.GetLayout(),pvsbc ) );


		Node::PSMaterialConstantFullmonte pmc;

		pmc.specularPower = shininess;
		pmc.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<Node::PSMaterialConstantFullmonte>>( gfx,pmc,1u ) );
	}
	else if( hasDiffuseMap && hasNormalMap )
	{
		hw3dexp::VertexBuffer vbuf( std::move(
			VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
			.Append( VertexLayout::Tangent )
			.Append( VertexLayout::Bitangent )
			.Append( VertexLayout::Texture2D )
		) );

		for( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3( mesh.mVertices[i].x * scale,mesh.mVertices[i].y * scale,mesh.mVertices[i].z * scale ),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
				//*reinterpret_cast<dx::XMFLOAT3*>()
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( gfx,meshTag,vbuf ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( gfx,meshTag,indices ) );

		auto pvs = VertexShader::Resolve( gfx,"PhongVSNormalMap.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( gfx,"PhongPSNormalMap.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( gfx,vbuf.GetLayout(),pvsbc ) );

		struct PSMaterialConstantDiffnorm
		{
			float specularIntensity = 0.18f;
			float specularPower;
			BOOL  normalMapEnabled = TRUE;
			float padding[1];
		} pmc;
		pmc.specularPower = shininess;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<PSMaterialConstantDiffnorm>>( gfx,pmc,1u ) );
	}
	else if( hasDiffuseMap )
	{
		hw3dexp::VertexBuffer vbuf( std::move(
			VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
			.Append( VertexLayout::Texture2D )
		) );

		for( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3( mesh.mVertices[i].x * scale,mesh.mVertices[i].y * scale,mesh.mVertices[i].z * scale ),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( gfx,meshTag,vbuf ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( gfx,meshTag,indices ) );

		auto pvs = VertexShader::Resolve( gfx,"PhongVS.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( gfx,"PhongPS.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( gfx,vbuf.GetLayout(),pvsbc ) );

		struct PSMaterialConstantDiffuse
		{
			float specularIntensity;
			float specularPower;
			float padding[2];
		} pmc;
		pmc.specularPower = shininess;
		pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.x) / 3.0f;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<PSMaterialConstantDiffuse>>( gfx,pmc,1u ) );
	}
	else if( !hasDiffuseMap && !hasNormalMap && !hasSpecularMap )
	{
		hw3dexp::VertexBuffer vbuf( std::move(
			VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
		) );

		for( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3( mesh.mVertices[i].x * scale,mesh.mVertices[i].y * scale,mesh.mVertices[i].z * scale ),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}

		bindablePtrs.push_back( VertexBuffer::Resolve( gfx,meshTag,vbuf ) );

		bindablePtrs.push_back( IndexBuffer::Resolve( gfx,meshTag,indices ) );

		auto pvs = VertexShader::Resolve( gfx,"PhongVSNotex.cso" );
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( PixelShader::Resolve( gfx,"PhongPSNotex.cso" ) );

		bindablePtrs.push_back( InputLayout::Resolve( gfx,vbuf.GetLayout(),pvsbc ) );
		

		Node::PSMaterialConstantNotex pmc;

		pmc.specularPower = shininess;
		pmc.specularColor = specularColor;
		pmc.materialColor = diffuseColor;
		// this is CLEARLY an issue... all meshes will share same mat const, but may have different
		// Ns (specular power) specified for each in the material properties... bad conflict
		bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<Node::PSMaterialConstantNotex>>( gfx,pmc,1u ) );
	}
	else
	{
		throw std::runtime_error( "terrible combination of textures in material smh" );
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