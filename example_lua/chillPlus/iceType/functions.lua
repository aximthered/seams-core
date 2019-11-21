print("\n Welcome to the manual lua function evaluation environment.\n");

--- Init Modules
local lfs = require"lfs"

--- Call functions defined in script file
package.path = './../lua_inputs/luaFunctions/?.lua;' .. package.path
local luaFunctions = require("scripts");

--- Make the directories
make_output_dirs( doBOP, topoOneDim, topoTwoDim, topoBulk );
clusterStatsFile();

for frame=targetFrame,finalFrame,frameGap do
   resCloud=readFrame(trajectory,frame,resCloud,oxygenAtomType,isSlice,sliceLowerLimits,sliceUpperLimits) --- Get the frame
   nList=neighborList(cutoffRadius, resCloud, oxygenAtomType); --- Calculate the neighborlist by ID
   ---
   resCloud=chillPlus_cij(resCloud,nList,isSlice); --- Calculate Cij (cloud,slice)
   resCloud=chillPlus_iceType(resCloud,nList,outDir,isSlice,chillPlus_noMod); --- Write out data (cloud,slice,name)
   writeDump(resCloud,outDir,dumpChillP); --- Dump the rescloud which currently has CHILL Plus classifications
   --- Modified CHILL+
   avgQ6=averageQ6(resCloud,nList,isSlice); --- Average Q6 (cloud,slice)
   resCloud=modifyChill(resCloud,avgQ6); --- Modification (cloud,q6)
   percentage_Ice(resCloud,outDir,isSlice,chillPlus_mod); --- Post reclassification writeOut
   writeDump(resCloud,outDir,dumpSupaaP); --- Dump the rescloud which now has the supaa CHILL Plus Trajectory
   --- Get the largest ice cluster. Here, iceNeighbourList is the neighbour list by index.
   clusterAnalysis(outDir, clusterCloud, resCloud, nList, iceNeighbourList, cutoffRadius, "q6");
   --- Recenter the cluster such that the centroid is at the center of the simulation box 
   recenterCluster(clusterCloud, iceNeighbourList);
   writeDump(resCloud,outDir,largestClusterDump); --- Dump the recentered largest ice cluster
end
