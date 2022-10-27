/
//  main.cpp
//  DigitalFilters
//
//  Created by Jignesh Panchal on 10/8/22.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
//#include <conio.h>
#include <stdlib.h>
//#include <graphics.h>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <cstdio>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLUT
#include <GL/glut.h>

using namespace std;

#define SCALE1 100
#define SCALE 250
#define HEIGHT 50
#define N 513
#define L 1
#define NO 256
#define LE 43

void signal(void);
void bitrev(void);
void fft(void);
void display2(void);
void filter(void);

float a[N],b[N];
int n,m,si;
float pi=3.141582653;
float ha[N],w[N];
float x[N],fs;
char str[80],grd[80]="c:\\borlandc\\bgi";

//const GLint wIDTH = 800, hEIGHT = 600;

void drawLine(int x1, int y1, int x2, int y2)
{
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

/**
* Draw a character string.
*
* @param x        The x position
* @param y        The y position
* @param z        The z position
* @param string   The character string
*/
void drawString(int x, int y, char *string)
{
    glRasterPos2f(x, y);

    for (char* c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);  // Updates the position
    }
}

int initDisplay()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "Digital Filters", nullptr, nullptr);
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        
        return -1;
    }
    
    glViewport(0,0,screenWidth,screenHeight);
    
    glClear( GL_COLOR_BUFFER_BIT );
    glTranslatef( 0, 0, 0 );
    glDrawArrays(GL_TRIANGLES, 0, 100);
    glColor3f( 1.0, 1.0, 0.0 );
    glRecti( -20, 20, 20, -20 );
    
    //Monitor this flag and either destroy the window
    //or give some kind of feedback to the user.
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

void MyDisplay()
 {
    float ma,mha,mw,mb;
    int c,i,xm,ym;
    
    glColor3f( 1.0, 1.0, 0.0 );
    
    //Draw a rectangle
    //glRecti( -20, 20, 20, -20 );
    
    //Draw a line
    //
    glLineWidth(10);
    glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(-200, -200);
    glEnd();
    
    drawString(20,20,"Jignesh");
    
    glutSwapBuffers();
}

void display1()
{
    float ma,mha,mw,mb;
    int c,i,xm,ym;

    //cleardevice();
    //setcolor(RED);
    
    //xm=getmaxx();
    xm=800;
    //ym=getmaxy();
    ym=600;
    
    glRectf(0,30,xm,ym);
    
    drawString(165,5,str);
    
    drawString(10,35," Impulse respose of Filter ( Time domain ) ");
    drawString(2*LE+NO+50,35," Window ( Time domain ) ");
    drawString(10,10+ym/2," Magnitude Vs Frequency plot ");
    drawString(2*LE+NO+50,10+ym/2," Phase Vs Frequency plot ");
    
    ma=0;
    mw=0;
    mha=0;
    mb=0;

    for(c=1;c<=n;c++){
        if(fabs(a[c])>ma) ma=fabs(a[c]);
        if(fabs(b[c])>mb) mb=fabs(b[c]);
        if(fabs(w[c])>mw) mw=fabs(w[c]);
        if(fabs(ha[c])>mha) mha=fabs(ha[c]);
    }

    for(c=1;c<=n;c++){
        a[c]=(a[c]/ma)*HEIGHT;
        b[c]=(b[c]/mb)*HEIGHT;
        w[c]=(w[c]/mw)*HEIGHT;
        ha[c]=(ha[c]/mha)*HEIGHT;
    }
    
    drawLine(5,ym-SCALE,xm,ym-SCALE);
    drawLine(5,ym-SCALE1,xm,ym-SCALE1);
    
    glRasterPos2f(LE,ym-SCALE);
    glBegin(GL_LINES);
    for(i=0,c=1;c<=n;i=i+1,c+=L){
        glVertex2f(i+LE,ym-SCALE-ha[c]);
    }
    glVertex2f(i+LE,ym-SCALE);
    glEnd();
    
    glRasterPos2f(2*LE+NO,ym-SCALE);
    glBegin(GL_LINES);
    for(i=0,c=1;c<=n;i=i+1,c+=L){
        glVertex2f(i+2*LE+NO,ym-SCALE-w[c]);
    }
    glVertex2f(i+2*LE+NO,ym-SCALE);
    glEnd();
    
    glRasterPos2f(LE,ym-SCALE1);
    glBegin(GL_LINES);
    for(i=0,c=1;c<=n;i=i+1,c+=L){
        glVertex2f(i+LE,ym-SCALE1-a[c]);
    }
    glVertex2f(i+LE,ym-SCALE1);
    glEnd();
    
    glRasterPos2f(2*LE+NO,ym-SCALE1);
    glBegin(GL_LINES);
    for(i=0,c=1;c<=n;i=i+1,c+=L){
        glVertex2f(i+2*LE+NO,ym-SCALE1-b[c]);
    }
    glVertex2f(i+2*LE+NO,ym-SCALE1);
    glEnd();
    
    drawString(200,ym-10," Press any key to continue...");
    getchar();
    //cleardevice();
    
    glutSwapBuffers();
    //glutPostRedisplay();
    
    drawString(20,ym-20,"Please Wait...");
    
}

