#include "Application.h"
#include "resources/shaders/shader_v.glsl"
#include "resources/shaders/shader_f.glsl"
#include <Keyboard.h>
#include <TextureManager.h>
#include <PerspectiveCamera.h>
#include <LightManager.h>
#include <Model.h>
#include "ActiveBlock.h"

Application::Application(const std::string &name,
                         const std::string &version) :
    GLFWApplication(name, version) {
}

void Application::parseArguments(int argc, char **argv) {
    GLFWApplication::parseArguments(argc, argv);
}

unsigned Application::init() {
    GLFWApplication::init();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);  // 4x
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // s*alpha + d(1-alpha)
    RenderCommands::setClearColor({0.5f, 0.0f, 0.0f, 1.0f});
    return 0;
}

int Application::run() {

    // CAMERA
    PerspectiveCamera camera({80.0f, static_cast<float>(winWidth),
                              static_cast<float>(winHeight), 1.0f, -10.0f},
                             {2.0f, 2.0f, 12.4f}, {2.0f, 2.0f, 0.0f});


    // MODELS
    ActiveBlock activeBlock;
    Model nearWall(GeometricTools::UnitGridGeometry3DWTCoords<5, 5>(),
                   GeometricTools::UnitGridTopologyTriangles<5, 5>(),
                   BufferLayout({{ShaderDataType::Float3, "position"},
                             {ShaderDataType::Float3, "color"},
                             {ShaderDataType::Float2, "texture"}}));
    Model sideWall(GeometricTools::UnitGridGeometry3DWTCoords<10, 5>(),
                   GeometricTools::UnitGridTopologyTriangles<10, 5>(),
                   BufferLayout({{ShaderDataType::Float3, "position"},
                                       {ShaderDataType::Float3, "color"},
                                       {ShaderDataType::Float2, "texture"}}));
    Model cube(GeometricTools::UnitCube3D24WNormals,
               GeometricTools::UnitCube3DTopologyTriangles24,
               BufferLayout({{ShaderDataType::Float3, "position"},
                             {ShaderDataType::Float3, "normals"}}));


    // SHADERS
    auto shader = std::make_shared<Shader>(vertexShaderSrc, shader_f);
    shader->setUniform("u_projection", camera.GetProjectionMatrix());
    shader->setUniform("u_view", camera.GetViewMatrix());
    shader->setUniform("u_cameraPos", camera.GetPosition());

    // LIGHTS
    setupAllLights(3.0f, 0.2f, 0.7f);
    shader->setUniform("u_specularStrength", 5.0f);

    // TEXTURES
    auto textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture2D("wall", "walls.jpg", 0);
    textureManager->LoadCubeMap("cube", {"cube.jpg"}, 1);
    textureManager->LoadCubeMap("active-box", {"transparent-box.png"}, 2);
    shader->setUniform("u_flatTexture", textureManager->GetUnitByName("wall"));
    shader->setUniform("u_cubeTexture", textureManager->GetUnitByName("cube"));

    // KEYBOARD
    auto keyEsc = Keyboard(GLFW_KEY_ESCAPE);        // Quit application
    auto keyQ = Keyboard(GLFW_KEY_Q);               // Quit the application
    auto keyUp = Keyboard(GLFW_KEY_UP, 350);        // Move up
    auto keyDown = Keyboard(GLFW_KEY_DOWN, 350);    // Move down
    auto keyLeft = Keyboard(GLFW_KEY_LEFT, 350);    // Move left
    auto keyRight = Keyboard(GLFW_KEY_RIGHT, 350);  // Move right
    auto keyX = Keyboard(GLFW_KEY_X, 350);          // Move inwards
    auto keySpace = Keyboard(GLFW_KEY_SPACE, 350);  // Move inwards entirely
    auto keyZ = Keyboard(GLFW_KEY_Z, 350);          // Rotate left
    auto keyC = Keyboard(GLFW_KEY_C, 350);          // Rotate right
    auto keyI = Keyboard(GLFW_KEY_I, 350);          // Toggle illumination
    auto keyT = Keyboard(GLFW_KEY_T, 500);          // Toggle blending


    // GAME VARIABLES
    bool squares[10][5][5] = {};           // true if block occupies space
    float interpolation[10][5][5] = {};    // >0.0f if block should be animated
    bool blendTexture = false;             // shader setting
    bool illuminate = false;               // shader setting
    float elapsedTime, deltaTime, lastTime, timeSinceLastDrop = 0.0f;

    // GAME LOOP
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        RenderCommands::clear();

        // update time
        elapsedTime = static_cast<float>(glfwGetTime());
        deltaTime = elapsedTime - lastTime;
        lastTime = elapsedTime;
        timeSinceLastDrop += deltaTime;

        // close window
        if (keyEsc.isPressed(window) || keyQ.isPressed(window))
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        // move active block
        if (keyUp.isPressed(window))
            activeBlock.moveSideways(squares, 0, 1);
        if (keyDown.isPressed(window))
            activeBlock.moveSideways(squares, 0, -1);
        if (keyLeft.isPressed(window))
            activeBlock.moveSideways(squares, -1, 0);
        if (keyRight.isPressed(window))
            activeBlock.moveSideways(squares, 1, 0);
        if (keyZ.isPressed(window))
            activeBlock.rotateLeft(squares);
        if (keyC.isPressed(window))
            activeBlock.rotateRight(squares);
        if (keyX.isPressed(window))
            activeBlock.goDown(squares, timeSinceLastDrop);
        if (keySpace.isPressed(window))
            activeBlock.goDown(squares, timeSinceLastDrop, true);

        // toggle illumination and textures
        if (keyI.isPressed(window))
            shader->setUniform("u_illuminate", (illuminate = !illuminate));
        if (keyT.isPressed(window))
            shader->setUniform("u_blend", (blendTexture = !blendTexture));

        // automatically move active block down
        if (timeSinceLastDrop > 2.0f)
            activeBlock.goDown(squares, timeSinceLastDrop);

        // update lights and draw models
        setLights(shader);
        drawWalls(nearWall, sideWall, shader);
        drawCubes(squares, interpolation, deltaTime, cube, shader);
        activeBlock.draw(shader, blendTexture);

        // reduce full tetris lines
        removeLines(squares, interpolation);

        glfwSwapBuffers(window);
    }
    return 0;
}

