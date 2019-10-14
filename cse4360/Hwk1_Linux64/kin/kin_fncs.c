#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415927
#endif
#define N 4

// units are meters:
const double d = 0.05;
const double l0 = 0.35;
const double l1 = 0.50;
const double l2 = 0.35;
const double l3 = 0.25;

// operations
void rotate_x(double mat[N][N], double angle);
void rotate_y(double mat[N][N], double angle);
void rotate_z(double mat[N][N], double angle);
void translate_x(double mat[N][N], double dist);
void translate_y(double mat[N][N], double dist);
void translate_z(double mat[N][N], double dist);

void posprint(double mat[N][N]);
void matprint(double mat[N][N]);
void matmul(double output[N][N], double lhs[N][N], double rhs[N][N]);
void normalize(double mat[N][N]);

int fwd_kin(double theta[6], double x[3]) {
    double output[N][N] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    // move to joint 1
    rotate_z(output, theta[0]);
    translate_x(output, -d);
    translate_z(output, l0);

    // move to joint 2
    rotate_y(output, theta[1]);
    translate_y(output, -d);
    translate_x(output, l1);

    // move to joint 3
    rotate_y(output, theta[2]);
    translate_y(output, d);
    translate_x(output, l2);

    // move to joint 4
    rotate_y(output, theta[3]);
    translate_y(output, -d);
    rotate_x(output, theta[4]);
    translate_x(output, l3);

    normalize(output);

    // last column is the answers
    x[0] = output[0][N - 1];
    x[1] = output[1][N - 1];
    x[2] = output[2][N - 1];

    return 0;
}


int inv_kin(double x[3], double theta[6]) {
    // theta[4] = theta[5] = 0.0 => always ignore these values
   
    
    
    return 0;
}

//
// Util Functions
//

void matprint(double mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%+4.2lf ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void posprint(double mat[N][N]) {
    normalize(mat);
    printf("(x, y, z) => (%+4.2lf, %+4.2lf, %+4.2lf)\n", mat[0][3], mat[1][3], mat[2][3]);
}

void matmul(double output[N][N], double lhs[N][N], double rhs[N][N]) {
    memset(output, 0, sizeof(double) * N * N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                output[i][j] += lhs[i][k] * rhs[k][j];
            }
        }
    }
}

void normalize(double mat[N][N]) {
    double val = mat[N - 1][N - 1];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j] /= val;
        }
    }
}

// forgive me Lord, for I have sinned
#define Rx11(theta) 1
#define Rx12(theta) 0
#define Rx13(theta) 0
#define Rx21(theta) 0
#define Rx22(theta) cos(theta)
#define Rx23(theta) -sin(theta)
#define Rx31(theta) 0
#define Rx32(theta) sin(theta)
#define Rx33(theta) cos(theta)

#define Ry11(theta) cos(theta)
#define Ry12(theta) 0
#define Ry13(theta) sin(theta)
#define Ry21(theta) 0
#define Ry22(theta) 1
#define Ry23(theta) 0
#define Ry31(theta) -sin(theta)
#define Ry32(theta) 0
#define Ry33(theta) cos(theta)

#define Rz11(theta) cos(theta) 
#define Rz12(theta) -sin(theta)
#define Rz13(theta) 0
#define Rz21(theta) sin(theta)
#define Rz22(theta) cos(theta)
#define Rz23(theta) 0
#define Rz31(theta) 0 
#define Rz32(theta) 0
#define Rz33(theta) 1

#define Rx(r, c, theta) Rx##r##c(theta)
#define Ry(r, c, theta) Ry##r##c(theta)
#define Rz(r, c, theta) Rz##r##c(theta)

#define rotation(rot, theta) {\
    {rot(1, 1, theta), rot(1, 2, theta), rot(1, 3, theta), 0},\
    {rot(2, 1, theta), rot(2, 2, theta), rot(2, 3, theta), 0},\
    {rot(3, 1, theta), rot(3, 2, theta), rot(3, 3, theta), 0},\
    {0, 0, 0, 1}}

#define ROTATION_FUNC(name, rot) \
    void rotate_##name (double mat [N][N], double angle) {\
        double transform[N][N] = rotation(rot, angle);\
        double tmp[N][N];\
        memcpy(tmp, mat, sizeof(tmp));\
        matmul(mat, tmp, transform);\
    }

ROTATION_FUNC(x, Rx);
ROTATION_FUNC(y, Ry);
ROTATION_FUNC(z, Rz);




#define X1(dist) dist
#define X2(dist) 0
#define X3(dist) 0

#define Y1(dist) 0
#define Y2(dist) dist 
#define Y3(dist) 0

#define Z1(dist) 0
#define Z2(dist) 0
#define Z3(dist) dist

#define X(n, dist) X##n(dist)
#define Y(n, dist) Y##n(dist)
#define Z(n, dist) Z##n(dist)

#define translation(dir, dist)  {\
    {1, 0, 0, dir(1, dist)},\
    {0, 1, 0, dir(2, dist)},\
    {0, 0, 1, dir(3, dist)},\
    {0, 0, 0, 1}}



#define TRANSLATION_FUNC(name, dir) \
    void translate_##name (double mat [N][N], double dist) {\
        double transform[N][N] = translation(dir, dist);\
        double tmp[N][N];\
        memcpy(tmp, mat, sizeof(tmp));\
        matmul(mat, tmp, transform);\
    }

TRANSLATION_FUNC(x, X);
TRANSLATION_FUNC(y, Y);
TRANSLATION_FUNC(z, Z);

