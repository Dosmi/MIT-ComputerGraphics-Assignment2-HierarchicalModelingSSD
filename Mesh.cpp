#include "Mesh.h"
// include input output stream class:
#include <fstream>

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces

	// Add your code here.

	char type;
	unsigned a_value, c_value, d_value, f_value, g_value, i_value, b_value,e_value,h_value;

	stringstream ss(filename);

	// char buffer[MAX_BUFFER_SIZE];
	// cin.getline(filename, MAX_BUFFER_SIZE);
	// cout << buffer << endl;
	float vertex_coord_1, vertex_coord_2, vertex_coord_3;
	unsigned vertex_index_1, vertex_index_2, vertex_index_3;
	vector<unsigned> face_information;

	std::ifstream input_file(filename);

	// This is the list of faces (indices into vecv and vecn)
	vector<vector<unsigned> > face_indices;

	std::string line;
	while (std::getline(input_file, line))
	{
		std::istringstream sstream(line);
		sstream >> type;
		if (!sstream >> type) {cerr << "unsucessful read of type" << endl; exit(0); } // ERROR
		if (type == 'v')
		{
			if (!(sstream >> vertex_coord_1 >> vertex_coord_2 >> vertex_coord_3)) { cerr << "unsucessful read of bindVertices" << endl; exit(0); }
			Vector3f v = Vector3f( vertex_coord_1, vertex_coord_2, vertex_coord_3 );
			bindVertices.push_back(v);
		}
		else if (type == 'f')
		{
			if(!(sstream >> vertex_index_1 >> vertex_index_2 >> vertex_index_3)) { cerr << "unsucessful read of faces" << endl; exit(0); }
			Tuple3u surface_triangle
      (
        vertex_index_1,
        vertex_index_2,
        vertex_index_3
      );
			faces.push_back(surface_triangle);
		}

		// process pair (a,b)
	}

	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
	// this way we keep the vertices up to date (update currentVertices, keep original)
}

void Mesh::draw()
{
	Vector3f point1, point2, point3;
	Vector3f edge1, edge2;

	Vector3f surface_normal;
	for(int i = 0; i < faces.size(); i++)
	{
		point1 = currentVertices[faces[i][0]-1];
		point2 = currentVertices[faces[i][1]-1];
		point3 = currentVertices[faces[i][2]-1];

		edge1 = point2 - point1;
		edge2 = point3 - point1;

		// Since these meshes don't have normals
		// .. we generate a normal per triangle.
		// Notice that since we have per-triangle normals
		// rather than the analytical normals from
		// assignment 1, the appearance is "faceted".
		surface_normal = Vector3f::cross(edge1, edge2);
		surface_normal = surface_normal.normalized();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBegin(GL_TRIANGLES);
        glNormal3f(surface_normal[0], surface_normal[1], surface_normal[2]);
				glVertex3f(point1[0], point1[1], point1[2]);
        glVertex3f(point2[0], point2[1], point2[2]);
        glVertex3f(point3[0], point3[1], point3[2]);
      glEnd();
	}
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments

	// each element of attachments is a vector< float > containing
	// one attachment weight per joint
	float weight;
	std::vector< float > weights;
	std::ifstream input_file(filename);

	std::string line;
	while(std::getline(input_file, line))
	{
		std::istringstream sstream(line);
		weights.clear();
		for(int i = 0; i < numJoints-1; i++)
		{
			sstream >> weight;
			weights.push_back(weight);
		}
		attachments.push_back(weights);
	}
}