/*
 * This function does quite a lot of things.
 * It draws solid cubes
 * It draws cubes interpolated cubes that are in the process of being removed
 * It moves cubes down after cubes have been fully removed
 */
void Application::drawCubes(bool (*squares)[5][5],
                            float (*interpolated)[5][5],
                            float dt,
                            Model &cube,
                            const std::shared_ptr<Shader> &shader) {

    shader->setUniform("u_cubeTexture",
                       TextureManager::GetInstance()->GetUnitByName("cube"));
    shader->setUniform("u_cubemap", true);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                bool solid = squares[i][j][k];
                float fade = interpolated[i][j][k];
                if (solid || fade > 0.0f) {
                    shader->setUniform("u_color", getLevelColor(i));
                    cube.setTranslation({k, j, i});
                    if (fade > 0.0f) {
                        float scale = lerp(1.0f, 0.0f,
                                           sstep3(1.0f-interpolated[i][j][k]));
                        cube.setScale({scale, scale, scale});
                        interpolated[i][j][k] -= dt;
                    } else {
                        cube.setScale({1.0f, 1.0f, 1.0f});
                    }
                    cube.draw(shader);
                }
                if (fade < 0.0f) {
                    int row = i;
                    while (squares[row+1][j][k] && row < 8) {
                        squares[row][j][k] = squares[row+1][j][k];
                        row++;
                    }
                    squares[row][j][k] = false;
                    interpolated[i][j][k] = 0.0f;
                }
            }
        }
    }
}

/**
 * Removes all full lines in the board. Checks both x and y axis, but not z
 * @param squares      - boolean matrix of where the blocks are
 * @param interpolated - adds interpolation time so that smooth animation can
 *                       be applied to the removed square
 */
void Application::removeLines(bool squares[10][5][5], float interpolated[10][5][5]) {
    int oldScore = score;
    for (int i = 0; i < 10; i++) {
        bool forRemoval[5][5] = {};
        for (int j = 0; j < 5; j++) {
            if (squares[i][j][0] && squares[i][j][1] &&
                squares[i][j][2] && squares[i][j][3] &&
                squares[i][j][4]) {
                for (int k = 0; k < 5; k++)
                    forRemoval[j][k] = true;
            }
            if (squares[i][0][j] && squares[i][1][j] &&
                squares[i][2][j] && squares[i][3][j] &&
                squares[i][4][j]) {
                for (int k = 0; k < 5; k++)
                    forRemoval[k][j] = true;
            }
        }
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (forRemoval[j][k]) {
                    squares[i][j][k] = false;
                    interpolated[i][j][k] = 1.0f;
                    score += 10;
                }
            }
        }
    }
    if (oldScore != score)
        std::cout << "Score: " << score << std::endl;
}

