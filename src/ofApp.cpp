#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
     ofSetVerticalSync(true);
     
     //setup the server to listen on 11999
     hyperdeck.setup("127.0.0.1");
     
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
     
     
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
     
     switch (key) {
          case '1':
               hyperdeck.play();
               break;
          case '2':
               hyperdeck.stop();
               break;
          case '3':
               hyperdeck.goToTimecode("00:00:30:22");
               hyperdeck.play();
               break;
          case '4':
               hyperdeck.goToClipStart();
               break;
          case '5':
               hyperdeck.goToClipEnd();
               break;
          case 's':
               hyperdeck.setVideoInputType("SDI");
               break;
               
               

     }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
