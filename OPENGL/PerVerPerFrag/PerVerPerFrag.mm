#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import "vmath.h"

#import "GLESView.h"

// sphere
#define BUFFER_SIZE 1024

struct vec_int* create_vec_int(void);
void push_vec_int(struct vec_int*, int);
void show_vec_int(struct vec_int*);
void destroy_vec_int(struct vec_int*);

struct vec_float* create_vec_float(void);
void push_vec_float(struct vec_float*, float);
void show_vec_float(struct vec_float*);
void destroy_vec_float(struct vec_float*);

char buffer[BUFFER_SIZE];

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

struct vec_int* create_vec_int(void);
void push_vec_int(struct vec_int*, int);
void show_vec_int(struct vec_int*);
void destroy_vec_int(struct vec_int*);

struct vec_float* create_vec_float(void);
void push_vec_float(struct vec_float*, float);
void show_vec_float(struct vec_float*);
void destroy_vec_float(struct vec_float*);

GLfloat gfShininess = 50.0f;

GLfloat light_ambient[4]  = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat light_diffuse[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_position[4] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat material_ambient[4]     = { 0.0f,0.0f,0.0f,0.0f };
GLfloat material_diffuse[4]     = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_specular[4]    = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_shininess      = gfShininess;

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_NORMAL,
    AMC_ATTRIBUTE_TEXCOORD0,
};


