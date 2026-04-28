//----------------------------------------------
// Program: ray_classes.cpp
// Purpose: Implementation of several C++ classes
//          to support ray tracing application.
//
//          ColorRGB - store RGB color values.
//          Point3D - store XYZ point coordinates.
//          Vector3D - store XYZ vector coordinates.
//          Ray3D - store ray (point and vector).
//          Sphere3D - store sphere (point and radius).
//          Phong - implement Phong shading.
//
//    These classes have no private variables so
//    they should be used with care (since there
//    is no data hiding provided).
//
// Author:  John Gauch
// Date:    Spring 2019
//----------------------------------------------
#include <ray_classes.h>

void ColorRGB::set(float r, float g, float b) {
  R = r;
  G = g;
  B = b;
  clamp();
}

//----------------------------------------------
string ColorRGB::print() {
  cout << "(" << int(R) << "," << int(G) << "," << int(B) << ")";
  return "";
}

//----------------------------------------------
void ColorRGB::mult(float c) {
  R *= c;
  G *= c;
  B *= c;
  clamp();
}

//----------------------------------------------
void ColorRGB::mult(ColorRGB c) {
  R *= c.R / 255;
  G *= c.G / 255;
  B *= c.B / 255;
  clamp();
}

//----------------------------------------------
void ColorRGB::add(ColorRGB c) {
  R += c.R;
  G += c.G;
  B += c.B;
  clamp();
}

//----------------------------------------------
void ColorRGB::sub(ColorRGB c) {
  R -= c.R;
  G -= c.G;
  B -= c.B;
  clamp();
}

//----------------------------------------------
void ColorRGB::clamp() {
  if (R < 0)
    R = 0;
  if (G < 0)
    G = 0;
  if (B < 0)
    B = 0;
  if (R > 255)
    R = 255;
  if (G > 255)
    G = 255;
  if (B > 255)
    B = 255;
}

//----------------------------------------------
void Point3D::set(float x, float y, float z) {
  px = x;
  py = y;
  pz = z;
}

//----------------------------------------------
string Point3D::print() {
  cout << "(" << px << "," << py << "," << pz << ")";
  return "";
}

//----------------------------------------------
float Point3D::distance(Point3D p) {
  float dx = px - p.px;
  float dy = py - p.py;
  float dz = pz - p.pz;
  return sqrt(dx * dx + dy * dy + dz * dz);
}

//----------------------------------------------
void Vector3D::set(float x, float y, float z) {
  vx = x;
  vy = y;
  vz = z;
}

//----------------------------------------------
string Vector3D::print() {
  cout << "(" << vx << "," << vy << "," << vz << ")";
  return "";
}

//----------------------------------------------
void Vector3D::normalize() {
  float length = sqrt(vx * vx + vy * vy + vz * vz);
  vx /= length;
  vy /= length;
  vz /= length;
}

//----------------------------------------------
float Vector3D::dot(Vector3D v) { return vx * v.vx + vy * v.vy + vz * v.vz; }

//----------------------------------------------
void Vector3D::mult(float c) {
  vx *= c;
  vy *= c;
  vz *= c;
}

//----------------------------------------------
void Vector3D::add(Vector3D v) {
  vx += v.vx;
  vy += v.vy;
  vz += v.vz;
}

//----------------------------------------------
void Vector3D::sub(Vector3D v) {
  vx -= v.vx;
  vy -= v.vy;
  vz -= v.vz;
}

//----------------------------------------------
void Ray3D::set(Point3D p, Vector3D d) {
  point = p;
  dir = d;
  dir.normalize();
}

//----------------------------------------------
void Ray3D::set(Point3D p1, Point3D p2) {
  point = p1;
  dir.set(p2.px - p1.px, p2.py - p1.py, p2.pz - p1.pz);
  dir.normalize();
}

//----------------------------------------------
string Ray3D::print() {
  point.print();
  cout << " ";
  dir.print();
  return "";
}

//----------------------------------------------
Point3D Ray3D::get_sample(float t) {
  Point3D result;
  result.set(point.px + t * dir.vx, point.py + t * dir.vy,
             point.pz + t * dir.vz);
  return result;
}

//----------------------------------------------
void Sphere3D::set(Point3D p, float r) {
  center = p;
  radius = r;
}

//----------------------------------------------
void Sphere3D::set(Point3D p, Vector3D m, float r) {
  center = p;
  motion = m;
  radius = r;
}

//----------------------------------------------
string Sphere3D::print() {
  center.print();
  cout << " ";
  motion.print();
  cout << " " << radius << endl;
  return "";
}

