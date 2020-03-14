
#ifndef PathNode_h
#define PathNode_h

#include "Color.h"
#include "Ray.h"

class PathNode {
 public:
    PathNode(Ray ray1, int depth1, Color color1) {
        ray = ray1;
        depth = depth1;
        color = color1;
    }
    virtual ~PathNode();

 private:
     float pdf; // cumulative pdf
     Color color;
     int depth;
     Ray ray; // outgoing ray from node
};

#endif
