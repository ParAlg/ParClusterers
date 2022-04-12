#pragma once

#include <limits>
#include <fstream>
#include <string>
#include <vector>

#include "utils/point.h"
#include "utils/unionfind.h"
#include "utils/utils.h"
#include "utils/chain.h"
#include "cacheUtilities.h"

using namespace std;
namespace research_graph {
namespace in_memory {
namespace internal {
namespace HACTree {

    /////// DualTree Traversal
    // find the farthest points between two trees
    // bichromatic furthest pair
    template<class nodeT>
    struct BCFP{
        EDGE __attribute__ ((aligned (16))) e;
        double ub = numeric_limits<double>::max();

        ~BCFP(){}
        BCFP(){e = EDGE(-1,-1,-1);}
        BCFP(EDGE ee):e(ee){}
        BCFP(EDGE ee, double t_ub):e(ee), ub(t_ub){}

        inline EDGE getResult(){return e;}
        inline double getResultW(){return e.getW();}
        inline bool isLeaf(nodeT *Q){return Q->isLeaf();} // || Q->size() < 400}
        
        inline bool Score(double d, nodeT *Q, nodeT *R){
            return d < e.getW();
        }

        inline bool Score(nodeT *Q, nodeT *R, bool check = true){
            return check && Score(nodeFarDistance<nodeT>(Q,R), Q, R);
        }

        inline void BaseCase(nodeT *Q, nodeT *R, int i, int j){
            auto f = [&](EDGE i, EDGE j) {return (i.getW() > j.getW());};
            double qrdist = (Q->at(i))->pointDist(*(R->at(j)));
            if(qrdist > ub){
                utils::writeMin(&e, EDGE(-1,-1,numeric_limits<double>::max()), f);
            }else{
                utils::writeMin(&e, EDGE(Q->at(i)->idx(),R->at(j)->idx(),qrdist), f);
            }
        }

        inline double NodeDistForOrder(nodeT *Q, nodeT *R){
            return nodeFarDistance<nodeT>(Q, R);
        }

        //if true, r first
        inline bool SpawnOrder(double l, double r){
            return l < r;
        }

        inline int SpawnOrder(int i){
            return 3-i;
        }

        inline void BasePost(nodeT *Q, nodeT *R){
            for(int i=0; i<Q->size(); ++i){
                for(int j=0;j<R->size() ; ++j){
                    BaseCase(Q,R,i,j);
                }
            }
        }
        inline void QLPost(nodeT *Q, nodeT *R){}
        inline void RLPost(nodeT *Q, nodeT *R){}
        inline void Post(nodeT *Q, nodeT *R){}
    };

    template<class nodeT, bool squared=false>
    struct AllPtsNN{
        EDGE *edges;
        edgeComparator2 EC2;

        AllPtsNN(EDGE *ee, double eps){
            EC2 = edgeComparator2(eps);
            edges = ee;
        }

        inline bool isLeaf(nodeT *Q){
            return Q->isLeaf();
        }

        inline bool Score(double d, nodeT *Q, nodeT *R){
            return d > (Q->getInfo()).getUB();
        }

        inline bool Score(nodeT *Q, nodeT *R, bool check = true){
            return check && Score(nodeDistance<nodeT>(Q,R), Q, R);
        }

        inline void BaseCase(nodeT *Q, nodeT *R, int i, int j){
            int ii = Q->at(i)->idx();
            int jj = R->at(j)->idx();
            if (ii == jj) return;
            double qrdist;
            if(squared){
                qrdist = (Q->at(i))->pointDistSq(*(R->at(j)));
            }else{
                qrdist = (Q->at(i))->pointDist(*(R->at(j)));
            }
            utils::writeMin(&edges[ii], EDGE(ii,jj,qrdist), EC2);
            
        }

        inline double NodeDistForOrder(nodeT *Q, nodeT *R){
            return nodeDistance<nodeT>(Q, R);
        }

        //if true, r first
        inline bool SpawnOrder(double l, double r){
            return l > r;
        }

        inline int SpawnOrder(int i){
            return i;
        }

