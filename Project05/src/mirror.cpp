#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ray_classes.h"

#define XDIM 800
#define YDIM 800

unsigned char image[YDIM][XDIM][3];

// Camera varaibles
float theta = 0.0f;
float theta_y = 0.3f;
float cam_dist = 6.0f;
float step = 0.05f;

// Rendering variables
string mode = "phong";
int max_depth = 3;

// Scene objects
Sphere3D sphere_red;
Sphere3D sphere_blue;
Sphere3D sphere_green;

//  Floor slab - not used
// Cube3D floor_slab;

Mirror3D mirror_left;
Mirror3D mirror_right;

// Phong shaders – one per material
Phong shader_red;
Phong shader_blue;
Phong shader_green;
Phong shader_floor;

// trace the ray from the scene
ColorRGB trace_ray(Ray3D ray, int depth) {
  ColorRGB black;
  black.set(0, 0, 0);

  // Find closest intersection
  float best_t = 1e30f;
  int hit_id = -1;
  // IDs: 0=red sphere, 1=blue sphere, 2=green sphere, 3=floor, 4=mirror_left,
  // 5=mirror_right

  Point3D hit_point;
  Vector3D hit_normal;

  // Red sphere
  {
    Point3D p;
    Vector3D n;
    if (sphere_red.get_intersection(ray, p, n)) {
      float t = ray.point.distance(p);
      if (t > 1e-3f && t < best_t) {
        best_t = t;
        hit_id = 0;
        hit_point = p;
        hit_normal = n;
      }
    }
  }

  // Blue sphere
  {
    Point3D p;
    Vector3D n;
    if (sphere_blue.get_intersection(ray, p, n)) {
      float t = ray.point.distance(p);
      if (t > 1e-3f && t < best_t) {
        best_t = t;
        hit_id = 1;
        hit_point = p;
        hit_normal = n;
      }
    }
  }

  // Green sphere
  {
    Point3D p;
    Vector3D n;
    if (sphere_green.get_intersection(ray, p, n)) {
      float t = ray.point.distance(p);
      if (t > 1e-3f && t < best_t) {
        best_t = t;
        hit_id = 2;
        hit_point = p;
        hit_normal = n;
      }
    }
  }

  // Floor slab
  //    {
  //       Point3D  p;  Vector3D n;
  //       if (floor_slab.get_intersection(ray, p, n))
  //       {
  //          float t = ray.point.distance(p);
  //          if (t > 1e-3f && t < best_t) { best_t = t; hit_id = 3; hit_point =
  //          p; hit_normal = n; }
  //       }
  //    }

  // Left mirror
  {
    Point3D p;
    Vector3D n;
    if (mirror_left.get_intersection(ray, p, n)) {
      float t = ray.point.distance(p);
      if (t > 1e-3f && t < best_t) {
        best_t = t;
        hit_id = 4;
        hit_point = p;
        hit_normal = n;
      }
    }
  }

  // Right mirror
  {
    Point3D p;
    Vector3D n;
    if (mirror_right.get_intersection(ray, p, n)) {
      float t = ray.point.distance(p);
      if (t > 1e-3f && t < best_t) {
        best_t = t;
        hit_id = 5;
        hit_point = p;
        hit_normal = n;
      }
    }
  }

  // Nothing hit
  if (hit_id < 0) {
    // Simple sky gradient: light blue at top, slightly darker at bottom
    // float y_norm = 0.5f * (ray.dir.vy + 1.0f);  // 0..1
    ColorRGB sky;
    // sky.set(30  + y_norm * 80,
    //         80  + y_norm * 100,
    //         180 + y_norm * 50);
    sky.set(100, 100, 100);
    return sky;
  }

  // Surface normal
  if (mode == "normal") {
    ColorRGB nc;
    nc.set(127 + hit_normal.vx * 127, 127 + hit_normal.vy * 127,
           127 + hit_normal.vz * 127);
    return nc;
  }

  // Mirror hit
  if (hit_id == 4 || hit_id == 5) {
    if (depth >= max_depth)
      return black; // too many bounces – return black

    Mirror3D &mir = (hit_id == 4) ? mirror_left : mirror_right;

    Ray3D reflected_ray;
    mir.get_reflected_ray(ray, hit_point, hit_normal, reflected_ray);

    // Apply tint to reflection to make mirror not perfect
    ColorRGB reflected_color = trace_ray(reflected_ray, depth + 1);
    reflected_color.mult(0.85f); // percentage bounced back
    return reflected_color;
  }

  // Phong shading
  ColorRGB shade;

  if (hit_id == 0) {
    shader_red.SetCamera(ray.point); // camera = current ray origin
    shader_red.GetShade(hit_point, hit_normal, shade);
  } else if (hit_id == 1) {
    shader_blue.SetCamera(ray.point);
    shader_blue.GetShade(hit_point, hit_normal, shade);
  } else if (hit_id == 2) {
    shader_green.SetCamera(ray.point);
    shader_green.GetShade(hit_point, hit_normal, shade);
  } else if (hit_id == 3) {
    shader_floor.SetCamera(ray.point);
    shader_floor.GetShade(hit_point, hit_normal, shade);
  }

  return shade;
}

