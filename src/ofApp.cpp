#include "ofApp.h"
#include "heightmap.h"
#include "perlin.h"
#include <iostream>
#include <string>
#include <math.h>

using std::cout;
using std::endl;

//--------------------------------------------------------------
void ofApp::setup(){

  PerlinGenerator generator(200);

  /*
  generator.addOctave(.5, 300);
  generator.addOctave(.25, 50);
  generator.addOctave(.25, 25);
  */
  generator.addOctave(8, 200);
  generator.addOctave(4, 100);
  generator.addOctave(2, 50);
  generator.addOctave(1, 25);

  Heightmap heightmap(200, generator);

  for(int i = 0; i < 30; i++)
  {
    heightmap.erode();
    for(int i = 0; i < 200; i++)
    {
      for(int j = 0; j < 200; j++)
      {
        grid[200 * i + j] = heightmap.getHeight()[i][j];
      }
    }

    image.setFromPixels(grid, 200, 200, OF_IMAGE_GRAYSCALE);

    std::string filename = "test" + std::to_string(i) + ".jpg";
    image.save(filename);
  }


  
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
  image.draw(0,0);
  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