        void BasePost(nodeT *Q, nodeT *R){
            for(int i=0; i<Q->size(); ++i){
                for(int j=0;j<R->size() ; ++j){
                    BaseCase(Q,R,i,j);
                }
            }

            double temp = edges[Q->at(0)->idx()].getW();
            for(int i=1; i<Q->size(); ++i){
               double eweight = edges[Q->at(i)->idx()].getW(); 
		    if(eweight > temp){
                    temp = eweight;
                }
            }
            if(squared){temp = sqrt(temp);}
            (Q->getInfo()).updateUB(temp);
        }

        inline void QLPost(nodeT *Q, nodeT *R){}
        inline void RLPost(nodeT *Q, nodeT *R){
            (Q->getInfo()).updateUB(max((Q->L()->getInfo()).getUB(), (Q->R()->getInfo()).getUB()));
        }
        inline void Post(nodeT *Q, nodeT *R){
            RLPost(Q,R);
        }
    };

    // find the NN of cluster cid
    //IMPORTANT: cid must be a valid cluster id, otherwise will find itself
    template<class nodeT>
    struct NNsingle{
        EDGE *e;
        UnionFind::ParUF<int> *uf;
        int cid; // cid of query cluster
        const edgeComparator2 EC2 = edgeComparator2();
        
        ~NNsingle(){
            delete e;
        }

        NNsingle(EDGE *t_e, UnionFind::ParUF<int> *t_uf, int t_cid):e(t_e),uf(t_uf), cid(t_cid){}

        NNsingle(UnionFind::ParUF<int> *t_uf, int t_cid):uf(t_uf), cid(t_cid){
            e = new EDGE(-1,-1,numeric_limits<double>::max());
        }
        
        inline bool isLeaf(nodeT *Q){
            return Q->isLeaf() || Q->size() < 400;
        }

        inline bool Score(double d, nodeT *Q, nodeT *R){
            return (R->getInfo().getCId() == cid) || (d > e->getW());
        }

        inline bool Score(nodeT *Q, nodeT *R, bool check = true){
            return check && Score(nodeDistance<nodeT>(Q,R), Q, R);
        }

        inline void BaseCase(nodeT *Q, nodeT *R, int i, int j){
            if(uf->find(R->at(j)->idx()) == cid) return;
            double qrdist = (Q->at(i))->pointDist(*(R->at(j)));
            utils::writeMin(e, EDGE(Q->at(i)->idx(),R->at(j)->idx(),qrdist), EC2);
        }

        inline double NodeDistForOrder(nodeT *Q, nodeT *R){
            return nodeDistance<nodeT>(Q, R);
        }

        //if true, r first
        inline bool SpawnOrder(double l, double r){
            return l > r;
        }

        inline int SpawnOrder(int i){
            return i;
        }

        inline void BasePost(nodeT *Q, nodeT *R){
            for(int i=0; i<Q->size(); ++i){
                for(int j=0;j<R->size(); ++j){
                    BaseCase(Q,R,i,j);
                }
            }
        }
        inline void QLPost(nodeT *Q, nodeT *R){}
        inline void RLPost(nodeT *Q, nodeT *R){}
        inline void Post(nodeT *Q, nodeT *R){}
    };

    /////// SingleTree Traversal

    // mark the if a tree node has points from a single cluster
    // used for complete linkage
    template<class nodeT>
    struct MarkClusterId{
        
        UnionFind::ParUF<int> *uf;
        int initVal = -1;

        MarkClusterId(UnionFind::ParUF<int> *t_uf):uf(t_uf){}
        MarkClusterId(){}
        inline bool doMark(int C, int round){ return round > 5 && C > 1;}
        inline bool isTopDown(int id){ return id != -1;}

        inline void TopDownNode(nodeT *Q, int id){
            if(!isTopDown(id)) return;
            Q->getInfo().setCId(id);
        }

        inline void BottomUpNode(nodeT *Q, int id){
            if(isTopDown(id)) return;
            int cidl = Q->L()->getInfo().getCId();
            if(cidl != -1){
                int cidr = Q->R()->getInfo().getCId();
                if(cidl == cidr)Q->getInfo().setCId(cidl);
            }
        }

        inline void BaseCase(nodeT *Q, int id){
            if(isTopDown(id)){
                Q->getInfo().setCId(id);
            }else{
                id = uf->find(Q->at(0)->idx());
                for(int i=0; i<Q->size(); ++i){
                    if(uf->find(Q->at(i)->idx())!= id){
                        Q->getInfo().setCId(-1);
                        return;
                    }
                }
                Q->getInfo().setCId(id);
            }
        }

