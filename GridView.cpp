//
//  GridView.cpp
//  159709
//
//  Created by Chen DONG on 9/03/17.
//  Copyright © 2017 Scorpiour. All rights reserved.
//

#include "Sprite.h"

#include <cmath>

using namespace std;

GridView::GridView(size_t _xlevel,size_t _ylevel,float _xdist,float _ydist):Sprite(){
    this->updateGrid(_xlevel,_ylevel,_xdist,_ydist);
}

GridView::~GridView(void){
    
}

void GridView::update(double dt){
    
    
}

void GridView::draw(double dt){
    this->program->use();
    glBindVertexArray(this->vao);
    
    glUniformMatrix4fv(this->program->getLoc("ModelMatrix"),1,GL_FALSE,modelMatrix);
    glUniform3fv(this->program->getLoc("translateOffset"),1,glm::value_ptr(position));
    glUniform3fv(this->program->getLoc("starColor"),1,glm::value_ptr(colourValue));
    glUniform3fv(this->program->getLoc("starIntensity"),1,glm::value_ptr(colourIntensity));
    
    this->camera->draw(this->program);
    
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

void GridView::updateGrid(size_t _xlevel,size_t _ylevel,float _xdist,float _ydist){
    this->xlevel = max((size_t)1,_xlevel);
    this->ylevel = max((size_t)1,_ylevel);
    this->xdist = min(220.f,_xdist);
    this->ydist = min(180.f,_ydist);
}
