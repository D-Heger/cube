/**
 * @file math3d.h
 * @author D. Heger
 * @brief 3D mathematics structures and function declarations
 * @version 1.1.0
 * @date 2025-08-05
 */

#ifndef MATH3D_H
#define MATH3D_H

/**
 * @brief A structure to represent a 3D point.
 */
typedef struct
{
    float x, y, z;
} Point3D;

/**
 * @brief A structure to represent a 3D rotation.
 */
typedef struct
{
    float x, y, z;
} Rotation3D;

/**
 * @brief Calculates the X coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * @param point The point to calculate the X coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The X coordinate of the point.
 */
float calculateX(Point3D point, Rotation3D rotation);

/**
 * @brief Calculates the Y coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * @param point The point to calculate the Y coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The Y coordinate of the point.
 */
float calculateY(Point3D point, Rotation3D rotation);

/**
 * @brief Calculates the Z coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * @param point The point to calculate the Z coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The Z coordinate of the point.
 */
float calculateZ(Point3D point, Rotation3D rotation);

#endif // MATH3D_H