        inline int SwitchMode(nodeT *Q, int id){
            if(isTopDown(id)) return id;
            int cid = Q->getInfo().getCId();
            if(cid == -1) return -1;
            return uf->find(cid); 
        }

        inline bool Par(nodeT *Q){return Q->size() > 2000;}

        inline bool Stop(nodeT *Q, int id){
            int cid = Q->getInfo().getCId();
            return cid != -1 && cid == id;
        }

    };

    // mark the min_n on a kd-tree of cluster centers, used for ward's linkage
    // TODO: clean the code to not mark this and only used the global min_n?
    template<class kdnodeT, class nodeInfo>
    struct MarkMinN{
        typedef typename nodeInfo::infoT infoT;

        int *sizes; //sizes of all clusters indexed by cluster id
        infoT initVal = nodeInfo().initInfoVal();

        MarkMinN(int *t_sizes):sizes(t_sizes){
        }
        MarkMinN(){
        }

        inline bool doMark(int C, int round){ return true;}

        inline bool isTopDown(infoT info){return false;}//{ return get<0>(info) != -1;}

        inline void TopDownNode(kdnodeT *Q, infoT info){
            if(!isTopDown(info)) return;
            Q->getInfo().setInfo(info);
        }

        inline void BottomUpNode(kdnodeT *Q, infoT info){
            if(isTopDown(info)) return;
            Q->getInfo().setMinN(min(Q->L()->getInfo().getMinN(), Q->R()->getInfo().getMinN()));
        }

        inline void BaseCase(kdnodeT *Q, infoT info){
            if(isTopDown(info)){
                Q->getInfo().setInfo(info);
            }else{
                int id = Q->items[0]->idx();
                int min_n = sizes[id]; 
                for(int i=1; i<Q->size(); ++i){
                    int id_temp = Q->at(i)->idx();
                    min_n = min(min_n, sizes[id_temp]);  
                }
                id = -1;
                Q->getInfo().setInfo(infoT(id, min_n));
            }
        }

        inline infoT SwitchMode(kdnodeT *Q, infoT info){ //must be -1
            return info;
        }

        inline bool Par(kdnodeT *Q){return Q->size() > 1000;}

        inline bool Stop(kdnodeT *Q, infoT info){
            return false;
        }

    };

    template<class nodeT, class F, class E>
    void singletree(nodeT *Q, F *f, E id){
        id = f->SwitchMode(Q, id);
        if(f->Stop(Q, id)) return;
        if(Q->isLeaf()){
            f->BaseCase(Q, id);
        }else{
            f->TopDownNode(Q, id);
            if(f->Par(Q)){
                parlay::par_do(
                [&](){singletree<nodeT, F, E>(Q->L(), f, id);}, 
                [&](){singletree<nodeT, F, E>(Q->R(), f, id);});  
            }else{
                singletree<nodeT, F, E>(Q->L(), f, id);
                singletree<nodeT, F, E>(Q->R(), f, id);
            }
            f->BottomUpNode(Q, id);
        }
    }

    // mark a tree with cid=cid
    template<class nodeT>
    void markTree(nodeT *Q, UnionFind::ParUF<int> *uf, int cid = -1){
        typedef MarkClusterId<nodeT> M;
        M *marker = new M(uf);
        singletree<nodeT, M, int>(Q, marker, cid);
    }


    ////// range search

    // used for kdtree
    template<int dim, class objT, class distT>
    struct RangeQueryCountF{
        typedef point<dim> pointT;
        typedef Node<dim> nodeT;
        typedef tuple<int, int, long> countCacheT;
        typedef tree<dim, objT, nodeInfo> kdtreeT;
        typedef node<dim, objT, nodeInfo> kdnodeT;
    
        UnionFind::ParUF<int> *uf;
        
