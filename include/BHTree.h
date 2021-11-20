#ifndef BH_TREE_H
#define BH_TREE_H

//--- Standard includes --------------------------------------------------------
#include <vector>

//--- Implementation -----------------------------------------------------------
#include "Vector.h"
#include "Star.h"

//------------------------------------------------------------------------------
/** \brief Implementation of a single node of the barnes hut tree. */
class BHTreeNode{
private:    
    
    double total_mass; // Mass of all the particles in this node
    int num_stars; // Total number of stars in this node
    BHTreeNode* parent; // Parent node of this node
    Vector com; // Centre of mass of the node
    Vector ul; // Coordinate of the upper left corner of the node
    Vector lr; // Coordinate of the lower right corner of the node
    Vector centre; //Centre of the node
    mutable bool Subdivided; // True if this node is too close to use the approximation for the force calculation

    // theta = d/r is the threshold used to decide whether child nodes have to be summed up separately while force calculation
    double theta; 
    Star star; // The Star stored in this particular quad tree

public:
    
    BHTreeNode* child[4];
    /** \brief Enumeration for the quadrants. */
    enum EQuadrant{NE,NW,SW,SE};

    BHTreeNode(Vector &ul,Vector &lr, BHTreeNode *parent=nullptr); // Constructor for a BHTree node
    ~BHTreeNode(); // Destructor for the node

    bool isExternal();
    bool isRoot();

    void Reset(Vector &ul, Vector &lr);
    void Insert(Star& star, int lvl);

    EQuadrant GetQuadrant(Star& star);

    void ComputeMassDistribution();
    Vector CalculateForce(Star& targetStar, int lvl=0);
    void CreateSubNode(EQuadrant quad);

    double ComputeMass();
    
    friend class Star;
};

#endif