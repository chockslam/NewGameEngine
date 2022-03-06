/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#include "VertexBuffer.h"
#include "BindableCodex.h"

VertexBuffer::VertexBuffer(Graphics& gfx, const std::string& tag, const std::vector<Vertex>& vertices)
	: 
	stride(sizeof(Vertex)),
	tag(tag)
{
	// Set up Vertex Buffer
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
	bd.StructureByteStride = stride;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
}

VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<Vertex>& vertices)
	:
	VertexBuffer(gfx, "?", vertices)
{

}

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(Graphics& gfx, const std::string& tag, const std::vector<Vertex>& vertices)
{
	assert(tag != "?");
	return Codex::Resolve<VertexBuffer>(gfx, tag, vertices);
}

std::string VertexBuffer::GetUID() const noexcept
{
	return GenerateUID( tag );
}

std::string VertexBuffer::GenerateUID_(const std::string& tag)
{
	using namespace std::string_literals;
	return typeid(VertexBuffer).name() + "#"s + tag;
}
