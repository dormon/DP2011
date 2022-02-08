#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include"standard.h"
#include"mymem.h"
#include"vector.h"

/**
 * @brief Spocita normaly
 *
 * @param Normal normaly
 * @param NormalStride prokladani normal
 * @param Points body
 * @param PointStride prokladani bodu
 * @param NumPoints pocet bodu
 * @param Triangles trojuhelniky
 * @param NumTriangles pocet trojuhelniku
 */
void geometry_ComputeNormal(
		float*Normal,unsigned NormalStride,
		float*Points,unsigned PointStride,unsigned NumPoints,
		unsigned*Triangles,unsigned NumTriangles);

/**
 * @brief Spocita propoje (hrany,pruziny)
 *
 * @param Joints propoje
 * @param NumJoints pocet propoju
 * @param Triangles trojuhelniky
 * @param NumPoints pocet bodu
 * @param NumTriangles pocet trojuhleniku
 * @param Full kazdy s kazdym
 */
void geometry_ComputeJoints(unsigned**Joints,unsigned*NumJoints,
		unsigned NumPoints,
		unsigned*Triangles,unsigned NumTriangles,
		unsigned Full);

#endif//_GEOMETRY_H_
