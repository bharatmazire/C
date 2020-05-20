// headers
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import <QuartzCore/CVDisplayLink.h>

#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

#import "vmath.h"

#define M_PI 3.14159265358979323846

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_NORMAL,
    AMC_ATTRIBUTE_TEXCOORD0,
};

struct vec_int* create_vec_int(void);
void push_vec_int(struct vec_int*, int);
void show_vec_int(struct vec_int*);
void destroy_vec_int(struct vec_int*);

struct vec_float* create_vec_float(void);
void push_vec_float(struct vec_float*, float);
void show_vec_float(struct vec_float*);
void destroy_vec_float(struct vec_float*);

struct vec_int
{
    int* data;
    int size;
};

struct vec_float
{
    float* data;
    int size;
};


// graph triangle
GLuint  vaoForTriangle;
GLuint  vboForPositionOfTriangle;
GLuint  vboForColorOfTriangle;

// graph in circle
GLuint  vaoForSmallCircle;
GLuint  vboForPositionOfSmallCircle;
GLuint  vboForColorOfSmallCircle;

// graph line
GLuint  vaoForLine;
GLuint  vboForPositionOfLine;
GLuint  vboForColorOfLine;

// variables for shapes
GLfloat radius;

GLfloat Ax = 0.0f;
GLfloat Ay = 0.5f;

GLfloat Bx = -0.5f;
GLfloat By = -0.5f;

GLfloat Cx = 0.5f;
GLfloat Cy = -0.5f;

GLfloat Ox = 0.0f;
GLfloat Oy = 0.0f;

GLfloat AreaOfTriangle;
GLfloat Height;

GLfloat a, b, c, s = 0.0f;

GLfloat XforTriangle = 3.0f;
GLfloat ZforTriangle = 0.0f;

GLfloat XforCircle = -3.0f;
GLfloat ZforCircle = 0.0f;

GLfloat YforLine = 3.0f;

GLfloat angle_rotation = 0.0f;

struct vec_float* colorForIncircle;
struct vec_float* verticesForIncircle;


// 'C' style global function declarations
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp *, const CVTimeStamp *, CVOptionFlags, CVOptionFlags *, void *);

// global variables
FILE *gpFile = NULL;

// interface declaration
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface GLView : NSOpenGLView
@end

// Entry-Point function
int main(int argc, const char * argv[])
{
    // code
    NSAutoreleasePool *pPool = [[NSAutoreleasePool alloc]init];
    NSApp = [NSApplication sharedApplication];
    [NSApp setDelegate : [[AppDelegate alloc]init]];
    [NSApp run];
    [pPool release];
    
    return(0);
}

// interface implementation
@implementation AppDelegate
{
@private
    NSWindow *window;
    GLView *glView;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // code
    // log file
    NSBundle *mainBundle            = [NSBundle mainBundle];
    NSString *appDirName            = [mainBundle bundlePath];
    NSString *parentDirPath         = [appDirName stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath   = [NSString stringWithFormat:@"%@/Log.txt",parentDirPath];
    const char *pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];
    gpFile=fopen(pszLogFileNameWithPath,"w");
    if(gpFile == NULL)
    {
        printf("Can not create log file. \n Exiting... \n");
        [self release];
        [NSApp terminate:self];
    }
    fprintf(gpFile,"Program is started sucessfully\n");

    // window
    NSRect win_rect;
    win_rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);

    // create simple window
    window = [[NSWindow alloc] initWithContentRect:win_rect
                                        styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                backing:NSBackingStoreBuffered
                                defer:NO];
    [window setTitle:@"macOS OpenGL Window"];
    [window center];

    glView=[[GLView alloc]initWithFrame:win_rect];

    [window setContentView:glView];
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
    // code
    fprintf(gpFile,"Program is Terminated sucessfully\n");

    if(gpFile)
    {
        fclose(gpFile);
        gpFile = NULL;
    }
}

- (void)windowWillClose:(NSNotification *)notification
{
    // code
    [NSApp terminate:self];
}

