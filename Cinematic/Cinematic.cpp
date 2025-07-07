/************************************************************
 *  RRR Kinematics – implementation (cotovelo sempre ↑)
 ************************************************************/
#include "cinematic.h"

/* --------- Auxiliares internos --------- */
static inline float sq   (float v)             { return v * v; }
static inline float clamp(float v)             { return v < -1.f ? -1.f : (v > 1.f ? 1.f : v); }

/* --------- Cinemática Direta --------- */
Pos cin_dir(const Dim& d, const Ang& a)
{
    const float t1 =  a.t1 * D2R;           // anti‑horário
    const float t2 = (a.t2 + 90.f) * D2R;   // 0° = braço vertical
    const float t3 =  a.t3 * D2R;           // 0° = esticado

    const float c1 = std::cos(t1),  s1 = std::sin(t1);
    const float c2 = std::cos(t2),  s2 = std::sin(t2);
    const float c23= std::cos(t2+t3), s23= std::sin(t2+t3);

    return { (d.L2*c2 + d.L3*c23)*c1,
             (d.L2*c2 + d.L3*c23)*s1,
              d.L1 + d.L2*s2 + d.L3*s23 };
}

/* --------- Cinemática Inversa  (cotovelo ↑ fixo) --------- */
bool cin_inv(const Dim& d, const Pos& p, Ang& solUp, bool checkLimits)
{
    const float r  = std::hypot(p.x, p.y);
    const float zp = p.z - d.L1;
    const float d2 = sq(r) + sq(zp);

    /* Alcance geométrico */
    if (d2 > sq(d.L2+d.L3) || d2 < sq(std::fabs(d.L2-d.L3)))
        return false;

    /* θ1 (anti‑horário) */
    const float t1_deg = std::atan2(p.y, p.x) * R2D;

    /* θ3 (cotovelo para cima) */
    const float cphi   = clamp((sq(d.L2)+sq(d.L3)-d2) / (2*d.L2*d.L3));
    const float phi    = std::acos(cphi);                /* 0…π */
    const float t3_rad =  M_PI - phi;                    /* rad */
    const float t3_deg =  t3_rad * R2D;                  /* deg */

    /* θ2 físico */
    const float k1 = d.L2 + d.L3*std::cos(t3_rad);
    const float k2 = d.L3*std::sin(t3_rad);
    const float t2_phys = std::atan2(zp, r) - std::atan2(k2, k1);
    const float t2_deg  =  t2_phys * R2D - 90.f;         /* relativo */

    solUp = { t1_deg, t2_deg, t3_deg };

    if (!checkLimits)  return true;

    /* Limites de junta (graus) */
    const bool ok =
        solUp.t1 >= -150.f && solUp.t1 <= 150.f &&
        solUp.t2 >=  -90.f && solUp.t2 <=  90.f &&
        solUp.t3 >=  -90.f && solUp.t3 <=  90.f;

    return ok;
}

