
#include "Mesh.h"
#include "LinearSystemSolver.h"

#pragma once
class Cotangent_Smoother
{
public:
	Cotangent_Smoother(Mesh * mesh);
	~Cotangent_Smoother();
	void Deform();
private:
	void BuildSystemMatrix();
	SparseLinearSystemSolver* solver;
	Mesh * mesh;
	Eigen::VectorXd vx, vy, vz;
	SparseMatrixBuilder* Lap = new SparseMatrixBuilder();
};
