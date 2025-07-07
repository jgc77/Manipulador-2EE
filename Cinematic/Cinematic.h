#ifndef RRR_KINEMATICS_H
#define RRR_KINEMATICS_H

#include <cmath>
/* --------- Estruturas básicas --------- */
struct Dim  { float L1, L2, L3; };   // comprimentos dos elos
struct Ang  { float t1, t2, t3; };   // graus (relativos)
struct Pos  { float x,  y,  z;  };   // unidades mesmas de L1..L3
/* --------- Conversão grau↔rad --------- */
constexpr float D2R = static_cast<float>(M_PI / 180.0);
constexpr float R2D = static_cast<float>(180.0 / M_PI);
/* --------- Protótipos --------- */
/* Cinemática Direta  –  devolve (x,y,z) para (θ1,θ2,θ3)               */
Pos  cin_dir(const Dim& d, const Ang& a);

/* Cinemática Inversa (cotovelo ↑) – devolve true se ponto alcançável.*/

bool cin_inv(const Dim& d, const Pos& p,
             Ang& solUp,
             bool checkLimits = true);

#endif  /* RRR_KINEMATICS_H */

