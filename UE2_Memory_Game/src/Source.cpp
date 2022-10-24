/*---------------------------------------------------------------------------
* Title: Pokemon memory game
* Author: Angelos Angelis
* Time to complete: 25 hours
* Additional information: did not implement any kind of animations
*-------------------------------------------------------------------------*/
#include "GL/glew.h" // to support GL_BGR
#include "GL/freeglut.h"
#include "stdio.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <random>
#include <iterator>

// checkerboard texture dimensions
#define checkImageWidth 64
#define checkImageHeight 64
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

int windowid; // the identifier of the GLUT window

GLubyte checkImage[checkImageHeight][checkImageWidth][4]; // image data
GLuint texName; // internal texture name
GLuint frontTexName;
GLuint removedCardtex;

// image data used for texturing
unsigned int imageWidth, imageHeight; // dimensions in pixels
unsigned char* imageData; // actual RGB data

char str[] = "backside.bmp"; // filename to be loaded
char* filenameandpath = &str[0]; // pointer to filename and path to be loaded
char str2[] = "fronttextures.bmp"; // filename to be loaded
char* filenameandpath2 = &str2[0]; // pointer to filename and path to be loaded
char str3[] = "Untitled.bmp"; // filename to be loaded
char* filenameandpath3 = &str3[0]; // pointer to filename and path to be loaded

//memory constants
const int MAX_CARDS = 8;
int cardCols = 4;
int cardRows = 2;
float colSize = 12.0f / cardCols;
float rowSize = 7.0f / cardRows;
float cardSize = (colSize < rowSize) ? colSize * 0.8f : rowSize * 0.8f;
int count_cards = 0;

//texture files
std::vector<int> v = { 0,0,1,1,2,2,3,3 }; // The type of every card{0=charmander; 1=grovyle; 2=squirtle; 3=bulbasaur}
std::random_device rd;
std::mt19937 g(rd());
std::map<int, bool> state; //saves the state of every card {back or front}
std::map<int, bool> removed; // To check if the cards are removed

std::vector<int> pair = { -1,-1,-1 }; //saves the pair of cards the player chooses
std::vector<int> pair_type = { -1,-1,-1 }; //saves the type of the cards of the pair the player chose


bool player_turn = false; //false p1 && true p2
int scorep1 = 0;
int scorep2 = 0;
int fails = 0;



void init_state() {
	for (int i = 0; i < 8; i++)
	{
		state.insert(std::make_pair(i, false));
		removed.insert(std::make_pair(i, false));
	}
}

void draw_front(int it, int i, int j) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, frontTexName);
	switch (it)
	{
	case 0://charmander
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0.5); glVertex3f((colSize * j), (rowSize * i), 0.1); //bottom left 
		glTexCoord2f(0.0, 1); glVertex3f((colSize * j), (rowSize * i) + cardSize, 0.1);//top left
		glTexCoord2f(0.5, 1); glVertex3f((colSize * j) + cardSize, (rowSize * i) + cardSize, 0.1);//topright
		glTexCoord2f(0.5, 0.5); glVertex3f((colSize * j) + cardSize, (rowSize * i), 0.1);//bottom right
		glEnd();
		break;
	case 1://grovyle
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.5, 0); glVertex3f((colSize * j), (rowSize * i), 0.1); //bottom left 
		glTexCoord2f(0.5, 0.5); glVertex3f((colSize * j), (rowSize * i) + cardSize, 0.1);//top left
		glTexCoord2f(1, 0.5); glVertex3f((colSize * j) + cardSize, (rowSize * i) + cardSize, 0.1);//topright
		glTexCoord2f(1, 0); glVertex3f((colSize * j) + cardSize, (rowSize * i), 0.1);//bottom right
		glEnd();
		break;
	case 2://squirtle
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f((colSize * j), (rowSize * i), 0.1); //bottom left 
		glTexCoord2f(0.0, 0.5); glVertex3f((colSize * j), (rowSize * i) + cardSize, 0.1);//top left
		glTexCoord2f(0.5, 0.5); glVertex3f((colSize * j) + cardSize, (rowSize * i) + cardSize, 0.1);//topright
		glTexCoord2f(0.5, 0); glVertex3f((colSize * j) + cardSize, (rowSize * i), 0.1);//bottom right
		glEnd();
		break;
	case 3://bulbasaur
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.5, 0.5); glVertex3f((colSize * j), (rowSize * i), 0.1); //bottom left 
		glTexCoord2f(0.5, 1); glVertex3f((colSize * j), (rowSize * i) + cardSize, 0.1);//top left
		glTexCoord2f(1, 1); glVertex3f((colSize * j) + cardSize, (rowSize * i) + cardSize, 0.1);//topright
		glTexCoord2f(1, 0.5); glVertex3f((colSize * j) + cardSize, (rowSize * i), 0.1);//bottom right
		glEnd();
		break;
	default:
		break;
	}
}