void Display( int x, int y )
{
    glClear( GL_COLOR_BUFFER_BIT );
    glTranslatef( x, y, 0 );
    //MyDisplay();
    display1();
}
void Mykeyboard( unsigned char c, int x, int y )
{
    switch( c )
    {
        case 'L': case 'l': Display( 0, 40 ); glutPostRedisplay(); break;
        case 'P': case 'p': Display( 0, -40 ); glutPostRedisplay(); break;
        case 'M': case 'm': Display( -40, 0 ); glutPostRedisplay(); break;
        case 'D': case 'd': Display( 40, 0 ); glutPostRedisplay(); break;
        default:
            cout << "Komande e pavlefshme...."; break;
   }
}

int initDisplayGLUT()
{
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(1000,600);
    glutCreateWindow("Digital Filters");
    
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glViewport( 0, 0, 800, 600 );
    gluOrtho2D( -200, 200, -200, 200 );
    
    //glutDisplayFunc( MyDisplay );
    glutDisplayFunc(display1);

    //glutKeyboardFunc( Mykeyboard );
    
    glutMainLoop();
    
    return 0;
}

int main(int argc, char *argv[])
{
    int i,j,no;
    char ch;
    int c;
    float t,fin,tmp,fc1,fc2,h[N];
    
    //int gd=EGA,gm=EGAHI;
    //clrscr();
    
    //initDisplay();
    glutInit(&argc, argv);
    initDisplayGLUT();
    
    printf("\nDesign of   F I R   filters ( FFT )");
    printf("\n===================================\n");
    
    printf("\nEnter no. of samples : ");
    scanf("%d",&n);
    
    for(i=0;i<=n;i++){
        h[i]=0;
        w[i]=0;
    }
    
    printf("\nEnter Length of filter (N) : ");
    scanf("%d",&no);
    
    printf("\nEnter frequency of sampling (Hz): ");
    scanf("%f",&fs);
    t=(1/fs);
    printf("Sampling time = %f sec.\n",t);
    
    //system("pause");
    
    printf("\nEnter choice of FILTER : ");
    printf("\n************************ ");
    printf("\nl O W   -  P A S S ");
    printf("\nh I G H -  P A S S ");
    printf("\nb A N D -  P A S S ");
    printf("\nB A N D -  s T O P ");
    printf("\nEnter your choice (l,h,b,s) : ");
    scanf("%1c",&ch);
    //ch = getchar(); getchar();
    //ch = getc(stdin); getc(stdin);
    //printf(ch);

    printf("\n");
    
    if(ch=='l'){
        fc1=0;
        printf("\nEnter CUT-OFF frequency (Hz) : ");
        scanf("%f",&fc2);
        strcpy(str," L o w   P a s s   F i t e r ");
    }
    
    if(ch=='h'){
        fc2=fs/2;
        printf("\nCut-off frequency of HPF should be less than %4.2lf Hz.",fc2);
        printf("\nEnter CUT-OFF frequency (Hz) : ");
        scanf("%f",&fc1);
        strcpy(str," H i g h   P a s s   F i t e r ");
    }
    
    if(ch=='b'){
        printf("\nEnter LOWER CUT-OFF frequency (Hz) : ");
        scanf("%f",&fc1);
        
        printf("\nEnter HIGHER CUT-OFF frequency (Hz) : ");
        scanf("%f",&fc2);
        strcpy(str," B a n d   P a s s   F i t e r ");
    }
    
    if(ch=='s'){
        printf("\nEnter LOWER CUT-OFF frequency (Hz) : ");
        scanf("%f",&fc1);
        
        printf("\nHigher Cut-off frequency of BSF should be less than %4.2lf Hz.",fs/2);
        printf("\nEnter HIGHER CUT-OFF frequency (Hz) : ");
        scanf("%f",&fc2);
        strcpy(str," B a n d   S t o p   F i t e r ");
        
        /*for(i=0;i<no;i++){
         h[i+1]=0;
         if(i!=0)h[i+1]=  (float)(1/(i*pi))*(sin((float)(2*pi*fc1*i*t))) +    (float)(1/(i*pi))*(sin((float)(pi*fs*i*t)) - sin((float)(2*pi*fc2*i*t)));
         else h[i+1]=  (float)((2/fs)*fc1)  +  (float)((2/fs)*((fs/2)-fc2));
         }*/
        
        for(j=-(no-1)/2,i=1;i<=no;i++,j++){
            h[i]=0;
            if(j!=0)h[i]= (float)(1/(j*pi))*(sin((float)(2*pi*fc1*j*t)))      +  (float)(1/(j*pi))*(sin((float)(pi*fs*j*t)) - sin((float)(2*pi*fc2*j*t)));
            else h[i]=(float)((2/fs)*fc1) + (float)((2/fs)*((fs/2)-fc2));
        }
    }
    
    
    if(ch!='s'){
        for(j=-(no-1)/2,i=1;i<=no;i++,j++){
            h[i]=0;
            if(j!=0)h[i]=(float)(1/(j*pi))*(sin((float)(2*pi*fc2*j*t)) - sin((float)(2*pi*fc1*j*t)));
            else h[i]=(float)((2/fs)*(fc2-fc1));
        }
    }
    
    
    printf("\nEnter choice of WINDOW : ");
    printf("\n************************ ");
    printf("\nr E C T A N G U L A R ");
    printf("\nb A R T L E T T ");
    printf("\nH a N N I N G ");
    printf("\nh A M M I N G ");
    printf("\nB l A C K M A N ");
    printf("\nEnter your choice (r,b,a,h,l) : ");
    //scanf("%c",&ch);
    ch = getchar(); getchar();
    printf("\n");
    
    if(ch=='r'){
        for(i=1;i<=no;i++){
            w[i]=1;
        }
    }
    
    if(ch=='h'){
        for(i=1;i<=no;i++){
            w[i]=0.54 - 0.46*cos((float)(2*pi*i/(no-1)));
        }
    }
    if(ch=='a'){
        for(i=1;i<=no;i++){
            w[i]=0.5 - 0.5*cos((float)(2*pi*i/(no-1)));
        }
    }
    if(ch=='l'){
        for(i=1;i<=no;i++){
            w[i]=0.42 - 0.5*cos((float)(2*pi*i/(no-1)))+0.08*cos((float)(4*pi*i/(no-1)));
        }
    }
    if(ch=='b'){
        for(i=1;i<no/2;i++){
            w[i]=(float)2.00*i/(no-1);
        }
        for(i=no/2;i<=no;i++){
            w[i]=2.00-(float)2.00*i/(no-1);
        }
    }
    for(i=1;i<=no;i++){
        ha[i]=h[i]*w[i];;
    }
    
    signal();
    
    printf("\nPlease wait... ");
    
    //initgraph(&gd,&gm,grd);
    initDisplay();
    
    filter();
    
    //closegraph();

    return 0;
}


