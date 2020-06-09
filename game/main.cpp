#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

class ShaderWrapper
{
public:
    ShaderWrapper() : m_vertexShaderId(0), m_fragmentShaderId(0), m_programId(0)
    {
    }

    void CreateShader(std::string& rVShaderCode, std::string &rFShaderCode)
    {
        m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        const char *pVs = rVShaderCode.c_str();
        glShaderSource(m_vertexShaderId,1, &pVs, nullptr);
        CompileShader(m_vertexShaderId);


        m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        const char *pFs = rFShaderCode.c_str();
        glShaderSource(m_fragmentShaderId,1, &pFs, nullptr);
        CompileShader(m_fragmentShaderId);
    }

    void CreateProgram()
    {
        m_programId = glCreateProgram();

        glAttachShader(m_programId, m_vertexShaderId);
        glAttachShader(m_programId, m_fragmentShaderId);
        glLinkProgram(m_programId);
        glValidateProgram(m_programId);

        glDeleteShader(m_vertexShaderId);
        glDeleteShader(m_fragmentShaderId);
    }

    unsigned int GetProgramId() const
    {
        return m_programId;
    }

private:

    void CompileShader(unsigned int shader)
    {
        glCompileShader(shader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    std::string m_vertexShaderCode;
    std::string m_fragmentShaderCode;

    unsigned int m_vertexShaderId;
    unsigned int m_fragmentShaderId;
    unsigned int m_programId;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

float g_TranslateX = 0.0f;
float g_TranslateY = 0.0f;
float g_Rotate    =  0.0f;
float g_Projection = 45.0f;


// timing
float g_DeltaTime = 0.0f;
float g_LastFrame = 0.0f;



// camera
glm::vec3 g_CameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 g_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 g_CameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);


// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

bool g_FirstMouse = true;
float g_Yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float g_Pitch =  0.0f;
float g_LastX =  (float)(SCR_WIDTH / 2);
float g_LastY =  (float)(SCR_HEIGHT / 2);
float g_Fov   =  45.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // CORE dlatego potrzebuje VAO

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DeathBall", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[8] = {
         0.5f,  0.5f, //0
         0.5f, -0.5f, //1
        -0.5f, -0.5f, //2
        -0.5f,  0.5f, //3
    };

    unsigned int indicesElement[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };


    float vertices2[27] = {
         -0.5f,  0.0f, 0.0f, //0
         -0.5f,  0.5f, 0.0f, //1
          0.0f,  0.0f, 0.0f, //2
          0.0f,  0.5f, 0.0f, //3
          0.5f,  0.5f, 0.0f, //4
          0.5f,  0.0f, 0.0f, //5
          0.5f, -0.5f, 0.0f, //6
          0.0f, -0.5f, 0.0f, //7
         -0.5f, -0.5f, 0.0f  //8
    };

