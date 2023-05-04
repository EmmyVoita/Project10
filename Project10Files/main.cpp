#include <iostream>  // iostream include

// GLEW
#define GLEW_STATIC // Define glew_static
#include <GL/glew.h> // glew include

// GLFW
#include <GLFW/glfw3.h> // glfw include

// SOIL
#include <SOIL/SOIL.h> // soil include

// GLM Mathematics
#include <glm/glm.hpp> // glm include
#include <glm/gtc/matrix_transform.hpp> // glm matrix math include
#include <glm/gtc/type_ptr.hpp> // glm gtc include

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Other includes
#include "shader.h" // Include shader class
#include "Camera.h" // Include Camera class
#include "Model.h" // Include Model class
#include "GenTexture.h"

const GLuint WIDTH = 800, HEIGHT = 600; // Global variables for width and height of window

GLFWwindow* window;
glm::mat4 ViewMatrix = glm::mat4(1.f);

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); // key_callback method
void do_movement(); // Movement method

Camera* camera; // Sets iniital camera pos (0, 0, 0)
GLfloat lastX = WIDTH / 2.0; // Used for camera motion
GLfloat lastY = HEIGHT / 2.0; // Used for camera motion
bool keys[1024]; // Allowable number of key strokes

glm::vec3 lightPos(0.0f, 3.0f, 2.0f); // Sets light position

GLfloat deltaTime = 0.0f; // Initialize deltaTime for camera movement
GLfloat lastTime = 0.0f; // Initialize lastFrame for camera movement
float curTime = 0.0f;

void updateDt()
{
	curTime = static_cast<float>(glfwGetTime());
	deltaTime = curTime - lastTime;
	lastTime = curTime;
}

// Initiates movement based on keyboard input
void updateKeyboardInput()
{
	//Camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->move(deltaTime, FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->move(deltaTime, BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->move(deltaTime, LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->move(deltaTime, RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		camera->move(deltaTime,DOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera->move(deltaTime,UP);
	}
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		camera->move(deltaTime,PITCHUP);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera->move(deltaTime,PITCHDOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera->move(deltaTime,YAWDOWN);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera->move(deltaTime,YAWUP);
	}
}

vector<float> GetCubeData()
{
    // Create an instance of the Assimp importer
    Assimp::Importer importer;

    // Specify the importer settings
   // importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_NORMALS, true); // Read normals
    //importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_TANGENTS_AND_BITANGENTS, true); // Read tangents and bitangents

    // Load the FBX file
    const aiScene* scene = importer.ReadFile("Models/cube.fbx", 
    aiProcess_Triangulate | 
    aiProcess_GenSmoothNormals | 
    aiProcess_FlipUVs | 
    aiProcess_CalcTangentSpace);

    std::vector<float> vertices;
 

    if (scene->mMeshes[0] != nullptr)
    {   
        
        aiMesh* mesh = scene->mMeshes[0];
        // Extract position data
        std::vector<float> positions;
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            positions.push_back(mesh->mVertices[j].x);
            positions.push_back(mesh->mVertices[j].y);
            positions.push_back(mesh->mVertices[j].z);
        }

        // Extract normal data
        std::vector<float> normals;
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            normals.push_back(mesh->mNormals[j].x);
            normals.push_back(mesh->mNormals[j].y);
            normals.push_back(mesh->mNormals[j].z);
        }

        // Extract texture coordinate data
        std::vector<float> texCoords;
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            texCoords.push_back(mesh->mTextureCoords[0][j].x);
            texCoords.push_back(mesh->mTextureCoords[0][j].y);
        }

        // Extract tangent data
        std::vector<float> tangents;
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            tangents.push_back(mesh->mTangents[j].x);
            tangents.push_back(mesh->mTangents[j].y);
            tangents.push_back(mesh->mTangents[j].z);
        }

        // Extract bitangent data
        std::vector<float> bitangents;
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            bitangents.push_back(mesh->mBitangents[j].x);
            bitangents.push_back(mesh->mBitangents[j].y);
            bitangents.push_back(mesh->mBitangents[j].z);
        }


        // Combine position, normal, texCoord, tangent, and bitangent into a single vertex array
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            vertices.push_back(positions[j * 3]);
            vertices.push_back(positions[j * 3 + 1]);
            vertices.push_back(positions[j * 3 + 2]);
            vertices.push_back(normals[j * 3]);
            vertices.push_back(normals[j * 3 + 1]);
            vertices.push_back(normals[j * 3 + 2]);
            vertices.push_back(texCoords[j * 2]);
            vertices.push_back(texCoords[j * 2 + 1]);
            vertices.push_back(tangents[j * 3]);
            vertices.push_back(tangents[j * 3 + 1]);
            vertices.push_back(tangents[j * 3 + 2]);
            vertices.push_back(bitangents[j * 3]);
            vertices.push_back(bitangents[j * 3 + 1]);
            vertices.push_back(bitangents[j * 3 + 2]);
        }
    }
    else
    {
        cout << "no model loaded" << endl;
    }
    

    
    
    return vertices;
   
}

