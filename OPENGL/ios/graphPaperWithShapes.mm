#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "vmath.h"

#import "GLESView.h"

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

// graph paper
GLuint  vaoForGraphPaper;
GLuint  vboForPositionOfGraphPaper;
GLuint  vboForColorOfGraphPaper;

// graph out circle
GLuint  vaoForBigCircle;
GLuint  vboForPositionOfBigCircle;
GLuint  vboForColorOfBigCircle;

// graph rectangle
GLuint  vaoForRectangle;
GLuint  vboForPositionOfRectangle;
GLuint  vboForColorOfRectangle;

// graph triangle
GLuint  vaoForTriangle;
GLuint  vboForPositionOfTriangle;
GLuint  vboForColorOfTriangle;

// graph in circle
GLuint  vaoForSmallCircle;
GLuint  vboForPositionOfSmallCircle;
GLuint  vboForColorOfSmallCircle;

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


struct vec_float* colorForGraphPaper;
struct vec_float* verticesForGraphPaper;

struct vec_float* colorForIncircle;
struct vec_float* verticesForIncircle;

struct vec_float* colorForOutCircle;
struct vec_float* verticesForOutCircle;


@implementation GLESView
{
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderBuffer;
    GLuint depthRenderBuffer;
    
    id displayLink;
    NSInteger animationFrameInterval;
    BOOL isAnimating;
    
    GLuint vertexShaderObject;
    GLuint fragmentShaderObject;
    GLuint shaderProgramObject;

    GLuint vao;
    GLuint vbo;
    
    GLuint mvpUniform;

    vmath::mat4 projectionMatrix;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if(self)
    {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)super.layer;
        
        eaglLayer.opaque                = YES;
        eaglLayer.drawableProperties    = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8,kEAGLDrawablePropertyColorFormat, nil];
        eaglContext                     = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if(eaglContext==nil)
        {
            [self release];
            return(nil);
        }
        [EAGLContext setCurrentContext:eaglContext];
        
        glGenFramebuffers(1, &defaultFramebuffer);
        glGenRenderbuffers(1, &colorRenderBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
        
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
        
        GLint backingWidth;
        GLint backingHeight;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Failed to create complete frame buffer object %x\n",glCheckFramebufferStatus(GL_FRAMEBUFFER));
            glDeleteFramebuffers(1, &defaultFramebuffer);
            glDeleteRenderbuffers(1, &colorRenderBuffer);
            glDeleteRenderbuffers(1, &depthRenderBuffer);
            
            return(nil);
        }
        printf("Renderer : %s | GL VERSION : %s | GLSL VERSION : %s \n", glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        isAnimating             = NO;
        animationFrameInterval  = 60;
        
        // VERTEX SHADER
        
        vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
        const GLchar *vertexShaderSourceCode =
            "#version 300 es" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec4 vColor;"  \

            "uniform mat4 u_mvp_matrix;" \
            "void main(void)" \
            "{" \
                "out_color = vColor;" \
                "gl_Position = u_mvp_matrix * vPosition;" \
            "}";
        glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

        // compile shader
        glCompileShader(vertexShaderObject);
        
        GLint iInfoLogLength        = 0;
        GLint iShaderCompiledStatus = 0;
        char *szInfoLog             = NULL;
        
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
                    printf("\n--------------------------------------------------- \nCompile time log for Vertex Shader : \n %s", szInfoLog);
                    free(szInfoLog);
                    [self release];
                }
            }
        }

        // Fragment shader
        // reinitialzie
        iInfoLogLength          = 0;
        iShaderCompiledStatus   = 0;
        szInfoLog               = NULL;

        // create shader
        fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

        // provide source code to shader
        const GLchar *fragmentShaderSourceCode =
            "#version 300 es" \
            "\n" \
            "precision highp float;" \
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
                    printf("\n--------------------------------------------------- \nCompile time log for Fragment Shader : \n %s", szInfoLog);
                    free(szInfoLog);
                    [self release];
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
                      printf("\n--------------------------------------------------- \nLink time log for Shader Program : \n %s", szInfoLog);
                      free(szInfoLog);
                      [self release];
                  }
              }
          }

          mvpUniform = glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");
        
        /*
        // pyramid
         const GLfloat TriangleVertices[] =
         {
             0.0f, 1.0f, 0.0f,
             -1.0f, -1.0f, 1.0f,
             1.0f, -1.0f, 1.0f,
         };

        

         // create vao
         glGenVertexArrays(1, &vao);
         glBindVertexArray(vao);

         // create vbo position
         glGenBuffers(1, &vbo);
         glBindBuffer(GL_ARRAY_BUFFER, vbo);
         glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);
         glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
         glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
         glBindBuffer(GL_ARRAY_BUFFER, 0);


         glBindVertexArray(0);
        */

        colorForGraphPaper      = create_vec_float();
        verticesForGraphPaper   = create_vec_float();

        colorForIncircle    = create_vec_float();
        verticesForIncircle = create_vec_float();

        colorForOutCircle       = create_vec_float();
        verticesForOutCircle    = create_vec_float();


        // positions and color
        for (GLfloat i = -1.0f; i <= 1.05f; i = i + 0.05f)
        {
            // vertical
            
            push_vec_float(colorForOutCircle,0.0f);
            push_vec_float(colorForOutCircle,0.0f);
            push_vec_float(colorForOutCircle,0.0f);
            push_vec_float(colorForOutCircle,1.0f);
            
            push_vec_float(verticesForGraphPaper,i);
            push_vec_float(verticesForGraphPaper,1.0f);
            push_vec_float(verticesForGraphPaper,0.0f);

            
            push_vec_float(colorForOutCircle,0.0f);
            push_vec_float(colorForOutCircle,0.0f);
            push_vec_float(colorForOutCircle,0.0f);
            push_vec_float(colorForOutCircle,1.0f);
           
            push_vec_float(verticesForGraphPaper,i);
            push_vec_float(verticesForGraphPaper,-1.0f);
            push_vec_float(verticesForGraphPaper,0.0f);

            // horizontal
            push_vec_float(colorForGraphPaper,0.0f);
            push_vec_float(colorForGraphPaper,0.0f);
            push_vec_float(colorForGraphPaper,0.0f);
            push_vec_float(colorForGraphPaper,1.0f);
            
            push_vec_float(verticesForGraphPaper,-1.0f);
            push_vec_float(verticesForGraphPaper,i);
            push_vec_float(verticesForGraphPaper,0.0f);

            push_vec_float(colorForGraphPaper,0.0f);
            push_vec_float(colorForGraphPaper,0.0f);
            push_vec_float(colorForGraphPaper,0.0f);
            push_vec_float(colorForGraphPaper,1.0f);
            
            push_vec_float(verticesForGraphPaper,1.0f);
            push_vec_float(verticesForGraphPaper,i);
            push_vec_float(verticesForGraphPaper,0.0f);
        }
        
        // create vao
        glGenVertexArrays(1, &vaoForGraphPaper);
        glBindVertexArray(vaoForGraphPaper);

        // vbo position graph
        glGenBuffers(1, &vboForPositionOfGraphPaper);
        glBindBuffer(GL_ARRAY_BUFFER, vboForPositionOfGraphPaper);
        glBufferData(GL_ARRAY_BUFFER, verticesForGraphPaper->size * sizeof(float), verticesForGraphPaper->data, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo color graph
        glGenBuffers(1, &vboForColorOfGraphPaper);
        glBindBuffer(GL_ARRAY_BUFFER, vboForColorOfGraphPaper);
        glBufferData(GL_ARRAY_BUFFER, colorForGraphPaper->size * sizeof(float), colorForGraphPaper->data, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // unbind vao
        glBindVertexArray(0);

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
                //colorForIncircle.push_back(1.0f - (i * (1.0f / 6.0f))); // for color, instead of writting same line for 6 times,
                                                    // written in loop. 6 = 3 + 3
                push_vec_float(colorForIncircle,1.0f - (i * (1.0f / 6.0f)));
            }
            //verticesForIncircle.push_back((GLfloat)cos(angle)* radius + Ox);
            //verticesForIncircle.push_back((GLfloat)sin(angle)* radius + Oy);

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

        const GLfloat rectangleVertices[] =
        {
             0.5f, 0.5f,
            -0.5f, 0.5f,
            -0.5f,-0.5f,
             0.5f,-0.5f
        };

        const GLfloat rectangleColors[] =
        {
            1.0f,0.0f,1.0f,0.4f,
            1.0f,0.0f,1.0f,0.4f,
            0.5f,0.0f,0.0f,0.3f,
            0.5f,0.0f,0.0f,0.3f
        };

        // create vao
        glGenVertexArrays(1, &vaoForRectangle);
        glBindVertexArray(vaoForRectangle);

        // vbo position graph
        glGenBuffers(1, &vboForPositionOfRectangle);
        glBindBuffer(GL_ARRAY_BUFFER, vboForPositionOfRectangle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo color graph
        glGenBuffers(1, &vboForColorOfRectangle);
        glBindBuffer(GL_ARRAY_BUFFER, vboForColorOfRectangle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleColors), rectangleColors, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // unbind vao
        glBindVertexArray(0);

        // Out Circle
        for (GLfloat angle = 0.0f; angle <= 2.0f * (M_PI); angle = angle + 0.001f)
        {
            for (int i = 0; i < 6; i++)
            {
                //colorForOutCircle.push_back(0.0f + (i * (1.0f / 6.0f)));    // for color, instead of writting same line for 6 times,
                                                    // written in loop. 6 = 3 + 3
                push_vec_float(colorForOutCircle,0.0f + (i * (1.0f / 6.0f)));

            }
            //verticesForOutCircle.push_back((GLfloat)cos(angle)* 0.71f);
            //verticesForOutCircle.push_back((GLfloat)sin(angle)* 0.71f);

            push_vec_float(verticesForOutCircle,(GLfloat)cos(angle)* 0.71f);
            push_vec_float(verticesForOutCircle,(GLfloat)sin(angle)* 0.71f);
        }

        // create vao
        glGenVertexArrays(1, &vaoForBigCircle);
        glBindVertexArray(vaoForBigCircle);

        // vbo position graph
        glGenBuffers(1, &vboForPositionOfBigCircle);
        glBindBuffer(GL_ARRAY_BUFFER, vboForPositionOfBigCircle);
        glBufferData(GL_ARRAY_BUFFER, verticesForOutCircle->size * sizeof(float), verticesForOutCircle->data, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // vbo color graph
        glGenBuffers(1, &vboForColorOfBigCircle);
        glBindBuffer(GL_ARRAY_BUFFER, vboForColorOfBigCircle);
        glBufferData(GL_ARRAY_BUFFER, colorForOutCircle->size * sizeof(float), colorForOutCircle->data, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // unbind vao
        glBindVertexArray(0);




        glClearDepthf(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_2D);
        
        projectionMatrix = vmath::mat4::identity();
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        
        
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipGestureRecognizer];
        
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(OnLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
    }
    return(self);
}

+(Class)layerClass
{
    return ([CAEAGLLayer class]);
}

- (void)drawView:(id)sender
{
    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(shaderProgramObject);

     vmath::mat4 modelViewMatrix;
     vmath::mat4 rotationMatrix;
     vmath::mat4 modelViewProjectionMatrix;

     modelViewMatrix            = vmath::mat4::identity();
     modelViewProjectionMatrix  = vmath::mat4::identity();
    
    modelViewMatrix             = vmath::translate(0.0f,0.0f,-6.0f);
    modelViewProjectionMatrix   = projectionMatrix * modelViewMatrix;
    
    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    /*
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glBindVertexArray(0);
    */

    glBindVertexArray(vaoForGraphPaper);
    glDrawArrays(GL_LINES, 0, verticesForGraphPaper->size);
    glBindVertexArray(0);
    
    glBindVertexArray(vaoForBigCircle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, verticesForOutCircle->size);
    glBindVertexArray(0);
    
    glBindVertexArray(vaoForRectangle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    glBindVertexArray(vaoForTriangle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    glBindVertexArray(0);
    
    glBindVertexArray(vaoForSmallCircle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, verticesForIncircle->size);
    glBindVertexArray(0);
    

    glUseProgram(0);
    

    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)layoutSubviews
{
    GLint width;
    GLint height;
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    
    glViewport(0, 0, width, height);
    
    GLfloat fWidth  = (GLfloat)width;
    GLfloat fHeight = (GLfloat)height;
    
    projectionMatrix = vmath::perspective(45.0f, width/height, 0.1f, 1000.0f);
    
    
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Failed to create complete frame buffer object %x\n",glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
    [self drawView:nil];
}

- (void)startAnimation
{
    if(!isAnimating)
    {
        displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink setPreferredFramesPerSecond:animationFrameInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        isAnimating = YES;
    }
}

- (void)stopAnimation
{
    if(isAnimating)
    {
        [displayLink invalidate];
        displayLink = nil;
        
        isAnimating = NO;
    }
}


- (BOOL)acceptsFirstResponder
{
    return(YES);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
        
}

- (void)onSingleTap:(UITapGestureRecognizer *)gr
{
   
}

- (void)onDoubleTap:(UITapGestureRecognizer *)gr
{
   
}

- (void)onSwipe:(UISwipeGestureRecognizer *)gr
{
    [self release];
    exit(0);
}

- (void)OnLongPress:(UILongPressGestureRecognizer *)gr
{
    
}

-(void)dealloc
{
    if(depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderBuffer);
        depthRenderBuffer = 0;
    }
    
    if(colorRenderBuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderBuffer);
        colorRenderBuffer = 0;
    }
    
    if(defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    
    if([EAGLContext currentContext] == eaglContext)
    {
        [EAGLContext setCurrentContext:nil];
    }
    [eaglContext release];
    eaglContext = nil;
    
    [super dealloc];
}

@end

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