    unsigned int indicesElement2[12] = {
        0, 1, 2,  // triangle 1
        3, 4, 2,  // triangle 2
        5, 6, 2,  // triangle 3
        2, 7, 8   // triangle 4
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), indicesElement, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2 ,0);
    glEnableVertexAttribArray(0);

    std::cout<<"VAO: "<<VAO<<" VBO: "<<VBO<< " EBO: "<<EBO<<std::endl;



    unsigned int VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(float), vertices2, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(float), indicesElement2, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3 ,0);
    glEnableVertexAttribArray(0);

    std::cout<<"VAO: "<<VAO2<<" VBO: "<<VBO2<< " EBO: "<<EBO2<<std::endl;



    // -------------------- SHADERS --------------------
    std::string vertexShaderSourceS = "#version 330 core\n"
        "layout (location = 0) in vec4 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = aPos;\n"
        "}\0";

    std::string vertexFragmentSourceS = "#version 330 core\n"
        "layout (location = 0) out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\0";

    std::string uniformVertexFragmentSourceS = "#version 330 core\n"
       "out vec4 FragColor;\n"
       "uniform vec4 u_Color;"
       "void main()\n"
       "{\n"
       "   FragColor = u_Color;\n"
       "}\0";

    std::string transformationVertexShaderSourceS = "#version 330 core\n"
        "layout (location = 0) in vec4 aPos;\n"
        "uniform mat4 u_Transform;"
        "void main()\n"
        "{\n"
        "   gl_Position = u_Transform * aPos;\n"
        "}\0";


    std::string transformationVertexShaderSourceS2 = "#version 330 core\n"
        "layout (location = 0) in vec4 aPos;\n"
        "uniform mat4 u_model;"
        "uniform mat4 u_view;"
        "uniform mat4 u_projection;"
        "void main()\n"
        "{\n"
        "   gl_Position = u_projection * u_view * u_model * aPos;\n"
        "}\0";

    // --------------------------------------------------

    ShaderWrapper ShaderObj;
    ShaderObj.CreateShader(transformationVertexShaderSourceS2,uniformVertexFragmentSourceS);
    ShaderObj.CreateProgram();
    glUseProgram(ShaderObj.GetProgramId());
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        g_DeltaTime = currentFrame - g_LastFrame;
        g_LastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        glm::mat4 model       = glm::mat4(1.0f);
        glm::mat4 view        = glm::mat4(1.0f);
        glm::mat4 projection  = glm::mat4(1.0f);



        model      = glm::rotate(model, glm::radians(g_Rotate), glm::vec3(1.0f,0.0f,0.0f));
        view       = glm::lookAt(g_CameraPos, g_CameraPos + g_CameraFront, g_CameraUp);


        projection = glm::perspective(glm::radians(g_Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);


        glUseProgram(ShaderObj.GetProgramId());

        unsigned int modelLoc = glGetUniformLocation(ShaderObj.GetProgramId(), "u_model");
        unsigned int viewLoc  = glGetUniformLocation(ShaderObj.GetProgramId(), "u_view");
        unsigned int projectLoc  = glGetUniformLocation(ShaderObj.GetProgramId(), "u_projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

        int uniformLocation = glGetUniformLocation(ShaderObj.GetProgramId(), "u_Color");
        glUniform4f(uniformLocation, 0.0f, glfwGetTime(), 0.8f, 1.0f);

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);


        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    float incrementSpeed = 2.5f * g_DeltaTime;
    float cameraSpeed = 2.5 * g_DeltaTime;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        g_TranslateY += incrementSpeed;
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        g_TranslateY -= incrementSpeed;
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        g_TranslateX -= incrementSpeed;
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        g_TranslateX += incrementSpeed;

    else if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        g_Rotate += incrementSpeed;
    else if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        g_Rotate -= incrementSpeed;
    else if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        g_Projection += incrementSpeed;
    else if(glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        g_Projection -= incrementSpeed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_CameraPos += cameraSpeed * g_CameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_CameraPos -= cameraSpeed * g_CameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_CameraPos -= glm::normalize(glm::cross(g_CameraFront, g_CameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_CameraPos += glm::normalize(glm::cross(g_CameraFront, g_CameraUp)) * cameraSpeed;
    //cout << "(x = "<<g_TranslateX <<", y = " <<g_TranslateY << ", r = " <<g_Rotate << ", p = " <<g_Projection <<")"<<endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (g_FirstMouse)
    {
        g_LastX = xpos;
        g_LastY = ypos;
        g_FirstMouse = false;
    }

    float xoffset = xpos - g_LastX;
    float yoffset = g_LastY - ypos; // reversed since y-coordinates go from bottom to top
    g_LastX = xpos;
    g_LastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    g_Yaw += xoffset;
    g_Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (g_Pitch > 89.0f)
        g_Pitch = 89.0f;
    if (g_Pitch < -89.0f)
        g_Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(g_Yaw)) * cos(glm::radians(g_Pitch));
    front.y = sin(glm::radians(g_Pitch));
    front.z = sin(glm::radians(g_Yaw)) * cos(glm::radians(g_Pitch));
    g_CameraFront = glm::normalize(front);


}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_Fov -= (float)yoffset;
    if (g_Fov < 1.0f)
        g_Fov = 1.0f;
    if (g_Fov > 45.0f)
        g_Fov = 45.0f;
}
