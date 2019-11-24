#include <list>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Mesh.h"
#include "Cotangent_Smoother.h"
#include "Uniform_Smoother.h"
#include "LinearSystemSolver.h"
#include "Deformer.h"


inline Eigen::Vector3d MinVector3d(Eigen::Vector3d v1, Eigen::Vector3d v2) {
    return Eigen::Vector3d(std::min(v1(0), v2(0)),
                           std::min(v1(1), v2(1)),
                           std::min(v1(2), v2(2)));
}

inline Eigen::Vector3d MaxVector3d(Eigen::Vector3d v1, Eigen::Vector3d v2) {
    return Eigen::Vector3d(std::max(v1(0), v2(0)),
                           std::max(v1(1), v2(1)),
                           std::max(v1(2), v2(2)));
}

OneRingHEdge::OneRingHEdge(const Vertex* v) {
    if (v == NULL) start = next = NULL;
    else start = next = v->HalfEdge();
}

HEdge* OneRingHEdge::NextHEdge() {
    HEdge* ret = next;
    if (next && next->Prev()->Twin() != start)
        next = next->Prev()->Twin();
    else
        next = NULL;
    return ret;
}

Mesh::~Mesh() {
    Clear();
}

const HEdgeList& Mesh::Edges() const {
    return heList;
}

const HEdgeList& Mesh::BoundaryEdges() const {
    return bheList;
}

const VertexList& Mesh::Vertices() const {
    return vList;
}

const FaceList& Mesh::Faces() const {
    return fList;
}

// load a .obj mesh definition file
bool Mesh::LoadObjFile(const char* filename) {
    if (filename == NULL || strlen(filename) == 0) return false;
    std::ifstream ifs(filename);
    if (ifs.fail()) return false;

    Clear();

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;
        // vertex
        if (type.compare("v") == 0) {
            double x, y, z;
            iss >> x >> y >> z;
            AddVertex(new Vertex(x, y, z));
        }
        // face
        else if (type.compare("f") == 0) {
            int index[3];
            iss >> index[0] >> index[1] >> index[2];
            AddFace(index[0] - 1, index[1] - 1, index[2] - 1);
        }
    }
    ifs.close();

    size_t i;
    Eigen::Vector3d box = this->MaxCoord() - this->MinCoord();
    for (i = 0; i < vList.size(); i++) vList[i]->SetPosition(vList[i]->Position() / box(0));

    Eigen::Vector3d tot = Eigen::Vector3d::Zero();
    for (i = 0; i < vList.size(); i++) tot += vList[i]->Position();
    Eigen::Vector3d avg = tot / vList.size();
    for (i = 0; i < vList.size(); i++) vList[i]->SetPosition(vList[i]->Position() - avg);

    HEdgeList list;
    for (i = 0; i < bheList.size(); i++)
        if (bheList[i]->Start()) list.push_back(bheList[i]);
    bheList = list;

    for (i = 0; i < vList.size(); i++) {
        vList[i]->adjHEdges.clear();
        vList[i]->SetIndex((int)i);
        vList[i]->SetFlag(0);
    }

    return true;
}

void Mesh::AddVertex(Vertex* v) {
    vList.push_back(v);
}

void Mesh::AddFace(int v1, int v2, int v3) {
    int i;
    HEdge *he[3], *bhe[3];
    Vertex* v[3];
    Face* f;

    // obtain objects
    for (i = 0; i < 3; i++) he[i] = new HEdge();
    for (i = 0; i < 3; i++) bhe[i] = new HEdge(true);
    v[0] = vList[v1];
    v[1] = vList[v2];
    v[2] = vList[v3];
    f = new Face();

    // connect prev-next pointers
    SetPrevNext(he[0], he[1]);
    SetPrevNext(he[1], he[2]);
    SetPrevNext(he[2], he[0]);
    SetPrevNext(bhe[0], bhe[1]);
    SetPrevNext(bhe[1], bhe[2]);
    SetPrevNext(bhe[2], bhe[0]);

    // connect twin pointers
    SetTwin(he[0], bhe[0]);
    SetTwin(he[1], bhe[2]);
    SetTwin(he[2], bhe[1]);

    // connect start pointers for bhe
    bhe[0]->SetStart(v[1]);
    bhe[1]->SetStart(v[0]);
    bhe[2]->SetStart(v[2]);
    for (i = 0; i < 3; i++) he[i]->SetStart(v[i]);

    // connect start pointers
    // connect face-hedge pointers
    for (i = 0; i < 3; i++) {
        v[i]->SetHalfEdge(he[i]);
        v[i]->adjHEdges.push_back(he[i]);
        SetFace(f, he[i]);
    }
    v[0]->adjHEdges.push_back(bhe[1]);
    v[1]->adjHEdges.push_back(bhe[0]);
    v[2]->adjHEdges.push_back(bhe[2]);

    // mearge boundary if in need
    for (i = 0; i < 3; i++) {
        Vertex* start = bhe[i]->Start();
        Vertex* end = bhe[i]->End();
        for (size_t j = 0; j < end->adjHEdges.size(); j++) {
            HEdge* curr = end->adjHEdges[j];
            if (curr->IsBoundary() && curr->End() == start) {
                SetPrevNext(bhe[i]->Prev(), curr->Next());
                SetPrevNext(curr->Prev(), bhe[i]->Next());
                SetTwin(bhe[i]->Twin(), curr->Twin());
                bhe[i]->SetStart(NULL); // mark as unused
                curr->SetStart(NULL); // mark as unused
                break;
            }
        }
    }

    // finally add hedges and faces to list
    for (i = 0; i < 3; i++) heList.push_back(he[i]);
    for (i = 0; i < 3; i++) bheList.push_back(bhe[i]);
    fList.push_back(f);
}