//----------------------------------------------
bool Sphere3D::get_intersection(Ray3D ray, Point3D &point, Vector3D &normal) {
  // Define oc vector
  Point3D g = center;
  Point3D p = ray.point;
  Vector3D oc;
  oc.set(p.px - g.px, p.py - g.py, p.pz - g.pz);

  // Calculate quadratic equation
  float A = ray.dir.dot(ray.dir);
  float B = 2 * oc.dot(ray.dir);
  float C = oc.dot(oc) - radius * radius;

  // Solve quadratic equation for intersection points
  float discriminant = B * B - 4 * A * C;
  if (discriminant >= 0) {
    // Calculate two roots
    float root1 = (-B - sqrt(discriminant)) / 2 * A;
    float root2 = (-B + sqrt(discriminant)) / 2 * A;
    float solution = 0;

    // No positive roots found
    if ((root1 < 0) && (root2 < 0))
      return false;

    // One positive root
    else if ((root1 < 0) && (root2 >= 0))
      solution = root2;

    // One positive root
    else if ((root2 < 0) && (root1 >= 0))
      solution = root1;

    // Two positive roots
    else if (root1 <= root2)
      solution = root1;

    // Two positive roots
    else if (root2 <= root1)
      solution = root2;

    // Get intersection point
    point = ray.get_sample(solution);

    // Get surface normal
    normal.set(point.px - center.px, point.py - center.py,
               point.pz - center.pz);
    normal.normalize();
    return true;
  }
  return false;
}

//----------------------------------------------
void Cone3D::set(Point3D a, Vector3D ax, float angle, float h) {
  apex = a;
  axis = ax;
  axis.normalize();
  half_angle = angle;
  height = h;
}

//----------------------------------------------
string Cone3D::print() {
  cout << "apex=";
  apex.print();
  cout << " axis=";
  axis.print();
  cout << " half_angle=" << half_angle << " height=" << height << endl;
  return "";
}

//----------------------------------------------
// Cone3D::get_intersection
//
// Derivation:
//   Let k  = cos^2(half_angle)  (a scalar precomputed once)
//   Let oc = ray.point - apex   (vector from apex to ray origin)
//   Let d  = ray.dir            (unit ray direction)
//
//   A point X = ray.origin + t*d lies on the infinite cone surface when:
//     dot(X - apex, axis)^2 = k * dot(X - apex, X - apex)
//
//   Expanding with P(t) = oc + t*d:
//     dot(P, axis)^2 = k * dot(P, P)
//
//   Let:
//     dA  = dot(d,  axis)
//     ocA = dot(oc, axis)
//
//   Then:
//     A_coef = dA^2  - k * dot(d,  d)
//     B_coef = 2*(dA*ocA - k * dot(d, oc))
//     C_coef = ocA^2 - k * dot(oc, oc)
//
//   Solve quadratic; accept roots where:
//     (a) t > 0  (in front of ray origin)
//     (b) 0 <= dot(hit - apex, axis) <= height  (within cone height)
//
//   The outward surface normal at a hit point P is:
//     N = (P - apex) - axis * dot(P - apex, axis) / cos^2(half_angle)
//   (gradient of the cone implicit function), then normalized.
//----------------------------------------------
bool Cone3D::get_intersection(Ray3D ray, Point3D &point, Vector3D &normal) {
  // Precompute k = cos^2(half_angle)
  float cosA = cos(half_angle);
  float k = cosA * cosA;

  // oc = ray origin - apex
  Vector3D oc;
  oc.set(ray.point.px - apex.px, ray.point.py - apex.py,
         ray.point.pz - apex.pz);

  // Dot products needed for quadratic coefficients
  float dA = ray.dir.dot(axis);
  float ocA = oc.dot(axis);
  float dd = ray.dir.dot(ray.dir); // 1.0 if dir is normalized
  float doc = ray.dir.dot(oc);
  float ooc = oc.dot(oc);

  // Quadratic A, B, C
  float A = dA * dA - k * dd;
  float B = 2.0f * (dA * ocA - k * doc);
  float C = ocA * ocA - k * ooc;

  float discriminant = B * B - 4.0f * A * C;
  if (discriminant < 0)
    return false;

  float sqrtDisc = sqrt(discriminant);
  float root1 = (-B - sqrtDisc) / (2.0f * A);
  float root2 = (-B + sqrtDisc) / (2.0f * A);

  // Helper lambda: check whether t gives a valid (in-height) cone hit
  // Returns the along-axis distance so caller can range-check it.
  auto valid_hit = [&](float t, float &along) -> bool {
    if (t < 1e-4f)
      return false; // behind or at ray origin
    Point3D hp = ray.get_sample(t);
    Vector3D ap;
    ap.set(hp.px - apex.px, hp.py - apex.py, hp.pz - apex.pz);
    along = ap.dot(axis);
    return (along >= 0.0f && along <= height);
  };

  // Pick the smallest valid t
  float solution = -1.0f;
  float along = 0.0f;
  float a1, a2;
  bool ok1 = valid_hit(root1, a1);
  bool ok2 = valid_hit(root2, a2);

  if (ok1 && ok2) {
    solution = (root1 <= root2) ? root1 : root2;
    along = (root1 <= root2) ? a1 : a2;
  } else if (ok1) {
    solution = root1;
    along = a1;
  } else if (ok2) {
    solution = root2;
    along = a2;
  } else {
    return false;
  }

  // Compute intersection point
  point = ray.get_sample(solution);

  // Compute outward normal
  // N = (point - apex) - axis * along / k
  // Then normalize.
  Vector3D ap;
  ap.set(point.px - apex.px, point.py - apex.py, point.pz - apex.pz);

  Vector3D axisScaled = axis;
  axisScaled.mult(along / k);

  normal = ap;
  normal.sub(axisScaled);

  // Guard: at the apex the normal length collapses to zero (degenerate point).
  // Return false so callers treat it as a miss rather than getting NaN normals.
  float len = sqrt(normal.vx * normal.vx + normal.vy * normal.vy +
                   normal.vz * normal.vz);
  if (len < 1e-6f)
    return false;

  normal.normalize();

  return true;
}

