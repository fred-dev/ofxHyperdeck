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
     
     hyperdeck.sendRaw("play");
     hyperdeck.sendRaw("\r\n");
    }

void ofxHyperdeck::stop(){
     
     hyperdeck.sendRaw("stop");
     hyperdeck.sendRaw("\r\n");
     
}

bool ofxHyperdeck::getRemote(){
     
     hyperdeck.sendRaw("remote");
      hyperdeck.sendRaw("\r\n");
}

void ofxHyperdeck::setOverride(bool _overRide){
 
     if (_overRide) {
          hyperdeck.sendRaw("remote:override:true");
           hyperdeck.sendRaw("\r\n");
     }
     if (!_overRide) {
          hyperdeck.sendRaw("remote:override:false");
           hyperdeck.sendRaw("\r\n");
     }
     
}
void ofxHyperdeck::setRemote(bool _remote){
   
     if (_remote) {
          hyperdeck.sendRaw("remote:enable:true");
           hyperdeck.sendRaw("\r\n");

     }
     if (!_remote) {
          hyperdeck.sendRaw("remote:enable:false");
           hyperdeck.sendRaw("\r\n");

     }
}
void ofxHyperdeck::exit(){
     
     hyperdeck.sendRaw("quit");
      hyperdeck.sendRaw("\r\n");
}
void ofxHyperdeck::togglePreview(bool _preview){
   
     if (_preview) {
          hyperdeck.sendRaw("preview:enable:true");
           hyperdeck.sendRaw("\r\n");
     }
     if (!_preview) {
          hyperdeck.sendRaw("preview:enable:false");
           hyperdeck.sendRaw("\r\n");
     }
     
}
void ofxHyperdeck::playWithSpeed(float speed){
     
     if (speed>-1600&&speed<1600) {
          hyperdeck.sendRaw("play:speed:"+ofToString(speed)+"");
           hyperdeck.sendRaw("\r\n");
     }
     else cout<< "Invalid speed, must be between -1600 and 1600"<<endl;
     
}
void ofxHyperdeck::playWithLoop(){
 
     hyperdeck.sendRaw("play:loop:true");
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::playSingleClip(){

     hyperdeck.sendRaw("play:single clip:true");
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::record(){
     
     hyperdeck.sendRaw("record");
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::recordWithName(string name){
   
     hyperdeck.sendRaw("record:name:" + name +"");
      hyperdeck.sendRaw("\r\n");

}

void ofxHyperdeck::goToClip(int clipId){
    
     hyperdeck.sendRaw("goto:clip:" + ofToString(clipId));
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::goToClipStart(){
     
     hyperdeck.sendRaw("goto:clip:start");
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::goToClipEnd(){
    
     hyperdeck.sendRaw("goto:clip:end");
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::goToTimecode(string timecode){
  
     hyperdeck.sendRaw("goto:timecode:" + timecode );
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::nudgeTimecodeForwards(string timecode){
    
     hyperdeck.sendRaw("goto:timecode:{+}{" + timecode );
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::nudgeTimecodeBack(string timecode){
    
     hyperdeck.sendRaw("goto:timecode:-" + timecode);
      hyperdeck.sendRaw("\r\n");
     
}
vector<string > ofxHyperdeck::getClips(){
    
     hyperdeck.sendRaw("clips get");
      hyperdeck.sendRaw("\r\n");

}
void ofxHyperdeck::setVideoInputType(string inputType){
     
     if (inputType=="SDI" || inputType=="HDMI" || inputType=="component") {
           hyperdeck.sendRaw("configuration:video input:"+ inputType);
           hyperdeck.sendRaw("\r\n");
     }
     
     else cout<< "Invalid format, options are SDI, HMDI or compnent"<<endl;
    
}
void ofxHyperdeck::setAudioInputType(string inputType){
    
     if (inputType=="embedded" || inputType=="XLR" || inputType=="RCA") {
          hyperdeck.sendRaw("configuration:audio input:"+ inputType);
           hyperdeck.sendRaw("\r\n");
     }
     
     else cout<< "Invalid format, options are embedded, XLR or RCA"<<endl;
}
void ofxHyperdeck::selectSlot(int slot){
   
     if (slot==1 or slot==2) {
          hyperdeck.sendRaw("slot select:slot:"+ofToString(slot));
           hyperdeck.sendRaw("\r\n");
     }
     else cout<< "Invalid slot, options are 1 or 2"<<endl;

}