#include "MatrixStack.h"
// Implemented 2019-12-01
MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
	Matrix4f identity_matrix(1);
	identity_matrix = identity_matrix.identity();
	m_matrices.push_back(identity_matrix);
}

void MatrixStack::clear()
{
	// Revert to just containing the identity matrix.
	m_matrices.clear();
	MatrixStack m_matrices();
}

Matrix4f MatrixStack::top()
{
	// Return the top of the stack
	Matrix4f top_transformation = m_matrices.back();
	return top_transformation;
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	Matrix4f top_transformation = m_matrices.back();
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
	Matrix4f related_to_parent = top_transformation * m;
	m_matrices.push_back(related_to_parent);
}

void MatrixStack::pop()
{
	// Remove the top element from the stack
	m_matrices.pop_back();
}