        double r;
        int cid;
        bool no_cache;
        pair<int, double> e;
        countCacheT *tb; //
        nodeT *nodes;
        nodeT *qnode;
        int *rootIdx;
        CacheTables<nodeT>* tbs; //
        int pid;
        distT *distComputer;
        edgeComparator2 EC2;
        double eps;
        const bool local = true;
        
        
        RangeQueryCountF(int t_cid, double _r, CacheTables<nodeT>*t_tbs, EDGE *t_edges,
            distT *t_distComputer, UnionFind::ParUF<int> *t_uf, bool t_no_cache, int C, double _eps):
            uf(t_uf), r(_r), cid(t_cid), no_cache(t_no_cache),//edges(t_edges), 
            distComputer(t_distComputer), eps(_eps){
            EC2 = edgeComparator2(eps);
            e = make_pair(t_edges[cid].second, t_edges[cid].getW());

            pid = parlay::worker_id();
            tb = distComputer->initClusterTb(pid, C);//clusterTbs[idx];

            tbs = t_tbs;
            nodes = t_tbs->nodes;
            rootIdx = t_tbs->rootIdx;
            qnode = getNode(cid);
        }

        ~RangeQueryCountF(){
        }

        inline int getFinalNN(){return e.first;}
        inline double getFinalDist(){return e.second;}

        inline int idx(nodeT* node){ return node->idx;}
        inline nodeT *getNode(int cid){return nodes+rootIdx[cid];}
        inline int idx(int cid){return idx(getNode(cid));}

        inline void updateDist(int Rid, bool reach_thresh){
            if(cid != Rid && Rid != e.first){

                if(!no_cache){
                bool success = tbs->insert_check(cid, Rid, true, true);
                if(!success){  // only compute distance once
                    double dist = tbs->find(cid, Rid);
                    // success = false only when insertions fail and reach_thresh is false
                    if(dist == UNFOUND_TOKEN){  cout << "should not find unfound_token" << endl;exit(1);}
                    if(dist != CHECK_TOKEN){              
                        if(e.second - dist > eps){ e = make_pair(Rid, dist);}  
                        else if(abs(e.second - dist) <= eps && Rid < e.first){e = make_pair(Rid, dist); }
                    }
                    return;  
                }
                }
                double dist = distComputer->getDistNaive(qnode, getNode(Rid), -1, e.second, false);
                // double dist = distComputer->getDistNaive(cid,Rid, -1, e.second, false); //, false
                if(!no_cache) tbs->insert(cid, Rid, dist); 
                if(e.second - dist > eps){ e = make_pair(Rid, dist);}  
                else if(abs(e.second - dist) <= eps && Rid < e.first){e = make_pair(Rid, dist); }
                // tb->deleteVal(Rid); //does not support delete and insert at the same time, need to remove if parallel
            }
        }
        
        inline tuple<int, bool> incrementTable(int Rid, int a = 1){
            if(get<0>(tb[Rid]) != distComputer->round || get<2>(tb[Rid]) != cid){
                tb[Rid] =  make_tuple(distComputer->round, a, (long)cid);//make_entry(round, cid, a);
            }else{
                get<1>(tb[Rid]) += a;
            }
            return make_tuple(get<1>(tb[Rid]), false);
        }

        inline bool isComplete(kdnodeT *Q){
            int  Rid = Q->getInfo().getCId();
            if(cid == Rid ) return true;
            if( Rid != -1){
                int ct; bool reach_thresh;
                tie(ct, reach_thresh) = incrementTable(Rid, Q->size());
                if (reach_thresh || ct ==  distComputer->kdtrees[Rid]->size()) updateDist(Rid, reach_thresh);
                return true;
            }else{
                return false;
            }
        }

        inline bool checkComplete(objT *p){
            // if(p->pointDist(qnode->center) > r + EC2.eps) return false;
            int  Rid = uf->find(p->idx());
            if(cid == Rid ) return false;
            int ct; bool reach_thresh;
            tie(ct, reach_thresh) = incrementTable(Rid);
            if (reach_thresh || ct ==  distComputer->kdtrees[Rid]->size()) updateDist(Rid, reach_thresh);
            return false;
        }

        inline bool Par(kdnodeT *Q){
            return false;  // have to be false if using hashtable for clsuterhash
        }

    };

    // need t_m active hash table size to store candidates
    // invariant: e contain the current nearest neighbor in tbs to cid
    // the tree is a tree of cluster centroids, each point's is the cluster's id
    // insert into (smallid, large id) table, only succeeded one compute and update
    template<int dim, class objT, class distT>
    struct RangeQueryCenterF{
        typedef point<dim> pointT;
        typedef Node<dim> nodeT;
        typedef tree<dim, objT, nodeInfo> kdtreeT;
        typedef node<dim, objT, nodeInfo> kdnodeT;

