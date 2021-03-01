#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 550, true, true, true);

	int span = 20;
	for (int x = span * 0.5; x <= ofGetWidth() - span * 0.5; x += span) {

		for (int y = span * 0.5; y <= ofGetHeight() - span * 0.5; y += span) {

			this->location_list.push_back(glm::vec2(x, y));
			this->value_list.push_back(0);
		}
	}

	this->word_index = 0;
	this->frame_span = 90;
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % this->frame_span == 0) {

		vector<string> word_list = { "L", "O", "V", "E", "!" };
		this->word_index = (this->word_index + 1) % word_list.size();
		string word = word_list[this->word_index];

		ofFbo fbo;
		fbo.allocate(ofGetWidth(), ofGetHeight());
		fbo.begin();
		ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
		ofClear(0);
		ofSetColor(0);
		this->font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word) * 0.5);
		fbo.end();
		ofPixels pixels;
		fbo.readToPixels(pixels);

		for (int i = 0; i < this->location_list.size(); i++) {

			ofColor color = pixels.getColor(this->location_list[i].x, this->location_list[i].y);
			if (color != ofColor(0, 0)) {

				this->value_list[i] = 1;
			}
			else {

				this->value_list[i] = 0;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	auto frame_param = ofGetFrameNum() % this->frame_span;
	for (int i = 0; i < this->location_list.size(); i++) {

		if (this->value_list[i] == 1) {

			auto noise_value = ofNoise(this->location_list[i] * 0.005);

			if (frame_param < this->frame_span * 0.6)	{

				noise_value += ofMap(frame_param, 0, this->frame_span * 0.3, 0, 1);
				if (noise_value > 1) { noise_value = 1; }
			}
			else {

				noise_value = ofMap(frame_param , this->frame_span * 0.6, this->frame_span, 1, 0);
				if (noise_value < 0) { noise_value = 0; }
			}

			ofDrawCircle(this->location_list[i], 3 + 5 * this->value_list[i] * noise_value);
		}
		else {

			ofDrawCircle(this->location_list[i], 3);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}