//----------------------------------------------
// Cube3D::set
//----------------------------------------------
void Cube3D::set(Point3D c, float hs) {
  center = c;
  half_size = hs;
}

//----------------------------------------------
string Cube3D::print() {
  cout << "center=";
  center.print();
  cout << " half_size=" << half_size << endl;
  return "";
}

//----------------------------------------------
// Cube3D::get_intersection
//
// Uses the slab method: intersect the ray with the three pairs of
// axis-aligned planes (x, y, z slabs) that bound the box.
// The ray hits the box when all three slab intervals overlap.
// The entry normal is the face whose tmin is largest.
//----------------------------------------------
bool Cube3D::get_intersection(Ray3D ray, Point3D &point, Vector3D &normal) {
  float ox = ray.point.px, oy = ray.point.py, oz = ray.point.pz;
  float dx = ray.dir.vx, dy = ray.dir.vy, dz = ray.dir.vz;

  float minx = center.px - half_size, maxx = center.px + half_size;
  float miny = center.py - half_size, maxy = center.py + half_size;
  float minz = center.pz - half_size, maxz = center.pz + half_size;

  float tmin = -1e30f, tmax = 1e30f;
  int normal_axis = 0;
  float normal_sign = 1.0f;

  // X slab
  if (fabs(dx) > 1e-8f) {
    float t1 = (minx - ox) / dx;
    float t2 = (maxx - ox) / dx;
    float sign = -1.0f;
    if (t1 > t2) {
      float tmp = t1;
      t1 = t2;
      t2 = tmp;
      sign = 1.0f;
    }
    if (t1 > tmin) {
      tmin = t1;
      normal_axis = 0;
      normal_sign = sign;
    }
    if (t2 < tmax)
      tmax = t2;
  } else if (ox < minx || ox > maxx)
    return false;

  // Y slab
  if (fabs(dy) > 1e-8f) {
    float t1 = (miny - oy) / dy;
    float t2 = (maxy - oy) / dy;
    float sign = -1.0f;
    if (t1 > t2) {
      float tmp = t1;
      t1 = t2;
      t2 = tmp;
      sign = 1.0f;
    }
    if (t1 > tmin) {
      tmin = t1;
      normal_axis = 1;
      normal_sign = sign;
    }
    if (t2 < tmax)
      tmax = t2;
  } else if (oy < miny || oy > maxy)
    return false;

  // Z slab
  if (fabs(dz) > 1e-8f) {
    float t1 = (minz - oz) / dz;
    float t2 = (maxz - oz) / dz;
    float sign = -1.0f;
    if (t1 > t2) {
      float tmp = t1;
      t1 = t2;
      t2 = tmp;
      sign = 1.0f;
    }
    if (t1 > tmin) {
      tmin = t1;
      normal_axis = 2;
      normal_sign = sign;
    }
    if (t2 < tmax)
      tmax = t2;
  } else if (oz < minz || oz > maxz)
    return false;

  if (tmax < tmin || tmax < 1e-4f)
    return false;
  float t = (tmin > 1e-4f) ? tmin : tmax;
  if (t < 1e-4f)
    return false;

  point = ray.get_sample(t);

  if (normal_axis == 0)
    normal.set(normal_sign, 0, 0);
  else if (normal_axis == 1)
    normal.set(0, normal_sign, 0);
  else
    normal.set(0, 0, normal_sign);

  return true;
}

