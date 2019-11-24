#include "BufferSmoothing.hpp"
#include <iostream>
#include <vector>
#include <queue>
#pragma once

BufferSmoothing::BufferSmoothing(Mesh * mesh)
{
    this->mesh = mesh;
    BuildSystemMatrix();
}


BufferSmoothing::~BufferSmoothing() {
    if (solver != NULL) {
       // delete solver;
//        delete Lap;
    }
}

void BufferSmoothing::Deform() {
    VertexList vList = mesh->Vertices();
    for (int i = 0; i < vList.size(); i++) {
        Eigen::Vector3d newPosition = Eigen::Vector3d(0.9*vx[i]+vList[i]->Position()(0), 0.9*vy[i]+vList[i]->Position()(1) , 0.9*vz[i]+vList[i]->Position()(2));
        vList[i]->SetPosition(newPosition);
    }
}

// build the differential operator matrix and do factorization
void BufferSmoothing::BuildSystemMatrix() {
    VertexList vList = mesh->Vertices();
    int flag[100000];
    memset(flag,0,sizeof(flag));
    for (int i = 0; i < vList.size(); i++) {
        double sum=0, multi=1.0;
        int k = vList[i]->Valence();
        HEdge* nextHedge = vList[i]->HalfEdge()->Twin();
        for (int r = 0; r < k; r++) {
            sum+=nextHedge->Start()->Flag();multi*=nextHedge->Start()->Flag();
            nextHedge = nextHedge->Next()->Twin();
        }
        if(multi==0&&sum!=0){flag[i]=1;}
    }
    int Buffersize = 30;
    int cnt = 0;
    std::queue<int> q; int dis[100000];
    memset(dis,0,sizeof(dis));
    for(int i=0;i<vList.size();i++)if(flag[i]){q.push(i);vList[i]->isBFSvisit = 1;dis[i]=0;}
    while(!q.empty()){
        cnt ++;
        int a = q.front(); q.pop();
        int k = vList[a]->Valence();
        if(dis[a]>Buffersize)continue;
        HEdge* nextHedge = vList[a]->HalfEdge()->Twin();
        for (int r = 0; r < k; r++) {
            int b = nextHedge->Start()->Index();
            if(!vList[b]->isBFSvisit){
                vList[b]->isBFSvisit = 1;
                dis[b] = dis[a]+1;
                q.push(b);
                flag[b] = 1;
            }
            nextHedge = nextHedge->Next()->Twin();
        }
    }
    std::cout<<cnt<<std::endl;
    for (int i = 0; i < vList.size(); i++)if(flag[i]){
        int k = vList[i]->Valence();
        HEdge* nextHedge = vList[i]->HalfEdge()->Twin();
        for (int r = 0; r < k; r++) {
            Lap->AddEntry(i, nextHedge->Start()->Index(), 1.0/k);
            nextHedge = nextHedge->Next()->Twin();
        }
        Lap->AddEntry(i, i, -1.0);
    }
    //get delta vectors of x, y, z coord for each vertex before transformation
    vx = Eigen::VectorXd::Zero(vList.size());
    vy = Eigen::VectorXd::Zero(vList.size());
    vz = Eigen::VectorXd::Zero(vList.size());

    for (int i = 0; i < vList.size(); i++){
        vx[i] = vList[i]->Position()(0);
        vy[i] = vList[i]->Position()(1);
        vz[i] = vList[i]->Position()(2);
    }

    Eigen::SparseMatrix<double> Laplacian(vList.size(), vList.size());
    Laplacian = Lap->ToSparseMatrix(vList.size(), vList.size());

    vx = Laplacian*vx;
    vy = Laplacian*vy;
    vz = Laplacian*vz;
}
