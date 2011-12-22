/*
 *  ofxKinectPointcloudRecorder.h
 *  PointcloudWriter
 *
 *  Created by Jim on 10/20/11.
 *  Copyright 2011 University of Washington. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxDepthImageCompressor.h"

typedef struct QueuedFrame {
	unsigned short* pixels;
	string directory;
	string filename;
	int timestamp;
};

//thread classes for callbacks
class ofxDepthImageRecorder;
class ofxRGBDRecorderThread : public ofThread {
public:
	ofxDepthImageRecorder* delegate;
	ofxRGBDRecorderThread(ofxDepthImageRecorder* d) : delegate(d){}	
	void threadedFunction();
};

class ofxRGBDEncoderThread : public ofThread {
public:
	ofxDepthImageRecorder* delegate;
	ofxRGBDEncoderThread(ofxDepthImageRecorder* d) : delegate(d){}
	void threadedFunction();	
};

class ofxDepthImageRecorder {
  public:
	ofxDepthImageRecorder();
	~ofxDepthImageRecorder();
	
	vector<string> getTakePaths();
	
	void setup();
	
	void setRecordLocation(string directory, string filePrefix);
	bool addImage(unsigned short* image);

	void incrementTake();
    //start converting the current directory
	void compressCurrentTake();
	
	
	int numFramesWaitingSave();
	int recordingStartTime; //in millis -- potentially should make this more accurate
	
	void encoderThreadCallback();
	void recorderThreadCallback();
	
  protected:
	ofxDepthImageCompressor compressor;
	ofxRGBDRecorderThread recorderThread;
	ofxRGBDEncoderThread encoderThread;
	
	ofImage compressedDepthImage;

	unsigned short* encodingBuffer;
	unsigned short* lastFramePixs;
	int folderCount;
    string currentFolderPrefix;
	string targetDirectory;
	string targetFilePrefix;
	int currentFrame;
	
	queue<QueuedFrame> saveQueue;
	queue<string> encodeDirectories;
};

//Threads