//----------------------------------------------
// Cylinder3D::set
//----------------------------------------------
void Cylinder3D::set(Point3D c, Vector3D ax, float r, float h) {
  center = c;
  axis = ax;
  axis.normalize();
  radius = r;
  height = h;
}

//----------------------------------------------
string Cylinder3D::print() {
  cout << "center=";
  center.print();
  cout << " axis=";
  axis.print();
  cout << " radius=" << radius << " height=" << height << endl;
  return "";
}

//----------------------------------------------
// Cylinder3D::get_intersection
//
// An infinite cylinder around 'axis' through 'center' satisfies:
//   |oc - dot(oc,axis)*axis|^2 = radius^2
// where oc = P - center.
//
// Expanding P = ray.origin + t*ray.dir gives a quadratic in t.
// We then check both roots against the height bounds [-h/2, +h/2]
// along the axis, and also test the two flat end caps.
//----------------------------------------------
bool Cylinder3D::get_intersection(Ray3D ray, Point3D &point, Vector3D &normal) {
  // oc = ray origin - cylinder center
  Vector3D oc;
  oc.set(ray.point.px - center.px, ray.point.py - center.py,
         ray.point.pz - center.pz);

  float dA = ray.dir.dot(axis);
  float ocA = oc.dot(axis);

  // Components perpendicular to axis
  Vector3D dPerp = ray.dir;
  Vector3D axTmp = axis;
  axTmp.mult(dA);
  dPerp.sub(axTmp);

  Vector3D ocPerp = oc;
  Vector3D axTmp2 = axis;
  axTmp2.mult(ocA);
  ocPerp.sub(axTmp2);

  float A = dPerp.dot(dPerp);
  float B = 2.0f * ocPerp.dot(dPerp);
  float C = ocPerp.dot(ocPerp) - radius * radius;

  float best_t = -1.0f;
  int hit_type = 0; // 1 = side, 2 = bottom cap, 3 = top cap

  // --- Side surface ---
  float disc = B * B - 4.0f * A * C;
  if (fabs(A) > 1e-8f && disc >= 0.0f) {
    float sqrtDisc = sqrt(disc);
    float t1 = (-B - sqrtDisc) / (2.0f * A);
    float t2 = (-B + sqrtDisc) / (2.0f * A);

    for (int i = 0; i < 2; i++) {
      float t = (i == 0) ? t1 : t2;
      if (t < 1e-4f)
        continue;
      Point3D hp = ray.get_sample(t);
      Vector3D cp;
      cp.set(hp.px - center.px, hp.py - center.py, hp.pz - center.pz);
      float along = cp.dot(axis);
      if (fabs(along) <= height * 0.5f) {
        if (best_t < 0 || t < best_t) {
          best_t = t;
          hit_type = 1;
        }
      }
    }
  }

  // --- End caps ---
  // Cap normal direction: axis or -axis
  float half_h = height * 0.5f;
  for (int cap = 0; cap < 2; cap++) {
    // cap==0 -> bottom (-half_h along axis), cap==1 -> top (+half_h)
    float sign = (cap == 0) ? -1.0f : 1.0f;
    float capD = sign * half_h; // signed distance along axis from center

    // Plane: dot(P - center, axis) = capD
    // => dot(ray.origin + t*ray.dir - center, axis) = capD
    // => ocA + t*dA = capD => t = (capD - ocA) / dA
    if (fabs(dA) < 1e-8f)
      continue;
    float t = (capD - ocA) / dA;
    if (t < 1e-4f)
      continue;

    Point3D hp = ray.get_sample(t);
    // Check radius
    Vector3D cp;
    cp.set(hp.px - center.px, hp.py - center.py, hp.pz - center.pz);
    Vector3D axTmpCap = axis;
    axTmpCap.mult(cp.dot(axis));
    cp.sub(axTmpCap);
    if (cp.dot(cp) <= radius * radius) {
      if (best_t < 0 || t < best_t) {
        best_t = t;
        hit_type = (cap == 0) ? 2 : 3;
      }
    }
  }

  if (best_t < 0)
    return false;

  point = ray.get_sample(best_t);

  if (hit_type == 1) {
    // Side normal: radial component from axis
    Vector3D cp;
    cp.set(point.px - center.px, point.py - center.py, point.pz - center.pz);
    Vector3D axProj = axis;
    axProj.mult(cp.dot(axis));
    cp.sub(axProj);
    cp.normalize();
    normal = cp;
  } else if (hit_type == 2) {
    // Bottom cap: normal points opposite to axis
    normal.set(-axis.vx, -axis.vy, -axis.vz);
  } else {
    // Top cap: normal points along axis
    normal = axis;
  }

  return true;
}

