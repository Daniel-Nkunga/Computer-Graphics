//---------------------------------------
// Program: glass.cpp
// Purpose: Ray trace a cube in front of a glass plane.
// Based on: cube.cpp / glass.cpp by John Gauch
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ray_classes.h"

// Image
#define XDIM 800
#define YDIM 600
unsigned char image[YDIM][XDIM][3];

// Camera orbit state
float theta = 0.0f;     // horizontal angle (around Y)
float theta_y = 0.2f;   // vertical   angle (elevation)
float cam_dist = 10.0f; // distance from origin
float step = 0.05f;     // angle step per key press

string mode = "phong";

//---------------------------------------
// Shade a hit point with Phong
//---------------------------------------
ColorRGB shade_point(Point3D &hit, Vector3D &norm, ColorRGB &obj_color,
                     float ka, float kd, float ks, float kp, Point3D &camera) {
  ColorRGB light_color;
  light_color.set(255, 255, 255);
  Vector3D light_dir;
  light_dir.set(1.0f, 2.0f, 1.0f);
  light_dir.normalize();

  Phong phong;
  phong.SetCamera(camera);
  phong.SetLight(light_color, light_dir);
  phong.SetObject(obj_color, ka, kd, ks, kp);

  ColorRGB result;
  phong.GetShade(hit, norm, result);
  return result;
}

//---------------------------------------
// Cast a ray - hits cube or returns black
//---------------------------------------
ColorRGB cast_ray_scene(Ray3D ray, Point3D camera, Cube3D &cube) {
  Point3D hit;
  Vector3D norm;
  if (cube.get_intersection(ray, hit, norm)) {
    if (mode == "normal") {
      ColorRGB c;
      c.set(127 + norm.vx * 127, 127 + norm.vy * 127, 127 + norm.vz * 127);
      return c;
    }
    ColorRGB obj_color;
    obj_color.set(0, 100, 200);
    return shade_point(hit, norm, obj_color, 0.2f, 0.5f, 0.5f, 20.0f, camera);
  }
  // Black background
  ColorRGB black;
  black.set(0, 0, 0);
  return black;
}

