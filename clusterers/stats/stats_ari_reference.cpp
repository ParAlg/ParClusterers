
#include <cmath> // For the log2 function
#include <map>
#include <vector>
#include <iostream>
using namespace std;

// https://github.com/topology-tool-kit/ttk/blob/731ed7cd29691cd2958d9bf574cb562a89bd17b6/core/base/clusteringMetrics/ClusteringMetrics.cpp
int computeContingencyTables(
  const int *clust1,
  const int *clust2,
  const size_t nPoint,
  std::vector<std::vector<int>> &contingencyMatrix,
  std::vector<int> &sumLin,
  std::vector<int> &sumCol) {
  if(nPoint == 0) {
    return 0;
  }

  std::map<int, int> values1ToId, values2ToId; //  cluster id to node ids
  size_t nbVal1 = 0, nbVal2 = 0; // number of unique clusters
  for(size_t i = 0; i < nPoint; i++) {
    int x1 = clust1[i], x2 = clust2[i];
    auto found1 = values1ToId.find(x1), found2 = values2ToId.find(x2);

    if(found1 == values1ToId.end()) {
      values1ToId[x1] = nbVal1;
      nbVal1++;
    }

    if(found2 == values2ToId.end()) {
      values2ToId[x2] = nbVal2;
      nbVal2++;
    }
  }

  size_t nCluster1 = nbVal1, nCluster2 = nbVal2; // number of unique clusters
  contingencyMatrix.resize(nCluster1);
  for(size_t i = 0; i < nCluster1; i++)
    contingencyMatrix[i].resize(nCluster2, 0); // resize matrix to nCluster1 x nCluster2
  sumLin.resize(nCluster1);
  sumCol.resize(nCluster2, 0);

  for(size_t i = 0; i < nPoint; i++) {
    int x1 = values1ToId[clust1[i]], x2 = values2ToId[clust2[i]]; // x1-th cluster and x2-th cluster, a mapping from cluster id to a consecutive cluster id
    contingencyMatrix[x1][x2]++; // number of points in x1-th cluster and x2-th cluster at the same time
  }

  for(size_t i1 = 0; i1 < nCluster1; i1++) { //  compute the sum of columns and rows
    int sum = 0;
    for(size_t i2 = 0; i2 < nCluster2; i2++) {
      sumCol[i2] += contingencyMatrix[i1][i2];
      sum += contingencyMatrix[i1][i2];
    }

    sumLin[i1] = sum;
  }

  return 0;
}

inline std::size_t nChoose2(std::size_t x) {
  return x * (x - 1) / 2 ;
}

int computeARI(
  const std::vector<std::vector<int>> &contingencyMatrix,
  const std::vector<int> &sumLin,
  const std::vector<int> &sumCol,
  const size_t nPoint,
  double &ariValue) {

  size_t nCluster1 = contingencyMatrix.size();
  size_t nCluster2 = contingencyMatrix[0].size();

  double sumNChooseContingency = 0;
#ifdef TTK_ENABLE_OPENMP
#pragma omp parallel for num_threads(this->threadNumber_) reduction(+:sumNChooseContingency)
#endif // TTK_ENABLE_OPENMP
  for(size_t i1 = 0; i1 < nCluster1; i1++) {
    for(size_t i2 = 0; i2 < nCluster2; i2++)
      sumNChooseContingency += nChoose2(contingencyMatrix[i1][i2]); // n choose 2 of all contingency matrix values
  }

  double sumNChoose2_1 = 0, sumNChoose2_2 = 0;
  for(size_t i = 0; i < nCluster1; i++) {

    sumNChoose2_1 += nChoose2(sumLin[i]);
  }
  for(size_t i = 0; i < nCluster2; i++) {

    sumNChoose2_2 += nChoose2(sumCol[i]);
  }

  double numerator = sumNChooseContingency
                     - (sumNChoose2_1 * sumNChoose2_2) / nChoose2(nPoint);
  double denominator = 0.5 * (sumNChoose2_1 + sumNChoose2_2)
                       - (sumNChoose2_1 * sumNChoose2_2) / nChoose2(nPoint);

    ariValue = numerator / denominator;

  return 0;
}

double execute(const int *clustering1,
                                    const int *clustering2,
                                    const size_t n,
                                    double &nmiValue,
                                    double &ariValue) {

  std::vector<std::vector<int>> contingencyMatrix;
  std::vector<int> sumLines, sumColumns;
  computeContingencyTables(
    clustering1, clustering2, n, contingencyMatrix, sumLines, sumColumns);

  computeARI(contingencyMatrix, sumLines, sumColumns, n, ariValue);

  cout << "Computed ARI value: " + std::to_string(ariValue) + "\n" << endl;
  // computeNMI(contingencyMatrix, sumLines, sumColumns, n, nmiValue);

  // this->printMsg("Size of output in ttk/base = 0\n");

  // this->printMsg("Computed NMI value: " + std::to_string(nmiValue) + "\n");
  // this->printMsg("Computed ARI value: " + std::to_string(ariValue) + "\n");
  // this->printMsg(ttk::debug::Separator::L2); // horizontal '-' separator
  // this->printMsg("Complete", 1, timer.getElapsedTime());
  // this->printMsg(ttk::debug::Separator::L1); // horizontal '=' separator
  return ariValue;
}


int main() {
    int size=5;
    int clustering1[5] = {1,1,1,2,2};
    int clustering2[5] = {2,2,2,1,1};
    double ari;
    double nmi;
    execute(&clustering1[0], &clustering2[0], size, ari, nmi);
    return 0;
}