//----------------------------------------------
void Glass3D::set(Point3D c, Vector3D n, float thick, float index,
                  ColorRGB color, float refl) {
  center = c;
  normal = n;
  normal.normalize();
  thickness = thick;
  ior = index;
  tint = color;
  reflect_amount = refl;
}

//----------------------------------------------
string Glass3D::print() {
  cout << "Glass center=";
  center.print();
  cout << " normal=";
  normal.print();
  cout << " thickness=" << thickness << " ior=" << ior << endl;
  return "";
}

//----------------------------------------------
// Schlick Fresnel approximation.
// R0 = ((n1-n2)/(n1+n2))^2, here n1=1 (air), n2=ior
// R  = R0 + (1-R0)*(1-cos(theta))^5
float Glass3D::fresnel(Ray3D ray) {
  // cos(theta) = |dot(ray.dir, normal)| (both normalized)
  Vector3D n = normal;
  float cosI = fabs(ray.dir.dot(n));
  float n1 = 1.0f, n2 = ior;
  float r0 = (n1 - n2) / (n1 + n2);
  r0 = r0 * r0;
  float f = r0 + (1.0f - r0) * pow(1.0f - cosI, 5.0f);
  // Blend with artist-controlled reflect_amount
  return reflect_amount + (1.0f - reflect_amount) * f;
}

//----------------------------------------------
// Intersect ray with the front face plane of the slab.
// Front face: dot(P - center, normal) = +thickness/2
// Back  face: dot(P - center, normal) = -thickness/2
bool Glass3D::get_intersection(Ray3D ray, Point3D &point,
                               Vector3D &normal_out) {
  // Plane equation for front face: dot(P - front_center, normal) = 0
  // where front_center is shifted by +thickness/2 along normal
  float half_t = thickness * 0.5f;

  // dot(ray.origin + t*ray.dir - center - half_t*normal, normal) = 0
  // => t = (half_t + dot(center - ray.origin, normal)) / dot(ray.dir, normal)
  float denom = ray.dir.dot(normal);
  if (fabs(denom) < 1e-8f)
    return false; // ray parallel to slab

  Vector3D oc;
  oc.set(center.px - ray.point.px, center.py - ray.point.py,
         center.pz - ray.point.pz);
  float ocN = oc.dot(normal);

  // t for front face (farther from center in normal direction)
  float t_front = (ocN + half_t) / denom;
  // t for back face
  float t_back = (ocN - half_t) / denom;

  // Ensure t_front < t_back (ray may enter from either side)
  if (t_front > t_back) {
    float tmp = t_front;
    t_front = t_back;
    t_back = tmp;
  }

  if (t_front < 1e-4f)
    return false; // slab is behind the ray

  point = ray.get_sample(t_front);

  // Outward normal should face towards ray origin
  if (denom < 0)
    normal_out = normal; // ray entering from front
  else {
    normal_out.set(-normal.vx, -normal.vy,
                   -normal.vz); // ray entering from back
  }
  return true;
}

