//
//  ofxHyperdeck.h
//
//  Control a Blackmagic HyperDeck over its Ethernet protocol (TCP port 9993).
//  Commands are sent as text lines. Replies and asynchronous notifications are
//  read in update() and kept as the latest transport and clip state.
//

#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

class ofxHyperdeck {
public:
	struct TransportInfo {
		std::string status;          // preview, stopped, play, forward, rewind, jog, shuttle, record
		float speed = 0;             // percent of normal speed
		int slotId = 0;
		int clipId = 0;
		bool singleClip = false;
		bool loop = false;
		std::string displayTimecode;
		std::string timecode;
		std::string videoFormat;
	};

	struct Clip {
		int id = 0;
		std::string name;
		std::string startTimecode;
		std::string duration;
	};

	struct Response {
		int code = 0;                // 1xx error, 2xx success, 5xx asynchronous notification
		std::string text;            // e.g. "ok", "transport info"
		std::vector<std::pair<std::string, std::string>> fields; // "key: value" lines of a multi-line reply
	};

	~ofxHyperdeck();

	/// Connects to the deck. update() reconnects automatically if the connection drops.
	bool setup(const std::string & ip, int port = 9993);
	/// Call every frame: reads replies and keeps the connection alive.
	void update();
	void close();
	bool isConnected();

	// Transport
	void play();
	void play(float speedPercent, bool loop = false, bool singleClip = false);
	void stop();
	void record();
	void record(const std::string & clipName);
	void gotoClip(int clipId);
	void gotoClipStart();
	void gotoClipEnd();
	void gotoTimecode(const std::string & timecode);    // "hh:mm:ss:ff"
	void jog(const std::string & relativeTimecode);     // "+00:00:01:00" or "-00:00:00:10"
	void shuttle(float speedPercent);

	// Setup
	void selectSlot(int slotId);
	void setVideoInput(const std::string & input);      // "SDI", "HDMI" or "component"
	void setAudioInput(const std::string & input);      // "embedded", "XLR" or "RCA"
	void setFileFormat(const std::string & format);
	void setRemote(bool enable);
	void setRemoteOverride(bool enable);
	void setPreview(bool enable);

	// Queries. Replies arrive through update() and responseEvent.
	void requestTransportInfo();
	void requestClips();
	void enableNotifications(bool transport = true, bool slot = true);

	/// Sends any raw protocol command, e.g. "device info".
	void send(const std::string & command);

	const TransportInfo & getTransportInfo() const { return transport; }
	const std::vector<Clip> & getClips() const { return clips; }
	const std::string & getLastError() const { return lastError; }
	const std::string & getModel() const { return model; }

	/// Fired for every reply and notification from the deck.
	ofEvent<Response> responseEvent;

private:
	void connect();
	void handleLine(const std::string & line);
	void finishResponse();
	void applyResponse(const Response & r);

	ofxTCPClient client;
	std::string ip;
	int port = 9993;
	bool wantConnection = false;
	uint64_t lastConnectAttempt = 0;

	std::string buffer;
	Response pending;
	bool inMultiline = false;

	TransportInfo transport;
	std::vector<Clip> clips;
	std::string lastError;
	std::string model;
};
