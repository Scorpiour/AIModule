//
//  TrailPoints.cpp
//  159709
//
//  Created by Chen DONG on 14/08/16.
//  Copyright © 2016 Scorpiour. All rights reserved.
//

#include "Sprite.h"

TrailPoints::TrailPoints(size_t max_count):Sprite(){
    this->maxCount = max_count;
}

TrailPoints::~TrailPoints(){
    
}

void TrailPoints::addPoint(const Point2F& pt){
    this->points.push_back(pt);
    if(this->points.size() > maxCount){
        this->points.pop_front();
    }
}

void TrailPoints::clear(){
	this->points.clear();
}

void TrailPoints::update(double dt){
    
}

void TrailPoints::draw(double dt){
    this->program->use();
    glBindVertexArray(this->vao);
    
    float additionIntensity = 0.5f;
    
    GLint modelMatrixLoc = this->program->getLoc("ModelMatrix");
    GLint translateLoc = this->program->getLoc("translateOffset");
    GLint starRadiusLoc = this->program->getLoc("radius");
    GLint starColorLoc = this->program->getLoc("starColor");
    GLint starIntensityLoc = this->program->getLoc("starIntensity");
    GLint highlightLoc = this->program->getLoc("additionIntensity");
    
    for(auto & pt : this->points){
        
        position.x = pt.x;
        position.z = pt.y;
        
        additionIntensity *= 0.9f;
        
        glUniform1f(program->getLoc("divideOuterLevel"),1.0);
        glUniform1f(program->getLoc("divideInnerLevel"),1.0);
        glUniformMatrix4fv(modelMatrixLoc,1,GL_FALSE,modelMatrix);
        glUniform3fv(translateLoc,1,glm::value_ptr(position));
        glUniform1f(starRadiusLoc,this->radius);
        glUniform3fv(starColorLoc,1,glm::value_ptr(colourValue));
        glUniform3fv(starIntensityLoc,1,glm::value_ptr(colourIntensity));
        glUniform1f(highlightLoc,additionIntensity);
    
        this->camera->draw(this->program);
    
        glDrawElements(GL_PATCHES,60,GL_UNSIGNED_INT,0);
    }
    glBindVertexArray(0);
}
