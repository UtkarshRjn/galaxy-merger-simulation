#include"BHTree.h"

//--- Standard includes --------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;
#define THETA 0.7

BHTreeNode::BHTreeNode(Vector &ul,Vector &lr, BHTreeNode *parent){
    this->parent = parent;
    this->ul = ul;
    this->lr = lr;
    this->centre = (lr + ul)/2;
    total_mass = 0;
    com = Vector(0,0);
    num_stars = 0;
    Subdivided = false;
    for(int i=0;i<4;i++){
        child[i] = NULL;
    }
    theta = THETA;
}

/*
 lr__________________
 |        |        |    Due to opposite convention of y-axis than normal in graphics library the convention for 
 |   SW   |   SE   |    North South East and West are different and is as follows:
 |________|C_______|    
 |        |        |
 |   NW   |   NE   |
 |________|________|ul

*/
BHTreeNode::EQuadrant BHTreeNode::GetQuadrant(Star &star){
    double x = star.state.pos.getX();
    double y = star.state.pos.getY();
    double x_cen = centre.getX();
    double y_cen = centre.getY();

    if (x<=x_cen && y<=y_cen)
    {
        return SW;
    }
    else if (x<=x_cen && y>=y_cen)
    {
        return NW;
    }
    else if (x>=x_cen && y>=y_cen)
    {
        return NE;
    }
    else if (x>=x_cen && y<=y_cen)
    {
        return SE;
    }
};

bool BHTreeNode::isExternal(){
    if(child[0]==NULL && child[1]==NULL && child[2]==NULL && child[3]==NULL) return true;
    else return false;
}

bool BHTreeNode::isRoot(){
    if(parent == nullptr) return true;
    else return false;
}

void BHTreeNode::CreateSubNode(EQuadrant quad){
    if(quad==SW){
        Vector new_ul = centre;
        Vector new_lr = lr;
        child[quad] = new BHTreeNode(new_ul,new_lr,this);
    }
    else if(quad==NW){
        Vector new_ul = Vector(centre.getX(),ul.getY());
        Vector new_lr = Vector(lr.getX(),centre.getY());
        child[quad] = new BHTreeNode(new_ul,new_lr,this);
    }
    else if(quad==NE){
        Vector new_ul = ul;
        Vector new_lr = centre;
        child[quad] = new BHTreeNode(new_ul,new_lr,this);
    }
    else if(quad==SE){
        Vector new_ul = Vector(ul.getX(),centre.getY());
        Vector new_lr = Vector(centre.getX(),lr.getY());
        child[quad] = new BHTreeNode(new_ul,new_lr,this);
    }
}

void BHTreeNode::Insert(Star& star, int lvl){
    
    // Insert a check case wherein new star has x and y outside the BHTree nodes
    double x = star.state.pos.getX();
    double y = star.state.pos.getY();
    
    if ( (x < lr.getX() || x > ul.getX()) || (y < lr.getY() || y > ul.getY()) ){
        return;
    }
    
    if(num_stars > 1){
        EQuadrant quad = GetQuadrant(star);
        if(child[quad] == NULL){
            CreateSubNode(quad);

        }
        child[quad]->Insert(star,lvl+1);
    }
    else if(num_stars == 1){
        
        assert(isExternal() || isRoot());
        
        EQuadrant quad = GetQuadrant(this->star); // Finding the quadrant of the existing star in this node
        if(child[quad] == NULL){
            CreateSubNode(quad);
        }
        child[quad]->Insert(this->star,lvl+1);

        quad = GetQuadrant(star); // Finding the quadrant of the new star to be inserted in this node
        if(child[quad] == NULL){
            CreateSubNode(quad);
        }
        child[quad]->Insert(star,lvl+1);
    }
    else if(num_stars == 0){
        this->star = star;
    }

    num_stars++;
};

void BHTreeNode::ComputeMassDistribution(){
    if(num_stars == 1){
        com = star.state.pos;
        total_mass = star.mass;
    }else{
        total_mass = 0;
        com = Vector(0,0);
        
        for(int i=0;i<4;i++){
            
            if(child[i]!=NULL){
                child[i]->ComputeMassDistribution();
                total_mass += child[i]->total_mass;
                Vector temp = child[i]->total_mass * child[i]->com ;
                com = com + temp;
            }
        }
        com = com/total_mass;
    }
}

/** \brief Calculates force on a particular star due to all other stars in the proximity(which are part of the BHTree) */
Vector BHTreeNode::CalculateForce(Star& targetStar,int lvl){
    Vector acc = Vector(0,0);

    if(num_stars==1){
        
        // Returns force vector acting on target star due to the star in this node
        acc = targetStar.isPulledby(star);

    }else{
       
        // r : distance from nodes centre of mass to target particle
        double r = sqrt( (com.getX()-targetStar.state.pos.getX())*(com.getX()-targetStar.state.pos.getX()) + 
                        (com.getY()-targetStar.state.pos.getY())*(com.getY()-targetStar.state.pos.getY()));
        
        // d : height of the node
        double d = abs(ul.getX() - lr.getX());
        
        // Using threshold theta to decide whether child nodes have to be summed up separately while force calculation or not
        if(d/r < theta){
            Subdivided = false;
            
            // Returns force vector acting on target star due to this node (with position at com and mass as total mass)
            double f = G*(total_mass)/(r*r*r);
            acc = f*(com - targetStar.state.pos);
        }
        else{
            Subdivided = true;

            for(int i=0;i<4;i++){ 
                if(child[i]!=NULL){
                    // When d/r > theta we are going one more level deep to find the force(acc)
                    acc = acc + child[i]->CalculateForce(targetStar,lvl+1);
                }
            }
        }
    }
    return acc;
}

/** \brief Reset function to reset the tree at each time frame. */
void BHTreeNode::Reset(Vector& ul,Vector& lr){
  
    for (int i=0; i<4; ++i){
        delete child[i];
        child[i] = NULL;
    }

    this->ul = ul;
    this->lr = lr;
    this->centre = (lr + ul)/2;
    total_mass = 0;
    com = Vector(0,0);
    num_stars = 0;
    Subdivided = false;
}

//------------------------------------------------------------------------------
BHTreeNode::~BHTreeNode()
{
  for (int i=0; i<4; ++i)
    delete child[i];
}