@implementation GLESView
{
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderBuffer;
    GLuint depthRenderBuffer;
    
    id displayLink;
    NSInteger animationFrameInterval;
    BOOL isAnimating;
    
    GLuint vertexShaderObject_perVer;
    GLuint fragmentShaderObject_perVer;
    GLuint shaderProgramObject_perVer;

    GLuint vertexShaderObject_perFrag;
    GLuint fragmentShaderObject_perFrag;
    GLuint shaderProgramObject_perFrag;



    GLuint vao;
    GLuint vbo_position;
    GLuint vbo_normal;
    GLuint vbo_indices;

    GLuint mUniform;
    GLuint vUniform;
    GLuint pUniform;

    // ambient
    GLuint laUniform;
    GLuint kaUniform;

    // diffuse
    GLuint ldUniform;
    GLuint kdUniform;

    // specular
    GLuint lsUniform;
    GLuint ksUniform;

    GLuint  lightPositionUniform;
    GLfloat materialShininessUniform;
    GLuint  lKeyIsPressedUniform;
    GLuint  isLightsOn        ;//= 0;
    GLuint isFragOn;
    
    int stackCount;
    float stackStep;
    float stackAngle;
    
    float x;
    float y;
    float z;
    float xy;
    
    float nx, ny, nz;
    float s,t;
    
    float radius;
    
    float lengthInv;
    
    float sectorStep;
    int sectorCount;
    float sectorAngle;

    int k1, k2;
    
    struct vec_float* vec_float_ver;
    struct vec_float* vec_float_nor;
    struct vec_float* vec_float_tex;
    
    struct vec_int* gp_indice;
    
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
        

        // PER VER ..........................................
        // VERTEX SHADER
        
        vertexShaderObject_perVer = glCreateShader(GL_VERTEX_SHADER);
        const GLchar *vertexShaderSourceCode =
            "#version 300 es" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormals;" \
    
            "uniform mat4 u_m_matrix;" \
            "uniform mat4 u_v_matrix;" \
            "uniform mat4 u_p_matrix;" \
    
            "uniform vec3    u_la;" \
            "uniform vec3    u_ld;" \
            "uniform vec3    u_ls;" \
            "uniform vec4    u_light_position;" \
    
            "uniform vec3    u_ka;" \
            "uniform vec3    u_kd;" \
            "uniform vec3    u_ks;" \
            "uniform float    u_material_shininess;" \
    
            "uniform int    u_lKeyIsPressed;" \
    
            "out vec3 phong_ADS_light;" \
    
            "void main(void)" \
            "{" \
                "if(u_lKeyIsPressed == 1)" \
                "{" \
                    "vec4 eye_coordinates       = u_v_matrix * u_m_matrix * vPosition;" \
                    "mat3 normal_matrix         = mat3(u_v_matrix * u_m_matrix);" \
                    "vec3 t_norm                = normalize (normal_matrix * vNormals);" \
                    "vec3 light_direction       = normalize (vec3 (u_light_position - eye_coordinates));" \
                    "float tn_dot_ld            = max (dot (light_direction, t_norm) , 0.0);" \
                    "vec3 reflection_vector     = reflect(-light_direction, t_norm) ;" \
                    "vec3 viewr_vector          = normalize (vec3 (-eye_coordinates.xyz)) ;" \
        
                    "vec3 Ambient       = u_la * u_ka ;" \
                    "vec3 Diffuse       = u_ld * u_kd * tn_dot_ld;" \
                    "vec3 Specular      = u_ls * u_ks * pow (max (dot (reflection_vector, viewr_vector), 0.0),u_material_shininess);" \
    
                    "phong_ADS_light            = Ambient + Diffuse + Specular;" \
                "}" \
                "else" \
                "{" \
                    "phong_ADS_light            = vec3(1.0,1.0,1.0);" \
                "}"    \
                "gl_Position =  u_p_matrix * u_v_matrix * u_m_matrix *vPosition;" \
            "}";
        glShaderSource(vertexShaderObject_perVer, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

        // compile shader
        glCompileShader(vertexShaderObject_perVer);
        
        GLint iInfoLogLength        = 0;
        GLint iShaderCompiledStatus = 0;
        char *szInfoLog             = NULL;
        
        glGetShaderiv(vertexShaderObject_perVer, GL_COMPILE_STATUS, &iShaderCompiledStatus);
        if(iShaderCompiledStatus == GL_FALSE)
        {
            glGetShaderiv(vertexShaderObject_perVer, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (GLchar *)malloc(iInfoLogLength);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(vertexShaderObject_perVer, iInfoLogLength, &written, szInfoLog);
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
        fragmentShaderObject_perVer = glCreateShader(GL_FRAGMENT_SHADER);

        // provide source code to shader
        const GLchar *fragmentShaderSourceCode =
            "#version 300 es" \
            "\n" \
            "precision highp float;" \
            "precision highp int;" \
            "in  vec3 phong_ADS_light;" \
            "out vec4 FragColor;" \
    
            "void main(void)" \
            "{" \
                "FragColor = vec4(phong_ADS_light,1.0);" \
            "}";
        glShaderSource(fragmentShaderObject_perVer, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

        // compile shader
        glCompileShader(fragmentShaderObject_perVer);
        glGetShaderiv(fragmentShaderObject_perVer, GL_COMPILE_STATUS, &iShaderCompiledStatus);
        if(iShaderCompiledStatus == GL_FALSE)
        {
            glGetShaderiv(fragmentShaderObject_perVer, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (GLchar *)malloc(iInfoLogLength);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(fragmentShaderObject_perVer, iInfoLogLength, &written, szInfoLog);
                    printf("\n--------------------------------------------------- \nCompile time log for Fragment Shader : \n %s", szInfoLog);
                    free(szInfoLog);
                    [self release];
                }
            }
        }

        // Shader program
          shaderProgramObject_perVer = glCreateProgram();
          glAttachShader(shaderProgramObject_perVer, vertexShaderObject_perVer);
          glAttachShader(shaderProgramObject_perVer,fragmentShaderObject_perVer);

          glBindAttribLocation(shaderProgramObject_perVer, AMC_ATTRIBUTE_POSITION, "vPosition");
          glBindAttribLocation(shaderProgramObject_perVer, AMC_ATTRIBUTE_NORMAL, "vNormals");
          
          glLinkProgram(shaderProgramObject_perVer);
          GLint iShaderProgramLinkStatus = 0;
          glGetProgramiv(shaderProgramObject_perVer, GL_LINK_STATUS, &iShaderProgramLinkStatus);
          if (iShaderProgramLinkStatus == GL_FALSE)
          {
              glGetProgramiv(shaderProgramObject_perVer, GL_INFO_LOG_LENGTH, &iInfoLogLength);
              if (iInfoLogLength > 0)
              {
                  szInfoLog = (GLchar *)malloc(iInfoLogLength);
                  if (szInfoLog != NULL)
                  {
                      GLsizei written;
                      glGetProgramInfoLog(shaderProgramObject_perVer, iInfoLogLength, &written, szInfoLog);
                      printf("\n--------------------------------------------------- \nLink time log for Shader Program : \n %s", szInfoLog);
                      free(szInfoLog);
                      [self release];
                  }
              }
          }

         mUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_m_matrix");
         vUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_v_matrix");
         pUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_p_matrix");
     
         laUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_la");
         ldUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_ld");
         lsUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_ls");
     
         kaUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_ka");
         kdUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_kd");
         ksUniform = glGetUniformLocation(shaderProgramObject_perVer, "u_ks");
         
         lightPositionUniform        = glGetUniformLocation(shaderProgramObject_perVer, "u_light_position");
         materialShininessUniform    = glGetUniformLocation(shaderProgramObject_perVer, "u_material_shininess");
         lKeyIsPressedUniform        = glGetUniformLocation(shaderProgramObject_perVer, "u_lKeyIsPressed");
         
        // .........................
        // Per Frag

        vertexShaderObject_perFrag = glCreateShader(GL_VERTEX_SHADER);
        const GLchar *vertexShaderSourceCode =
            "#version 300 es" \
            "\n" \
            "in vec4 vPosition;" \
            "in vec3 vNormals;" \
    
            "uniform mat4 u_m_matrix;" \
            "uniform mat4 u_v_matrix;" \
            "uniform mat4 u_p_matrix;" \
    
            "uniform vec4    u_light_position;" \
            "uniform int    u_lKeyIsPressed;" \
    
            "out vec3 t_norm;" \
            "out vec3 light_direction;" \
            "out vec3 viewr_vector;" \
    
            "void main(void)" \
            "{" \
                "if(u_lKeyIsPressed == 1)" \
                "{" \
                    "vec4 eye_coordinates    = u_v_matrix * u_m_matrix * vPosition;" \
                    "mat3 normal_matrix        = mat3(u_v_matrix * u_m_matrix);" \
                    "t_norm                    = normal_matrix * vNormals;" \
                    "light_direction        = vec3 (u_light_position - eye_coordinates);" \
                    "viewr_vector            = vec3 (-eye_coordinates.xyz) ;" \
                "}" \
                "gl_Position =  u_p_matrix * u_v_matrix * u_m_matrix *vPosition;" \
            "}";
        glShaderSource(vertexShaderObject_perFrag, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

        // compile shader
        glCompileShader(vertexShaderObject_perFrag);
        
        GLint iInfoLogLength        = 0;
        GLint iShaderCompiledStatus = 0;
        char *szInfoLog             = NULL;
        
        glGetShaderiv(vertexShaderObject_perFrag, GL_COMPILE_STATUS, &iShaderCompiledStatus);
        if(iShaderCompiledStatus == GL_FALSE)
        {
            glGetShaderiv(vertexShaderObject_perFrag, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (GLchar *)malloc(iInfoLogLength);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(vertexShaderObject_perFrag, iInfoLogLength, &written, szInfoLog);
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
        fragmentShaderObject_perFrag = glCreateShader(GL_FRAGMENT_SHADER);

        // provide source code to shader
        const GLchar *fragmentShaderSourceCode =
            "#version 300 es" \
            "\n" \
            "precision highp float;" \
            "precision highp int;" \
            "uniform vec3    u_la;" \
            "uniform vec3    u_ld;" \
            "uniform vec3    u_ls;" \
            
    
            "uniform vec3    u_ka;" \
            "uniform vec3    u_kd;" \
            "uniform vec3    u_ks;" \
            "uniform float    u_material_shininess;" \
    
            "uniform int    u_lKeyIsPressed;" \
    
        
            "in vec3 t_norm;" \
            "in vec3 light_direction;" \
            "in vec3 viewr_vector;" \
    
            "out vec4 FragColor;" \
    
            "void main(void)" \
            "{" \
                "if(u_lKeyIsPressed == 1)" \
                "{" \
                    "vec3 normalized_t_norm             = normalize (t_norm);" \
                    "vec3 normalized_light_direction    = normalize (vec3 (light_direction));" \
                    "vec3 normalized_view_vector        = normalize (viewr_vector);" \
    
                    "float tn_dot_ld                    = max (dot (normalized_light_direction, normalized_t_norm) , 0.0);" \
                    "vec3 reflection_vector             = reflect(-normalized_light_direction, normalized_t_norm) ;" \
    
                    "vec3 Ambient    = u_la * u_ka ;" \
                    "vec3 Diffuse    = u_ld * u_kd * tn_dot_ld;" \
                    "vec3 Specular   = u_ls * u_ks * pow (max (dot (reflection_vector, normalized_view_vector), 0.0),u_material_shininess);" \
    
                    "vec3 phong_ADS_light            = Ambient + Diffuse + Specular;" \
                    "FragColor = vec4(phong_ADS_light,1.0);" \
                "}" \
                "else" \
                "{" \
                    "FragColor = vec4(1.0f,1.0f,1.0f,1.0);" \
                "}"    \
        "}";
        glShaderSource(fragmentShaderObject_perFrag, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

        // compile shader
        glCompileShader(fragmentShaderObject_perFrag);
        glGetShaderiv(fragmentShaderObject_perFrag, GL_COMPILE_STATUS, &iShaderCompiledStatus);
        if(iShaderCompiledStatus == GL_FALSE)
        {
            glGetShaderiv(fragmentShaderObject_perFrag, GL_INFO_LOG_LENGTH, &iInfoLogLength);
            if (iInfoLogLength > 0)
            {
                szInfoLog = (GLchar *)malloc(iInfoLogLength);
                if (szInfoLog != NULL)
                {
                    GLsizei written;
                    glGetShaderInfoLog(fragmentShaderObject_perFrag, iInfoLogLength, &written, szInfoLog);
                    printf("\n--------------------------------------------------- \nCompile time log for Fragment Shader : \n %s", szInfoLog);
                    free(szInfoLog);
                    [self release];
                }
            }
        }

        // Shader program
          shaderProgramObject_perFrag = glCreateProgram();
          glAttachShader(shaderProgramObject_perFrag, vertexShaderObject_perFrag);
          glAttachShader(shaderProgramObject_perFrag,fragmentShaderObject_perFrag);

          glBindAttribLocation(shaderProgramObject_perFrag, AMC_ATTRIBUTE_POSITION, "vPosition");
          glBindAttribLocation(shaderProgramObject_perFrag, AMC_ATTRIBUTE_NORMAL, "vNormals");
          
          glLinkProgram(shaderProgramObject_perFrag);
          GLint iShaderProgramLinkStatus = 0;
          glGetProgramiv(shaderProgramObject_perFrag, GL_LINK_STATUS, &iShaderProgramLinkStatus);
          if (iShaderProgramLinkStatus == GL_FALSE)
          {
              glGetProgramiv(shaderProgramObject_perFrag, GL_INFO_LOG_LENGTH, &iInfoLogLength);
              if (iInfoLogLength > 0)
              {
                  szInfoLog = (GLchar *)malloc(iInfoLogLength);
                  if (szInfoLog != NULL)
                  {
                      GLsizei written;
                      glGetProgramInfoLog(shaderProgramObject_perFrag, iInfoLogLength, &written, szInfoLog);
                      printf("\n--------------------------------------------------- \nLink time log for Shader Program : \n %s", szInfoLog);
                      free(szInfoLog);
                      [self release];
                  }
              }
          }

         mUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_m_matrix");
         vUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_v_matrix");
         pUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_p_matrix");
     
         laUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_la");
         ldUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_ld");
         lsUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_ls");
     
         kaUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_ka");
         kdUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_kd");
         ksUniform = glGetUniformLocation(shaderProgramObject_perFrag, "u_ks");
         
         lightPositionUniform        = glGetUniformLocation(shaderProgramObject_perFrag, "u_light_position");
         materialShininessUniform    = glGetUniformLocation(shaderProgramObject_perFrag, "u_material_shininess");
         lKeyIsPressedUniform        = glGetUniformLocation(shaderProgramObject_perFrag, "u_lKeyIsPressed");

         // ..........................................
        
        radius      = 1.0;
        lengthInv   = 1.0 / radius;
        
        stackCount  = 50;
        stackStep   = M_PI / stackCount;
        
        sectorCount = 50;
        sectorStep  = 2 * M_PI / sectorCount;
       
        vec_float_ver = create_vec_float();
        vec_float_nor = create_vec_float();
        vec_float_tex = create_vec_float();
    
        gp_indice = create_vec_int();
        
        for (int i = 0; i <= stackCount; i++)
        {
            stackAngle    = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy            = radius * (float)cos(stackAngle);        // r * cos(u)
            z            = radius * (float)sin(stackAngle);        // r * sin(u)
            
            // add (sector count + 1) vertices per stack
            // the first and last vertices have same position and normals, but diff text coord
            for (int j = 0; j <= sectorCount; j++)
            {
                sectorAngle = j * sectorStep;        // starting from 0 to 2pi
    
                // vertex position
                x = xy * (float)cos(sectorAngle);    // r * cos(u) * cos(v)
                y = xy * (float)sin(sectorAngle);    // r * cos(u) * sin(v)
    
                push_vec_float(vec_float_ver, x);
                push_vec_float(vec_float_ver, y);
                push_vec_float(vec_float_ver, z);
                
                //fprintf(gpFile,"Value of x : %f , y : %f , z : %f \n",x,y,z);
    
                // normalized vertex normals
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
    
                push_vec_float(vec_float_nor, nx);
                push_vec_float(vec_float_nor, ny);
                push_vec_float(vec_float_nor, nz);
    
                // texcords
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
    
                push_vec_float(vec_float_tex, s);
                push_vec_float(vec_float_tex, t);
            }
    
        }
    
        for (int i = 0; i < stackCount; i++)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;
    
            for (int j = 0; j < sectorCount; j++, k1++, k2++)
            {
                if (i != 0)
                {
                    push_vec_int(gp_indice, k1);
                    push_vec_int(gp_indice, k2);
                    push_vec_int(gp_indice, k1 + 1);
                }
    
                if (i != (stackCount - 1))
                {
                    push_vec_int(gp_indice, k1 + 1);
                    push_vec_int(gp_indice, k2);
                    push_vec_int(gp_indice, k2 + 1);
                }
            }
        }
        
        //show_vec_int(gp_indice);
        
        //show_vec_float(vec_float_ver);
        //show_vec_float(vec_float_nor);
        //show_vec_float(vec_float_tex);
        
        // create vao
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    
        // create vbo
        glGenBuffers(1, &vbo_position);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
        glBufferData(GL_ARRAY_BUFFER, vec_float_ver->size * sizeof(float), vec_float_ver->data, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    
        glGenBuffers(1, &vbo_normal);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
        glBufferData(GL_ARRAY_BUFFER, vec_float_nor->size * sizeof(float), vec_float_nor->data, GL_STATIC_DRAW);
        glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
        glGenBuffers(1, &vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, gp_indice->size * sizeof(float), gp_indice->data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    
        glBindVertexArray(0);


        
        
        
        glClearDepthf(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_2D);
        
        projectionMatrix = vmath::mat4::identity();
        
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        
        isLightsOn    = 0;
        isFragOn = 0;
        
        
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

    if(isFragOn == 0)
    {
        glUseProgram(0);
        glUseProgram(shaderProgramObject_perVer);
    }
    else
    {
        glUseProgram(0);
        glUseProgram(shaderProgramObject_perFrag);
    }

    vmath::mat4 modelMatrix;
    vmath::mat4 viewMatrix;
    vmath::mat4 ProjectionMatrix;

    modelMatrix         = vmath::mat4::identity();
    viewMatrix          = vmath::mat4::identity();
    modelMatrix         = vmath::translate(0.0f,0.0f,-3.0f);
    ProjectionMatrix    = projectionMatrix;

    // send necessary matrices to shader in respective uniform
    glUniformMatrix4fv(mUniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(vUniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, ProjectionMatrix);

    if (isLightsOn)
    {
        glUniform1i(lKeyIsPressedUniform, 1);

        glUniform3fv(laUniform, 1, light_ambient);
        glUniform3fv(ldUniform, 1, light_diffuse);
        glUniform3fv(lsUniform, 1, light_specular);

        glUniform3fv(kaUniform, 1, material_ambient);
        glUniform3fv(kdUniform, 1, material_diffuse);
        glUniform3fv(ksUniform, 1, material_specular);
        
        glUniform4fv(lightPositionUniform, 1, light_position);
        material_shininess = gfShininess;
        glUniform1f(materialShininessUniform, material_shininess);

    }
    else
    {
        glUniform1i(lKeyIsPressedUniform, 0);
    }
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, gp_indice->size, GL_UNSIGNED_INT, 0);
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
    
    projectionMatrix = vmath::perspective(45.0f, fWidth/fHeight, 0.1f, 1000.0f);
    
    
    
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
    if(isLightsOn == 0)
        isLightsOn = 1;
    else
        isLightsOn = 0;
}

- (void)onDoubleTap:(UITapGestureRecognizer *)gr
{
   if(isFragOn == 0)
        isFragOn = 1;
    else
        isFragOn = 0;
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


struct vec_int* create_vec_int(void)
{
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
    for(int i = 0 ; i < temp_struct_vec_int->size; i++)
    {
        printf("vec_int_ver[%d] : %d \n",i,temp_struct_vec_int->data[i]);
    }
}

void show_vec_float(struct vec_float* temp_struct_vec_float)
{
    for(int i = 0 ; i < temp_struct_vec_float->size; i++)
    {
        printf("vec_int_ver[%d] : %f \n",i,temp_struct_vec_float->data[i]);
    }
}


@end
