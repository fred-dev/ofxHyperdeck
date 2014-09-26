//
//  ofxHyperdeck.cpp
//  ofxHyperdeck
//
//  Created by Fred on 25/09/14.
//
//

#include "ofxHyperdeck.h"


void ofxHyperdeck::setup(string ip){
     _connected=false;
     _ip=ip;
     hyperdeck.setup(ip,9993);
     _connected=true;
     //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/hyperdeck]
     hyperdeck.setMessageDelimiter("\n");
     if (hyperdeck.isConnected()) {
          cout<<"Connected"<<endl;
     }
     
}
void ofxHyperdeck::reconnect(){
     if (checkConnection()==false) {
           hyperdeck.setup(_ip, 9993);
     }
    
     
}

bool ofxHyperdeck::checkConnection(){
    _connected=hyperdeck.isConnected();
     return _connected;
     
}
void ofxHyperdeck::play(){
     
     hyperdeck.send("play");
     hyperdeck.send("\r");
    }

void ofxHyperdeck::stop(){
     
     hyperdeck.send("stop");
     hyperdeck.send("\r");
     
}

bool ofxHyperdeck::getRemote(){
     
     hyperdeck.send("remote");
      hyperdeck.send("\r\n");
}

void ofxHyperdeck::setOverride(bool _overRide){
 
     if (_overRide) {
          hyperdeck.send("remote:override:true");
           hyperdeck.send("\r\n");
     }
     if (!_overRide) {
          hyperdeck.send("remote:override:false");
           hyperdeck.send("\r\n");
     }
     
}
void ofxHyperdeck::setRemote(bool _remote){
   
     if (_remote) {
          hyperdeck.send("remote:enable:true");
           hyperdeck.send("\r\n");

     }
     if (!_remote) {
          hyperdeck.send("remote:enable:false");
           hyperdeck.send("\r\n");

     }
}
void ofxHyperdeck::exit(){
     
     hyperdeck.send("quit");
      hyperdeck.send("\r\n");
}
void ofxHyperdeck::togglePreview(bool _preview){
   
     if (_preview) {
          hyperdeck.send("preview:enable:true");
           hyperdeck.send("\r\n");
     }
     if (!_preview) {
          hyperdeck.send("preview:enable:false");
           hyperdeck.send("\r\n");
     }
     
}
void ofxHyperdeck::playWithSpeed(float speed){
     
     if (speed>-1600&&speed<1600) {
          hyperdeck.send("play:speed:"+ofToString(speed)+"");
           hyperdeck.send("\r\n");
     }
     else cout<< "Invalid speed, must be between -1600 and 1600"<<endl;
     
}
void ofxHyperdeck::playWithLoop(){
 
     hyperdeck.send("play:loop:true");
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::playSingleClip(){

     hyperdeck.send("play:single clip:true");
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::record(){
     
     hyperdeck.send("record");
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::recordWithName(string name){
   
     hyperdeck.send("record:name:" + name +"");
      hyperdeck.send("\r\n");

}

void ofxHyperdeck::goToClip(int clipId){
    
     hyperdeck.send("goto:clip:" + ofToString(clipId));
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::goToClipStart(){
     
     hyperdeck.send("goto:clip:start");
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::goToClipEnd(){
    
     hyperdeck.send("goto:clip:end");
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::goToTimecode(string timecode){
  
     hyperdeck.send("goto:timecode:" + timecode );
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::nudgeTimecodeForwards(string timecode){
    
     hyperdeck.send("goto:timecode:{+}{" + timecode );
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::nudgeTimecodeBack(string timecode){
    
     hyperdeck.send("goto:timecode:-" + timecode);
      hyperdeck.send("\r\n");
     
}
vector<string > ofxHyperdeck::getClips(){
    
     hyperdeck.send("clips get");
      hyperdeck.send("\r\n");

}
void ofxHyperdeck::setVideoInputType(string inputType){
     
     if (inputType=="SDI" || inputType=="HDMI" || inputType=="component") {
           hyperdeck.send("configuration:video input:"+ inputType);
           hyperdeck.send("\r\n");
     }
     
     else cout<< "Invalid format, options are SDI, HMDI or compnent"<<endl;
    
}
void ofxHyperdeck::setAudioInputType(string inputType){
    
     if (inputType=="embedded" || inputType=="XLR" || inputType=="RCA") {
          hyperdeck.send("configuration:audio input:"+ inputType);
           hyperdeck.send("\r\n");
     }
     
     else cout<< "Invalid format, options are embedded, XLR or RCA"<<endl;
}
void ofxHyperdeck::selectSlot(int slot){
   
     if (slot==1 or slot==2) {
          hyperdeck.send("slot select:slot:"+ofToString(slot));
           hyperdeck.send("\r\n");
     }
     else cout<< "Invalid slot, options are 1 or 2"<<endl;

}