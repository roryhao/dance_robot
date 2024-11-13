#include <stdio.h>
#include <GL/glut.h>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "glm.h"
GLMmodel * model=NULL;
GLMmodel * head;
GLMmodel * body=NULL;
GLMmodel * arm1=NULL, * arm2=NULL;
GLMmodel * hand1=NULL, * hand2=NULL;
GLMmodel * bot=NULL;
GLMmodel * leg1=NULL, * leg2=NULL;
GLMmodel * knee1=NULL, * knee2=NULL;
GLMmodel * foot1=NULL, * foot2=NULL;
int show[10]={1,1,0,1,1,1,1,1,1,1};
float angle=0;
void resize(int width, int height);
void timer(int t);
void display();
int myTexture(char *filename);
void loadGundamAll();

void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ar, 0.001, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt(0,0,3, 0,0,0, 0,1,0);
}
void timer(int t)
{
    glutTimerFunc(30, timer, t+1);
    angle++;
    glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	int now=key-'0';
	if( isdigit(key) ) show[now] = !show[now];
	glutPostRedisplay();
}
void display()
{
    glClearColor(0.5,0.5,0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        glTranslatef(0, -1, 0);
        glutSolidSphere(0.1, 10,10);
        glRotatef(angle, 0,1,0);
        ///glmDraw(model, GLM_SMOOTH | GLM_TEXTURE);
		if(show[1]==1) glmDraw(head, GLM_SMOOTH | GLM_TEXTURE);
        if(show[2]==1) glmDraw(body, GLM_SMOOTH | GLM_TEXTURE);
        if(show[3]==1) glmDraw(arm1, GLM_SMOOTH | GLM_TEXTURE);
        if(show[4]==1) glmDraw(arm2, GLM_SMOOTH | GLM_TEXTURE);
		if(show[5]==1) glmDraw(hand1, GLM_SMOOTH | GLM_TEXTURE);
		if(show[6]==1) glmDraw(hand2, GLM_SMOOTH | GLM_TEXTURE);
        if(show[7]==1) glmDraw(bot, GLM_SMOOTH | GLM_TEXTURE);
        if(show[8]==1) glmDraw(leg1, GLM_SMOOTH | GLM_TEXTURE);
		if(show[9]==1) glmDraw(leg2, GLM_SMOOTH | GLM_TEXTURE);
		glmDraw(knee1, GLM_SMOOTH | GLM_TEXTURE);
        glmDraw(knee2, GLM_SMOOTH | GLM_TEXTURE);
        glmDraw(foot1, GLM_SMOOTH | GLM_TEXTURE);
        glmDraw(foot2, GLM_SMOOTH | GLM_TEXTURE);
    glPopMatrix();
    glutSwapBuffers();
}
int myTexture(char *filename)
{
    IplImage * img = cvLoadImage(filename); ///OpenCV讀圖
    cvCvtColor(img,img, CV_BGR2RGB); ///OpenCV轉色彩 (需要cv.h)
    glEnable(GL_TEXTURE_2D); ///1. 開啟貼圖功能
    GLuint id; ///準備一個 unsigned int 整數, 叫 貼圖ID
    glGenTextures(1, &id); /// 產生Generate 貼圖ID
    glBindTexture(GL_TEXTURE_2D, id); ///綁定bind 貼圖ID
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); /// 貼圖參數, 超過包裝的範圖T, 就重覆貼圖
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); /// 貼圖參數, 超過包裝的範圖S, 就重覆貼圖
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /// 貼圖參數, 放大時的內插, 用最近點
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /// 貼圖參數, 縮小時的內插, 用最近點
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->imageData);
    return id;
}
void loadGundamAll()
{
    float dimensions[3];
    if(model==NULL){
        model = glmReadOBJ("Gundam.obj");
        head = glmReadOBJ("head.obj");
        body = glmReadOBJ("body.obj");
        arm1 = glmReadOBJ("arm1.obj");
        arm2 = glmReadOBJ("arm2.obj");
        hand1 = glmReadOBJ("hand1.obj");
        hand2 = glmReadOBJ("hand2.obj");
        bot = glmReadOBJ("bot.obj");
        leg1 = glmReadOBJ("leg1.obj");
        leg2 = glmReadOBJ("leg2.obj");
        knee1 = glmReadOBJ("knee1.obj");
        knee2 = glmReadOBJ("knee2.obj");
        foot1 = glmReadOBJ("foot1.obj");
        foot2 = glmReadOBJ("foot2.obj");
        glmDimensions(model, dimensions);
        glmScale(model, 2.0/dimensions[1]);
        glmScale(head, 2.0/dimensions[1]);
        glmScale(body, 2.0/dimensions[1]);
        glmScale(arm1, 2.0/dimensions[1]);
        glmScale(arm2, 2.0/dimensions[1]);
        glmScale(hand1, 2.0/dimensions[1]);
        glmScale(hand2, 2.0/dimensions[1]);
        glmScale(bot, 2.0/dimensions[1]);
        glmScale(leg1, 2.0/dimensions[1]);
        glmScale(leg2, 2.0/dimensions[1]);
        glmScale(knee1, 2.0/dimensions[1]);
        glmScale(knee2, 2.0/dimensions[1]);
        glmScale(foot1, 2.0/dimensions[1]);
        glmScale(foot2, 2.0/dimensions[1]);
    }
    myTexture("Diffuse.jpg");
}
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(400,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    loadGundamAll();
    glutMainLoop();

    return EXIT_SUCCESS;
}