void draw_back(int i, int j) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f((colSize * j), (rowSize * i), 0);
	glTexCoord2f(0, 1); glVertex3f((colSize * j), (rowSize * i) + cardSize, 0);
	glTexCoord2f(1, 1); glVertex3f((colSize * j) + cardSize, (rowSize * i) + cardSize, 0);
	glTexCoord2f(1, 0); glVertex3f((colSize * j) + cardSize, (rowSize * i), 0);
	glEnd();
}

//Just draws a black texture over the cards
void draw_removed(int i, int j) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, removedCardtex);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f((colSize * j), (rowSize * i), 0.2);
	glTexCoord2f(0, 1); glVertex3f((colSize * j), (rowSize * i) + cardSize, 0.2);
	glTexCoord2f(1, 1); glVertex3f((colSize * j) + cardSize, (rowSize * i) + cardSize, 0.2);
	glTexCoord2f(1, 0); glVertex3f((colSize * j) + cardSize, (rowSize * i), 0.2);
	glEnd();
}

void reset_state() { // resets after every second card that got chosen
	for (int i = 0; i < 8; i++)
	{
		state.find(i)->second = false;
	}
}

void render_card() {
	auto it = v.begin();
	for (int i = 0; i < 2; i++) //rows
	{
		for (int j = 0; j < 4; j++) { //colls
			if (state.find(i * 4 + j)->second == false) {
				draw_back(i, j);
			}
			else
			{
				draw_front(*it, i, j);
			}
			if (removed.find(i * 4 + j)->second == true)
			{
				draw_removed(i, j);
			}
			it++;
		}
	}
	if (count_cards == 2)
	{
		if (pair_type[1] == pair_type[2])
		{
			removed.find(pair[1])->second = true;
			removed.find(pair[2])->second = true;
			if (player_turn)
			{
				scorep2 += 10;
			}
			else
			{
				scorep1 += 10;
			}
			player_turn = !player_turn;
			fails--;
		}
		fails++;
		count_cards = 0;
		reset_state();
		player_turn = !player_turn;
		if (player_turn)
		{
			std::cout << "Its p2's turn" << "\n";
		}
		else
		{
			std::cout << "Its p1's turn" << "\n";
		}
	}
	if (scorep1 + scorep2 == 40)
	{
		glutDestroyWindow(windowid);
		std::cout << "Score:" << "\n";
		std::cout << "P1: " << scorep1 << "\n";
		std::cout << "P2: " << scorep2 << "\n";
		std::cout << "Fail attempts : " << fails << "\n";
		exit(0);
	}
	glFlush();
	glDisable(GL_TEXTURE_2D);
}



/* This method is an adapted version of a common BMP loader. It takes path+
filename as input parameters and tries to open and parse a BMP image. The
actual image date will be stored in the byte array imageData. Metadata like
width and height will be available. */
int loadBMP_custom(char* imagepath) {
	unsigned char header[54]; // each BMP file begins by a 54-bytes header
	unsigned int dataPos; // position in the file where actual data begins
	unsigned int imageSize; // = width * height * 3 (3 bytes, 1 per colour)
	FILE* file;
	fopen_s(&file, imagepath, "rb"); // open the file
	if (!file) {
		printf("Image could not be opened\n");
		return 0;
	}
	else if (fread(header, 1, 54, file) != 54) { // if not 54 bytes read
		printf("Not a correct BMP file\n");
		return 0;
	}
	else if (header[0] != 'B' || header[1] != 'M') { // if header is messed up
		printf("Not a correct BMP file\n");
		return 0;
	}
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	imageWidth = *(int*)&(header[0x12]);
	imageHeight = *(int*)&(header[0x16]);
	// Some BMP files are misformatted, we try to guess the missing information
	// 3 : one byte for each Red, Green and Blue component
	if (imageSize == 0) imageSize = imageWidth * imageHeight * 3;
	if (dataPos == 0) dataPos = 54; // the BMP header is done that way
	printf("imageSize: %i\n dataPos: %i\n", imageSize, dataPos);
	imageData = new unsigned char[imageSize]; // create a buffer
	fread(imageData, 1, imageSize, file); // read data from the file into buffer
	fclose(file); // everything is in memory now, the file can be closed
}



