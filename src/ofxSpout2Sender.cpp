#include "ofxSpout2Sender.h"
#include "ofTexture.h"
#include "ofLog.h"
#include "ofEvents.h"

using namespace ofx::spout2;
using namespace std;

Sender::~Sender()
{
	release();
	if(auto_.enabled) {
		ofRemoveListener(ofEvents().update, this, &Sender::sendFrame);
	}
}

bool Sender::setup(const std::string &name, unsigned int width, unsigned int height, DWORD dw_format)
{
	release();
	sender_ = make_shared<SpoutSender>();
	return sender_->CreateSender((char*)name.c_str(), width, height, dw_format);
}


bool Sender::setupAuto(const std::string &name, ofTexture &texture, bool invert)
{
	auto_.texture = texture;
	auto_.invert = invert;
	auto_.enabled = true;

	DWORD format = 0;
	switch(texture.texData.glInternalFormat) {
		case GL_RGB32F:
		case GL_RGBA32F:
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case GL_RGB16F:
		case GL_RGBA16F:
			format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;
		case GL_RGB16:
		case GL_RGB16UI:
		case GL_RGB16I:
		case GL_RGB16_SNORM:
		case GL_RGBA16:
		case GL_RGBA16UI:
		case GL_RGBA16I:
		case GL_RGBA16_SNORM:
			format = DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		case GL_RGB10_A2:
		case GL_RGB10_A2UI:
			DXGI_FORMAT_R10G10B10A2_UNORM;
			break;
		case GL_RGB8:
		case GL_RGB8UI:
		case GL_RGB8I:
		case GL_RGBA8:
		case GL_RGBA8UI:
		case GL_RGBA8I:
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case GL_RGB8_SNORM:
		case GL_RGBA8_SNORM:
			format = DXGI_FORMAT_B8G8R8A8_UNORM;
			break;
		default:
			ofLogWarning() << "unhandled format";
			format = DXGI_FORMAT_R8G8B8A8_UINT;
			break;
	}
	if(!setup(name, texture.getWidth(), texture.getHeight(), format)) {
		return false;
	}
	ofAddListener(ofEvents().update, this, &Sender::sendFrame);
	return true;
}

bool Sender::send(ofTexture &tex, bool invert)
{
	assert(sender_);
	return sender_->SendTexture(tex.texData.textureID, tex.texData.textureTarget, tex.getWidth(), tex.getHeight(), invert);
}

void Sender::release()
{
	if(sender_) {
		sender_->ReleaseSender();
	}
	sender_.reset();
}

