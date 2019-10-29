#include "ofxSpout2Receiver.h"
#include "SpoutReceiver.h"
#include "ofEvents.h"
#include "ofLog.h"

using namespace ofx::spout2;
using namespace std;

Receiver::~Receiver()
{
	release();
	if(auto_.enabled) {
		ofRemoveListener(ofEvents().update, this, &Receiver::receiveFrame);
	}
}
bool Receiver::setup(const std::string &name)
{
	unsigned int width, height;
	release();
	sender_name_ = name;
	receiver_ = make_shared<SpoutReceiver>();
	char buf[256]={0};
	strcpy_s(buf, sender_name_.c_str());
	return receiver_->CreateReceiver(buf, width, height);
}
bool Receiver::setupAuto(const std::string &name, ofTexture &texture, bool invert)
{
	auto_.texture = &texture;
	auto_.invert = invert;
	auto_.enabled = true;
	if(!setup(name)) {
		return false;
	}
	ofAddListener(ofEvents().update, this, &Receiver::receiveFrame);
	return true;

}

bool Receiver::receive(ofTexture &texture, bool invert)
{
	assert(receiver_);
	unsigned int width = texture.getWidth();
	unsigned int height = texture.getHeight();
	char buf[256]={0};
	strcpy_s(buf, sender_name_.c_str());
	if(!receiver_->ReceiveTexture(buf, width, height, texture.texData.textureID, texture.texData.textureTarget, invert)
	|| width != texture.getWidth()
	|| height != texture.getHeight()) {
		HANDLE handle;
		DWORD dw_format;
		if(!receiver_->GetSenderInfo(buf, width, height, handle, dw_format)) {
			return false;
		}
		GLint gl_format;
		GLenum format;
		GLenum type;
		switch(dw_format) {
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				gl_format = GL_RGBA32F;
				format = GL_RGBA;
				type = GL_FLOAT;
				break;
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				gl_format = GL_RGBA16F;
				format = GL_RGBA;
				type = GL_HALF_FLOAT;
				break;
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				gl_format = GL_RGB10_A2;
				format = GL_RGBA;
				type = GL_UNSIGNED_INT_2_10_10_10_REV;
				break;
			case DXGI_FORMAT_R16G16B16A16_UNORM:
				gl_format = GL_RGBA16UI;
				format = GL_RGBA_INTEGER;
				type = GL_UNSIGNED_SHORT;
				break;
			case DXGI_FORMAT_R11G11B10_FLOAT:
				gl_format = GL_R11F_G11F_B10F;
				format = GL_RGB;
				type = GL_UNSIGNED_INT_10F_11F_11F_REV;
				break;
			case DXGI_FORMAT_R8_UNORM:
				gl_format = GL_LUMINANCE;
				format = GL_LUMINANCE;
				type = GL_UNSIGNED_BYTE;
				break;
			case DXGI_FORMAT_R16_UNORM:
				gl_format = GL_LUMINANCE;
				format = GL_LUMINANCE;
				type = GL_UNSIGNED_SHORT;
				break;
			case DXGI_FORMAT_R16_FLOAT:
				gl_format = GL_LUMINANCE;
				format = GL_LUMINANCE;
				type = GL_HALF_FLOAT;
				break;
			case DXGI_FORMAT_R32_FLOAT:
				gl_format = GL_LUMINANCE;
				format = GL_LUMINANCE;
				type = GL_FLOAT;
				break;
			case DXGI_FORMAT_R8G8_UNORM:
				gl_format = GL_RG;
				format = GL_RG;
				type = GL_UNSIGNED_BYTE;
				break;
			case DXGI_FORMAT_R16G16_UNORM:
				gl_format = GL_RG16;
				format = GL_RG;
				type = GL_UNSIGNED_SHORT;
				break;
			case DXGI_FORMAT_R16G16_FLOAT:
				gl_format = GL_RG16F;
				format = GL_RG;
				type = GL_HALF_FLOAT;
				break;
			case DXGI_FORMAT_R32G32_FLOAT:
				gl_format = GL_RG32F;
				format = GL_RG;
				type = GL_FLOAT;
				break;
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
				gl_format = GL_SRGB8_ALPHA8;
				format = GL_RGBA;
				type = GL_UNSIGNED_BYTE;
				break;
			default:
				ofLogWarning() << "unhandled format defaulting to GL_RGBA";
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				gl_format = GL_RGBA;
				format = GL_RGBA;
				type = GL_UNSIGNED_BYTE;
				break;
		}
		texture.allocate(width, height, gl_format, false, format, type);
		return receive(texture, invert);
	}
	return true;
}

void Receiver::release()
{
	if(receiver_) {
		receiver_->ReleaseReceiver();
	}
	receiver_.reset();
}