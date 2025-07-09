/************************************************************
 *  RRR Kinematics – implementation (cotovelo sempre para cima)
 ************************************************************/
#include "cinematic.h"
#include <math.h> // garante funções matemáticas

/* --------- Auxiliares internos --------- */
static inline float sq(float v)    { return v * v; }
static inline float clamp(float v) { return v < -1.f ? -1.f : (v > 1.f ? 1.f : v); }

/* --------- Cinemática Direta --------- */
Pos cin_dir(const Dim& d, const Ang& a)
{
    const float t1 =  a.t1 * D2R;
    const float t2 = (a.t2 + 90.f) * D2R;
    const float t3 =  a.t3 * D2R;

    const float c1 = cos(t1),  s1 = sin(t1);
    const float c2 = cos(t2),  s2 = sin(t2);
    const float c23 = cos(t2 + t3), s23 = sin(t2 + t3);

    return {
        (d.L2 * c2 + d.L3 * c23) * c1,
        (d.L2 * c2 + d.L3 * c23) * s1,
         d.L1 + d.L2 * s2 + d.L3 * s23
    };
}

/* --------- Cinemática Inversa (cotovelo ↑ fixo) --------- */
Ang cin_inv(const Dim& d, const Pos& p)
{
    float r  = sqrt(p.x * p.x + p.y * p.y);  // substitui hypot
    float zp = p.z - d.L1;
    float d2 = sq(r) + sq(zp);

    if (d2 > sq(d.L2 + d.L3) || d2 < sq(fabs(d.L2 - d.L3)))
        return { NAN, NAN, NAN }; // fora do alcance

   float t1_deg = atan2(p.y, p.x) * R2D;

    float cphi   = clamp((sq(d.L2) + sq(d.L3) - d2) / (2 * d.L2 * d.L3));
    float phi    = acos(cphi);
    float t3_rad = M_PI - phi;
    float t3_deg = t3_rad * R2D;

    float k1 = d.L2 + d.L3 * cos(t3_rad);
    float k2 = d.L3 * sin(t3_rad);
    float t2_phys = atan2(zp, r) - atan2(k2, k1);
    float t2_deg  = t2_phys * R2D - 90.f;

    return { t1_deg, t2_deg, t3_deg };
}