//----------------------------------------------
// Refract through the slab using Snell's law.
// 1. Refract at entry surface (air -> glass, n1=1, n2=ior)
// 2. March the refracted ray to the exit surface
// 3. Refract again at exit surface (glass -> air, n1=ior, n2=1)
bool Glass3D::get_refracted_ray(Ray3D ray, Point3D entry_point,
                                Point3D &exit_point, Ray3D &refracted) {
  // --- Refraction helper: Snell's law vector form ---
  // n = surface normal pointing into the medium the ray is leaving
  // i = incident ray direction (unit)
  // eta = n1/n2
  // Returns refracted direction, or sets ok=false on TIR
  auto refract = [](Vector3D i, Vector3D n, float eta, Vector3D &t_dir,
                    bool &ok) {
    float cosI = -(i.dot(n));
    float sin2T = eta * eta * (1.0f - cosI * cosI);
    if (sin2T > 1.0f) {
      ok = false;
      return;
    } // total internal reflection
    float cosT = sqrt(1.0f - sin2T);
    // t = eta*i + (eta*cosI - cosT)*n
    t_dir.set(eta * i.vx + (eta * cosI - cosT) * n.vx,
              eta * i.vy + (eta * cosI - cosT) * n.vy,
              eta * i.vz + (eta * cosI - cosT) * n.vz);
    t_dir.normalize();
    ok = true;
  };

  bool ok;
  Vector3D entry_normal;
  // Normal at entry: face towards incoming ray
  float denom = ray.dir.dot(normal);
  if (denom < 0)
    entry_normal = normal; // ray from front
  else
    entry_normal.set(-normal.vx, -normal.vy, -normal.vz); // ray from back

  // Refract entering glass
  Vector3D refr1_dir;
  refract(ray.dir, entry_normal, 1.0f / ior, refr1_dir, ok);
  if (!ok)
    return false;

  // March refracted ray to the back face
  // back face plane: dot(P - center, normal) = -thickness/2  (from front)
  // But use the same-signed normal as entry face's opposite
  float half_t = thickness * 0.5f;
  // Ray inside glass starts at entry_point going in refr1_dir
  Ray3D inside_ray;
  inside_ray.set(entry_point, refr1_dir);

  // Intersect inside_ray with the exit plane
  // exit plane normal points OUT of glass on the far side = -entry_normal
  Vector3D exit_normal;
  exit_normal.set(-entry_normal.vx, -entry_normal.vy, -entry_normal.vz);

  float denom2 = refr1_dir.dot(exit_normal);
  if (fabs(denom2) < 1e-8f)
    return false;

  Vector3D oc2;
  oc2.set(center.px - entry_point.px, center.py - entry_point.py,
          center.pz - entry_point.pz);
  // Exit plane: dot(P - center, -entry_normal) = half_t
  // => dot(entry_point + t*refr1 - center, exit_normal) = half_t
  // => t = (half_t - oc2 . exit_normal) / (refr1 . exit_normal)
  //    Note: oc2 = center - entry_point, so dot(entry_point-center,
  //    exit_normal) = -oc2.exit_normal
  float t_exit = (half_t + oc2.dot(entry_normal)) / denom2;
  if (t_exit < 1e-4f)
    return false;

  exit_point = inside_ray.get_sample(t_exit);

  // Refract exiting glass: n1=ior, n2=1, normal = exit_normal (pointing out)
  // The exit_normal points away from center of slab on far side
  Vector3D refr2_dir;
  refract(refr1_dir, exit_normal, ior / 1.0f, refr2_dir, ok);
  if (!ok)
    return false;

  refracted.set(exit_point, refr2_dir);
  return true;
}

//----------------------------------------------
// Mirror3D::set
//----------------------------------------------
void Mirror3D::set(Point3D c, Vector3D n, Vector3D u, float w, float h) {
  center = c;
  normal = n;
  normal.normalize();
  up = u;
  up.normalize();
  width = w;
  height = h;
}

//----------------------------------------------
string Mirror3D::print() {
  cout << "Mirror center=";
  center.print();
  cout << " normal=";
  normal.print();
  cout << " up=";
  up.print();
  cout << " width=" << width << " height=" << height << endl;
  return "";
}

//----------------------------------------------
// Mirror3D::get_intersection
//
// Step 1: Ray / infinite-plane intersection.
//   The mirror plane satisfies  dot(P - center, normal) = 0.
//   Substituting P = ray.origin + t * ray.dir:
//     t = dot(center - ray.origin, normal) / dot(ray.dir, normal)
//
// Step 2: Finite-rectangle test.
//   Project the hit point onto the two in-plane axes:
//     right = normal x up  (computed on the fly)
//     up    (stored field)
//   The hit is inside the rectangle when:
//     |dot(offset, right)| <= width/2   AND
//     |dot(offset, up)|    <= height/2
//
// Step 3: Orient the returned normal toward the incoming ray so Phong
//   shading receives a consistent facing normal regardless of which side
//   of the mirror the ray comes from.
//----------------------------------------------
bool Mirror3D::get_intersection(Ray3D ray, Point3D &point,
                                Vector3D &normal_out) {
  // Step 1: plane intersection
  float denom = ray.dir.dot(normal);
  if (fabs(denom) < 1e-8f)
    return false; // ray parallel to mirror plane

  Vector3D oc;
  oc.set(center.px - ray.point.px, center.py - ray.point.py,
         center.pz - ray.point.pz);

  float t = oc.dot(normal) / denom;
  if (t < 1e-4f)
    return false; // intersection is behind ray origin

  // Step 2: finite-rectangle test
  point = ray.get_sample(t);

  Vector3D offset;
  offset.set(point.px - center.px, point.py - center.py, point.pz - center.pz);

  // "right" axis = normal cross up (gives the horizontal in-plane direction)
  Vector3D right;
  right.set(normal.vy * up.vz - normal.vz * up.vy,
            normal.vz * up.vx - normal.vx * up.vz,
            normal.vx * up.vy - normal.vy * up.vx);
  right.normalize();

  float u_coord = offset.dot(right);
  float v_coord = offset.dot(up);

  if (fabs(u_coord) > width * 0.5f)
    return false;
  if (fabs(v_coord) > height * 0.5f)
    return false;

  // Step 3: orient normal toward the ray origin
  if (denom < 0.0f)
    normal_out = normal; // ray from the front face
  else
    normal_out.set(-normal.vx, -normal.vy, -normal.vz); // ray from back

  return true;
}

