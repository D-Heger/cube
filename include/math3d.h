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
 * @brief 3D point structure with floating-point coordinates
 * 
 * Represents a point in 3D Cartesian coordinate space using single-precision
 * floating-point values. Used for cube vertices and surface calculations.
 */
typedef struct
{
    float x;    /**< X-coordinate (left-right axis) */
    float y;    /**< Y-coordinate (up-down axis) */  
    float z;    /**< Z-coordinate (forward-backward axis) */
} Point3D;

/**
 * @brief 3D rotation structure with Euler angles
 * 
 * Represents rotation around the three coordinate axes using Euler angles
 * in radians. Rotations are applied in the order: X, then Y, then Z.
 */
typedef struct
{
    float x;    /**< Rotation around X-axis (pitch) in radians */
    float y;    /**< Rotation around Y-axis (yaw) in radians */
    float z;    /**< Rotation around Z-axis (roll) in radians */
} Rotation3D;

/**
 * @brief Calculate transformed X coordinate after 3D rotation
 * 
 * Applies the full 3D rotation transformation matrix to compute the new
 * X coordinate. Uses combined rotation matrices for X, Y, and Z axes.
 * 
 * @param point The original 3D point before transformation
 * @param rotation The rotation angles to apply (in radians)
 * @return The transformed X coordinate as a float
 * 
 * @note This function does not validate input parameters
 */
float calculateX(Point3D point, Rotation3D rotation);

/**
 * @brief Calculate transformed Y coordinate after 3D rotation
 * 
 * Applies the full 3D rotation transformation matrix to compute the new
 * Y coordinate. Uses combined rotation matrices for X, Y, and Z axes.
 * 
 * @param point The original 3D point before transformation
 * @param rotation The rotation angles to apply (in radians)
 * @return The transformed Y coordinate as a float
 * 
 * @note This function does not validate input parameters
 */
float calculateY(Point3D point, Rotation3D rotation);

/**
 * @brief Calculate transformed Z coordinate after 3D rotation
 * 
 * Applies the full 3D rotation transformation matrix to compute the new
 * Z coordinate. Uses combined rotation matrices for X, Y, and Z axes.
 * 
 * @param point The original 3D point before transformation
 * @param rotation The rotation angles to apply (in radians)
 * @return The transformed Z coordinate as a float
 * 
 * @note This function does not validate input parameters
 */
float calculateZ(Point3D point, Rotation3D rotation);

#endif // MATH3D_H