void Mesh::Clear() {
    size_t i;
    for (i = 0; i < heList.size(); i++) delete heList[i];
    for (i = 0; i < bheList.size(); i++) delete bheList[i];
    for (i = 0; i < vList.size(); i++) delete vList[i];
    for (i = 0; i < fList.size(); i++) delete fList[i];
    heList.clear();
    bheList.clear();
    vList.clear();
    fList.clear();
}

Eigen::Vector3d Mesh::MinCoord() const {
    Eigen::Vector3d minCoord = Eigen::Vector3d::Zero();
    for (size_t i = 0; i < vList.size(); i++)
        minCoord = MinVector3d((vList[i])->Position(), minCoord);
    return minCoord;
}

Eigen::Vector3d Mesh::MaxCoord() const {
    Eigen::Vector3d maxCoord = Eigen::Vector3d::Zero();
    for (size_t i = 0; i < vList.size(); i++)
        maxCoord = MaxVector3d((vList[i])->Position(), maxCoord);
    return maxCoord;
}

void Mesh::DisplayMeshInfo() {
    /*************************/
    /* insert your code here */
    /*************************/

    /*====== Programming Assignment 0 ======*/
	int Num_vertices = vList.size();
	int Num_hedges = heList.size();
	int Num_bhedges = bheList.size();
	int Num_edges;
	int Num_fcaes = fList.size();

	int boundaries;
	int component;
	int genus;
	int Euler;

	Num_edges = (Num_hedges + Num_bhedges) / 2; 
	boundaries = CountBoundaryLoops();
	component = CountConnectedComponents();
	Euler = Num_vertices - Num_edges + Num_fcaes; 
	genus = (2 - Euler - boundaries) / 2; 

	std::cout << "Number of Vertices: " << Num_vertices << std::endl;
	std::cout << "Number of Edges: " << Num_edges << std::endl;
	std::cout << "Number of Faces: " << Num_fcaes << std::endl;
	std::cout << "Number of Boundaries: " << boundaries << std::endl;
	std::cout << "Number of Genus: " << genus << std::endl;
	std::cout << "Number of Components: " << component << std::endl;
	std::cout << "Euler characteristic: " << Euler << std::endl;


}

// compute the normal of each vertex
void Mesh::ComputeVertexNormals() {
    /*************************/
    /* insert your code here */
    /*************************/

    /*====== Programming Assignment 0 ======*/
	const double PI = 3.14159265;
	Eigen::Vector3d t1, t2, nor, Ver1;

	int i, k, j;
	for (i = 0; i < vList.size(); i++){

		//OneRingVertex ring(vList[i]);  // I didn't use the one ring function. it's a little bit confused especially for new comer of C++;

		k = vList[i]->Valence(); //get the degree of this vertex;
		HEdge* nextHedge;
		Vertex* nextVertex;

		t1 = Eigen::Vector3d(0, 0, 0); //define two vectors;
		t2 = Eigen::Vector3d(0, 0, 0);

		if (vList[i]->IsBoundary()){
			
			nextHedge = vList[i]->HalfEdge()->Twin();  //get the first half edge and its twin, one ring vertex;
			nextVertex = nextHedge->Start();

			//make sure the starting half edge is boundary edge, so the next vertex is also on the boundary;
			//while (!(nextVertex->IsBoundary())){
				//nextHedge = nextHedge->Next()->Twin();
				//nextVertex = nextHedge->Start();
			//}
			
			Ver1 = nextVertex->Position(); //now, the starting vertex is denoted as Ver1;
			
			if (k == 2){
				nextVertex = nextHedge->Next()->Twin()->Start();
				t1 = Ver1 - nextVertex->Position();
				t2 = Ver1 + nextVertex->Position() - vList[i]->Position();
			}
			else if (k == 3){
				nextHedge = nextHedge->Next()->Twin();
				nextVertex = nextHedge->Start();
				t2 = nextVertex->Position() - vList[i]->Position();
				nextHedge = nextHedge->Next()->Twin();
				nextVertex = nextHedge->Start();
				t1 = Ver1 - nextVertex->Position(); 
			}
			else {
				for (j = 1; j < k-1; j++){
					nextHedge = nextHedge->Next()->Twin();
					nextVertex = nextHedge->Start();
					t2[0] += (2 * cos(PI / (k - 1)) - 2)* sin(j*PI / (k - 1))* nextVertex->Position()[0];
					t2[1] += (2 * cos(PI / (k - 1)) - 2)* sin(j*PI / (k - 1))* nextVertex->Position()[1];
					t2[2] += (2 * cos(PI / (k - 1)) - 2)* sin(j*PI / (k - 1))* nextVertex->Position()[2];

				}
				nextHedge = nextHedge->Next()->Twin();
				nextVertex = nextHedge->Start();
				t1 = Ver1 - nextVertex->Position();
				t2 = t2 + sin(PI / (k - 1))* t1;
			}

		}
		else{
			nextHedge = vList[i]->HalfEdge()->Twin();  //get the first half edge that points to this vertex;
			nextVertex = nextHedge->Start();

			for (j = 0; j < k; j++){

				t1 += cos(2 * PI*j / k)*(nextVertex->Position() - vList[i]->Position());
				t2 += sin(2 * PI*j / k)*(nextVertex->Position() - vList[i]->Position());

				nextHedge = nextHedge->Next()->Twin(); //get next half edge that points to this vertex;
				nextVertex = nextHedge->Start();

			}

		}

	    //set the normalied product of two vertors as vertex normal
		nor = - t1.cross(t2); // why it nagates itself??
		nor.normalize();
		vList[i]->SetNormal(nor);	
	}
	
	
}

