#ifndef VNODE_H
#define VNODE_H

struct vNode {
    long long cumHammingDistance;
    bool inputArrivalBit;
    int state;

    bool operator<(const vNode& other) const {
        return state < other.state;
    }
};

#endif