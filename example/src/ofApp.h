#pragma once

#include "ofMain.h"
#include "ofxHyperdeck.h"

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;

	void onResponse(ofxHyperdeck::Response & response);

	ofxHyperdeck hyperdeck;
	std::deque<std::string> log;
	uint64_t lastPoll = 0;
};