void signal(void)
{
    int i,j;
    char ch;
    float fin;

    printf("\nEnter choice of Input SIGNAL : ");
    printf("\n****************************** ");
    printf("\ns I N E \n");
    printf("N o I S Y \n");
    printf("S q U A R E \n");
    printf("Enter your choice (s,o,q) : ");
    scanf("%c",&ch);
    //ch=getche();
    //getch();
        
    printf("\n");
        
    x[0]=0;
    if(ch=='s'){
        printf("\nEnter frequency of sine wave (Hz): ");
        scanf("%f",&fin);
        for(i=1;i<=n;i++){
            x[i]=sin(2*pi*(float)(fin*i/fs));
        }
    }
    
    //randomize();
    
    if(ch=='o'){
        printf("\nEnter choice of Noisy SIGNAL : ");
        printf("\n++++++++++++++++++++++++++++++ ");
        printf("\nn O I S E \n");
        printf("N o I S E + S I N E \n");
        printf("Enter your choice (n,o) : ");
        scanf("%c",&ch);
        //ch=getche();
        //getch();
                
        printf("\n");
        
        j=0;
        printf("\nEnter frequency of noisy sinewave signal (Hz): ");
        scanf("%f",&fin);
        for(i=1;i<=n;i++){
            
            x[i]=(float)(0.1*(rand()%10))+sin(2*pi*(float)(fin*i/fs));
            
            if(ch=='n'){
                if((rand()%10)>=5)j=-1;
                else j=1;
                x[i]=(x[i]*(float)j);
            }
            
        }
    }

    if(ch=='q'){
        for(i=1;i<n/4;i++){
            x[i]=0;
        }
        for(i=n/4;i<=n/2;i++){
            x[i]=1;
        }
        for(i=n/2;i<3*n/4;i++){
            x[i]=-1;
        }
        for(i=3*n/4;i<=n;i++){
            x[i]=0;
        }
    }


}




