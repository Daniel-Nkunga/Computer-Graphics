//----------------------------------------------
// Program: ray_classes.h
// Purpose: Definition of several C++ classes
//          to support ray tracing application.
//
//          ColorRGB   - store RGB color values.
//          Point3D    - store XYZ point coordinates.
//          Vector3D   - store XYZ vector coordinates.
//          Ray3D      - store ray (point and vector).
//          Sphere3D   - store sphere (point and radius).
//          Cone3D     - store cone (apex, axis, half-angle, height).
//          Cube3D     - store axis-aligned cube (center, half-size).
//          Cylinder3D - store cylinder (center, axis, radius, height).
//          Phong      - implement Phong shading.
//
//    These classes have no private variables so
//    they should be used with care (since there
//    is no data hiding provided).
//
// Author:  John Gauch
// Date:    Spring 2019
// Updated: Added Cone3D, Cube3D, Cylinder3D classes
//----------------------------------------------
#include <iostream>
#include <cmath>
using namespace std;

#ifndef _RAY_CLASSES_
#define _RAY_CLASSES_

const bool DEBUG = false;

//----------------------------------------------
class ColorRGB
{
   public:
      float R, G, B;

      void set(float r, float g, float b);
      string print();
      void mult(float c);
      void mult(ColorRGB c);
      void add(ColorRGB c);
      void sub(ColorRGB c);
      void clamp();
};

//----------------------------------------------
class Point3D
{
   public:
      float px, py, pz;

      void set(float x, float y, float z);
      string print();
      float distance(Point3D p);
};

//----------------------------------------------
class Vector3D
{
   public:
      float vx, vy, vz;

      void set(float x, float y, float z);
      string print();
      void normalize();
      float dot(Vector3D v);
      void mult(float c);
      void add(Vector3D v);
      void sub(Vector3D v);
};

//----------------------------------------------
class Ray3D
{
   public:
      Point3D point;
      Vector3D dir;

      void set(Point3D p, Vector3D d);
      void set(Point3D p1, Point3D p2);
      string print();
      Point3D get_sample(float t);
};

//----------------------------------------------
class Sphere3D
{
   public:
      Point3D center;
      Vector3D motion;
      float radius;

      void set(Point3D p, float r);
      void set(Point3D p, Vector3D m, float r);
      string print();
      bool get_intersection(Ray3D ray, Point3D &point, Vector3D &normal);
};

//----------------------------------------------
// Cone3D: A cone clipped to [0, height] along the axis.
//   apex       - the tip of the cone
//   axis       - unit vector pointing along the cone's central axis
//   half_angle - half-angle (radians) between axis and surface
//   height     - how far along the axis the cone extends
//----------------------------------------------
class Cone3D
{
   public:
      Point3D  apex;
      Vector3D axis;
      float    half_angle;
      float    height;

      void set(Point3D a, Vector3D ax, float angle, float h);
      string print();
      bool get_intersection(Ray3D ray, Point3D &point, Vector3D &normal);
};

//----------------------------------------------
// Cube3D: An axis-aligned box defined by center and half-size.
//   center    - center of the cube
//   half_size - half the side length (cube extends +/- half_size on each axis)
//----------------------------------------------
class Cube3D
{
   public:
      Point3D center;
      float   half_size;

      void set(Point3D c, float hs);
      string print();
      bool get_intersection(Ray3D ray, Point3D &point, Vector3D &normal);
};

//----------------------------------------------
// Cylinder3D: A capped cylinder defined by center, axis, radius, and height.
//   center - center of the cylinder (midpoint of axis)
//   axis   - unit vector along the cylinder's central axis
//   radius - radius of the cylinder
//   height - total height (cylinder extends +/- height/2 along axis)
//----------------------------------------------
class Cylinder3D
{
   public:
      Point3D  center;
      Vector3D axis;
      float    radius;
      float    height;

      void set(Point3D c, Vector3D ax, float r, float h);
      string print();
      bool get_intersection(Ray3D ray, Point3D &point, Vector3D &normal);
};

//----------------------------------------------
class Phong
{
 public:
   Phong();
   ~Phong();

   void SetCamera(Point3D pos);
   void SetLight(ColorRGB color, Vector3D dir);
   void SetObject(ColorRGB color, float ka, float kd, float ks, float kp);
   void GetShade(Point3D point, Vector3D normal, ColorRGB &color);

 private:
   Point3D  CameraPos;
   ColorRGB LightColor;
   Vector3D LightDir;
   ColorRGB ObjectColor;
   float Ka, Kd, Ks, Kp;
};

#endif