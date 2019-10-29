// Spout : http://spout.zeal.co/

#pragma once

#include <string>
#include <memory>
#include "ofTexture.h"

class ofEventArgs;
class SpoutSender;

namespace ofx {
namespace spout2 {
class Sender
{
public:
	virtual ~Sender();
	bool setup(const std::string &name, unsigned int width, unsigned int height, DWORD dw_format);
	bool setupAuto(const std::string &name, ofTexture &texture, bool invert=false);
	bool send(ofTexture &texture, bool invert=false);
	void release();
protected:
	void sendFrame(ofEventArgs&) {
		send(auto_.texture, auto_.invert);
	}
	std::shared_ptr<SpoutSender> sender_;
	struct Auto {
		ofTexture texture;
		bool invert;
		bool enabled;
	} auto_;
};
}}
