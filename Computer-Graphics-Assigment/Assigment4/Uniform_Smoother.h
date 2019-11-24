
#include "Mesh.h"
#include "LinearSystemSolver.h"

#pragma once
class Uniform_Smoother
{
public:
	Uniform_Smoother(Mesh * mesh);
	~Uniform_Smoother();
	void Deform();
private:
	void BuildSystemMatrix();

	SparseLinearSystemSolver* solver;
	Mesh * mesh;

	Eigen::VectorXd vx, vy, vz;
	SparseMatrixBuilder* Lap = new SparseMatrixBuilder();

};