// Create scene
void setup_scene() {
  // Light for phong shading
  ColorRGB light_color;
  light_color.set(220, 220, 220);
  Vector3D light_dir;
  light_dir.set(-1, -2, -1);
  light_dir.normalize();

  // Red sphere
  {
    Point3D c;
    c.set(-0.7f, 0.1f, 3.0f);
    sphere_red.set(c, 0.75f);

    ColorRGB col;
    col.set(220, 50, 50);
    shader_red.SetLight(light_color, light_dir);
    shader_red.SetObject(col, 0.2f, 0.5f, 0.4f, 12);
  }

  // Blue sphere
  {
    Point3D c;
    c.set(0.7f, -0.1f, 4.0f);
    sphere_blue.set(c, 0.75f);

    ColorRGB col;
    col.set(50, 80, 220);
    shader_blue.SetLight(light_color, light_dir);
    shader_blue.SetObject(col, 0.2f, 0.5f, 0.5f, 20);
  }

  // Green sphere
  {
    Point3D c;
    c.set(0.0f, 0.5f, 5.5f);
    sphere_green.set(c, 0.4f);

    ColorRGB col;
    col.set(40, 200, 80);
    shader_green.SetLight(light_color, light_dir);
    shader_green.SetObject(col, 0.2f, 0.5f, 0.6f, 30);
  }

  //    // ---- Floor slab: a flat box 20 units wide, 0.1 thick, 20 deep ----
  //    {
  //       Point3D c; c.set(0.0f, -1.15f, 4.0f);
  //     //   floor_slab.set(c, 10.0f);  // half_size=10 → 20x20x20 cube used as
  //     floor
  //       // (Only the top face will be hit from above, so it acts as a floor)

  //       ColorRGB col; col.set(160, 155, 145);
  //       shader_floor.SetLight(light_color, light_dir);
  //       shader_floor.SetObject(col, 0.3f, 0.5f, 0.15f, 5);
  //    }

  // Left mirror
  // Faces right (+X direction), so its normal = (1,0,0)
  {
    Point3D c;
    c.set(-2.8f, 0.5f, 4.0f);
    Vector3D n;
    n.set(1, 0, 0); // faces +X (towards right/viewer side)
    Vector3D u;
    u.set(0, 1, 0);                       // up is +Y
    mirror_left.set(c, n, u, 1.5f, 3.0f); // 1.5 wide, 3 tall
  }

  // Right mirror
  // Faces left (-X direction), so its normal = (-1,0,0)
  {
    Point3D c;
    c.set(2.8f, 0.5f, 4.0f);
    Vector3D n;
    n.set(-1, 0, 0); // faces -X (towards left/viewer side)
    Vector3D u;
    u.set(0, 1, 0);
    mirror_right.set(c, n, u, 1.5f, 3.0f);
  }
}

