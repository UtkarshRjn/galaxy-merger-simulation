#include"BHTree.h"

//--- Standard includes --------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <sstream>

BHTreeNode::BHTreeNode(Vector &ul,Vector &lr, BHTreeNode *parent=nullptr){
    this->parent = parent;
    this->ul = ul;
    this->lr = lr;
    this->centre = (lr + ul)/2;
    total_mass = 0;
    com = Vector(0,0);
    num_stars = 0;
    Subdivided = false;
    child[0] = child[1] = child[2] = child[3] = NULL;
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
    if(num_stars > 1){
        EQuadrant quad = GetQuadrant(star);
        if(child[quad] == NULL){
            CreateSubNode(quad);

        }
        child[quad]->Insert(star,lvl+1);
    }
    else if(num_stars == 1){
        EQuadrant quad = GetQuadrant(this->star); // Finding the quadrant of the existing star in this node
        if(child[quad] == NULL){
            CreateSubNode(quad);
        }
        child[quad]->Insert(this->star,lvl+1);

        EQuadrant quad = GetQuadrant(star); // Finding the quadrant of the new star to be inserted in this node
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
        
        for(int i=0;i<4 && child[i]!=NULL;i++){
            child[i]->ComputeMassDistribution();
            total_mass += child[i]->total_mass;
            Vector temp = child[i]->total_mass * child[i]->com ;
            com = com + temp;
        }
        com = com/total_mass;
    }
}

Vector BHTreeNode::CalculateForce(Star& targetStar){
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
            acc = targetStar.isPulledby(this);
        }
        else{
            Subdivided = true;
            for(int i=0;i<4 && child[i]!=NULL;i++){
                
                // When d/r > theta we are going one more level deep to find the force(acc)
                acc = acc + child[i]->CalculateForce(targetStar);
            }
        }
    }

    return acc;
}
