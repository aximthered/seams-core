#include <pntCorrespondence.hpp>

// Fills up an eigen matrix point set for an HC, according to an input
// pointCloud, the relative order given by the basal rings, beginning from the
// startingIndex
Eigen::MatrixXd pntToPnt::fillPointSetHC(
    molSys::PointCloud<molSys::Point<double>, double> *yCloud,
    std::vector<std::vector<int>> relOrder, int startingIndex) {
  //
  int dim = 3;                        // Number of dimensions (3)
  int ringSize = relOrder[0].size();  // Number of elements in each basal ring
  int nop = ringSize * relOrder.size();  // Number of particles in the HC
  Eigen::MatrixXd pointSet(nop, dim);    // Output set of 3D points
  // Indices for the first and second basal rings being filled
  int iBasalOne, iBasalTwo;  // Indices being filled
  int currentPosition;       // Current index in the first or second basal rings
                             // inside relOrder
  int index;                 // Index in yCloud

  // Check
  if (startingIndex >= ringSize || startingIndex < 0) {
    startingIndex = 0;
  }  // end of check

  // Beginning from the starting index, get the points such that the first basal
  // ring is filled up first, followed by the connected second basal ring
  for (int i = 0; i < ringSize; i++) {
    //
    iBasalOne = i;             // basal one ring is filled first
    iBasalTwo = i + ringSize;  // basal two ring is filled after basal one is
                               // filled in sequential order
    // Getting currentIndex
    currentPosition = startingIndex + i;
    // Wrapping around for the ring
    if (currentPosition >= ringSize) {
      currentPosition -= ringSize;
    }  // end of wrap-around
    //
    // -------------------
    // basal one points
    index = relOrder[0][currentPosition];  // Index of the current point
    pointSet(iBasalOne, 0) = yCloud->pts[index].x;  // x coord
    pointSet(iBasalOne, 1) = yCloud->pts[index].y;  // y coord
    pointSet(iBasalOne, 2) = yCloud->pts[index].z;  // z coord
    // -------------------
    // basal two points
    index = relOrder[1][currentPosition];  // Index of the current point
    pointSet(iBasalTwo, 0) = yCloud->pts[index].x;  // x coord
    pointSet(iBasalTwo, 1) = yCloud->pts[index].y;  // y coord
    pointSet(iBasalTwo, 2) = yCloud->pts[index].z;  // z coord
  }  // end of point filling from the relative ordering vector of vectors

  // Return the set of points
  return pointSet;
}  // end function