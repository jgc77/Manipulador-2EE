#ifndef CINEMATIC_H
#define CINEMATIC_H

/*****************************************************************
 *  RRR Kinematics – header (versão compatível com Arduino)
 *****************************************************************/
#include <math.h>  // substitui <cmath> para o Arduino

#ifndef M_PI
#define M_PI 3.14159265f
#endif

/* --------- Estruturas básicas --------- */
struct Dim  { float L1, L2, L3; };   // comprimentos
struct Ang  { float t1, t2, t3; };   // ângulos (graus relativos)
struct Pos  { float x,  y,  z;  };   // posição (cartesiana)

/* --------- Conversão grau↔rad --------- */
constexpr float D2R = M_PI / 180.0f;
constexpr float R2D = 180.0f / M_PI;

/* --------- Protótipos --------- */
Pos cin_dir(const Dim& d, const Ang& a);
Ang cin_inv(const Dim& d, const Pos& p);

#endif  /* CINEMATIC_H */