// Ray trace the image from the camera
void ray_trace() {
  // Center of the scene
  const float cx = 0.0f, cy = 0.0f, cz = 3.5f;

  // Camera position
  Point3D camera;
  camera.set(cam_dist * cos(theta_y) * sin(theta) + cx,
             cam_dist * sin(theta_y) + cy,
             cam_dist * cos(theta_y) * cos(theta) + cz);

  // Forward vector: vector from camera to scene
  Vector3D forward;
  forward.set(cx - camera.px, cy - camera.py, cz - camera.pz);
  forward.normalize();

  Vector3D world_up;
  world_up.set(0, 1, 0);

  Vector3D right;
  right.set(forward.vy * world_up.vz - forward.vz * world_up.vy,
            forward.vz * world_up.vx - forward.vx * world_up.vz,
            forward.vx * world_up.vy - forward.vy * world_up.vx);
  right.normalize();

  // cam_up = right x forward
  Vector3D cam_up;
  cam_up.set(right.vy * forward.vz - right.vz * forward.vy,
             right.vz * forward.vx - right.vx * forward.vz,
             right.vx * forward.vy - right.vy * forward.vx);
  cam_up.normalize();

  // ---- Image plane: 1 unit in front of camera ----
  // Each pixel maps to a point on this plane.
  const float PLANE_DIST = 1.5f; // focal-length feel

  setup_scene();

  for (int y = 0; y < YDIM; y++) {
    for (int x = 0; x < XDIM; x++) {
      // NDC in [-1, 1]
      float u = (x - XDIM / 2.0f) * 2.0f / XDIM;
      float v = (y - YDIM / 2.0f) * 2.0f / YDIM;

      // Image-plane point = camera + forward*PLANE_DIST + right*u + cam_up*v
      Point3D img_point;
      img_point.set(
          camera.px + forward.vx * PLANE_DIST + right.vx * u + cam_up.vx * v,
          camera.py + forward.vy * PLANE_DIST + right.vy * u + cam_up.vy * v,
          camera.pz + forward.vz * PLANE_DIST + right.vz * u + cam_up.vz * v);

      Ray3D ray;
      ray.set(camera, img_point);

      ColorRGB color = trace_ray(ray, 0);

      image[y][x][0] = (unsigned char)color.R;
      image[y][x][1] = (unsigned char)color.G;
      image[y][x][2] = (unsigned char)color.B;
    }
  }
}

//---------------------------------------
// OpenGL init
//---------------------------------------
void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);

  cout << "Mirror Demo – controls:\n"
       << "   'x' / left  arrow - orbit camera left\n"
       << "   'X' / right arrow - orbit camera right\n"
       << "   'y' / down  arrow - orbit camera down\n"
       << "   'Y' / up    arrow - orbit camera up\n"
       << "   '+' - zoom in\n"
       << "   '-' - zoom out\n"
       << "   'p' - Phong shading   (default)\n"
       << "   'n' - surface normals (diagnostic)\n"
       << "   '1' - '4' - mirror bounce depth\n"
       << "   'q' - quit\n";

  setup_scene();
  ray_trace();
}

//---------------------------------------
// Display callback
//---------------------------------------
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
  glFlush();
}

//---------------------------------------
// Keyboard callback
//---------------------------------------
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'q':
    exit(0);
    break;

  // Orbit camera
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

  // Zoom
  case '+':
    if (cam_dist > 1.5f)
      cam_dist /= 1.15f;
    break;
  case '-':
    if (cam_dist < 20.f)
      cam_dist *= 1.15f;
    break;

  // Render mode
  case 'n':
    mode = "normal";
    break;
  case 'p':
    mode = "phong";
    break;

  // Mirror bounce depth
  case '1':
    max_depth = 1;
    break;
  case '2':
    max_depth = 2;
    break;
  case '3':
    max_depth = 3;
    break;
  case '4':
    max_depth = 4;
    break;
  }

  // Clamp vertical angle so the camera doesn't flip over the poles
  if (theta_y > 1.4f)
    theta_y = 1.4f;
  if (theta_y < -1.4f)
    theta_y = -1.4f;

  //    cout << "Rendering (depth=" << max_depth
  //         << ", dist=" << cam_dist
  //         << ", theta=" << theta
  //         << ", theta_y=" << theta_y << ") ...\n";

  ray_trace();
  glutPostRedisplay();
}

//---------------------------------------
// Special (arrow-key) callback – mirrors the dice project exactly
//---------------------------------------
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

//---------------------------------------
// Main
//---------------------------------------
int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitWindowSize(XDIM, YDIM);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Mirror Scene");
  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMainLoop();
  return 0;
}