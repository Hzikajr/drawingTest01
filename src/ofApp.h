#pragma once

#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxPostProcessing.h"
#include "ofxTriangle.h"

//#define DEBUG_MODE

/**
 swingCamera class
    using ofGetElapsedTimef() to make swinging movement
    of ofCamera
 */
class swingCamera{
public:
    swingCamera()
    {
        pos.set(0, 0, 0);
        dest.set(0, 0, 0);
        lookAt.set(0, 0, 0);
        dir.set(0, 0, 0);
        
        bIsMoving = false;
    }
    ~swingCamera(){};
    
    void setup(ofVec3f position, ofVec3f lookAtPoint)
    {
        pos = position;
        lookAt = lookAtPoint;
        
        camera.setPosition(0, 0, 0);
        camera.setGlobalPosition(pos);
        camera.lookAt(lookAt);
    }
    
    void update()
    {
        camera.move(sin(ofGetElapsedTimef())*2,
                    cos(ofGetElapsedTimef()),
                    0);
    }
    
    void begin()
    {
        camera.begin();
    }
    
    void end()
    {
        camera.end();
    }
    
private:
    ofCamera    camera;
    ofVec3f     pos, dest, lookAt, dir;
    bool        bIsMoving;
    float       vel;
};

/**
 polyAurora class
    array of triangles that are colored randomly
 */
class polyAurora
{
public:
    polyAurora(){};
    ~polyAurora()
    {
        delete vertices;
        delete cols;
    };
    
    void setup(unsigned numOfTriangles)
    {
        vertices = new ofPoint[numOfTriangles+3];
        cols = new ofColor[numOfTriangles];
        size = numOfTriangles;
        
        vertices[0] = ofPoint(ofRandom(-50, 50),
                              ofRandom(-50, 50),
                              ofRandom(-50, 50));
        vertices[1] = ofPoint(ofRandom(-50, 50),
                              ofRandom(-50, 50),
                              ofRandom(-50, 50));
        vertices[2] = ofPoint(ofRandom(-50, 50),
                              ofRandom(-50, 50),
                              ofRandom(-50, 50));
        for (int i = 0; i < size; i++) {
            vertices[i+3] = ofPoint(ofRandom(vertices[i+2].x-100, vertices[i+2].y+100),
                                    ofRandom(vertices[i+2].y-100, vertices[i+2].y+100),
                                    ofRandom(vertices[i+2].z-100, vertices[i+2].z+100));
            cols[i] = ofColor(ofRandom(0, 250), ofRandom(0, 250), ofRandom(0, 250), ofRandom(255));
        }
        
        prevTime = ofGetElapsedTimef();
    }
    
    void update()
    {
        for (int i = 0; i < size+3; i++) {
            vertices[i] += ofSignedNoise(ofGetElapsedTimef());
        }
    }
    
    void draw()
    {
        for (int i = 0; i < size; i++) {
            ofPushStyle();
            ofSetColor(cols[i], cols[i].a+ofRandom(-20, 20));
            ofTriangle(vertices[i], vertices[i+1], vertices[i+2]);
            ofPopStyle();
        }
    }
    
private:
    ofPoint     *vertices;
    ofColor     *cols;
    float       prevTime;
    unsigned    size;
};

/**
 noisyFont class
 */
class noisyFont
{
public:
    noisyFont(){};
    ~noisyFont(){};
    
    void setup()
    {
        en.loadFont("en.ttf", 72, true, true, true);
        jp.loadFont("jp.otf", 72, true, true, true);
    }

    void update()
    {
        if (bNextWordInserted && boundaryPts.size()) {
            boundaryPts[boundaryPts.size()-1].getTessellation().getVertices().erase(
                                                boundaryPts[boundaryPts.size()-1].getTessellation().getVertices().end()-80,
                                                boundaryPts[boundaryPts.size()-1].getTessellation().getVertices().end());
            if (boundaryPts[boundaryPts.size()-1].getTessellation().getVertices().size() < 80) {
                boundaryPts.erase(boundaryPts.end());
            }
        }
        else if (bNextWordInserted && !boundaryPts.size()) {
            boundaryPts = (bEnOrJp ? jp.getStringAsPoints(word.c_str())
                                   : en.getStringAsPoints(word.c_str()));
            
            bNextWordInserted = false;
        }
    }
    
    void draw()
    {
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        bEnOrJp ? ofTranslate(-(jp.getStringBoundingBox(word.c_str(), 0, 0).width/2), 0)
                : ofTranslate(-(en.getStringBoundingBox(word.c_str(), 0, 0).width/2), 0);
        ofScale(1.4f, 1.4f);
        ofSetColor(255);
        ofSetLineWidth(.5f);
        
        for (int i = 0; i < boundaryPts.size(); i++) {
            for (int j = 0; j < boundaryPts[i].getTessellation().getNumVertices()-1; j+=(int)ofRandom(10)) {
                ofLine(boundaryPts[i].getTessellation().getVertices()[j].x,
                       boundaryPts[i].getTessellation().getVertices()[j].y,
                       boundaryPts[i].getTessellation().getVertices()[j+1].x,
                       boundaryPts[i].getTessellation().getVertices()[j+1].y);
                ofCircle(boundaryPts[i].getTessellation().getVertices()[j].x,
                         boundaryPts[i].getTessellation().getVertices()[j].y,
                         3);
            }
        }
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    void inputWord(string input_word, bool JapaneseOrEnglish)
    {
        word = input_word;
        bEnOrJp = JapaneseOrEnglish;
        bNextWordInserted = true;
    }
    
private:
    ofTrueTypeFont  en;
    ofxTrueTypeFontUC jp;
    
    string          word;
    vector<ofTTFCharacter>  boundaryPts;
    bool            bEnOrJp;            // 0: en, 1: jp
    bool            bNextWordInserted;
};

/**
 triangleParticles class
    bang with some triangles
 */
class triangleParticles
{
public:
    triangleParticles()
    {
        speed = 0.0f;
        bTriangleDrawing = false;
    }
    ~triangleParticles(){};
    
