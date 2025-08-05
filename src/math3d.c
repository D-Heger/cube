/**
 * @file math3d.c
 * @author D. Heger
 * @brief 3D mathematics functions implementation
 * @version 1.1.0
 * @date 2025-08-05
 */

#include <math.h>
#include "math3d.h"

/**
 * @brief Calculates the X coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * This function applies 3D rotation transformation matrix calculations to determine
 * the new X coordinate after rotation around all three axes.
 *
 * @param point The point to calculate the X coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The X coordinate of the point.
 */
float calculateX(Point3D point, Rotation3D rotation)
{
    return point.y * sin(rotation.x) * sin(rotation.y) * cos(rotation.z) -
           point.z * cos(rotation.x) * sin(rotation.y) * cos(rotation.z) +
           point.y * cos(rotation.x) * sin(rotation.z) +
           point.z * sin(rotation.x) * sin(rotation.z) +
           point.x * cos(rotation.y) * cos(rotation.z);
}

/**
 * @brief Calculates the Y coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * This function applies 3D rotation transformation matrix calculations to determine
 * the new Y coordinate after rotation around all three axes.
 *
 * @param point The point to calculate the Y coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The Y coordinate of the point.
 */
float calculateY(Point3D point, Rotation3D rotation)
{
    return point.y * cos(rotation.x) * cos(rotation.z) +
           point.z * sin(rotation.x) * cos(rotation.z) -
           point.y * sin(rotation.x) * sin(rotation.y) * sin(rotation.z) +
           point.z * cos(rotation.x) * sin(rotation.y) * sin(rotation.z) -
           point.x * cos(rotation.y) * sin(rotation.z);
}

/**
 * @brief Calculates the Z coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * This function applies 3D rotation transformation matrix calculations to determine
 * the new Z coordinate after rotation around all three axes.
 *
 * @param point The point to calculate the Z coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The Z coordinate of the point.
 */
float calculateZ(Point3D point, Rotation3D rotation)
{
    return point.z * cos(rotation.x) * cos(rotation.y) -
           point.y * sin(rotation.x) * cos(rotation.y) +
           point.x * sin(rotation.y);
}