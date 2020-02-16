#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <vecmath.h>

struct Joint
{
	int index;											// index of the joint as given in the .skel file
	int parent;								 			// added index of each joint's parent
	Matrix4f transform;				 			// transform relative to its parent (dynamic)
	Matrix4f initialTransform; 			// used to save the original transform relative to its parent (const reference matrix)
	std::vector< Joint* > children; // list of children

	// This matrix transforms world space into joint space for the initial ("bind") configuration of the joints.
	Matrix4f bindWorldToJointTransform;

	// This matrix maps joint space into world space for the *current* configuration of the joints.
	Matrix4f currentJointToWorldTransform;
};

#endif
