#include "SkeletalModel.h"

#include <FL/Fl.H>
#include <fstream> // header providing file stream classes

using namespace std;

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	// push camera as first thing on the stack:
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

// 1.1. Implement method to load a skeleton.
// This method computes m_rootJoint and populate m_joints.
void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
	cerr << "provided filename:" << endl;
	cerr << filename << endl;
	std::ifstream input_row(filename);

	// translation vector data relative to parent:
	float x, y, z;
	int parentID;

	int joint_index = 0;
	while (input_row >> x >> y >> z >> parentID)
	{
		cerr << x << " " << y << " " << z << " " << parentID << endl;
		Joint* joint = new Joint;

		if(joint_index == 0)
		{
			m_rootJoint = joint;
		}

		Matrix4f tranlation_wrt_parent(1); // translation matrix with respect to parent node
		tranlation_wrt_parent = tranlation_wrt_parent.translation(x, y, z);

		joint->transform = tranlation_wrt_parent;
		joint->index = joint_index;
		if(parentID != -1)
			m_joints[parentID]->children.push_back(joint);

		m_joints.push_back(joint);
		joint_index++;
	}

	for (int i = 0; i < m_joints.size(); i++)
	{
		cerr << "Children of joint index: " << i << endl;
		for( int j = 0; j < m_joints[i]->children.size(); j++)
		{
			cerr << m_joints[i]->children[j]->index << " ";
		}
		cerr << endl;
	}


}

void SkeletalModel::getChild( Joint* joint )
{
	// recursion ending condition:
	if (joint->children.size() == 0)
	{
		// if the passed joint does not have any child joints,
		// ... then simply draw it
		cerr << joint->index << " ";
		// left here on 2019-12-01
	}
	else
	{
		cerr << "keep on looking . . ." << endl;
	}
}

void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function
	// ...to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.

	// since our Joint Hierarchical Structure is a DAG (directed acyclic graph),
	// ... it has one root node with 0 parents (since it is acyclic)
	// We start the recursive helper function from the root:
	this->getChild(m_joints[0]);
}

void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
}
