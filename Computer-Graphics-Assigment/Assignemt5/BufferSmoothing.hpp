#include "Mesh.h"
#include "LinearSystemSolver.h"

#pragma once
class BufferSmoothing
{
public:
    BufferSmoothing(Mesh * mesh);
    ~BufferSmoothing();
    void Deform();
private:
    void BuildSystemMatrix();
    SparseLinearSystemSolver* solver;
    Mesh * mesh;
    Eigen::VectorXd vx, vy, vz;
    SparseMatrixBuilder* Lap = new SparseMatrixBuilder();
};
