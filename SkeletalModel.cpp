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
	cerr << "initializes here? camera \n";
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
		joint->index = joint_index; // enumerate the joints

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

void SkeletalModel::getChild( Joint* joint, float boneLength )
{
	// recursion ending condition:
	if (joint->children.size() == 0)
	{
		// if the passed joint does not have any child joints,
		// ... then simply draw it
		cerr << "Drawing joint index: " << joint->index << " ";

		m_matrixStack.push(joint->transform);

		Matrix4f top_transformation = m_matrixStack.top();
		top_transformation.print();
		// float m[16] = {1,0,0,0,0,1,0,0,0,0,1,0,1,2,3,1};
		glLoadMatrixf(top_transformation);
		// glGetFloatv (GL_MODELVIEW_MATRIX, check_mat);

		// GLUQuadric *sphere;
		GLUquadricObj *sphere;
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL );
		gluSphere( sphere, 0.025f, 12, 12 );
		// glutSolidCube( 1.0f );

		// Purple side - RIGHT
		// glBegin(GL_POLYGON);
		// glColor3f(  1.0,  0.0,  1.0 );
		// glVertex3f( 0.05, -0.05, -0.05 );
		// glVertex3f( 0.05,  0.05, -0.05 );
		// glVertex3f( 0.05,  0.05,  0.05 );
		// glVertex3f( 0.05, -0.05,  0.05 );
		// glEnd();

		// // Green side - LEFT
		// glBegin(GL_POLYGON);
		// glColor3f(   0.0,  1.0,  0.0 );
		// glVertex3f( -0.05, -0.05,  0.05 );
		// glVertex3f( -0.05,  0.05,  0.05 );
		// glVertex3f( -0.05,  0.05, -0.05 );
		// glVertex3f( -0.05, -0.05, -0.05 );
		// glEnd();

		// Blue side - TOP
		glBegin(GL_POLYGON);
		glColor3f(   0.0f,  0.0f,  1.0f );
		glVertex3f(  boneLength,  boneLength,  boneLength );
		glVertex3f(  boneLength,  boneLength, -boneLength );
		glVertex3f( -boneLength,  boneLength, -boneLength );
		glVertex3f( -boneLength,  boneLength,  boneLength );
		glEnd();

		// Red side - BOTTOM
		glBegin(GL_POLYGON);
		glColor3f(   1.0,  0.0,  0.0 );
		glVertex3f(  0.05, 0.0, -0.05 );
		glVertex3f(  0.05, 0.0,  0.05 );
		glVertex3f( -0.05, 0.0,  0.05 );
		glVertex3f( -0.05, 0.0, -0.05 );
		glEnd();
		// glutSolidSphere( 0.025f, 12, 12 );
		// m_matrixStack.pop();
		// left here on 2019-12-01, ... continuing on 2019-12-18
	}
	else
	{
		cerr << "keep on looking . . ." << endl;
		cerr << joint->index << " has children: " << joint->children.size() << endl;
		// m_matrixStack.push(joint->transform);
		// Matrix4f top_transformation = m_matrixStack.top();
		// top_transformation.print();

		Matrix4f previousJoint = m_matrixStack.top();
		Matrix4f currentJoint = previousJoint * (joint->transform);

		m_matrixStack.push(joint->transform);

		for(int child = 0; child < joint->children.size(); child++ )
		{
			// float x1 = joint->transform(0,3), x2 = joint->children[child]->transform(0,3);
			// float y1 = joint->transform(1,3), y2 = joint->children[child]->transform(1,3);
			// float z1 = joint->transform(2,3), z2 = joint->children[child]->transform(2,3);
			int scaling = 1;
			float x1 = currentJoint(0,3)/scaling, x2 = previousJoint(0,3)/scaling;
			float y1 = currentJoint(1,3)/scaling, y2 = previousJoint(1,3)/scaling;
			float z1 = currentJoint(2,3)/scaling, z2 = previousJoint(2,3)/scaling;

			cerr << x1 << ", " << y1 << ", " << z1 << std::endl;
			cerr << x2 << ", " << y2 << ", " << z2 << std::endl;

			long double boneLength = sqrt(pow(x1-x2,2.0)+pow(y1-y2,2.0)+pow(z1-z2,2.0));

			cerr << "length of bone: " << boneLength << endl;

			this->getChild(joint->children[child], boneLength);
			cerr << "EXITING RECURSION . . . popping and drawing index: " << joint->index << std::endl;

			Matrix4f top_transformation = m_matrixStack.top();
			top_transformation.print();

			glLoadMatrixf(top_transformation);

			GLUquadricObj *sphere;
			sphere = gluNewQuadric();
			gluQuadricDrawStyle(sphere, GLU_FILL );
			gluSphere( sphere, 0.025f, 12, 12 );

			m_matrixStack.pop();
		}
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
	// add the root transformation
	m_matrixStack.push(m_joints[0]->transform);
	Matrix4f top_transformation = m_matrixStack.top();
	top_transformation.print();

	glLoadMatrixf(top_transformation);

	cerr << "TOP Level children: " << m_joints[0]->children.size() << std::endl;

	for (int child = 0; child < m_joints[0]->children.size(); child++ )
	// for (int child = 0; child < 1; child++ )
	{
		// if (child != 0)
		{
			cerr << "	-> getting child #" << child << std::endl;
			this->getChild(m_joints[0]->children[child], 0.0);

			cerr << "EXITING branch of root child #" << child << std::endl;
			Matrix4f top_transformation = m_matrixStack.top();
			top_transformation.print();

			glLoadMatrixf(top_transformation);

			GLUquadricObj *sphere;
			sphere = gluNewQuadric();
			gluQuadricDrawStyle(sphere, GLU_FILL );
			gluSphere( sphere, 0.025f, 12, 12 );

			// m_matrixStack.pop();

		}
	}
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