// compute the vertex curvature of the graph
void Mesh::ComputeVertexCurvatures() {

    
}

// umbrella smoothing
// uniformWeights: true for uniform-weight Laplacian, false for cotangent-weight Laplacian
void Mesh::UmbrellaSmooth(bool uniformWeights) {

    
}

// implicit umbrella smoothing
// uniformWeights: true for uniform-weight Laplacian, false for cotangent-weight Laplacian
void Mesh::ImplicitUmbrellaSmooth(bool uniformWeights) {
	if (!uniformWeights) {
		Cotangent_Smoother cotangent_laplacian(this);
		cotangent_laplacian.Deform();
	}
	else {
		Uniform_Smoother uniform_laplacian(this);
		uniform_laplacian.Deform();
	}
}

int Mesh::CountBoundaryLoops() {
    /*************************/
    /* insert your code here */
    /*************************/

    /*====== Programming Assignment 0 ======*/
	return 0;
}

void Mesh::TraverseComponents(HEdge* he)
{
	return;
}


int Mesh::CountConnectedComponents() 
{
    /*************************/
    /* insert your code here */
    /*************************/

    /*====== Programming Assignment 0 ======*/
	return 0;
}

void Mesh::GroupingVertexFlags() {
    // set vertex flag to be 255 initially
    for (size_t i = 0; i < vList.size(); i++)
        if (vList[i]->Flag() != 0)
            vList[i]->SetFlag(255);

    int id = 0;
    VertexList tmpList;
    for (int i = 0; i < vList.size(); i++)
        if (vList[i]->Flag() == 255) {
            id++;
            vList[i]->SetFlag(id);
            tmpList.push_back(vList[i]);
            while (! tmpList.empty()) {
                Vertex* v = tmpList.back();
                tmpList.pop_back();
                OneRingVertex ring = OneRingVertex(v);
                while (Vertex* v2 = ring.NextVertex()) {
                    if (v2->Flag() == 255) {
                        v2->SetFlag(id);
                        tmpList.push_back(v2);
                    }
                }
            }
        }
}


void Mesh::SetPrevNext(HEdge* e1, HEdge* e2) {
    e1->SetNext(e2);
    e2->SetPrev(e1);
}

void Mesh::SetTwin(HEdge* e1, HEdge* e2) {
    e1->SetTwin(e2);
    e2->SetTwin(e1);
}

void Mesh::SetFace(Face* f, HEdge* e) {
    f->SetHalfEdge(e);
    e->SetFace(f);
}

double Mesh::Cot(const Eigen::Vector3d& p1, const Eigen::Vector3d& p2, const Eigen::Vector3d& p3) {
    Eigen::Vector3d v1 = p1 - p2;
    Eigen::Vector3d v2 = p3 - p2;

    double _dot_res = v1.normalized().dot(v2.normalized());
    if (_dot_res < -1.0) {
        _dot_res = -1.0;
    }
    else if (_dot_res >  1.0) {
        _dot_res = 1.0;
    }
    return 1.0 / std::tan(std::acos(_dot_res));
}

double Mesh::TriArea(const Eigen::Vector3d& p1, const Eigen::Vector3d& p2, const Eigen::Vector3d& p3) {
    Eigen::Vector3d v1 = p2 - p1;
    Eigen::Vector3d v2 = p3 - p1;
    return v1.cross(v2).norm() / 2.0;
}
