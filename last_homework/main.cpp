#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <GL/glut.h>
#include <stdio.h>
#include "glm.h"

float dx=0,dy=0,dz=0,oldX=0,oldY=0;
GLuint tex1,tex2,tex3;
float angleX[20],angleY[20];
//float angle[20];
int angleID=0;
float NewAngleX[20],OldAngleX[20],NewAngleY[20],OldAngleY[20];
FILE * fout=NULL,* fin = NULL;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int myTexture(char * filename)
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
void myLight()
{
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
}
GLMmodel * all_body = NULL;
GLMmodel * hair = NULL;
GLMmodel * head = NULL;
GLMmodel * body = NULL;
GLMmodel * Rarm1 = NULL, * Rarm2 = NULL, * Rfoot1 = NULL, * Rfoot2 = NULL, * Rhand = NULL;
GLMmodel * Larm1 = NULL, * Larm2 = NULL, * Lfoot1 = NULL, * Lfoot2 = NULL, * Lhand = NULL;
GLMmodel * pmodel =NULL;
GLMmodel * myRead(char * filename)
{
    GLMmodel * one =NULL;
    if( one == NULL){
        one = glmReadOBJ(filename);
        //glmUnitize(one);    ///把模型pmodel 調整成單位大小-1.....+1
        glmScale(one,1/16.0);
        glmFacetNormals(one);    ///把Facet面的法向量Normals算出來
        glmVertexNormals(one,90);    ///把Vertex法向量，用面平均算出來
    }
    return one;
}
void myReadAll()
{
    pmodel = myRead("data/all_body.obj");
    hair = myRead("data/hair.obj");
    head = myRead("data/head.obj");
    body = myRead("data/body.obj");
    Rarm1 = myRead("data/Rarm1.obj");
    Rarm2 = myRead("data/Rarm2.obj");
    Rfoot1 = myRead("data/Rfoot1.obj");
    Rfoot2 = myRead("data/Rfoot2.obj");
    Larm1 = myRead("data/Larm1.obj");
    Larm2 = myRead("data/Larm2.obj");
    Lfoot1 = myRead("data/Lfoot1.obj");
    Lfoot2 = myRead("data/Lfoot2.obj");
}
void reshape(int w,int h)
{
    float ar = (float)w/(float)h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,ar,0.1,100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,2, 0,0,0, 0,1,0);
}
void myWrite()
{
    if(fin!=NULL){
        fclose(fin);
        fin=NULL;
        ///最好這裡再做個警告, 因為整個file.txt將被清空!!
    }
    if(fout==NULL)fout=fopen("file.txt","w+");
    for(int i=0;i<20;i++)
    {
         /*printf(     "%.2f ",angle[i]);
        fprintf(fout,"%.2f ",angle[i]);*/
         printf(     "%.2f ",angleX[i]);
        fprintf(fout,"%.2f ",angleX[i]);
    }
    printf("\n");
    fprintf(fout,"\n");
    for(int i=0;i<20;i++)
    {
         printf(     "%.2f ",angleY[i]);
        fprintf(fout,"%.2f ",angleY[i]);
    }
    printf("\n");
    fprintf(fout,"\n");
}
void myRead1()
{
    if(fout!=NULL){
            fclose(fout);///用來確認正在讀的檔案是否關閉
            fout=NULL;
    }
    if(fin==NULL)fin = fopen("file.txt","r");
    for(int i=0;i<20;i++){
        OldAngleX[i]=NewAngleX[i];
        fscanf(fin, "%f",&NewAngleX[i]);
    }
    for(int i=0;i<20;i++){
        OldAngleY[i]=NewAngleY[i];
        fscanf(fin, "%f",&NewAngleY[i]);
    }
    glutPostRedisplay(); ///重畫畫面
}
void myInterpolate(float alpha)
{
    for(int i=0;i<20;i++)
    {
        //angle[i]=alpha*NewAngle[i]+(1-alpha)*OldAngle[i];
        angleX[i]=alpha*NewAngleX[i]+(1-alpha)*OldAngleX[i];
        printf("%.2f ",angleX[i]);

    }
    printf("\n");
    for(int i=0;i<20;i++)
    {
        angleY[i]=alpha*NewAngleY[i]+(1-alpha)*OldAngleY[i];
        printf("%.2f ",angleY[i]);

    }
    glutPostRedisplay();
}
void timer(int t)
{
    float alpha = (t%13)/13.0;
    if(t%13==0)myRead1();
    myInterpolate(alpha);
    glutTimerFunc(33,timer,t+1);
}
void keyboard(unsigned char key,int x,int y)
{
    if(key=='p')
    {    ///播放
        myRead1();   ///先讀一行
        glutTimerFunc(0,timer,0);   ///馬上開始播動畫
        PlaySound("Goodbye Declaration .wav",NULL,SND_ASYNC);
    }
    if(key=='r') myRead1();///讀檔
    if(key=='s') myWrite();///存檔
    if(key=='0') angleID=0;
    if(key=='1') angleID=1;
    if(key=='2') angleID=2;
    if(key=='3') angleID=3;
    if(key=='4') angleID=4;
    if(key=='5') angleID=5;
    if(key=='6') angleID=6;
    if(key=='7') angleID=7;
    if(key=='8') angleID=8;
    if(key=='9') angleID=9;
}
void mouse(int button,int state,int x,int y)
{
    oldX=x; oldY=y;
}
void motion(int x,int y)
{
    //dx+=(x-oldX)/150.0;   dy-=(y-oldY)/150.0;
    //myWrite();
    //angle[angleID] += (x-oldX);
    angleX[angleID] += (x-oldX);
    angleY[angleID] += (y-oldY);
    //printf("glTranslatef( %.2f, %.2f, %.2f );\n",dx,dy,dz);
    oldX=x;         oldY=y;
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D,tex3);
    glNormal3f(0,0,+1);
    glBegin(GL_POLYGON);
        glTexCoord2f(0,1); glVertex3f(-4.5,-2.5,-2);
        glTexCoord2f(1,1); glVertex3f(+4.5,-2.5,-2);
        glTexCoord2f(1,0); glVertex3f(+4.5,+2.5,-2);
        glTexCoord2f(0,0); glVertex3f(-4.5,+2.5,-2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,tex2);
    //glmDraw(pmodel,GLM_TEXTURE|GLM_SMOOTH);
    glutSolidTeapot(0.03);
    glPushMatrix();
        glScalef(0.8,0.8,0.8);
        glTranslatef( 0.01, 0.79, 0.00 );
        //glRotatef(angle[1],0,0,1);
        glRotatef(angleX[1],0,0,1);
        glRotatef(angleY[1],0,1,0);
        glTranslatef( -1.21, -3.94, 0.00 );
        //glTranslatef(dx,dy,dz);
        glmDraw(hair,GLM_TEXTURE|GLM_SMOOTH);   ///頭髮
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D,tex1);
    glPushMatrix();
        glScalef(0.8,0.8,0.8);
        glTranslatef( 0.01, 0.79, 0.00 );
        //glRotatef(angle[1],0,0,1);
        glRotatef(angleX[1],0,0,1);
        glRotatef(angleY[1],0,1,0);
        glTranslatef( -1.21, -2.03, 0.00 );
        //glTranslatef(dx,dy,dz);
        glmDraw(head,GLM_TEXTURE|GLM_SMOOTH);   ///臉孔
    glPopMatrix();
    glPushMatrix();
        glScalef(0.8,0.8,0.8);
        glTranslatef( 0.00, 0.30, 0.00 );
        glRotatef(angleX[0],0,1,0);
        glTranslatef( 0.00, -1.52, 0.00 );
        //glTranslatef(dx,dy,dz);
        glmDraw(body,GLM_TEXTURE|GLM_SMOOTH);   ///身體
    glPopMatrix();
    ///左半身
    glPushMatrix();
        glScalef(0.8,0.8,0.8);
        glTranslatef( -0.12, 0.66, 0.00 );
        //glRotatef(angle[2],0,0,1);
        glRotatef(angleX[2],0,1,0);
        glRotatef(angleY[2],0,0,1);
        glTranslatef( 0.55, -1.88, 0.00 );
        //glTranslatef(dx,dy,dz);
        glmDraw(Larm1,GLM_TEXTURE|GLM_SMOOTH);  ///左上臂
        glPushMatrix();
            glTranslatef( -0.85, 1.67, 0.00 );
            //glRotatef(angle[3],0,1,0);
            glRotatef(angleX[3],0,1,0);
            glRotatef(angleY[3],0,0,1);
            glTranslatef( 1.51, -1.65, 0.00 );
            //glTranslatef(dx,dy,dz);
            glmDraw(Larm2,GLM_TEXTURE|GLM_SMOOTH);  ///左下臂
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
         glScalef(0.8,0.8,0.8);
        glTranslatef( -0.08, -0.05, 0.00 );
        //glRotatef(angle[4],0,0,1);
        glRotatef(angleX[4],0,0,1);
        glRotatef(angleY[4],1,0,0);
        glTranslatef( 1.34, -1.34, 0.00 );
        //glTranslatef(dx,dy,dz);
        glmDraw(Lfoot1,GLM_TEXTURE|GLM_SMOOTH);  ///左大腿
        glPushMatrix();
            glTranslatef( -1.39, 0.73, 0.00 );
            //glRotatef(angle[5],0,1,0);
            glRotatef(angleX[5],0,1,0);
            glRotatef(angleY[5],1,0,0);
            glTranslatef( 1.19, -0.69, 0.00 );
            //glTranslatef(dx,dy,dz);
            glmDraw(Lfoot2,GLM_TEXTURE|GLM_SMOOTH);  ///左小腿
        glPopMatrix();
    glPopMatrix();
    ///右半身
    glPushMatrix();
        glScalef(0.8,0.8,0.8);
        glTranslatef( 0.13, 0.66, 0.00 );
        //glRotatef(angle[6],0,0,1);
        glRotatef(angleX[6],0,1,0);
        glRotatef(angleY[6],0,0,1);
        glTranslatef( -0.69, -1.88, 0.00 );
        //glTranslatef(dx,dy,dz);
        glmDraw(Rarm1,GLM_TEXTURE|GLM_SMOOTH);  ///右上臂
        glPushMatrix();
            glTranslatef( 1.01, 1.66, 0.00 );
            //glRotatef(angle[7],0,1,0);
            glRotatef(angleX[7],0,1,0);
            glRotatef(angleY[7],0,0,1);
            glTranslatef( -1.28, -1.66, 0.00 );
            //glTranslatef(dx,dy,dz);
            glmDraw(Rarm2,GLM_TEXTURE|GLM_SMOOTH);  ///右下臂
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        glScalef(0.8,0.8,0.8);
        glTranslatef( 0.09, -0.02, 0.00 );
        //glRotatef(angle[8],0,0,1);
        glRotatef(angleX[8],0,0,1);
        glRotatef(angleY[8],1,0,0);
        glTranslatef( -0.57, -1.37, 0.00 );
        //glTranslatef(dx,dy,dz);
        glmDraw(Rfoot1,GLM_TEXTURE|GLM_SMOOTH);  ///右大腿
        glPushMatrix();
            glTranslatef( 0.62, 0.69, 0.00 );
            //glRotatef(angle[9],0,1,0);
            glRotatef(angleX[9],0,1,0);
            glRotatef(angleY[9],1,0,0);
            glTranslatef( -0.74, -0.68, 0.00 );
            //glTranslatef(dx,dy,dz);
            glmDraw(Rfoot2,GLM_TEXTURE|GLM_SMOOTH);  ///右小腿
        glPopMatrix();
     glPopMatrix();
    glutSwapBuffers();

}
int main(int argc,char**argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("last_homework");
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    myLight();
    tex1=myTexture("data/Diffuse.jpg");
    tex2=myTexture("data/HairD.jpg");
    tex3=myTexture("data/1.jpg");
    glClearColor(1,1,0,0);
    myReadAll();

    glutMainLoop();

}
