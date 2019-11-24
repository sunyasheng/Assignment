#include "Uniform_Smoother.h"

Uniform_Smoother::Uniform_Smoother(Mesh * mesh)
{
	this->mesh = mesh;
	BuildSystemMatrix();
}


Uniform_Smoother::~Uniform_Smoother() {
	if (solver != NULL) {
		delete solver;
		delete Lap;
	}
}

void Uniform_Smoother::Deform() {
	VertexList vList = mesh->Vertices();

	SparseMatrixBuilder* Identity = new SparseMatrixBuilder();
	for (int i = 0; i < vList.size(); i++) {
		Identity->AddEntry(i, i, 1.0);
	}
	Eigen::SparseMatrix<double> I(vList.size(), vList.size());
	I = Identity->ToSparseMatrix(vList.size(), vList.size());

	Eigen::SparseMatrix<double> Laplacian(vList.size(), vList.size());
	Laplacian = Lap->ToSparseMatrix(vList.size(), vList.size());

	Eigen::SparseMatrix<double> A(vList.size(), vList.size());
	double lamba = 0.8;
	A = I - lamba*Laplacian;

	vx = Eigen::VectorXd::Zero(vList.size());
	vy = Eigen::VectorXd::Zero(vList.size());
	vz = Eigen::VectorXd::Zero(vList.size());

	for (int i = 0; i < vList.size(); i++) {
		vx[i] = vList[i]->Position()(0);
		vy[i] = vList[i]->Position()(1);
		vz[i] = vList[i]->Position()(2);
	}

	Eigen::VectorXd bx = A.transpose()*vx;
	Eigen::VectorXd by = A.transpose()*vy;
	Eigen::VectorXd bz = A.transpose()*vz;
	Eigen::VectorXd vx2 = Eigen::VectorXd::Zero(vList.size());
	Eigen::VectorXd vy2 = Eigen::VectorXd::Zero(vList.size());
	Eigen::VectorXd vz2 = Eigen::VectorXd::Zero(vList.size());

	solver = new SparseLinearSystemSolver(A.transpose()*A);
	vx2 = solver->Solve(bx);
	vy2 = solver->Solve(by);
	vz2 = solver->Solve(bz);
	for (int i = 0; i < vList.size(); i++) {
		Eigen::Vector3d newPosition = Eigen::Vector3d(vx2[i], vy2[i], vz2[i]);
		vList[i]->SetPosition(newPosition);
	}
}

void Uniform_Smoother::BuildSystemMatrix() {
	VertexList vList = mesh->Vertices();

	for (int i = 0; i < vList.size(); i++) {
		int k = vList[i]->Valence();
		HEdge* nextHedge = vList[i]->HalfEdge()->Twin();
		for (int r = 0; r < k; r++) {
			Lap->AddEntry(i, nextHedge->Start()->Index(), 1.0/k);
			nextHedge = nextHedge->Next()->Twin();
		}
		Lap->AddEntry(i, i, -1.0);
	}
}