- (void)dealloc
{
    // code
    [glView release];
    [window release];
    [super dealloc];
}
@end

@implementation GLView
{
@private
    CVDisplayLinkRef displayLink;

    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;

    //GLuint vao;
    //GLuint vbo;
    
    GLuint mvpUniform;

    vmath::mat4 projectionMatrix;
}

-(id)initWithFrame:(NSRect)frame;
{
    // code
    self = [super initWithFrame:frame];

    if(self)
    {
        [[self window]setContentView:self];

        NSOpenGLPixelFormatAttribute attrs[] =
        {
            // Must specify the 4.1 core profile to use openGL 4.1
            NSOpenGLPFAOpenGLProfile,
            NSOpenGLProfileVersion4_1Core,
            // specify the display id to associate the gl context with (main display for now)
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFAColorSize,24,
            NSOpenGLPFADepthSize,24,
            NSOpenGLPFAAlphaSize,8,
            NSOpenGLPFADoubleBuffer,
            0}; // last 0 is must

        NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc]initWithAttributes:attrs] autorelease];

        if(pixelFormat == nil)
        {
            fprintf(gpFile,"No Valid OpenGL pixel format is available. Exiting ...\n");
            [self release];
            [NSApp terminate:self];
        }

        NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil] autorelease];
        [self setPixelFormat:pixelFormat];
        [self setOpenGLContext:glContext]; // it automatically release the older context, if present, and set the newer one
    }
    return(self);
}

-(CVReturn)getFrameForTime:(const CVTimeStamp *)pOutputTime
{
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    [self drawView];
    [pool release];
    return(kCVReturnSuccess);
}