void initTextures(void) {
	//load back side textures
	loadBMP_custom(str);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);

	loadBMP_custom(str2);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &frontTexName);
	glBindTexture(GL_TEXTURE_2D, frontTexName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);

	loadBMP_custom(str3);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &removedCardtex);
	glBindTexture(GL_TEXTURE_2D, removedCardtex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);
}

/* The display method is used as a callback for rendering the scene. It
will as usual be called either through glutPostRedisplay or when the
window is updated through the callback functionality. */
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render_card();
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-5.5f, -4.0f, -15.0f);
	glutSwapBuffers();
}


unsigned char prev_key;
void change_state(unsigned char key) {
	if (prev_key != key )
	{
		prev_key = key;
		switch (key) {
		case '0':
			state.find(0)->second = true;
			count_cards++;
			pair[count_cards] = 0;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case '1':
			state.find(1)->second = true;
			count_cards++;
			pair[count_cards] = 1;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case '2':
			state.find(2)->second = true;
			count_cards++;
			pair[count_cards] = 2;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case '3':
			state.find(3)->second = true;
			count_cards++;
			pair[count_cards] = 3;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case '4':
			state.find(4)->second = true;
			count_cards++;
			pair[count_cards] = 4;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case '5':
			state.find(5)->second = true;
			count_cards++;
			pair[count_cards] = 5;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case '6':
			state.find(6)->second = true;
			count_cards++;
			pair[count_cards] = 6;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case '7':
			state.find(7)->second = true;
			count_cards++;
			pair[count_cards] = 7;
			pair_type[count_cards] = v.at(pair[count_cards]);
			break;
		case 27: // escape key
			glutDestroyWindow(windowid);
			exit(0);
			break;
		}
	}
}

/* The keyboard method will be used in this example */
void keyboard(unsigned char key, int x, int y) {
	std::cout << key << std::endl;
	change_state(key);
	glutPostRedisplay();
}

void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float _x = x / (WINDOW_WIDTH / 2.0f) - 1;
		float _y = -(y / (WINDOW_HEIGHT / 2.0f) - 1);
		float colSize = 12.0f / cardCols;
		float rowSize = 7.0f / cardRows;
		int col = ceil(((_x + 1) * 12.0f / 2.0f) / colSize) - 1;
		int row = ceil(((_y + 1) * 7.0f / 2.0f) / rowSize) - 1;
		int sid = cardCols * row + col;
		change_state('0' + sid);
	}
	
}


/* This function should be called when the window is resized. It has to be
registered as a callback in glutReshapeFunc. The function sets up the
correct perspective projection. Don't worry about it we will not go into
detail but we need it for correct perspective 3D rendering. */
void reshapeFunc(int x, int y) {
	if (y == 0 || x == 0) return; //Nothing is visible then, so return
	glMatrixMode(GL_PROJECTION); //Set a new projection matrix
	glLoadIdentity();
	gluPerspective(40.0f, (GLdouble)x / (GLdouble)y, 0.5f, 40.0f);
	glViewport(0, 0, x, y); //Use the whole window for rendering
}



/* This is our main method which will perform the setup of our fourth OpenGL
window. The command line parameters will be passed but not used in the
context of our application. Callbacks have been registered already and
are prepared for future use during the lab. */
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(500, 500); // initial position of the window
	glutInitWindowSize(800, 600); // initial size of the window
	windowid = glutCreateWindow("Pokemon Memory Game"); // create window
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	std::shuffle(v.begin(), v.end(), g);
	init_state();
	initTextures();

	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(onMouseClick);
	glutMainLoop(); // start the main loop of GLUT

	return 0;
}