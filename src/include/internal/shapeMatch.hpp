#ifndef __SHAPEMATCH_H_
#define __SHAPEMATCH_H_

#include <math.h>
#include <sys/stat.h>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <absOrientation.hpp>
#include <mol_sys.hpp>
#include <pntCorrespondence.hpp>
#include <ring.hpp>
#include <seams_input.hpp>
#include <seams_output.hpp>

namespace match {

// Shape-matching for a pair of polygon basal rings. Returns true if the pair of
// basal rings form a prism block.
bool matchPrism(molSys::PointCloud<molSys::Point<double>, double> *yCloud,
                std::vector<std::vector<int>> nList,
                const Eigen::MatrixXd &refPoints, std::vector<int> *basal1,
                std::vector<int> *basal2, std::vector<double> *rmsdPerAtom,
                bool isPerfect = true);

// Shape-matching for a pair of polygon basal rings, comparing with a complete
// prism block. Returns true if the pair of basal rings form a prism block.
bool matchPrismBlock(molSys::PointCloud<molSys::Point<double>, double> *yCloud,
                     std::vector<std::vector<int>> nList,
                     const Eigen::MatrixXd &refPoints, std::vector<int> *basal1,
                     std::vector<int> *basal2, int *beginIndex);

// Update the per-particle RMSD for a prism block basal ring.
int updatePerAtomRMSDRing(std::vector<int> basalRing, int startingIndex,
                          std::vector<double> rmsdFromMatch,
                          std::vector<double> *rmsdPerAtom);

}  // namespace match

#endif  // __SHAPEMATCH_H_
