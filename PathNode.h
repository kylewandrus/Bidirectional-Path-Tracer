
#ifndef PathNode_h
#define PathNode_h

#include "Color.h"
#include "Ray.h"

class PathNode {
 public:
    PathNode(Ray ray1, Color color1, Vector normal1) {
        ray = ray1;
        color = color1;
        normal = normal1;
    }
    ~PathNode() {}

    Point getRayOrig() {
        return ray.origin();
    }
    Vector getNormal() {
        return normal;
    }
    Color getColor() {
        return color;
    }

 private:
     float pdf; // cumulative pdf

     Color color;
     Ray ray; // outgoing ray from node
     Vector normal;
};

#endif