-(void)prepareOpenGL
{
    // code
    // openGL info
    fprintf(gpFile,"OpenGL Version : %s \n",glGetString(GL_VERSION));
    fprintf(gpFile,"GLSL version : %s \n",glGetString(GL_SHADING_LANGUAGE_VERSION));

    [[self openGLContext]makeCurrentContext];

    GLint swapInt = 1;
    [[self openGLContext]setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

    // ** vertex shader
    // create shader
    vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    // provide source code to shader
    const GLchar *vertexShaderSourceCode =
        "#version 410" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec4 vColor;"  \
        "out vec4 out_color;" \
        "uniform mat4 u_mvp_matrix;" \
        "void main(void)" \
        "{" \
            "out_color = vColor;" \
            "gl_Position = u_mvp_matrix * vPosition;" \
        "}";
    glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

    // compile shader
    glCompileShader(vertexShaderObject);
    GLint iInfoLogLength = 0;
    GLint iShaderCompiledStatus = 0;
    char *szInfoLog = NULL;
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
    if(iShaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (GLchar *)malloc(iInfoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObject, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile, "\n--------------------------------------------------- \nCompile time log for Vertex Shader : \n %s", szInfoLog);
                fflush(gpFile);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    // Fragment shader
    // reinitialzie
    iInfoLogLength = 0;
    iShaderCompiledStatus = 0;
    szInfoLog = NULL;

    // create shader
    fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    // provide source code to shader
    const GLchar *fragmentShaderSourceCode =
        "#version 410" \
        "\n" \
        "in vec4 out_color;" \
        "out vec4 FragColor;" \
        "void main(void)" \
        "{" \
            "FragColor = out_color;" \
        "}";
    glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

    // compile shader
    glCompileShader(fragmentShaderObject);
    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
    if(iShaderCompiledStatus == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (GLchar *)malloc(iInfoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragmentShaderObject, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile, "\n--------------------------------------------------- \nCompile time log for Fragment Shader : \n %s", szInfoLog);
                fflush(gpFile);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }
    // Shader program
    shaderProgramObject = glCreateProgram();
    glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject,fragmentShaderObject);

    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "vPosition");
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "vColor");
    
    glLinkProgram(shaderProgramObject);
    GLint iShaderProgramLinkStatus = 0;
    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
    if (iShaderProgramLinkStatus == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
        if (iInfoLogLength > 0)
        {
            szInfoLog = (GLchar *)malloc(iInfoLogLength);
            if (szInfoLog != NULL)
            {
                GLsizei written;
                glGetProgramInfoLog(shaderProgramObject, iInfoLogLength, &written, szInfoLog);
                fprintf(gpFile, "\n--------------------------------------------------- \nLink time log for Shader Program : \n %s", szInfoLog);
                fflush(gpFile);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
            }
        }
    }

    mvpUniform = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
  

    
    // Triangle

    const GLfloat triangleVertices[] =
    {
        Ax,Ay,
        Bx,By,
        Cx,Cy
    };
    const GLfloat triangleColores[] =
    {
        1.0f,0.0f,0.0f,0.8f,
        1.0f,0.0f,1.0f,0.5f,
        1.0f,0.0f,1.0f,0.5f
    };
    // create vao
    glGenVertexArrays(1, &vaoForTriangle);
    glBindVertexArray(vaoForTriangle);

    // vbo position graph
    glGenBuffers(1, &vboForPositionOfTriangle);
    glBindBuffer(GL_ARRAY_BUFFER, vboForPositionOfTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo color graph
    glGenBuffers(1, &vboForColorOfTriangle);
    glBindBuffer(GL_ARRAY_BUFFER, vboForColorOfTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColores), triangleColores, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind vao
    glBindVertexArray(0);

    // calculate sides
    c = (GLfloat)sqrt((Ax - Bx)*(Ax - Bx) + (Ay - By)*(Ay - By));
    a = (GLfloat)sqrt((Bx - Cx)*(Bx - Cx) + (By - Cy)*(By - Cy));
    b = (GLfloat)sqrt((Ax - Cx)*(Ax - Cx) + (Ay - Cy)*(Ay - Cy));

    // origin
    Ox = (a*Ax + b * Bx + c * Cx) / (a + b + c);
    Oy = (a*Ay + b * By + c * Cy) / (a + b + c);

    // perimeter
    s = (a + b + c) / 2.0f;

    // area of triangle
    AreaOfTriangle = (GLfloat)sqrt(s*(s - a)*(s - b)*(s - c));

    // radius
    radius = AreaOfTriangle / s;

    // In Circle
    for (GLfloat angle = 0.0f; angle <= (2.0f * M_PI); angle = angle + 0.001f)
    {
        for (int i = 0; i < 6; i++)
        {
            push_vec_float(colorForIncircle,1.0f - (i * (1.0f / 6.0f)));
        }
        push_vec_float(verticesForIncircle,(GLfloat)cos(angle)* radius + Ox);
        push_vec_float(verticesForIncircle,(GLfloat)sin(angle)* radius + Oy);

    }

    // create vao
    glGenVertexArrays(1, &vaoForSmallCircle);
    glBindVertexArray(vaoForSmallCircle);

    // vbo position graph
    glGenBuffers(1, &vboForPositionOfSmallCircle);
    glBindBuffer(GL_ARRAY_BUFFER, vboForPositionOfSmallCircle);
    glBufferData(GL_ARRAY_BUFFER, verticesForIncircle->size * sizeof(float), verticesForIncircle->data, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo color graph
    glGenBuffers(1, &vboForColorOfSmallCircle);
    glBindBuffer(GL_ARRAY_BUFFER, vboForColorOfSmallCircle);
    glBufferData(GL_ARRAY_BUFFER, colorForIncircle->size * sizeof(float), colorForIncircle->data, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind vao
    glBindVertexArray(0);

    // Square

    const GLfloat lineVertices[] =
    {
        0.0f, 0.5f,
        0.0f, -0.5f
    };

    const GLfloat lineColors[] =
    {
        1.0f,0.0f,1.0f,1.0f,
        1.0f,0.0f,1.0f,1.0f,
    };

    // create vao
    glGenVertexArrays(1, &vaoForLine);
    glBindVertexArray(vaoForLine);

    // vbo position graph
    glGenBuffers(1, &vboForPositionOfLine);
    glBindBuffer(GL_ARRAY_BUFFER, vboForPositionOfLine);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vbo color graph
    glGenBuffers(1, &vboForColorOfLine);
    glBindBuffer(GL_ARRAY_BUFFER, vboForColorOfLine);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineColors), lineColors, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind vao
    glBindVertexArray(0);


    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    // set background color
    glClearColor(1.0f,1.0f,1.0f,1.0f); // blue

    projectionMatrix = vmath::mat4::identity();

    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink,&MyDisplayLinkCallback,self);
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink,cglContext,cglPixelFormat);
    CVDisplayLinkStart(displayLink);
}

