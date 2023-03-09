#include <windows.h>
#include <gl.h>
#include <glu.h>
#include <glaux.h>
#include <cmath>
#include "tree.h"
#include "land.h"
#include "dinosaur.h"
#include "texture.h"
#include<glut.h>
#include <string>


HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

bool startGame=false;
bool GameOver,Win;
int dinoImage=0,dinoSize,bestScore,curScore;
double speed,jump,TreeX,dis;
double treeX,treeY,treeCounter,treeSize;
int numberOfTrees,numberOfJumps,cntJump;

bool onGround=true,isFalling=false;
double gravity=0.5,dinoY=0,dinoX=0,jumpedValue;
void initGameLogic();
void controll()
{
	if(keys['S']&&!startGame)
	{
		initGameLogic();
		startGame^=1;
	}
	if(keys['R']&&(Win||GameOver))
	{
		startGame=true;
		initGameLogic();
	}
	if(keys['Q'])
	{
		exit(0);
	}
	if(keys[VK_SPACE]&&cntJump<=numberOfJumps&&onGround)
	{
		cntJump++;
		if(isFalling)
		{
			if(dinoY<=0)
			{
				isFalling=false;
				onGround=true;
				dinoY=0;
				jumpedValue=0;
			}
			else
			{
				jumpedValue-=gravity;
				dinoY-=gravity;
			}
		}
		else
		{
			onGround=false;
			if(dinoY<jump)
			{
				dinoY+=gravity;
				jumpedValue+=gravity;
			}
			else
			{
				isFalling=true;
			} 
		}
	}
	if(!onGround)
	{
		if(isFalling)
		{
			if(dinoY<=0)
			{
				isFalling=false;
				onGround=true;
				dinoY=0;
				jumpedValue=0;
			}
			else
			{
				jumpedValue-=gravity;
				dinoY-=gravity;
			}
		}
		else
		{
			onGround=false;
			if(dinoY<jump)
			{
				dinoY+=gravity;
				jumpedValue+=gravity;
			}
			else
			{
				isFalling=true;
			} 
		}
	}

}

void initGameLogic()
{
	numberOfTrees=5;
	numberOfJumps=3;
	cntJump=0;
	speed=0.3;
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	jump=5;
	treeX=0;
	TreeX=13;
	treeY=2;
	treeCounter=0;
	gravity=0.5;
	dinoY=0;
	dinoX=-10;
	dinoSize=3;
	onGround=true;
	isFalling=false;
	treeSize=0.6;
	dis=0;
	GameOver=false;
	Win=false;
	curScore=0;
}


int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	
	dinoImage=LoadTexture("dino.bmp",255);
	
	initGameLogic();
	return TRUE;										// Initialization Went OK
}

void drawBitmapText(char *string,float x,float y,float z) {    char *c;  glRasterPos3f(x, y,z);   for (c=string; *c != '\0'; c++)   {     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);   } } 


void DrawGLScene(GLvoid) // main function for me									// Here's Where We Do All The Drawing
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslated(0,-5,-15);
	
	// function to controll the game
	controll();

	if(GameOver)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
		glColor3f(1.0f,0.0f,0.0);
		drawBitmapText("GAME OVER",-2,10,0);
		glColor3f(1.0f,1.0f,1.0);
		drawBitmapText("To restart the game press 'R'",-2,7,0);
		drawBitmapText("To Exit the game press 'Q'",-2,3,0);
		std::string best="Best Score: ";
		std::string temp=std::to_string(bestScore);
		best+=temp;
		best+=" / ";
		temp=std::to_string(numberOfTrees);
		best+=temp;
		char best_array[200];
		std::strcpy(best_array,best.c_str());
		drawBitmapText(best_array,-6,1,0);

		std::string cur="Cur Score: ";
		temp=std::to_string(curScore);
		cur+=temp;
		cur+=" / ";
		temp=std::to_string(numberOfTrees);
		cur+=temp;
		char cur_array[200];
		std::strcpy(cur_array,cur.c_str());
		drawBitmapText(cur_array,0,1,0);

	}
	else if(Win)
	{
		glColor3f(0.0f,1.0f,0.0);
		drawBitmapText("CONGRATULATIONS YOU WON THE GAME",-2,10,0);
		glColor3f(1.0f,1.0f,1.0);
		drawBitmapText("To restart the game press 'R'",-2,7,0);
		drawBitmapText("To Exit the game press 'Q'",-2,5,0);
		std::string best="Best Score: ";
		std::string temp=std::to_string(bestScore);
		best+=temp;
		best+=" / ";
		temp=std::to_string(numberOfTrees);
		best+=temp;
		char best_array[200];
		std::strcpy(best_array,best.c_str());
		drawBitmapText(best_array,-6,1,0);

		std::string cur="Cur Score: ";
		temp=std::to_string(curScore);
		cur+=temp;
		cur+=" / ";
		temp=std::to_string(numberOfTrees);
		cur+=temp;
		char cur_array[200];
		std::strcpy(cur_array,cur.c_str());
		drawBitmapText(cur_array,0,1,0);
	}
	else
	{
		// starting game logic
		if(startGame)
		{
			// draw land and sky
			glPushMatrix();
			land l1;
			l1.drawLand();
			glPopMatrix();

			// draw dinosaur 
			glPushMatrix();
			glTranslated(0,dinoY,0);
			dinosaur d(dinoX,0,dinoImage,dinoSize);
			d.drawDinosaur();
			glPopMatrix();
			glPushMatrix();
			glTranslated(treeX,0,0);
			if(treeCounter<numberOfTrees)
			{
				tree t1(TreeX,0);
				t1.drawTree();
			}
			treeX-=speed;
			dis+=speed;
			float curX=TreeX-dis;
			float curY=jumpedValue;
			if(((dinoX>=curX&&dinoX<=curX+treeSize)||(dinoX+dinoSize-0.8>=curX&&dinoX+dinoSize-0.8<=curX+treeSize))&&curY<=treeY)
			{
				bestScore=max(bestScore,curScore);
				GameOver=true;
			}
			if(treeX<-25)
			{
				treeCounter++;
				curScore++;
				speed+=0.1;
				gravity+=0.1;
				if(treeCounter==numberOfTrees&&cntJump>=numberOfJumps)
				{
					bestScore=max(bestScore,curScore);
					Win=true;
				}
				treeX=0;
				dis=0;
			}
			if(treeCounter==numberOfTrees)
			{
				bestScore=max(bestScore,curScore);
				Win=true;
			}
			glPopMatrix();
		}
		else
		{
			drawBitmapText("To start the game press 's'",-2,10,0);
			drawBitmapText("To Exit the game press 'Q'",-2,5,0);
		}
	}
	//DO NOT REMOVE THIS
	SwapBuffers(hDC);			
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	static PAINTSTRUCT ps;

	switch (uMsg)									// Check For Windows Messages
	{
		case WM_PAINT: 
			DrawGLScene();
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;
	
		case WM_TIMER:
			DrawGLScene();
			return 0;

		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Dinosaur Game",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}


	//Set drawing timer to 20 frame per second
	UINT timer=SetTimer(hWnd,0,50,(TIMERPROC) NULL);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
			TranslateMessage(&msg);	
			DispatchMessage(&msg);
	}

	return 0;

}
