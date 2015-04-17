#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
#ifdef DEBUG_MODE
    // free run
    ofSetVerticalSync(false);
    ofSetFrameRate(0);
#endif
#ifndef DEBUG_MODE
    // vertical syncing is on and framerate will be set as 60fps
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
#endif
    ofSetCoordHandedness(OF_RIGHT_HANDED);
    ofEnableAlphaBlending();
    
    // classes settings
    cam.setup(ofVec3f(0, 0, 500), ofVec3f(0, 0, 0));
    aurora.setup(20);
    font.setup();
    font.inputWord("Alphabet", 0);
    rhyme.setup();
    rhyme.setColor(ofColor::lightBlue);
    
    // sound settings
    ss.listDevices();
    sound.assign(ofGetWidth()*2, 0.0);
    ss.setup(this, 0, 1, 44100, ofGetWidth()*2, 4);
    
    // read lyrics data
    file = ofBufferFromFile("lyrics.txt");
    while (file.isLastLine() == false) {
        buf = file.getNextLine();
        if (!buf.empty()) {
            lyrics.push_back(buf);
        }
        cout << buf << endl;
    }
    lyricsPos = 0;
    
    // initialize visual effect system
    pspr.init();
    pspr.createPass<HorizontalTiltShifPass>()->setEnabled(false);
    pspr.createPass<LimbDarkeningPass>()->setEnabled(false);
    pspr.createPass<PixelatePass>()->setEnabled(false);
    pspr.createPass<RGBShiftPass>()->setEnabled(false);
    pspr.createPass<BloomPass>()->setEnabled(false);
    pspr.createPass<SSAOPass>()->setEnabled(false);
    pspr.createPass<ZoomBlurPass>()->setEnabled(false);
    
    // setup parameters
    bDrawAurora = false;
    bDrawWave = false;
    bDrawFont = false;
    bDrawGrid = false;
    bDrawRhyme = false;
    rot = 0.0f;
    
    // debug use
    debugCam.setDistance(500);
}

//--------------------------------------------------------------
void ofApp::update(){
    // update classes and a parameter
    cam.update();
    aurora.update();
    tris.update();
    font.update();
    rhyme.update();
    rot += ofSignedNoise(ofGetElapsedTimef())*3;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // set background color as black
    ofBackground(0);
    
    // visual effect start from here
    glPushAttrib(GL_ENABLE_BIT);
    pspr.begin();
    
    // camera area start from here
    cam.begin();
    
    // draw grid
    if (bDrawGrid) {
        ofSetColor(255, 200);
        for (int x = -5; x < 5; x++) {
            for (int y = -5; y < 5; y++) {
                for (int z = -5; z < 5; z++) {
                    ofLine(200*x-4, 200*y, 200*z, 200*x+4, 200*y, 200*z);
                    ofLine(200*x, 200*y-4, 200*z, 200*x, 200*y+4, 200*z);
                    ofLine(200*x, 200*y, 200*z-4, 200*x, 200*y, 200*z+4);
                }
            }
        }
    }
    
    // draw "rhymeWire"
    if (bDrawRhyme) {
        ofPushMatrix();
        ofTranslate(-(ofGetWidth()/4), -(ofGetHeight()/4));
        ofRotateX((ofGetElapsedTimef())*5);
        ofRotateY((ofGetElapsedTimef())*5);
        rhyme.draw();
        ofPopMatrix();
    }
    
    // draw "polyAurora"
    if (bDrawAurora) {
        ofPushMatrix();
        ofRotate(cos(ofGetElapsedTimef())*40, 1, -1, 0);
        
        ofPushMatrix();
        ofTranslate(ofSignedNoise(ofGetElapsedTimef())*10, 0, 0);
        glEnable(GL_DEPTH_TEST);
        aurora.draw();
        glDisable(GL_DEPTH_TEST);
        ofPopMatrix();
    
        ofPopMatrix();
    }
    
    // draw "triangleParticles"
    tris.draw();
    
    // camera area end here
    cam.end();
    
    // draw sound wave
    if (bDrawWave) {
        ofPushStyle();
        ofNoFill();
        ofSetColor(ofColor::white);
        ofSetLineWidth(3);
        ofBeginShape();
        for (int i = 0; i < ofGetWidth()*2; i++) {
            ofVertex(i/2, ofGetHeight()/2+sound[i]*360.f);
        }
        ofEndShape();
        ofPopStyle();
    }
    
    // visual effect end here
    pspr.end();
    glPopAttrib();
    
    // draw "noisyFont"
    if (bDrawFont) {
        font.draw();
    }
}

// function to get audio signal data
void ofApp::audioIn(float *input, int bufferSize, int nChannels)
{
    for (int i = 0; i < bufferSize; i++) {
        sound[i]    = input[i]*0.5;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // switching visual effects
    if (key == 'q') {
        pspr[0]->setEnabled(true);
    }
    if (key == 'w') {
        pspr[1]->setEnabled(true);
    }
    if (key == 'e') {
        pspr[2]->setEnabled(true);
    }
    if (key == 'r') {
        pspr[3]->setEnabled(true);
    }
    if (key == 't') {
        pspr[4]->setEnabled(true);
    }
    if (key == 'y') {
        pspr[5]->setEnabled(true);
    }
    if (key == 'u') {
        pspr[6]->setEnabled(true);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // control each drawing patterns
    if (key == 'z') {
        bDrawWave ? bDrawWave = false : bDrawWave = true;
    }
    if (key == 'x') {
        bDrawAurora ? bDrawAurora = false : bDrawAurora = true;
    }
    if (key == OF_KEY_RETURN) {
        aurora.setup(50);
    }
    if (key == 'v') {
        tris.begin();
    }
    if (key == 'b') {
        bDrawGrid ? bDrawGrid = false : bDrawGrid = true;
    }
    if (key == '1') {
        bool enOrJp = (lyricsPos+1)%3 ? 0 : 1;
        font.inputWord(lyrics[lyricsPos], enOrJp);
        rhyme.addWord(lyrics[lyricsPos]);
        lyricsPos++;
    }
    if (key == 'c') {
        bDrawFont ? bDrawFont = false : bDrawFont = true;
    }
    if (key == '0') {
        font.inputWord("Alphabet", 0);
    }
    if (key == 'n') {
        bDrawRhyme ? bDrawRhyme = false : bDrawRhyme = true;
    }
    
    // switching visual effects
    if (key == 'q') {
        pspr[0]->setEnabled(false);
    }
    if (key == 'w') {
        pspr[1]->setEnabled(false);
    }
    if (key == 'e') {
        pspr[2]->setEnabled(false);
    }
    if (key == 'r') {
        pspr[3]->setEnabled(false);
    }
    if (key == 't') {
        pspr[4]->setEnabled(false);
    }
    if (key == 'y') {
        pspr[5]->setEnabled(false);
    }
    if (key == 'u') {
        pspr[6]->setEnabled(false);
    }
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
