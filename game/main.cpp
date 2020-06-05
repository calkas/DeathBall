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
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    unsigned int VBO, VAO, EBO;
//    float vertices[6] = {
//        -0.5f, -0.5f, //0
//         0.5f, -0.5f, //1
//         0.0f,  0.5f, //2
//    };



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
    // --------------------------------------------------

    ShaderWrapper ShaderObj;
    ShaderObj.CreateShader(transformationVertexShaderSourceS,uniformVertexFragmentSourceS);
    ShaderObj.CreateProgram();
    glUseProgram(ShaderObj.GetProgramId());


    float greenColor = 0.01f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));


        glUseProgram(ShaderObj.GetProgramId());

        unsigned int transformLoc = glGetUniformLocation(ShaderObj.GetProgramId(), "u_Transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        int uniformLocation = glGetUniformLocation(ShaderObj.GetProgramId(), "u_Color");
        glUniform4f(uniformLocation, 0.0f, greenColor, 0.8f, 1.0f);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (greenColor > 1.0f)
        {
            greenColor = 0.01f;
        }
        else
        {
            greenColor += 0.01f;
        }


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
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
