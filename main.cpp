#include <string.h>
#include <memory>
#include "GL/glui.h"
#include "io.h"
#include "mesh.h"
#include "loop_schema.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float xy_aspect;
int last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;
int main_window;

/** These are the live variables passed into GLUI ***/
int wireframe = 0;
int obj = 0;
int segments = 8;


GLUI_Checkbox *checkbox;
GLUI_Spinner *spinner;
GLUI_RadioGroup *radio;

/* std::unique_ptr<Mesh> mesh_object; */
Mesh mesh_object;



void control_cb(int control) {
  printf("callback: %d\n", control);
  /* printf("             checkbox: %d\n", checkbox->get_int_val());
  printf("              spinner: %d\n", spinner->get_int_val()); */
  printf("          radio group: %d\n", radio->get_int_val());
}

void initObject()
{
  mesh_object = readMesh("test.obj", MeshType::OBJ);
}

void applySchema() {
  LoopSchema schema;
  mesh_object = schema.apply(mesh_object);
  glutPostRedisplay();
}

void drawMesh(Mesh mesh)
{
  Color color = mesh.faceColor;
  for (auto face : mesh.FacesArray)
  {
    glBegin(GL_POLYGON);
    for (int vertexIdx : face.v)
    {
      MeshVertex v = mesh.VerticesArray[vertexIdx];
      //glColor3i(color.r, color.g, color.b);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3d(v.x / 3.0, v.y / 3.0, v.z / 3.0);
    }
    glEnd();
  }

  for (auto face : mesh.FacesArray)
  {
    glBegin(GL_LINE_LOOP);
    for (int vertexIdx : face.v)
    {
      MeshVertex v = mesh.VerticesArray[vertexIdx];
      //glColor3i(color.r, color.g, color.b);
      glLineWidth(10.0);
      glColor3f(0.0, 0.0, 0.0);
      glVertex3d(v.x / 3.0, v.y / 3.0, v.z / 3.0);
      printf("%lf %lf %lf\n", v.x, v.y, v.z);
    }
    glEnd();
  }
}

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch (Key)
  {
    // A few keys here to test the sync_live capability.
  case 'o':
    // Cycle through object types
    ++obj %= 3;
    GLUI_Master.sync_live_all();
    break;
  case 'w':
    // Toggle wireframe mode
    wireframe = !wireframe;
    GLUI_Master.sync_live_all();
    break;
  case 27:
  case 'q':
    exit(0);
    break;
  };
  glutPostRedisplay();
}

/***************************************** myGlutMenu() ***********/

void myGlutMenu(int value)
{
  myGlutKeyboard(value, 0, 0);
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN)
  {
    last_x = x;
    last_y = y;
  }
}

/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y)
{
  rotationX += (float)(y - last_y);
  rotationY += (float)(x - last_x);

  last_x = x;
  last_y = y;

  glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
  xy_aspect = (float)x / (float)y;
  glViewport(0, 0, x, y);

  glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay()
{
  glClearColor(.9f, .9f, .9f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-xy_aspect * .08, xy_aspect * .08, -.08, .08, .1, 15.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -1.6f);
  glRotatef(rotationY, 0.0, 1.0, 0.0);
  glRotatef(rotationX, 1.0, 0.0, 0.0);

  /*** Now we render object, using the variables 'obj', 'segments', and
    'wireframe'.  These are _live_ variables, which are transparently
    updated by GLUI ***/

  if (obj == 0)
  {
    drawMesh(mesh_object);
  }
  else if (obj == 1)
  {
    if (wireframe)
      glutWireTorus(.2, .5, 16, segments);
    else
      glutSolidTorus(.2, .5, 16, segments);
  }

  glDisable(GL_LIGHTING); /* Disable lighting while we render text */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 100.0, 0.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3ub(0, 0, 0);
  glRasterPos2i(10, 10);


  glutSwapBuffers();
}

/**************************************** main() ********************/

int main(int argc, char *argv[])
{
  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/

  initObject();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(600, 600);

  main_window = glutCreateWindow("GLUI Example 2");
  glutDisplayFunc(myGlutDisplay);
  glutReshapeFunc(myGlutReshape);
  glutKeyboardFunc(myGlutKeyboard);
  glutMotionFunc(myGlutMotion);
  glutMouseFunc(myGlutMouse);

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/

  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);

  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  GLUI *glui = GLUI_Master.create_glui("GLUI", 0, 800, 50); /* name, flags,
								 x, and y */
  new GLUI_StaticText(glui, "GLUI Example 2");
  new GLUI_Separator(glui);
  GLUI_Panel *obj_panel = new GLUI_Panel(glui, "Schema Type");
  new GLUI_Button(glui, "Apply schema", 1, (GLUI_Update_CB)applySchema);
  radio = new GLUI_RadioGroup(obj_panel, &obj, 2, control_cb);
  new GLUI_RadioButton(radio, "Loop");
  new GLUI_RadioButton(radio, "Butterfly");
  new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);

  glui->set_main_gfx_window(main_window);

  /* We register the idle callback with GLUI, *not* with GLUT */
  //GLUI_Master.set_glutIdleFunc( myGlutIdle );
  GLUI_Master.set_glutIdleFunc(NULL);

  glutMainLoop();

  return EXIT_SUCCESS;
}