        // UnionFind::ParUF<int> *uf;
        double r;
        int cid;
        EDGE *edges;
        nodeT *nodes;
        nodeT *qnode;
        int *rootIdx;
        // distCacheT **tbs; //
        CacheTables<nodeT> *tb;
        edgeComparator2 EC2;
        distT *distComputer;
        bool no_cache;
        const bool local = false; // writemin after

        RangeQueryCenterF(int t_cid, double _r, CacheTables<nodeT> *t_tbs, EDGE *t_edges,
            distT *t_distComputer, UnionFind::ParUF<int> *t_uf, bool t_no_cache, int C, double eps):
            r(_r), cid(t_cid), //clusteredPts(t_clusteredPts), uf(t_uf), 
            distComputer(t_distComputer),
            no_cache(t_no_cache){
                EC2 = edgeComparator2(eps);
            // keep nn candidate when merging
            tb = t_tbs;
            edges = t_edges;
            nodes = t_tbs->nodes;
            rootIdx = t_tbs->rootIdx;
            // f = new F();
            qnode = getNode(cid);
        }

        ~RangeQueryCenterF(){
        }
        inline int getFinalNN(){return edges[cid].second;}
        inline double getFinalDist(){return edges[cid].getW();}

        inline int idx(nodeT* node){ return node->idx;}
        inline nodeT *getNode(int cid){return nodes+rootIdx[cid];}
        inline int idx(int cid){return idx(getNode(cid));}

        inline double my_node_distance_sq(kdnodeT *Q) {
            pointT qcenter = qnode->center;
            for (int d = 0; d < dim; ++ d) {
                if (Q->getMin()[d] > qcenter[d] || qcenter[d] > Q->getMax()[d]) {
                // disjoint at dim d, and intersect on dim < d
                double rsqr = 0;
                for (int dd = d; dd < dim; ++ dd) {
                    double tmp = max(Q->getMin()[dd] - qcenter[dd], qcenter[dd] - Q->getMax()[dd]);
                    tmp = max(tmp, (double)0);
                    rsqr += tmp * tmp;
                }
                return rsqr;
                }
            }
            return 0; // intersect
        }

        inline void updateDist(int Rid){ // need another table!
            // only first inserted is true, CHECK_TOKEN as a placeholder
            // if already in table, entry will not be replaced because CHECK_TOKEN is inserted
            // success = true when pair not in tbs and this is the first insert
            if(!no_cache){
            bool success = tb->insert_check(cid, Rid, true, true);
            if(!success){  // only compute distance once
                double dist = tb->find(cid, Rid);
                // success = false only when insertions fail and reach_thresh is false
                if(dist == UNFOUND_TOKEN){  cout << "should not find unfound_token" << endl;exit(1);}
                if(dist != CHECK_TOKEN){              
                    utils::writeMin(&edges[cid], EDGE(cid, Rid, dist), EC2);
                    utils::writeMin(&edges[Rid], EDGE(Rid, cid, dist), EC2);
                }
                return;  
            }
            }
            
            double dist = distComputer->getDistNaive(qnode, getNode(Rid));
            if(!no_cache) tb->insert(cid, Rid, dist); 
            //in case Rid searches for cid
            utils::writeMin(&edges[cid], EDGE(cid, Rid, dist), EC2);
            utils::writeMin(&edges[Rid], EDGE(Rid, cid, dist), EC2);

        }

        inline bool isComplete(kdnodeT *Q){
            // if(distComputer->method == WARD){
            //     double dsq = my_node_distance_sq(Q);
            //     double min_n = (double)Q->getInfo().getMinN();
            //     double qn = (double)qnode->size();
            //     if(dsq > (qn + min_n)/min_n / qn  / 2 * r * r ) return true;
            // }
            return false;
        }

        inline bool checkComplete(objT *p){
            //already checked by iteminball
            // if(p->pointDist(qnode->center) > r) return false; //eps already added in get box
            int  Rid = p->idx(); //should all be centers uf->find(p->idx());
            if(cid != Rid && Rid != edges[cid].second) updateDist(Rid);
            return false;
        }

        inline bool Par(kdnodeT *Q){
            return Q->size() > 2000; 
        }

    };

}}}}