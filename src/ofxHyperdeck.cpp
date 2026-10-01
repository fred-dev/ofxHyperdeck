//
//  ofxHyperdeck.cpp
//

#include "ofxHyperdeck.h"

namespace {
	std::string boolString(bool b) { return b ? "true" : "false"; }
}

ofxHyperdeck::~ofxHyperdeck(){
	close();
}

bool ofxHyperdeck::setup(const std::string & _ip, int _port){
	ip = _ip;
	port = _port;
	wantConnection = true;
	connect();
	return client.isConnected();
}

void ofxHyperdeck::connect(){
	lastConnectAttempt = ofGetElapsedTimeMillis();
	buffer.clear();
	inMultiline = false;
	if (client.setup(ip, port, false)) {
		ofLogNotice("ofxHyperdeck") << "Connected to " << ip << ":" << port;
	} else {
		ofLogWarning("ofxHyperdeck") << "Could not connect to " << ip << ":" << port << ", will retry";
	}
}

void ofxHyperdeck::close(){
	wantConnection = false;
	if (client.isConnected()) {
		client.sendRaw("quit\r\n");
		client.close();
	}
}

bool ofxHyperdeck::isConnected(){
	return client.isConnected();
}

void ofxHyperdeck::update(){
	if (!client.isConnected()) {
		if (wantConnection && ofGetElapsedTimeMillis() - lastConnectAttempt > 2000) {
			connect();
		}
		return;
	}
	buffer += client.receiveRaw();
	size_t pos;
	while ((pos = buffer.find('\n')) != std::string::npos) {
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 1);
		if (!line.empty() && line.back() == '\r') line.pop_back();
		handleLine(line);
	}
}

// Replies are "NNN text" on one line, or "NNN text:" followed by
// "key: value" lines and a blank line.
void ofxHyperdeck::handleLine(const std::string & line){
	if (inMultiline) {
		if (line.empty()) {
			finishResponse();
			return;
		}
		size_t colon = line.find(':');
		if (colon == std::string::npos) {
			pending.fields.push_back({line, ""});
		} else {
			pending.fields.push_back({ofTrim(line.substr(0, colon)), ofTrim(line.substr(colon + 1))});
		}
		return;
	}
	if (line.size() < 3 || !isdigit(line[0])) return;
	pending = Response();
	pending.code = ofToInt(line.substr(0, 3));
	pending.text = ofTrim(line.substr(3));
	if (!pending.text.empty() && pending.text.back() == ':') {
		pending.text.pop_back();
		inMultiline = true;
	} else {
		finishResponse();
	}
}

void ofxHyperdeck::finishResponse(){
	inMultiline = false;
	applyResponse(pending);
	ofNotifyEvent(responseEvent, pending, this);
}

void ofxHyperdeck::applyResponse(const Response & r){
	if (r.code >= 100 && r.code < 200) {
		lastError = ofToString(r.code) + " " + r.text;
		ofLogWarning("ofxHyperdeck") << lastError;
		return;
	}
	if (r.text == "transport info") {
		for (auto & f : r.fields) {
			if (f.first == "status") transport.status = f.second;
			else if (f.first == "speed") transport.speed = ofToFloat(f.second);
			else if (f.first == "slot id") transport.slotId = ofToInt(f.second);
			else if (f.first == "clip id") transport.clipId = ofToInt(f.second);
			else if (f.first == "single clip") transport.singleClip = (f.second == "true");
			else if (f.first == "loop") transport.loop = (f.second == "true");
			else if (f.first == "display timecode") transport.displayTimecode = f.second;
			else if (f.first == "timecode") transport.timecode = f.second;
			else if (f.first == "video format") transport.videoFormat = f.second;
		}
	} else if (r.text == "clips info") {
		clips.clear();
		for (auto & f : r.fields) {
			if (f.first == "clip count" || f.first.empty() || !isdigit(f.first[0])) continue;
			// "<id>: <name> <start timecode> <duration>"; the name may contain spaces.
			std::vector<std::string> parts = ofSplitString(f.second, " ", true, true);
			Clip c;
			c.id = ofToInt(f.first);
			if (parts.size() >= 3) {
				c.duration = parts.back(); parts.pop_back();
				c.startTimecode = parts.back(); parts.pop_back();
			}
			c.name = ofJoinString(parts, " ");
			clips.push_back(c);
		}
	} else if (r.text == "connection info") {
		for (auto & f : r.fields) {
			if (f.first == "model") model = f.second;
		}
	}
}

void ofxHyperdeck::send(const std::string & command){
	if (!client.isConnected()) {
		ofLogWarning("ofxHyperdeck") << "Not connected, dropped: " << command;
		return;
	}
	client.sendRaw(command + "\r\n");
}

// Transport
void ofxHyperdeck::play(){ send("play"); }

void ofxHyperdeck::play(float speedPercent, bool loop, bool singleClip){
	speedPercent = ofClamp(speedPercent, -5000, 5000);
	// Multi-parameter commands: one parameter per line, ended by a blank line.
	send("play:\r\nspeed: " + ofToString(int(speedPercent)) + "\r\nloop: " + boolString(loop) + "\r\nsingle clip: " + boolString(singleClip) + "\r\n");
}

void ofxHyperdeck::stop(){ send("stop"); }
void ofxHyperdeck::record(){ send("record"); }
void ofxHyperdeck::record(const std::string & clipName){ send("record: name: " + clipName); }
void ofxHyperdeck::gotoClip(int clipId){ send("goto: clip id: " + ofToString(clipId)); }
void ofxHyperdeck::gotoClipStart(){ send("goto: clip: start"); }
void ofxHyperdeck::gotoClipEnd(){ send("goto: clip: end"); }
void ofxHyperdeck::gotoTimecode(const std::string & timecode){ send("goto: timecode: " + timecode); }
void ofxHyperdeck::jog(const std::string & relativeTimecode){ send("jog: timecode: " + relativeTimecode); }
void ofxHyperdeck::shuttle(float speedPercent){ send("shuttle: speed: " + ofToString(int(ofClamp(speedPercent, -5000, 5000)))); }

// Setup
void ofxHyperdeck::selectSlot(int slotId){ send("slot select: slot id: " + ofToString(slotId)); }

void ofxHyperdeck::setVideoInput(const std::string & input){
	if (input != "SDI" && input != "HDMI" && input != "component") {
		ofLogWarning("ofxHyperdeck") << "Video input must be SDI, HDMI or component, not " << input;
		return;
	}
	send("configuration: video input: " + input);
}

void ofxHyperdeck::setAudioInput(const std::string & input){
	if (input != "embedded" && input != "XLR" && input != "RCA") {
		ofLogWarning("ofxHyperdeck") << "Audio input must be embedded, XLR or RCA, not " << input;
		return;
	}
	send("configuration: audio input: " + input);
}

void ofxHyperdeck::setFileFormat(const std::string & format){ send("configuration: file format: " + format); }
void ofxHyperdeck::setRemote(bool enable){ send("remote: enable: " + boolString(enable)); }
void ofxHyperdeck::setRemoteOverride(bool enable){ send("remote: override: " + boolString(enable)); }
void ofxHyperdeck::setPreview(bool enable){ send("preview: enable: " + boolString(enable)); }

// Queries
void ofxHyperdeck::requestTransportInfo(){ send("transport info"); }
void ofxHyperdeck::requestClips(){ send("clips get"); }

void ofxHyperdeck::enableNotifications(bool transportNotify, bool slotNotify){
	send("notify: transport: " + boolString(transportNotify));
	send("notify: slot: " + boolString(slotNotify));
}
