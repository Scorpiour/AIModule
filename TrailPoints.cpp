//
//  TrailPoints.cpp
//  159709
//
//  Created by Chen DONG on 14/08/16.
//  Copyright © 2016 Scorpiour. All rights reserved.
//

#include "Sprite.h"

TrailPoints::TrailPoints(TrailType _t,size_t max_count):Sprite(){
    this->maxCount = max_count;
	this->type = _t;
	this->points.clear();
}

TrailPoints::~TrailPoints(){
    
}

void TrailPoints::addPoint(const Point2F& pt){
    this->points.push_back(pt);
    if(this->points.size() > maxCount){
        this->points.pop_front();
    }
}

void TrailPoints::removeFrontPoint(){
	if(this->points.size() > 0){
		this->points.pop_front();
	}
}

void TrailPoints::removeBackPoint(){
	if(this->points.size() > 0){
		this->points.pop_back();
	}
}

const Point2F& TrailPoints::getFrontPoint()const{
	return this->points.front();
}

const Point2F& TrailPoints::getBackPoint()const{
	return this->points.back();
}

size_t TrailPoints::size()const{
	return this->points.size();
}

void TrailPoints::clear(){
	this->points.clear();
}

void TrailPoints::update(double dt){
    switch(this->type){
	case TrailType_Line:{
			for(int i=0;i<16;i++){
				modelMatrix[i] = 0.f;
			}

			modelMatrix[0] = 1.f;
			modelMatrix[5] = 1.f;
			modelMatrix[10] = 1.f;
			modelMatrix[15] = 1.f;

			GlobalVariables::Mat4Multi(rotateMatrix,modelMatrix,modelMatrix);
			GlobalVariables::Mat4Multi(scaleMatrix,modelMatrix,modelMatrix);


		}break;
	}
}

void TrailPoints::draw(double dt){
    this->program->use();
    glBindVertexArray(this->vao);
    
	switch(this->type){
	case TrailType_Dot:{

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
		}break;
	case TrailType_Line:{
			
			if(this->points.size() < 1){
				break;
			}

			int i = 0;
			Point2F prevPt = this->points.front();
			for(auto & pt : this->points){
				if(i == 0){
					i++;
					continue;
				}
				const Point2F& currentPt = pt;
				float dy = currentPt.y - prevPt.y;
				float dx = currentPt.x - prevPt.x;
				float r = sqrt(dy*dy + dx*dx);
				float arc = GlobalVariables::Atan2(dy,dx);
				
				dx = (currentPt.x + prevPt.x)/2;
				dy = (currentPt.y + prevPt.y)/2;

				this->setPosition(glm::vec3(dx,-0.15,dy));
				this->setScale(glm::vec3(0.8*r,0.1,0.1));
				this->setAngle(glm::vec3(0,arc,0));
				
				this->update(dt);

				glUniformMatrix4fv(this->program->getLoc("ModelMatrix"),1,GL_FALSE,modelMatrix);
				glUniform3fv(this->program->getLoc("translateOffset"),1,glm::value_ptr(position));
				glUniform3fv(this->program->getLoc("starColor"),1,glm::value_ptr(colourValue));
				glUniform3fv(this->program->getLoc("starIntensity"),1,glm::value_ptr(colourIntensity));

				this->camera->draw(this->program);

				glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);


				prevPt = pt;
				i++;
			}

		}break;
	}
    glBindVertexArray(0);
}