int main() {
    // Init GLFW
    glfwInit(); // Initialize GLFW
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set minor context version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set profiles
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Set resizable to false

    // Create GLFWwindow
    window = glfwCreateWindow(WIDTH, HEIGHT, "Project 9", nullptr, nullptr); // Create window
    glfwMakeContextCurrent(window); // Make context method

    // Set required callback functions
    glfwSetKeyCallback(window, key_callback); // Set key_callback method

    glewExperimental = GL_TRUE; // Set glew to experimental
 
    glewInit(); // Initialize GLEW

    glViewport(0, 0, WIDTH, HEIGHT); // Define viewport dimensions

    glEnable(GL_DEPTH_TEST); // Set up OpenGL options
    glEnable(GL_TEXTURE_CUBE_MAP);

    camera = new Camera(glm::vec3(0.f, 0.f, 15.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), &ViewMatrix);


    // INSERT SHADERS HERE FOR PROJECT 10
    Shader checkerboardShader("checkerboard.vs", "checkerboard.frag"); // Create shader for checkerboard
    Shader cubeShader("cube.vs", "cube.frag"); // Create shader for cube object
    Shader cylinderShader("cylinder.vs", "cylinder.frag"); // Create shader for cylinder object
    Shader sphereShader("sphere.vs", "sphere.frag"); // Create shader for sphere object

    // Models for Cylinder and Sphere
    Model sphereModel("sphere.obj"); // Define model for sphere using obj
    Model cylinderModel("cylinder.obj"); // Defines model for cylinder using obj



    vector<float> vertices = GetCubeData();

    // Loop through vector and print each element
    for (int i = 0; i < vertices.size(); i++) 
    {
        //std::cout << "Vertex " << i << ": ";
        for(int j = 0; j < 14; j++)
        {
            //cout << vertices[j + i * 14] << " : ";
        }
        //cout << endl;
    }

     // Create and bind the VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO); // Generate VAO
    glGenBuffers(1, &VBO); // Generate VBO

    glBindVertexArray(VAO);  // Bind VAO
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Specify the layout of the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0); // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float))); // normal
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float))); // texture coordinates
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float))); // tangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float))); // bitangent

    // Enable the vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindVertexArray(0); // Unbind VAO
    


    // DEFINE TEXTURES HERE Project 10 --> NOTE FOR PROJECT 10
    GenTexture bumpMap("Textures/Bump-Map.jpg", GL_TEXTURE_2D);
    GenTexture diffuseTexture("Textures/Bump-Picture.jpg", GL_TEXTURE_2D);

    GenTexture brickDiffuseMap("Textures/brick_diffuse.jpg", GL_TEXTURE_2D);
    GenTexture brickNormalMap("Textures/bricks2_normal.jpg", GL_TEXTURE_2D);
    GenTexture brickDisplacementMap("Textures/bricks2_disp.jpg", GL_TEXTURE_2D);


   // Generate a texture object for the cube map
    GLuint cubeMapTex;
    glGenTextures(1, &cubeMapTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex);

    // Load each texture and bind it to its corresponding cube map face
    GenTexture right("Textures/CubeMap/posx.jpg",  GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, right.getWidth(), right.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, right.GetData());

    GenTexture left("Textures/CubeMap/negx.jpg",  GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, left.getWidth(), left.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, left.GetData());

    GenTexture top("Textures/CubeMap/posy.jpg",  GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, top.getWidth(), top.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, top.GetData());

    GenTexture bottom("Textures/CubeMap/negy.jpg",  GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bottom.getWidth(), bottom.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom.GetData());

    GenTexture front("Textures/CubeMap/posz.jpg",  GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, front.getWidth(), front.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, front.GetData());

    GenTexture back("Textures/CubeMap/negz.jpg",  GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, back.getWidth(), back.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, back.GetData());

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);





    brickDiffuseMap.bind(0);
    brickNormalMap.bind(1);
    brickDisplacementMap.bind(2);
    bumpMap.bind(3);
    diffuseTexture.bind(4);
   

    GLuint textureUnit = 5;
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex);
        

    // Game Loop
    while (!glfwWindowShouldClose(window)) {

        updateDt();


        // Check for events
        glfwPollEvents(); // Callback glfwPollEvents to check for events
        updateKeyboardInput(); // Callback do_movement()

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers

        
        // Initialize Camera
        //view = camera.GetViewMatrix(); // Set view based on camera
        glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 200.0f); // Initialize projection using initial values
        glm::mat4 model = glm::mat4(1.0f); // Initialize model to be 4x4 identity


        // BIND TEXTURES FOR CYLINDER SHADER
        cylinderShader.Use();
       
        // Set the uniform variables to the corresponding texture units
        const GLuint bumpMapLoc = glGetUniformLocation(cylinderShader.Program, "bumpMapTexture");
        const GLuint diffuseTextureLoc = glGetUniformLocation(cylinderShader.Program, "diffuseTexture");

        glUniform1i(bumpMapLoc, 3);
        glUniform1i(diffuseTextureLoc, 4);
        
        // BIND TEXTURES FOR CUBE SHADER
        cubeShader.Use();
        // Set the uniform variables to the corresponding texture units
        const GLuint diffuseLocCube = glGetUniformLocation(cubeShader.Program, "diffuseMap");
        const GLuint normalLocCube = glGetUniformLocation(cubeShader.Program, "normalMap");
        const GLuint disLocCube = glGetUniformLocation(cubeShader.Program, "depthMap");

        glUniform1i(diffuseLocCube, 0);
        glUniform1i(normalLocCube, 1);
        glUniform1i(disLocCube, 2);

        // BIND TEXTURE FOR SPHERE SHADER
        sphereShader.Use();
        glUniform1i(glGetUniformLocation(sphereShader.Program, "skybox"), 5);




        // CHECKERBOARD
        checkerboardShader.Use(); // Use checkerboard shader


        GLint squareColorLoc = glGetUniformLocation(checkerboardShader.Program, "squareColor"); // Retrieve uniform location for squareColor
        GLint lightColorLoc = glGetUniformLocation(checkerboardShader.Program, "lightColor"); // Retrieve uniform location for lightColor
        GLint lightPosLoc = glGetUniformLocation(checkerboardShader.Program, "lightPos"); // Retrieve uniform location for lightPos
        GLint viewPosLoc = glGetUniformLocation(checkerboardShader.Program, "viewPos"); // Retrieve uniform location for viewPos

        glUniform3f(squareColorLoc, 1.0f, 0.0f, 0.0f);
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Pass white color to lightColorLoc uniform
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); // Pass light position to lightPosLoc uniform
        glUniform3f(viewPosLoc, camera->position.x, camera->position.y, camera->position.z); // Pass camera position to viewPosLoc uniform

        glm::mat4 view_square = ViewMatrix; // Create mat4 view_square equal to view generic defined above
        model = glm::mat4(1.0f);

        GLint modelLoc = glGetUniformLocation(checkerboardShader.Program, "model"); // Retrieve model uniform location
        GLint viewLoc = glGetUniformLocation(checkerboardShader.Program, "view"); // Retrieve view uniform location
        GLint projLoc = glGetUniformLocation(checkerboardShader.Program, "projection"); // Retrieve projection uniform location

        for (int i = 0; i < 8; i++) { // For 8 rows
            for (int j = 0; j < 8; j++) { // For 8 columns
                if ((i+j) % 2 == 0) { // Check if i+j is odd or even for color purposes
                    glUniform3f(squareColorLoc, 1.0f, 0.0f, 0.0f); // If even square color is purple --> pass purple to uniform
                } else {
                    glUniform3f(squareColorLoc, 1.0f, 1.0f, 1.0f); // If even square color is white --> pas white to uniform
                }
                model = glm::translate(model, glm::vec3(j-4.0f, -0.5f, i-9.0f)); // Translate square to posiiton [setting x and z for grid]
                model = glm::scale(model, glm::vec3(0.5f, 0.1f, 0.5f)); // Scale squares to be like tiles
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Pass model to uniform
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_square)); // Pass view_square to uniform
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection to uniform
                // Draw square
                glBindVertexArray(VAO); // Bind vertex arrays
                glDrawArrays(GL_TRIANGLES, 0, 36); // Draw arrays for cube
                model = glm::mat4(1.0f); // Reset model to original view identity [i.e. Translations are independent]
                
            }
        }




        // CUBE
        cubeShader.Use(); // Activate cube shader

    

        // Set uniform locations
        GLint cubeColorLoc = glGetUniformLocation(cubeShader.Program, "cubeColor"); // Retrieve uniform location
        GLint lightColorLoc2 = glGetUniformLocation(cubeShader.Program, "lightColor"); // Reset uniform location for cubeShader
        GLint lightPosLoc2 = glGetUniformLocation(cubeShader.Program, "lightPos"); // Reset uniform location for cubeShader
        GLint viewPosLoc2 = glGetUniformLocation(cubeShader.Program, "viewPos"); // Reset uniform location for cubeShader

        // Pass to shaders
        glUniform3f(cubeColorLoc, 0.0f, 0.0f, 1.0f); // Pass cube color to uniform
        glUniform3f(lightColorLoc2, 1.0f, 1.0f, 1.0f); // Pass light color to uniform
        glUniform3f(lightPosLoc2, lightPos.x, lightPos.y, lightPos.z); // Pass light position to uniform
        glUniform3f(viewPosLoc2, camera->position.x, camera->position.y, camera->position.z); // Pass camera position to uniform

        glm::mat4 view_cube = ViewMatrix;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // Translate cube back
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

        // Get uniform location
        modelLoc = glGetUniformLocation(cubeShader.Program, "model"); // Reset modelLoc using cubeShader
        viewLoc = glGetUniformLocation(cubeShader.Program, "view"); // Reset viewLoc using cubeShader
        projLoc = glGetUniformLocation(cubeShader.Program, "projection"); // Reset projLoc using cubeShader
        // Pass locations to shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Pass model to shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_cube)); // Pass view_cube to shader
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection to shader
        // Draw cube
        glBindVertexArray(VAO); // Bind vertex arrays
        glDrawArrays(GL_TRIANGLES, 0, 36); // Draw cube

        
        // CYLINDER
        cylinderShader.Use(); // Activate cylinder shader

        GLint cylinderColorLoc = glGetUniformLocation(cylinderShader.Program, "cylinderColor"); // Retrieve cylinderColor location
        GLint lightColorLoc3 = glGetUniformLocation(cylinderShader.Program, "lightColor"); // Reset lightColor location
        GLint lightPosLoc3 = glGetUniformLocation(cylinderShader.Program, "lightPos"); // Reset lightPos location
        GLint viewPosLoc3 = glGetUniformLocation(cylinderShader.Program, "viewPos"); // Reset viewPos location


        glUniform3f(cylinderColorLoc, 1.0f, 1.0f, 0.0f); // Pass color to uniform
        glUniform3f(lightColorLoc3, 1.0f, 1.0f, 1.0f); // Pass light color to uniform
        glUniform3f(lightPosLoc3, lightPos.x, lightPos.y, lightPos.z); // Pass light position to uniform
        glUniform3f(viewPosLoc3, camera->position.x, camera->position.y, camera->position.z); // Pass camera position to uniform

        modelLoc = glGetUniformLocation(cylinderShader.Program, "model"); // Reset view location for cylinderShader
        viewLoc = glGetUniformLocation(cylinderShader.Program, "view"); // Reset view location for cylinderShader
        projLoc = glGetUniformLocation(cylinderShader.Program, "projection"); // Reset view location for cylinderShader

        glm::mat4 view_cylinder = ViewMatrix; // Create mat4 view_cylinder using generic view identity
        view_cylinder = glm::translate(view_cylinder, glm::vec3(-1.8f, -3.0f, -5.5f)); // Translate cylinder back, to the right, and down
        view_cylinder = glm::scale(view_cylinder, glm::vec3(0.5, 3.0, 0.5)); // Increase height of cylinder

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_cylinder)); // Pass view_cylinder to shader
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection to shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Pass moel to shader

        cylinderModel.Draw(cylinderShader); // Draw obj model

        
        // SPHERE
        //glDepthFunc(GL_LEQUAL);
        sphereShader.Use(); // Activate sphereShader

   
        GLint sphereColorLoc = glGetUniformLocation(sphereShader.Program, "sphereColor"); // Retrieve sphereColor location
        GLint lightColorLoc4 = glGetUniformLocation(sphereShader.Program, "lightColor"); // Reset lightColor location for sphereShader
        GLint lightPosLoc4 = glGetUniformLocation(sphereShader.Program, "lightPos"); // Reset lightPos location for sphereShader
        GLint viewPosLoc4 = glGetUniformLocation(sphereShader.Program, "viewPos"); // Reset viewPos location for sphereShader

        glUniform3f(sphereColorLoc, 0.0f, 1.0f, 0.0f); // Pass in sphere color to uniform
        glUniform3f(lightColorLoc4, 1.0f, 1.0f, 1.0f); // Pass in light color to uniform
        glUniform3f(lightPosLoc4, lightPos.x, lightPos.y, lightPos.z); // Pass in light position to uniform
        glUniform3f(viewPosLoc4, camera->position.x, camera->position.y, camera->position.z); // Pass in camera position to uniform

        modelLoc = glGetUniformLocation(sphereShader.Program, "model"); // Reset model uniform location for sphereShader
        viewLoc = glGetUniformLocation(sphereShader.Program, "view"); // Reset view uniform location for sphereShader
        projLoc = glGetUniformLocation(sphereShader.Program, "projection"); // Reset projection uniform location for sphereShader

        //glm::mat4 view_sphere = glm::mat4(glm::mat3(ViewMatrix)); // Create mat4 view_sphere equal to view identity
        glm::mat4 view_sphere = ViewMatrix;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, 0.0f, -5.0f)); // Translate sphere back and to the left
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Scale down sphere
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_sphere)); // Pass view_sphere to uniform
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection to uniform
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // Pass model to uniform

        sphereModel.Draw(sphereShader); // Draw sphere obj model
        //glBindVertexArray(VAO); // Bind vertex arrays
        //glDrawArrays(GL_TRIANGLES, 0, 36); // Draw cube

        //glDepthFunc(GL_LESS);

        glBindVertexArray(0); // Bind zero at end
        glfwSwapBuffers(window); // Swap screen buffers

    }


    // Deallocate resources
    glDeleteVertexArrays(1, &VAO); // Deallocate vertex arrays
    glDeleteBuffers(1, &VBO); // Deallocate buffers
    glfwTerminate(); // Terminate window
    return 0; // Returns 0 for end of int main()

}

// Method for key input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { // If ESC pressed
        glfwSetWindowShouldClose(window, GL_TRUE); // Close window
    } if (key >= 0 && key < 1024) { // Allow for 1024 key presses
        if (action == GLFW_PRESS) { // If pressed
            keys[key] = true; // Set keys[key] = true [key pressed]
        } else if (action == GLFW_RELEASE) { // If released
            keys[key] = false; // Set keys[key] = false [key not pressed]
        }
    }
}


