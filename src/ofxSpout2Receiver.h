// Spout : http://spout.zeal.co/

#pragma once

#include <string>
#include <memory>
#include "ofTexture.h"

class SpoutReceiver;
class ofEventArgs;

namespace ofx {
namespace spout2 {
class Receiver
{
public:
	~Receiver();
	bool setup(const std::string &name);
	bool setupAuto(const std::string &name, ofTexture &texture, bool invert=false);
	bool receive(ofTexture &texture, bool invert=false);
	void release();
protected:
	void receiveFrame(ofEventArgs&) {
		receive(*auto_.texture, auto_.invert);
	}
	std::shared_ptr<SpoutReceiver> receiver_;
	std::string sender_name_;
	struct Auto {
		ofTexture* texture;
		bool invert;
		bool enabled;
	} auto_;
};
}}
