#include "ofApp.h"

// Set this to your HyperDeck's IP address (Setup menu on the deck).
static const std::string HYPERDECK_IP = "192.168.1.50";

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofBackground(20);
	ofAddListener(hyperdeck.responseEvent, this, &ofApp::onResponse);
	hyperdeck.setup(HYPERDECK_IP);
}

//--------------------------------------------------------------
void ofApp::update(){
	bool wasConnected = hyperdeck.isConnected();
	hyperdeck.update();
	if (hyperdeck.isConnected() && !wasConnected) {
		hyperdeck.enableNotifications();
		hyperdeck.requestTransportInfo();
		hyperdeck.requestClips();
	}
	// Timecode isn't pushed by notifications, so poll it.
	if (hyperdeck.isConnected() && ofGetElapsedTimeMillis() - lastPoll > 250) {
		hyperdeck.requestTransportInfo();
		lastPoll = ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	auto & t = hyperdeck.getTransportInfo();
	std::stringstream s;
	s << "HyperDeck " << HYPERDECK_IP << "  " << (hyperdeck.isConnected() ? "connected" : "not connected")
	  << (hyperdeck.getModel().empty() ? "" : "  (" + hyperdeck.getModel() + ")") << "\n\n";
	s << "status:   " << t.status << "\n";
	s << "speed:    " << t.speed << "%\n";
	s << "timecode: " << t.displayTimecode << "\n";
	s << "slot:     " << t.slotId << "   clip: " << t.clipId << "\n";
	s << "format:   " << t.videoFormat << "\n";
	if (!hyperdeck.getLastError().empty()) s << "last error: " << hyperdeck.getLastError() << "\n";
	s << "\nKeys: space play/stop  r record  1-9 go to clip  [ ] clip start/end\n"
	  << "      left/right jog 1 second  -/= shuttle -200%/+200%  c refresh clips\n\n";
	s << "Clips:\n";
	for (auto & c : hyperdeck.getClips()) {
		s << "  " << c.id << "  " << c.name << "  " << c.startTimecode << "  " << c.duration << "\n";
	}
	ofDrawBitmapString(s.str(), 20, 30);

	std::string recent;
	for (auto & l : log) recent += l + "\n";
	ofDrawBitmapString("Deck replies:\n" + recent, ofGetWidth() / 2 + 20, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	auto & t = hyperdeck.getTransportInfo();
	if (key == ' ') {
		if (t.status == "play") hyperdeck.stop(); else hyperdeck.play();
	}
	if (key == 'r') hyperdeck.record();
	if (key >= '1' && key <= '9') hyperdeck.gotoClip(key - '0');
	if (key == '[') hyperdeck.gotoClipStart();
	if (key == ']') hyperdeck.gotoClipEnd();
	if (key == OF_KEY_RIGHT) hyperdeck.jog("+00:00:01:00");
	if (key == OF_KEY_LEFT) hyperdeck.jog("-00:00:01:00");
	if (key == '-') hyperdeck.shuttle(-200);
	if (key == '=') hyperdeck.shuttle(200);
	if (key == 'c') hyperdeck.requestClips();
}

//--------------------------------------------------------------
void ofApp::onResponse(ofxHyperdeck::Response & response){
	if (response.text == "transport info" && response.code == 208) return; // our own polling, too noisy
	log.push_back(ofToString(response.code) + " " + response.text);
	while (log.size() > 30) log.pop_front();
}