-(void)reshape
{
    // code
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

    NSRect rect = [self bounds];

    GLfloat width = rect.size.width;
    GLfloat height = rect.size.height;

    if(height == 0)
        height = 1;

    glViewport(0,0,(GLsizei)width, (GLsizei)height);

    projectionMatrix = vmath::perspective(45.0f, width/height, 0.1f, 100.0f);

    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

- (void)drawRect:(NSRect)dirtyRect
{
    //code
    [self drawView];
}

- (void)drawView
{
    // code
   
    [[self openGLContext]makeCurrentContext];

    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgramObject);

    vmath::mat4 translateMatrix;
    vmath::mat4 rotationMatrix;

    vmath::mat4 modelViewMatrix;
    vmath::mat4 modelViewProjectionMatrix;


    // ................... LINE
    translateMatrix 			= vmath::mat4::identity();
    rotationMatrix 				= vmath::mat4::identity();
    modelViewMatrix 			= vmath::mat4::identity();
    modelViewProjectionMatrix 	= vmath::mat4::identity();

    modelViewMatrix 			= vmath::translate(0.0f,YforLine,-3.0f);

    modelViewProjectionMatrix 	= projectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, modelViewProjectionMatrix);
    
    glBindVertexArray(vaoForLine);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);


    // ....................... TRIANGLE
    translateMatrix 			= vmath::mat4::identity();
    rotationMatrix 				= vmath::mat4::identity();
    modelViewMatrix 			= vmath::mat4::identity();
    modelViewProjectionMatrix 	= vmath::mat4::identity();

    modelViewMatrix 			= vmath::translate(XforTriangle,0.0f,ZforTriangle);
    rotationMatrix				= vmath::rotate(angle_rotation,0.0f,1.0f,0.0f);
    modelViewMatrix 			= modelViewMatrix * rotationMatrix;

    modelViewProjectionMatrix 	= projectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    
    glBindVertexArray(vaoForTriangle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    glBindVertexArray(0);
    


    // ..................... CIRCLE 
    translateMatrix 			= vmath::mat4::identity();
    rotationMatrix 				= vmath::mat4::identity();
    modelViewMatrix 			= vmath::mat4::identity();
    modelViewProjectionMatrix 	= vmath::mat4::identity();

    modelViewMatrix 			= vmath::translate(XforCircle,0.0f,ZforCircle);
    rotationMatrix				= vmath::rotate(angle_rotation,0.0f,1.0f,0.0f);
    modelViewMatrix 			= modelViewMatrix * rotationMatrix;

    modelViewProjectionMatrix 	= projectionMatrix * modelViewMatrix;

    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    glBindVertexArray(vaoForSmallCircle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, verticesForIncircle->size);
    glBindVertexArray(0);
    
    glUseProgram(0);

    CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);


    // update
    if(angle_rotation < 360.0f)
    	angle_rotation += 0.1f;
    else
    	angle_rotation = 0.0f;

    if(YforLine > 0.0f)
    	YforLine -= 0.01f;

    if(XforTriangle > 0.0f)
    	XforTriangle -= 0.01;

    if(XforCircle < 0.0f)
    	XforCircle += 0.01f;

    if(ZforCircle > -3.0f)
    {
    	ZforCircle -= 0.01f;
    	ZforTriangle -= 0.01f;
    }

}

- (BOOL)acceptsFirstResponder
{
    // code
    [[self window]makeFirstResponder:self];
    return(YES);
}