void Application::setLights(const std::shared_ptr<Shader> &shader) {
    auto lightManager = LightManager::GetInstance();
    Model cube(GeometricTools::UnitCube3D24WNormals,
               GeometricTools::UnitCube3DTopologyTriangles24,
               BufferLayout({{ShaderDataType::Float3, "position"},
                             {ShaderDataType::Float3, "normals"}}));
    cube.setScale({0.1f, 0.1f, 0.1f});
    float time = glfwGetTime() * 0.5f;
    std::stringstream ss;
    for (int i = 0; i < 10; i++) {
        ss << "spot" << i+1;
        lightManager->setPosition(ss.str(), {calcLight2DPos(time), 0.5f+i});
        shader->uploadSpotlight(ss.str());
        ss.str("");
    }
}


glm::vec2 Application::calcLight2DPos(float time) {
    while (time > 4.0f)
        time -= 4.0f;
    if (time > 3.0f) {
        time -= 3.0f;
        return {-0.5f, lerp(4.5f, -0.5f, sstep3(time))};
    } else if (time > 2.0f) {
        time -= 2.0f;
        return {lerp(4.5f, -0.5f, sstep3(time)), 4.5f};
    } else if (time > 1.0f) {
        time -= 1.0f;
        return {4.5f, lerp(-0.5f, 4.5f, sstep3(time))};
    } else {
        return {lerp(-0.5f, 4.5f, sstep3(time)), -0.5f};
    }
}

float Application::lerp(float start, float end, float pt) {
    return (1.0f - pt) * start + pt * end;
}

float Application::sstep3(float pt) {
    return pt * pt * (3.0f - 2.0f * pt);
}

void Application::setupAllLights(float constant, float linear, float quadric) {
    auto lightManager = LightManager::GetInstance();
    std::stringstream ss;
    for (int i = 1; i <= 10; i++) {
        ss << "spot" << i;
        lightManager->addSpotLight({ss.str(), constant, linear, quadric});
        ss.str("");
    }
}

void Application::drawWalls(Model &farWall, Model &sideWall,
                            const std::shared_ptr<Shader> &shader) {
    shader->setUniform("u_walls", true);

    // bottom
    farWall.setScale({5.0f, 5.0f, 5.0f});
    farWall.setTranslation({2.0f, 2.0f, -0.5f});
    farWall.draw(shader);

    // left
    shader->setUniform("u_walls_normal", {1.0f, 0.0f, 0.0f});
    sideWall.setScale({10.0f, 5.0f, 5.0f});
    sideWall.setRotation({0.0f, 1.0f, 0.0f}, -90);
    sideWall.setTranslation({-0.5f, 2.0f, 4.5f});
    sideWall.draw(shader);

    // right
    shader->setUniform("u_walls_normal", {-1.0f, 0.0f, 0.0f});
    sideWall.setTranslation({4.5f, 2.0f, 4.5f});
    sideWall.draw(shader);

    // top
    shader->setUniform("u_walls_normal", {0.0f, -1.0f, 0.0f});
    sideWall.setRotation({0.0f, 0.0f, 1.0f}, 90);
    sideWall.addRotation({0.0f, 1.0f, 0.0f}, 90);
    sideWall.setTranslation({2.0f, 4.5f, 4.5f});
    sideWall.draw(shader);

    // bottom
    shader->setUniform("u_walls_normal", {0.0f, 1.0f, 0.0f});
    sideWall.setTranslation({2.0f, -0.5f, 4.5f});
    sideWall.draw(shader);
    shader->setUniform("u_walls", false);

}

glm::vec4 Application::getLevelColor(const int level) {
    switch (level) {
        case 0: return {1.0f, 0.1f, 0.1f, 1.0f};  // red
        case 1: return {0.1f, 1.0f, 0.5f, 1.0f};  // green
        case 2: return {0.1f, 0.5f, 1.0f, 1.0f};  // light blue
        case 3: return {1.0f, 0.1f, 1.0f, 1.0f};  // purple
        case 4: return {1.0f, 1.0f, 0.1f, 1.0f};  // yellow
        case 5: return {0.1f, 0.5f, 0.2f, 1.0f};  // dark green
        case 6: return {1.0f, 1.0f, 1.0f, 1.0f};  // white
        case 7: return {1.0f, 0.1f, 0.5f, 1.0f};  // pink
        case 8: return {0.1f, 0.1f, 1.0f, 1.0f};  // dark blue
        case 9: return {1.0f, 0.1f, 0.1f, 1.0f};  // red
        default: return {1.0f, 1.0f, 1.0f, 1.0f};
    }
}