void filter(void)
{
    int i,j;
    char ch,st[80];
    float t,fs,fin,ax[N],bx[N];

        for(i=0;i<N;i++){
        a[i]=0;
        b[i]=0;
        ax[i]=0;
        bx[i]=0;
    }

        m=0;
    i=1;
    while(n>i){
        m++;
        i=pow(2,(float)m);
    }

    for(i=1;i<=n;i++){
        a[i]=ha[i];
        b[i]=0;
    }
    si=-1;
    bitrev();
    fft();

      for(i=1;i<=n;i++){
        ax[i]=a[i];
        bx[i]=b[i];
        a[i]=0;
        a[i]=(sqrt(pow(ax[i],2) + pow(bx[i],2)));
        b[i]=0;
        b[i]=-atan(bx[i]/ax[i]);
    }
    
    initDisplayGLUT();
    //display1();

    for(i=1;i<=n;i++){
            a[i]=0;
        a[i]=x[i];
        b[i]=0;
    }
    bitrev();
    si=-1;
    fft();

    for(i=1;i<=n;i++){
        b[i] = (a[i]*bx[i]) + (ax[i]*b[i]);
        a[i] = (a[i]*ax[i]) - (b[i]*bx[i]);
        b[i] = b[i]/n;
        a[i] = a[i]/n;
    }

    bitrev();
    si=1;
    fft();
    
    //display2();
}



void bitrev(void)
{
    int n2,n1,i,j,k;
    float ta,tb;

    n2=(int)(n/2);
    n1=n-1;
    j=1;
    for(i=1;i<=n1;i++){
        if(i<j){
            ta=a[j];
            a[j]=a[i];
            a[i]=ta;
            tb=b[j];
            b[j]=b[i];
            b[i]=tb;
        }
        k=n2;
           while(k<j){
            j-=k;
            k=(int)(k/2);
        }

        j+=k;
    }
}



void fft(void)
{
    int l,le,l2,g,h,ip;
    float wa,wb,va,vb,ua[N],ub[N];

    for(l=1;l<=m;l++){
        le=(int)pow(2,(float)l);
        l2=(int)(le/2);
        ua[1]=1.0;
        ub[1]=0.0;
        wa=cos(pi/(float)l2);
        wb=(float)si*sin(pi/(float)l2);
        for(g=1;g<=l2;g++){
            for(h=g;h<=n;h+=le){
                ip = h + l2;
                va=(a[ip]*ua[g]) - (b[ip]*ub[g]);
                vb=(b[ip]*ua[g]) + (a[ip]*ub[g]);
                a[ip]=a[h]-va;
                b[ip]=b[h]-vb;
                a[h]=a[h]+va;
                b[h]=b[h]+vb;
            }
            ua[g+1]=(ua[g]*wa) - (ub[g]*wb);
            ub[g+1]=(ub[g]*wa) + (ua[g]*wb);
        }

    }
}