//---------------------------------------
// Ray trace the scene
//---------------------------------------
void ray_trace() {
  // Camera position from spherical coords
  Point3D camera;
  camera.set(cam_dist * cos(theta_y) * sin(theta), cam_dist * sin(theta_y),
             cam_dist * cos(theta_y) * cos(theta));

  // Build camera frame
  Vector3D forward;
  forward.set(-camera.px, -camera.py, -camera.pz);
  forward.normalize();

  Vector3D world_up;
  world_up.set(0, 1, 0);

  Vector3D right;
  right.set(forward.vy * world_up.vz - forward.vz * world_up.vy,
            forward.vz * world_up.vx - forward.vx * world_up.vz,
            forward.vx * world_up.vy - forward.vy * world_up.vx);
  right.normalize();

  Vector3D cam_up;
  cam_up.set(right.vy * forward.vz - right.vz * forward.vy,
             right.vz * forward.vx - right.vx * forward.vz,
             right.vx * forward.vy - right.vy * forward.vx);
  cam_up.normalize();

  const float PLANE_DIST = 1.5f;
  const float aspect = float(XDIM) / float(YDIM);

  // Blue cube in front of the glass (z = 2.0)
  Cube3D cube;
  {
    Point3D c;
    c.set(0.0f, 0.0f, 2.0f);
    cube.set(c, 1.0f);
  }

  // Glass slab: sits at z=0, normal +Z, thickness 0.3
  Glass3D glass;
  {
    Point3D gc;
    gc.set(0.0f, 0.0f, 0.0f);
    Vector3D gn;
    gn.set(0.0f, 0.0f, 1.0f);
    ColorRGB gt;
    gt.set(200, 235, 255); // faint blue tint
    glass.set(gc, gn, 0.3f, 1.5f, gt, 0.08f);
  }

  for (int y = 0; y < YDIM; y++)
    for (int x = 0; x < XDIM; x++) {
      image[y][x][0] = image[y][x][1] = image[y][x][2] = 0;

      float u = ((x + 0.5f) / XDIM * 2.0f - 1.0f) * aspect;
      float v = -((y + 0.5f) / YDIM * 2.0f - 1.0f);

      Point3D vp;
      vp.set(camera.px + forward.vx * PLANE_DIST + right.vx * u + cam_up.vx * v,
             camera.py + forward.vy * PLANE_DIST + right.vy * u + cam_up.vy * v,
             camera.pz + forward.vz * PLANE_DIST + right.vz * u +
                 cam_up.vz * v);

      Ray3D ray;
      ray.set(camera, vp);

      // --- Glass intersection ---
      Point3D glass_hit;
      Vector3D glass_norm;
      bool hit_glass = glass.get_intersection(ray, glass_hit, glass_norm);

      ColorRGB pixel;

      if (!hit_glass) {
        // No glass hit: just render cube (or black)
        pixel = cast_ray_scene(ray, camera, cube);
      } else {
        // Is the cube CLOSER than the glass?
        Point3D ch;
        Vector3D cn;
        bool hit_cube = cube.get_intersection(ray, ch, cn);
        float glass_t = camera.distance(glass_hit);
        float cube_t = hit_cube ? camera.distance(ch) : 1e30f;

        if (hit_cube && cube_t < glass_t) {
          // Cube is in front of glass - shade normally
          if (mode == "normal")
            pixel.set(127 + cn.vx * 127, 127 + cn.vy * 127, 127 + cn.vz * 127);
          else {
            ColorRGB obj;
            obj.set(0, 100, 200);
            pixel = shade_point(ch, cn, obj, 0.2f, 0.5f, 0.5f, 20.0f, camera);
          }
        } else {
          // Ray hits glass - refract/reflect
          if (mode == "normal") {
            pixel.set(127 + glass_norm.vx * 127, 127 + glass_norm.vy * 127,
                      127 + glass_norm.vz * 127);
          } else {
            // Refracted ray (sees cube behind glass)
            Point3D exit_pt;
            Ray3D refr_ray;
            bool refracted =
                glass.get_refracted_ray(ray, glass_hit, exit_pt, refr_ray);

            float fr = glass.fresnel(ray);

            // Reflected ray
            float rdotn = ray.dir.dot(glass_norm);
            Vector3D rdir;
            rdir.set(ray.dir.vx - 2 * rdotn * glass_norm.vx,
                     ray.dir.vy - 2 * rdotn * glass_norm.vy,
                     ray.dir.vz - 2 * rdotn * glass_norm.vz);
            Ray3D refl_ray;
            refl_ray.set(glass_hit, rdir);
            ColorRGB refl_color = cast_ray_scene(refl_ray, glass_hit, cube);

            ColorRGB trans_color;
            if (!refracted) {
              fr = 1.0f;
              trans_color.set(0, 0, 0);
            } else {
              trans_color = cast_ray_scene(refr_ray, exit_pt, cube);
              trans_color.mult(glass.tint);
            }

            pixel.set(fr * refl_color.R + (1 - fr) * trans_color.R,
                      fr * refl_color.G + (1 - fr) * trans_color.G,
                      fr * refl_color.B + (1 - fr) * trans_color.B);
            pixel.clamp();

            // Glass specular highlight
            ColorRGB white;
            white.set(255, 255, 255);
            ColorRGB spec = shade_point(glass_hit, glass_norm, white, 0.0f,
                                        0.0f, 0.5f, 80.0f, camera);
            pixel.add(spec);
            pixel.clamp();
          }
        }
      }

      image[y][x][0] = (unsigned char)pixel.R;
      image[y][x][1] = (unsigned char)pixel.G;
      image[y][x][2] = (unsigned char)pixel.B;
    }
}

//---------------------------------------
void init() {
  glClearColor(0, 0, 0, 1);
  cout << "Glass Ray Tracer - Controls:\n"
       << "   left/right arrows or x/X - orbit horizontally\n"
       << "   up/down   arrows or y/Y - orbit vertically\n"
       << "   '+' - zoom in\n"
       << "   '-' - zoom out\n"
       << "   'p' - Phong shading + refraction\n"
       << "   'n' - show normals\n"
       << "   'q' - quit\n";
  ray_trace();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
  glFlush();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'q':
    exit(0);
    break;
  case 'x':
    theta -= step;
    break;
  case 'X':
    theta += step;
    break;
  case 'y':
    theta_y -= step;
    break;
  case 'Y':
    theta_y += step;
    break;
  case '+':
    if (cam_dist > 2.0f)
      cam_dist /= 1.1f;
    break;
  case '-':
    if (cam_dist < 30.0f)
      cam_dist *= 1.1f;
    break;
  case 'n':
    mode = "normal";
    break;
  case 'p':
    mode = "phong";
    break;
  }
  if (theta_y > 1.4f)
    theta_y = 1.4f;
  if (theta_y < -1.4f)
    theta_y = -1.4f;
  ray_trace();
  glutPostRedisplay();
}

void special(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_LEFT:
    theta -= step;
    break;
  case GLUT_KEY_RIGHT:
    theta += step;
    break;
  case GLUT_KEY_UP:
    theta_y += step;
    break;
  case GLUT_KEY_DOWN:
    theta_y -= step;
    break;
  }
  if (theta_y > 1.4f)
    theta_y = 1.4f;
  if (theta_y < -1.4f)
    theta_y = -1.4f;
  ray_trace();
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitWindowSize(XDIM, YDIM);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Ray Trace - Glass Scene");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMainLoop();
  return 0;
}