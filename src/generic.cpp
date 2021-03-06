#include <generic.hpp>
#include <iostream>

/********************************************/ /**
*  Function for printing out
 info in a PointCloud object.
 *  @param[in] yCloud The input PointCloud to be printed.
 *  @param[in] outFile The name of the output file to which the information will
be printed.
***********************************************/
int gen::prettyPrintYoda(
    molSys::PointCloud<molSys::Point<double>, double> *yCloud,
    std::string outFile) {
  std::ofstream outputFile;
  // Create a new file in the output directory
  outputFile.open(outFile);

  if (outputFile.is_open()) {
    // First line
    outputFile << "# Frame\tAtomID\tx\ty\tz\tcij\ticeType\n";
    // Write out all the information out line by line
    for (int i = 0; i < yCloud->nop; i++) {
      outputFile << yCloud->currentFrame << "\t" << yCloud->pts[i].atomID
                 << "\t" << yCloud->pts[i].x << "\t" << yCloud->pts[i].y << "\t"
                 << yCloud->pts[i].z << "\t";
      // Print out cij
      // for(int c=0; c<yCloud->pts[i].c_ij.size(); c++){outputFile <<
      // yCloud->pts[i].c_ij[c]<<"\t";} Print out the classifier
      outputFile << yCloud->pts[i].iceType << "\n";
    }
  }
  // Close the file
  outputFile.close();
  return 0;
}

/********************************************/ /**
*  Function for getting the unwrapped coordinates
 of a pair of atoms.
 *  @param[in] yCloud The input PointCloud to be printed.
 *  @param[in] iatomIndex Index of the \f$ i^{th} \f$ atom.
 *  @param[in] jatomIndex Index of the \f$ j^{th} \f$ atom.
 *  @param[in, out] x_i X Coordinate of the \f$ i^{th} \f$ atom corresponding to the unwrapped distance.
 *  @param[in, out] y_i Y Coordinate of the \f$ i^{th} \f$ atom corresponding to the unwrapped distance.
 *  @param[in, out] z_i Z Coordinate of the \f$ i^{th} \f$ atom corresponding to the unwrapped distance.
 *  @param[in, out] x_j X Coordinate of the \f$ j^{th} \f$ atom corresponding to the unwrapped distance.
 *  @param[in, out] y_j Y Coordinate of the \f$ j^{th} \f$ atom corresponding to the unwrapped distance.
 *  @param[in, out] z_j Z Coordinate of the \f$ j^{th} \f$ atom corresponding to the unwrapped distance.
***********************************************/
int gen::unwrappedCoordShift(
    molSys::PointCloud<molSys::Point<double>, double> *yCloud, int iatomIndex,
    int jatomIndex, double *x_i, double *y_i, double *z_i, double *x_j,
    double *y_j, double *z_j) {
  //
  double x_iatom, y_iatom, z_iatom;
  double x_jatom, y_jatom, z_jatom;
  double x_ij, y_ij, z_ij;  // Relative distance
  std::vector<double> box = yCloud->box;
  double xPBC, yPBC, zPBC;  // Actual unwrapped distance

  // ----------------------------------------------------------------------
  // INIT
  // iatom
  x_iatom = yCloud->pts[iatomIndex].x;
  y_iatom = yCloud->pts[iatomIndex].y;
  z_iatom = yCloud->pts[iatomIndex].z;
  // jatom
  x_jatom = yCloud->pts[jatomIndex].x;
  y_jatom = yCloud->pts[jatomIndex].y;
  z_jatom = yCloud->pts[jatomIndex].z;
  // ----------------------------------------------------------------------
  // GET RELATIVE DISTANCE
  x_ij = x_iatom - x_jatom;
  y_ij = y_iatom - y_jatom;
  z_ij = z_iatom - z_jatom;
  // ----------------------------------------------------------------------
  // SHIFT COORDINATES IF REQUIRED
  // Shift x
  if (fabs(x_ij) > 0.5 * box[0]) {
    // Get the actual distance
    xPBC = box[0] - fabs(x_ij);
    if (x_ij < 0) {
      x_jatom = x_iatom - xPBC;
    }  // To the -x side of currentIndex
    else {
      x_jatom = x_iatom + xPBC;
    }  // Add to the + side
  }    // Shift nextElement
  //
  // Shift y
  if (fabs(y_ij) > 0.5 * box[1]) {
    // Get the actual distance
    yPBC = box[1] - fabs(y_ij);
    if (y_ij < 0) {
      y_jatom = y_iatom - yPBC;
    }  // To the -y side of currentIndex
    else {
      y_jatom = y_iatom + yPBC;
    }  // Add to the + side
  }    // Shift nextElement
  //
  // Shift z
  if (fabs(z_ij) > 0.5 * box[2]) {
    // Get the actual distance
    zPBC = box[2] - fabs(z_ij);
    if (z_ij < 0) {
      z_jatom = z_iatom - zPBC;
    }  // To the -z side of currentIndex
    else {
      z_jatom = z_iatom + zPBC;
    }  // Add to the + side
  }    // Shift nextElement
  // ----------------------------------------------------------------------
  // Assign values
  *x_i = x_iatom;
  *y_i = y_iatom;
  *z_i = z_iatom;
  *x_j = x_jatom;
  *y_j = y_jatom;
  *z_j = z_jatom;

  return 0;
}

/********************************************/ /**
 *  Function for obtaining the angle between two input vectors (std::vector).
 Internally, the vectors are converted to GSL vectors. The dot product between
 the input vectors is used to calculate the angle between them.
 *  @param[in] OO The O--O vector (but can be any vector, in general).
 *  @param[in] OH The O-H vector (but can be any vector, in general).
 *  \return The output angle between the input vectors, in radians
 ***********************************************/
double gen::gslVecAngle(std::vector<double> OO, std::vector<double> OH) {
  gsl_vector *gOO = gsl_vector_alloc(3);
  gsl_vector *gOH = gsl_vector_alloc(3);
  double norm_gOO, norm_gOH, xDummy, angle;
  for (int i = 0; i < 3; i++) {
    gsl_vector_set(gOO, i, OO[i]);
    gsl_vector_set(gOH, i, OH[i]);
  }
  norm_gOO = gsl_blas_dnrm2(gOO);
  norm_gOH = gsl_blas_dnrm2(gOH);
  gsl_blas_ddot(gOO, gOH, &xDummy);
  angle = acos(xDummy / (norm_gOO * norm_gOH));
  gsl_vector_free(gOO);
  gsl_vector_free(gOH);
  return angle;
}