//----------------------------------------------
// Mirror3D::get_reflected_ray
//
// Perfect specular reflection using the standard formula:
//   r = d - 2 * dot(d, n) * n
// where d is the (unit) incoming direction and n is the outward normal
// oriented toward the incoming ray.
//
// The reflected ray originates at hit_point (offset slightly along the
// normal to avoid self-intersection) and travels in direction r.
//----------------------------------------------
void Mirror3D::get_reflected_ray(Ray3D ray, Point3D hit_point,
                                 Vector3D hit_normal, Ray3D &reflected) {
  // Compute reflection direction:  r = d - 2*(d·n)*n
  float d_dot_n = ray.dir.dot(hit_normal);
  Vector3D r_dir;
  r_dir.set(ray.dir.vx - 2.0f * d_dot_n * hit_normal.vx,
            ray.dir.vy - 2.0f * d_dot_n * hit_normal.vy,
            ray.dir.vz - 2.0f * d_dot_n * hit_normal.vz);
  r_dir.normalize();

  // Offset origin slightly along the normal to prevent self-intersection
  const float EPSILON = 1e-3f;
  Point3D origin;
  origin.set(hit_point.px + EPSILON * hit_normal.vx,
             hit_point.py + EPSILON * hit_normal.vy,
             hit_point.pz + EPSILON * hit_normal.vz);

  reflected.set(origin, r_dir);
}

//----------------------------------------------
Phong::Phong() {
  CameraPos.set(0, 0, 0);
  LightColor.set(0, 0, 0);
  LightDir.set(0, 0, 0);
  ObjectColor.set(0, 0, 0);
  Ka = 0;
  Kd = 0;
  Ks = 0;
  Kp = 0;
}

//----------------------------------------------
Phong::~Phong() {}

//----------------------------------------------
void Phong::SetCamera(Point3D pos) {
  CameraPos = pos;
  if (DEBUG)
    cout << CameraPos.print() << endl;
}

//----------------------------------------------
void Phong::SetLight(ColorRGB color, Vector3D dir) {
  LightColor = color;
  if (DEBUG)
    cout << LightColor.print() << endl;
  LightDir = dir;
  LightDir.normalize();
  if (DEBUG)
    cout << LightDir.print() << endl;
}

//----------------------------------------------
void Phong::SetObject(ColorRGB color, float ka, float kd, float ks, float kp) {
  ObjectColor = color;
  if (DEBUG)
    cout << ObjectColor.print() << endl;
  Ka = ka;
  Kd = kd;
  Ks = ks;
  Kp = kp;
}

//----------------------------------------------
void Phong::GetShade(Point3D point, Vector3D normal, ColorRGB &color) {
  // Normalize surface normal
  normal.normalize();

  // Calculate ambient term
  ColorRGB ambient;
  ambient = LightColor;
  ambient.mult(ObjectColor);
  ambient.mult(Ka);
  if (DEBUG)
    cout << "ambient " << ambient.print() << endl;
  color = ambient;

  // Calculate NdotL value
  float NdotL = normal.dot(LightDir);
  if (DEBUG)
    cout << "NdotL " << NdotL << endl;
  if (NdotL > 0) {
    // Calculate diffuse term
    ColorRGB diffuse;
    diffuse = LightColor;
    diffuse.mult(ObjectColor);
    diffuse.mult(Kd * NdotL);
    if (DEBUG)
      cout << "diffuse " << diffuse.print() << endl;
    color.add(diffuse);

    // Calculate R vector
    Vector3D R = normal;
    R.mult(2 * NdotL);
    R.sub(LightDir);
    R.normalize();

    // Calculate V vector
    Vector3D V;
    float Vx = CameraPos.px - point.px;
    float Vy = CameraPos.py - point.py;
    float Vz = CameraPos.pz - point.pz;
    V.set(Vx, Vy, Vz);
    V.normalize();

    // Calculate RdotV value
    float RdotV = R.dot(V);
    if (DEBUG)
      cout << "RdotV " << RdotV << endl;
    if (RdotV > 0) {
      // Calculate specular term
      RdotV = pow(RdotV, Kp);
      ColorRGB specular;
      specular = LightColor;
      specular.mult(Ks * RdotV);
      if (DEBUG)
        cout << "specular " << specular.print() << endl;
      color.add(specular);
    }
  }
}

