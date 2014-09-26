//
//  ofxHyperdeck.cpp
//  ofxHyperdeck
//
//  Created by Fred on 25/09/14.
//
//

#include "ofxHyperdeck.h"


void ofxHyperdeck::setup(string ip){
     hyperdeck.setup(ip,9993);
     //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/hyperdeck]
     hyperdeck.setMessageDelimiter("\n");
}

void ofxHyperdeck::play(){
     hyperdeck.send("play");
}

bool ofxHyperdeck::getRemote(){
     hyperdeck.send("remote");
}

void ofxHyperdeck::setOverride(bool _overRide){
     if (_overRide) {
          hyperdeck.send("remote:override:true");
     }
     if (!_overRide) {
          hyperdeck.send("remote:override:false");
     }
     
}
void ofxHyperdeck::setRemote(bool _remote){
     if (_remote) {
          hyperdeck.send("remote:enable:true");

     }
     if (!_remote) {
          hyperdeck.send("remote:enable:false");

     }
}
void ofxHyperdeck::exit(){
     hyperdeck.send("quit");
}
void ofxHyperdeck::togglePreview(bool _preview){
     if (_preview) {
          hyperdeck.send("preview:enable:true");
     }
     if (!_preview) {
          hyperdeck.send("preview:enable:false");
     }
     
}
void ofxHyperdeck::playWithSpeed(float speed){
     if (speed>-1600&&speed<1600) {
          hyperdeck.send("play:speed:"+ofToString(speed));
     }
     else cout<< "Invalid speed, must be between -1600 and 1600"<<endl;
     
}
void ofxHyperdeck::playWithLoop(){
     hyperdeck.send("play:loop:true");

}
void ofxHyperdeck::playSingleClip(){
     hyperdeck.send("play:single clip:true");

}
void ofxHyperdeck::record(){
     hyperdeck.send("record");

}
void ofxHyperdeck::recordWithName(string name){
     hyperdeck.send("record:name:{" + name +"}");

}
void ofxHyperdeck::stop(){
     hyperdeck.send("stop");

}
void ofxHyperdeck::goToClip(int clipId){
     hyperdeck.send("goto:clip:{" + ofToString(clipId)+"}");

}
void ofxHyperdeck::goToClipStart(){
     hyperdeck.send("goto:clip:{start}");

}
void ofxHyperdeck::goToClipEnd(){
     hyperdeck.send("goto:clip:{end}");

}
void ofxHyperdeck::goToTimecode(string timecode){
     hyperdeck.send("goto:timecode:{" + timecode + "}");

}
void ofxHyperdeck::nudgeTimecodeForwards(string timecode){
     hyperdeck.send("goto:timecode:{+}{" + timecode + "}");

}
void ofxHyperdeck::nudgeTimecodeBack(string timecode){
     hyperdeck.send("goto:timecode:{-}{" + timecode + "}");
     
}
vector<string > ofxHyperdeck::getClips(){
     hyperdeck.send("clips get");

}
void ofxHyperdeck::setVideoInputType(string inputType){
     if (inputType=="SDI" || inputType=="HDMI" || inputType=="component") {
           hyperdeck.send("configuration:video input:{"+ inputType+"}");
     }
     
     else cout<< "Invalid format, options are SDI, HMDI or compnent"<<endl;
    
}
void ofxHyperdeck::setAudioInputType(string inputType){
     if (inputType=="embedded" || inputType=="XLR" || inputType=="RCA") {
          hyperdeck.send("configuration:audio input:{"+ inputType+"}");
     }
     
     else cout<< "Invalid format, options are embedded, XLR or RCA"<<endl;
}
void ofxHyperdeck::selectSlot(int slot){
     if (slot==1 or slot==2) {
          hyperdeck.send("slot select:slot:{"+ofToString(slot)+"}");
     }
     else cout<< "Invalid slot, options are 1 or 2"<<endl;

}