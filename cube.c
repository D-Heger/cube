#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
    float x, y, z;
} Point3D;

typedef struct
{
    float x, y, z;
} Rotation3D;

float cubeWidth = 10;
int windowWidth = 160, windowHeight = 44;
float zBuffer[160 * 44];
char frameBuffer[160 * 44];
int backgroundChar = ' ';
float cubeIncrementStep = 1;
int viewerDistance = 60;
float projectionScaleFactor = 40;
int bufferIndex;

Rotation3D rotationAngles = {0, 0, 0}; // Set initial values

float calculateX(Point3D point, Rotation3D rotation)
{
    return point.y * sin(rotation.x) * sin(rotation.y) * cos(rotation.z) -
           point.z * cos(rotation.x) * sin(rotation.y) * cos(rotation.z) +
           point.y * cos(rotation.x) * sin(rotation.z) +
           point.z * sin(rotation.x) * sin(rotation.z) +
           point.x * cos(rotation.y) * cos(rotation.z);
}

float calculateY(Point3D point, Rotation3D rotation)
{
    return point.y * cos(rotation.x) * cos(rotation.z) +
           point.z * sin(rotation.x) * cos(rotation.z) -
           point.y * sin(rotation.x) * sin(rotation.y) * sin(rotation.z) +
           point.z * cos(rotation.x) * sin(rotation.y) * sin(rotation.z) -
           point.x * cos(rotation.y) * sin(rotation.z);
}

float calculateZ(Point3D point, Rotation3D rotation)
{
    return point.z * cos(rotation.x) * cos(rotation.y) -
           point.y * sin(rotation.x) * cos(rotation.y) +
           point.x * sin(rotation.y);
}

void calculateForSurface(Point3D cubePoint, int ch, Rotation3D rotation)
{
    Point3D projected;
    projected.x = calculateX(cubePoint, rotation);
    projected.y = calculateY(cubePoint, rotation);
    projected.z = calculateZ(cubePoint, rotation) + viewerDistance;

    float inverseDepth = 1 / projected.z;

    int screenX = (int)(windowWidth / 2 + projectionScaleFactor * inverseDepth * projected.x * 2);
    int screenY = (int)(windowHeight / 2 + projectionScaleFactor * inverseDepth * projected.y);

    bufferIndex = screenX + screenY * windowWidth;
    if (bufferIndex >= 0 && bufferIndex < windowWidth * windowHeight)
    {
        if (inverseDepth > zBuffer[bufferIndex])
        {
            zBuffer[bufferIndex] = inverseDepth;
            frameBuffer[bufferIndex] = ch;
        }
    }
}

void initializeFrameBuffer()
{
    memset(frameBuffer, backgroundChar, windowWidth * windowHeight);
}

void initializeDepthBuffer()
{
    memset(zBuffer, 0, windowWidth * windowHeight * sizeof(float));
}

void drawCube(Rotation3D rotation)
{
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += cubeIncrementStep)
    {
        for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += cubeIncrementStep)
        {
            calculateForSurface((Point3D){cubeX, cubeY, -cubeWidth}, '.', rotation);
            calculateForSurface((Point3D){cubeWidth, cubeY, cubeX}, '$', rotation);
            calculateForSurface((Point3D){-cubeWidth, cubeY, -cubeX}, '~', rotation);
            calculateForSurface((Point3D){-cubeX, cubeY, cubeWidth}, '#', rotation);
            calculateForSurface((Point3D){cubeX, -cubeWidth, -cubeY}, ';', rotation);
            calculateForSurface((Point3D){cubeX, cubeWidth, cubeY}, '+', rotation);
        }
    }
}

int main()
{
    printf("\x1b[2J"); // Clear screen control sequence
    while (1)
    {
        initializeFrameBuffer();
        initializeDepthBuffer();

        drawCube(rotationAngles);

        printf("\x1b[H");
        for (int k = 0; k < windowWidth * windowHeight; k++)
        {
            putchar(k % windowWidth ? frameBuffer[k] : '\n');
        }

        rotationAngles.x += 0.005;
        rotationAngles.y += 0.005;
        usleep(1000);
    }

    return 0;
}