- (void)keyDown:(NSEvent *)theEvent
{
    // code
    int key=(int)[[theEvent characters]characterAtIndex:0];
    switch(key)
    {
        case 27: // Esc key
            [self release];
            [NSApp terminate:self];
            break;
        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self]; // repainting occurs automatically
            break;
        default:
            break;
    }
}

- (void)mouseDown:(NSEvent *)theEvent
{
    // code
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    // code
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
    // code
}

- (void) dealloc
{
    // code
    glDetachShader(shaderProgramObject, vertexShaderObject);
    glDetachShader(shaderProgramObject, fragmentShaderObject);

    glDeleteShader(vertexShaderObject);
    vertexShaderObject = 0;

    glDeleteShader(fragmentShaderObject);
    fragmentShaderObject = 0;

    glDeleteProgram(shaderProgramObject);
    shaderProgramObject = 0;

    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);

    [super dealloc];
}

@end

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *pNow, const CVTimeStamp *pOutputTime, CVOptionFlags flagsIn,
                                CVOptionFlags *pFlagsOut, void *pDisplayLinkContext)
{
    CVReturn result = [(GLView *)pDisplayLinkContext getFrameForTime:pOutputTime];
    return(result);
}

struct vec_int* create_vec_int(void)
{
    fprintf(gpFile,"Come Till Here create_vec_int() \n");
    struct vec_int* temp_struct_vec_int = NULL;
    temp_struct_vec_int = (struct vec_int*)malloc(sizeof(struct vec_int));
    memset(temp_struct_vec_int, 0, sizeof(struct vec_int));
    return(temp_struct_vec_int);
}

void push_vec_int(struct vec_int* temp_struct_vec_int, int new_data)
{
    temp_struct_vec_int->data = (int*)realloc(temp_struct_vec_int->data, ((temp_struct_vec_int->size) + 1) * sizeof(int));
    temp_struct_vec_int->size = temp_struct_vec_int->size + 1;
    temp_struct_vec_int->data[(temp_struct_vec_int->size) - 1] = new_data;
}

void destroy_vec_int(struct vec_int* temp_struct_vec_int)
{
    free(temp_struct_vec_int->data);
    free(temp_struct_vec_int);
    temp_struct_vec_int = NULL;
}


struct vec_float* create_vec_float(void)
{
    fprintf(gpFile,"Come Till Here create_vec_float() \n");
    struct vec_float* temp_struct_vec_float = NULL;
    temp_struct_vec_float = (struct vec_float*)malloc(sizeof(struct vec_float));
    memset(temp_struct_vec_float, 0, sizeof(struct vec_float));
    return(temp_struct_vec_float);
}

void push_vec_float(struct vec_float* temp_struct_vec_float, float new_data)
{
    //fprintf(gpFile,"Come Till Here push_vec_float() \n");
    temp_struct_vec_float->data = (float*)realloc(temp_struct_vec_float->data, ((temp_struct_vec_float->size) + 1) * sizeof(float));
    temp_struct_vec_float->size = temp_struct_vec_float->size + 1;
    temp_struct_vec_float->data[(temp_struct_vec_float->size) - 1] = new_data;
}

 void destroy_vec_float(struct vec_float* temp_struct_vec_float)
{
    free(temp_struct_vec_float->data);
    free(temp_struct_vec_float);
    temp_struct_vec_float = NULL;
}

void show_vec_int(struct vec_int* temp_struct_vec_int)
{
    fprintf(gpFile,"size of temp_struct_vec_int is : %d \n",temp_struct_vec_int->size);
    for(int i = 0 ; i < temp_struct_vec_int->size; i++)
    {
        fprintf(gpFile,"vec_int_ver[%d] : %d \n",i,temp_struct_vec_int->data[i]);
    }
}

void show_vec_float(struct vec_float* temp_struct_vec_float)
{
    fprintf(gpFile,"size of temp_struct_vec_float is : %d \n",temp_struct_vec_float->size);
    for(int i = 0 ; i < temp_struct_vec_float->size; i++)
    {
        fprintf(gpFile,"vec_int_ver[%d] : %f \n",i,temp_struct_vec_float->data[i]);
    }
}