//----------------------------------------------
int test_main() {
  cout << "\nTest point class\n";
  Point3D p1;
  p1.set(1, 2, 3);
  cout << "p1 = ";
  p1.print();
  cout << endl;

  Point3D p2;
  p2.set(3, 4, 5);
  cout << "p2 = ";
  p2.print();
  cout << endl;

  float dist12 = p1.distance(p2);
  float dist21 = p2.distance(p1);
  cout << "dist12 = " << dist12 << endl;
  cout << "dist21 = " << dist21 << endl;

  cout << "\nTest vector class\n";
  Vector3D v1;
  v1.set(2, 1, 0);
  cout << "v1 = ";
  v1.print();
  cout << endl;

  Vector3D v2;
  v2.set(-1, 1, 0);
  cout << "v2 = ";
  v2.print();
  cout << endl;

  v2.normalize();
  cout << "v2 = ";
  v2.print();
  cout << endl;

  float v1dotv1 = v1.dot(v1);
  float v1dotv2 = v1.dot(v2);
  float v2dotv2 = v2.dot(v2);
  cout << "v1dotv1 = " << v1dotv1 << endl;
  cout << "v1dotv2 = " << v1dotv2 << endl;
  cout << "v2dotv2 = " << v2dotv2 << endl;

  cout << "\nTest ray class\n";
  Ray3D r1;
  r1.set(p1, v1);
  cout << "r1(p1,v1) = ";
  r1.print();
  cout << endl;

  Ray3D r2;
  r2.set(p2, p1);
  cout << "r2(p2,p1) = ";
  r2.print();
  cout << endl;

  for (int i = 0; i <= 10; i++) {
    cout << "r1(" << i / 10.0 << ") = ";
    r1.get_sample(i / 10.0).print();
    cout << endl;
  }

  cout << "\nTest sphere class\n";
  Sphere3D sphere;
  Point3D center;
  center.set(0, 0, 0);
  sphere.set(center, 2);
  cout << "sphere = ";
  sphere.print();
  cout << endl;

  for (int px = -3; px <= 3; px++) {
    Point3D point;
    point.set(px, 0, 0);
    Vector3D dir;
    dir.set(1, 0, 0);
    Ray3D ray;
    ray.set(point, dir);
    cout << "ray = ";
    ray.print();
    cout << endl;

    Point3D p;
    Vector3D n;
    if (!sphere.get_intersection(ray, p, n))
      cout << "no intersection\n";
    else {
      cout << "point = ";
      p.print();
      cout << endl;
      cout << "normal = ";
      n.print();
      cout << endl;
    }
  }

  cout << "\nTest phong class\n";
  Phong shader;
  string name;
  while (cin >> name) {
    if (name == "Camera") {
      // Read and save camera information
      Point3D pos;
      cin >> pos.px >> pos.py >> pos.pz;
      shader.SetCamera(pos);
    }

    else if (name == "Light") {
      // Read and save light information
      ColorRGB color;
      Vector3D dir;
      cin >> color.R >> color.G >> color.B;
      cin >> dir.vx >> dir.vy >> dir.vz;
      shader.SetLight(color, dir);
    }

    else if (name == "Object") {
      // Read and save object information
      ColorRGB color;
      float ka, kd, ks, kp;
      cin >> color.R >> color.G >> color.B;
      cin >> ka >> kd >> ks >> kp;
      shader.SetObject(color, ka, kd, ks, kp);
    }

    else if (name == "Point") {
      // Read and use point information
      Point3D point;
      Vector3D normal;
      ColorRGB color;
      cin >> point.px >> point.py >> point.pz;
      cin >> normal.vx >> normal.vy >> normal.vz;
      shader.GetShade(point, normal, color);
      point.print();
      cout << " ";
      color.print();
      cout << endl;
    }

    else {
      cout << "Ignoring '" << name << "' token\n";
    }
  }

  return 0;
}