
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Mmsystem.h>
//#include <mciapi.h>
#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <iostream>
#include <fstream>
#include <sstream>
#include "MainMenu.hpp"
#include "Button.hpp"
#include "Camera.hpp"
#include "World.hpp"





// Global Variables
GLint winWidth = 800;
GLint winHeight = 700;
GLint worldOption = 1;
GLint displayGameWorld;
MainMenu menu;  // Main menu object
Button startButton;
Button resumeButton;
Button quitButton;
Button restartButton;
Camera myCamera;
World myWorld;
int health = 3;
GLint upPressed = 2;
GLint downPressed = 2;
GLint leftPressed = 2;
GLint rightPressed = 2;

GLint paused = 2;
GLint pauseFlag = 2;


Shape *sshapep = myWorld.list[0];
float timeFactor = 1;
int MAX_BOULDERS = 5;


#if __APPLE__
GLfloat translateConstant = 0.15;
GLfloat translateConstant2 = 0.04;
#else
GLfloat translateConstant = 0.002;
GLfloat translateConstant2 = 0.0004;
#endif


void output(float x, float y, float z, float r, float g, float b, char *string)
{
  glColor3f( r, g, b );
  glRasterPos3f(x, y, z);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

void MyKeyboardFunc(unsigned char Key, int x, int y){
    GLfloat rx, ry, rz;
    if(worldOption == 2) {
        if(Key == 'a'){
            leftPressed = 100;
            
        }if(Key == 'd'){
            rightPressed = 100;
            
        }else if(Key == 'w'){
            upPressed = 100;
            
        }else if(Key == 's'){
            downPressed = 100;
            
        }else if(Key == 'q'){
            rx = sshapep->getMC().mat[0][0];
            ry = sshapep->getMC().mat[1][0];
            rz = sshapep->getMC().mat[2][0];
            sshapep->rotateMC(rx, ry, rz, -2);
        }else if(Key == 'e'){
            rx = sshapep->getMC().mat[0][0];
            ry = sshapep->getMC().mat[1][0];
            rz = sshapep->getMC().mat[2][0];
            sshapep->rotateMC(rx, ry, rz, 2);
            
        }else if (Key == 27) {
            paused = 3;
        }
    }
}


void MyKeyboardUpFunc(unsigned char key, int x, int y) {
    if(worldOption == 2){
        if(key == 'a'){
            leftPressed = 2;
            
        }if(key == 'd'){
            rightPressed = 2;
            
        }else if(key == 'w'){
            upPressed = 2;
            
        }else if(key == 's'){
            downPressed = 2;
        }
    }
}



void MyArrowFunc(int Key, int x, int y) {
    
    if(worldOption == 2){
        if(Key == GLUT_KEY_LEFT){
            leftPressed = 100;
            
        }else if(Key == GLUT_KEY_RIGHT){
            rightPressed = 100;
            
        }else if(Key == GLUT_KEY_UP){
            upPressed = 100;
            
        }else if(Key == GLUT_KEY_DOWN){
            downPressed = 100;
        }
    }
}



void SpecialKeysUp(int key, int x, int y) {
    if(worldOption == 2) {
        if(key == GLUT_KEY_LEFT){
            leftPressed = 2;
            
        }else if(key == GLUT_KEY_RIGHT){
            rightPressed = 2;
            
        }else if(key == GLUT_KEY_UP){
            upPressed = 2;
            
        }else if(key == GLUT_KEY_DOWN){
            downPressed = 2;
        }
    }
}


void Update() {
    if(worldOption == 2) {
        if (upPressed == 100) {
            sshapep->translate(0.0, translateConstant, 0.0);
        }
        if(downPressed == 100) {
            sshapep->translate(0.0, -translateConstant, 0.0);
        }
        if(leftPressed == 100) {
            sshapep->translate(-translateConstant,0.0,0.0);
        }
        if(rightPressed == 100){
            sshapep->translate(translateConstant, 0, 0);
        }
    }
}





void physics(void){

	GLfloat rx, ry, rz;
	int i = 1;
	int j = 3;
	myWorld.list[0]->stayInBound();
	if(timeFactor / 10 < 3){
		j = 3 + int(timeFactor /10);
	}else{
		j = 6;
	}
	for(; i < j; i++){
		if(i != 3){
			int c = myWorld.list[0]->checkCollision(myWorld.list[i]);
			Shape *temp = myWorld.list[0]; //must grab a temp pointer or else getters wont work
//			GLfloat x = temp->getX();
//			GLfloat y = temp->getY();
//			temp = myWorld.list[1];
//			GLfloat x1 = temp->getX();
//			GLfloat y1 = temp->getY();
//			temp = myWorld.list[2];
//			GLfloat x2 = temp->getX();
//			GLfloat y2 = temp->getY();
			//printf("p:(%.1f, %.1f) ", x, y);
			//printf("o1: (%.1f, %.1f) ", x1, y1);
			//printf("o2: (%.1f, %.1f)\n", x2, y2);
//
			if(myWorld.list[i]->outOfBounds()){
				myWorld.list[i]->randomX();
				myWorld.list[i]->translate(0,15,0);
				myWorld.list[i]->randomY();
			}
			if(c == 1){
				char oof[3] = {'O','O','F'};
				output(0,0,0,1,1,1,oof);
				myWorld.list[i]->translate(0,12, 0);
				health -=1;
				//PlaySound((LPCSTR) "death_sound.wav", NULL, SND_FILENAME | SND_ASYNC );
				//PlaySound((LPCSTR) "music.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			myWorld.list[i]->translate(0, -translateConstant2 *timeFactor, 0);
			rx = myWorld.list[3]->getMC().mat[0][0];
			ry = myWorld.list[3]->getMC().mat[1][0];
			rz = myWorld.list[3]->getMC().mat[2][0];
			myWorld.list[3]->rotateMC(rx, ry, rz, 0.02);
			rx = myWorld.list[4]->getMC().mat[0][0];
			ry = myWorld.list[4]->getMC().mat[1][0];
			rz = myWorld.list[4]->getMC().mat[2][0];
			myWorld.list[4]->rotateMC(rx, ry, rz, 0.02);
			rx = myWorld.list[5]->getMC().mat[0][0];
			ry = myWorld.list[5]->getMC().mat[1][0];
			rz = myWorld.list[5]->getMC().mat[2][0];
			myWorld.list[5]->rotateMC(rx, ry, rz, 0.02);
		}
	}
}




void pauseGame() {
    glutIdleFunc(NULL);
    if(pauseFlag == 2) {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        
        myCamera.eye.set(0, 0, 11);
        myCamera.setProjectionMatrix();
        
        myWorld.list[0]->translate(20.0,0.0,0.0);
        myWorld.list[1]->translate(20.0,0.0,0.0);
        myWorld.list[2]->translate(20.0,0.0,0.0);
        
        // Add myworld.list[3].. for buttons as cubes and delete them when I resume
        
        
        pauseFlag = 3;
    }
}
void resumeGame() {
    glClearColor(0.4, 1.0, 1.0, 0.0);
    myCamera.eye.set(5, 5, 10);
    myCamera.setProjectionMatrix();
    glutIdleFunc(Update);
    physics();
    timeFactor = timeFactor + 0.0001;
    //printf("%.6f\n", timeFactor);
}



void restartGame() {
    
}



void quitGame() {
    
}




void init(void) {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 400);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Mountaineer");
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, winWidth, winHeight, 0.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //PlaySound((LPCSTR) "music.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    //Backface Test
    glEnable(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // Camera setup
	myCamera.setDefaultCamera();
	myWorld.list[0]->translate(0,-2,0);
    glutPostRedisplay();


}





void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World Options
    if(worldOption == 1) {   // Main Menu World
        menu.displayMainMenu();
        glFlush();
        glutSwapBuffers();
    }
    
    if(worldOption == 2) {      // Game World
        
        if(paused == 3) {
            pauseGame();
            
        }else if(health > 0) {
            resumeGame();

			char score[100] = {'S','C','O','R','E', ':'};
			char scoreBuffer[10];
			itoa(int(timeFactor* 100), scoreBuffer, 10);
			output(-4,3.9, 1,1,1,1, score);
			output(-2,3.99, 1,1,1,1, scoreBuffer);
            myWorld.draw();
        }else{

			upPressed = 0;
			leftPressed = 0;
			downPressed = 0;
			rightPressed = 0;

			int j;

			float rx = myWorld.list[0]->getMC().mat[0][2];
			float ry = myWorld.list[0]->getMC().mat[1][2];
			float rz = myWorld.list[0]->getMC().mat[2][2];
			sshapep->rotateMC(rx,ry,rz, 0.5);
			for(j = 0; j <40; j++){
				myWorld.list[0]->translate(0,-translateConstant/2, 0);
				myWorld.draw();
				glFlush();
				glutSwapBuffers();
				glutPostRedisplay();
			}
			myWorld.draw();
			//move to death menu here
        }

        glFlush();
        glutSwapBuffers();
    }
    glutPostRedisplay();

}





void winReshapeFcn(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    winWidth = newWidth;
    winHeight = newHeight;
}





void mouseAction(int button, int action, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
        //printf("Clicked at %d, %d\n", x, y);
        if(worldOption == 1 && x < startButton.x1 && x > startButton.x2 && y > 554 && y < 621) {  // Start Button clicked
            menu.startButtonClicked();
            displayGameWorld = 1;
        }
    }
    
    if(button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
        if (displayGameWorld == 1) {
            worldOption = 2;    // Start game
        }
        
        if(paused == 3) {   // If we're on pause menu
            // Check if a button is pressed
            //            if(/* button pressed*/) {
            //                restartGame();
            //            }
            //            if(/* button pressed*/) {
            //                resumeGame();
            //            }
            //            if(/* button pressed*/) {
            //                quitGame();
            //            }
        }

        
    }
    glutPostRedisplay();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutReshapeFunc(winReshapeFcn);
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseAction);
    glutKeyboardFunc(MyKeyboardFunc);
    glutSpecialFunc(MyArrowFunc);
    glutSpecialUpFunc(SpecialKeysUp);
    glutKeyboardUpFunc(MyKeyboardUpFunc);
    glutMainLoop();
    return 0;
}