    void begin()
    {
        for (int i = 0; i < 256; i++) {
            plc[i].set(0, 0, 0);
            col[i].set(ofRandom(255), 0, ofRandom(255));
            rot[i].set(ofRandom(180), ofRandom(1), ofRandom(1), ofRandom(1));
        }
        speed = 255;
        bTriangleDrawing = true;
    }
    
    void update()
    {
        if (bTriangleDrawing) {
            for (int i = 0; i < 256; i++) {
                plc[i] += ofVec3f(ofRandom(1)*20, ofRandom(1)*20, 0);
            }
        
            speed -= 3;
            if (speed == -1) {
                bTriangleDrawing = false;
            }
        }
    }
    
    void draw()
    {
        if (bTriangleDrawing) {
            for (int i = 0; i < 256; i++) {
                ofSetColor(col[i], speed);
                ofRotate(rot[i].x, rot[i].y, rot[i].z, rot[i].w);
                ofTriangle(plc[i].x-4, plc[i].y-10, plc[i].z,
                           plc[i].x-4, plc[i].y+10, plc[i].z,
                           plc[i].x+12, plc[i].y, plc[i].z);
            }
        }
    }
    
private:
    ofPoint plc[256];
    ofColor col[256];
    ofVec4f rot[256];
    
    float   speed;
    bool    bTriangleDrawing;
};

class rhymeWire
{
public:
    rhymeWire(){};
    ~rhymeWire()
    {
        triangle.clear();
        polyline.clear();
        words.clear();
        boundary.clear();
        pts.clear();
        transPts.clear();
    }
    
    void setup()
    {
        font.loadFont("jp.otf", 72, true, true, true);
        prevTime = ofGetElapsedTimef();
        color.set(ofColor::white);
        pt.set(-40, 10, 20);
    }
    
    void update()
    {
        if (ofGetElapsedTimef() - prevTime > 0.05) {
            polyline.clear();
            
            for (int i = 0; i < words.size(); i++) {
                boundary = font.getStringAsPoints(words[i]);
                
                pts.clear();
                triangle.clear();
                
                for (int j = 0; j < boundary.size(); j++) {
                    for (int k = 0; k < boundary[j].getOutline().size(); k++) {
                        for (int l = 0; l < boundary[j].getOutline()[k].getVertices().size(); l+=(int)ofRandom(20)) {
                            pts.push_back(boundary[j].getOutline()[k].getVertices()[l]);
                        }
                    }
                }
                triangle.triangulate(pts, boundary.size()*15);
                
                for (int a = 0; a < triangle.triangles.size(); a++) {
                    polyline.addVertex(triangle.triangles[a].a+transPts[words.size()-i-1]);
                    polyline.addVertex(triangle.triangles[a].b+transPts[words.size()-i-1]);
                    polyline.addVertex(triangle.triangles[a].c+transPts[words.size()-i-1]);
                }
            }
            
            prevTime = ofGetElapsedTimef();
        }
    }
    
    void draw()
    {
        ofPushStyle();
        ofSetColor(color);
        polyline.draw();
        ofPopStyle();
    }
    
    void addWord(string newWord)
    {
        words.push_back(newWord);
        pt += ofPoint(200, -10, 10);
        pt.rotate(-10, -10, 15);
        transPts.push_back(pt);
    }
    
    void setColor(ofColor newColor)
    {
        color = newColor;
    }
    
private:
    ofxTriangle     triangle;
    ofPolyline      polyline;
    ofxTrueTypeFontUC   font;
    
    string          buf;
    ofColor         color;
    vector<string>  words;
    vector<ofTTFCharacter>  boundary;
    vector<ofPoint> pts;
    vector<ofPoint> transPts;
    ofPoint         pt;
    float           prevTime;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // sound related
    void audioIn(float *input, int bufferSize, int nChannels);
    ofSoundStream   ss;
    vector<float>   sound;
		
    // instances of my classes
    swingCamera         cam;
    polyAurora          aurora;
    triangleParticles   tris;
    noisyFont           font;
    rhymeWire           rhyme;
    
    // camera for debug use
    ofEasyCam   debugCam;
    
    // lyrics related
    ofBuffer        file;
    string          buf;
    vector<string>  lyrics;
    unsigned        lyricsPos;
    
    // visual effect
    ofxPostProcessing   pspr;
    
    // params
    float       rot;
    bool        bDrawAurora, bDrawWave, bDrawFont, bDrawGrid, bDrawRhyme;
};
