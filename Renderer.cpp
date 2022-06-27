#include "Renderer.h"

// An quad covering the screen
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,
    -1.0f,
    0.0f,

    1.0f,
    -1.0f,
    0.0f,

    -1.0f,
    1.0f,
    0.0f,

    1.0f,
    -1.0f,
    0.0f,

    1.0f,
    1.0f,
    0.0f,

    -1.0f,
    1.0f,
    0.0f,
};

const double pi = 3.14159265359;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

Renderer::Renderer(int width, int height, function<void(Camera &, vector<Object> &, int)> scene) : width(width), height(height), scene(scene)
{
    output.create(width, height, sf::ContextSettings{0, 0, 0, 4, 2});

    if (!fpsFont.loadFromFile("res/Fonts/Roboto-Medium.ttf"))
        cout << "Error loading font\n";
}

void Renderer::Render(int startFrame, int endFrame, int samples)
{
    output.setActive(true);

    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    vec3 lightDir(0.5, -1, 0.2);
    lightDir = normalize(lightDir);

    Texture skybox("res/SkyBox.png");
    skybox.SetFilters(GL_LINEAR, GL_LINEAR);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint objectbuffer;
    glGenBuffers(1, &objectbuffer);

    Shader def("res/vert.glsl", "res/frag.glsl");

    for (int frame = startFrame; frame < endFrame; frame++)
    {

        Camera camera(Transform(), float(width) / float(height));
        vector<Object> objects;

        scene(camera, objects, frame);

        def.BindShader();

        glBindBuffer(GL_UNIFORM_BUFFER, objectbuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Object) * objects.size(), &objects[0], GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, objectbuffer);

        mat4 _CameraInverseProjection = inverse(camera.ProjectionMatrix());
        mat4 _CameraToWorld = inverse(camera.ViewMatrix());

        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2f(glGetUniformLocation(def, "u_resolution"), width, height);
        glUniformMatrix4fv(glGetUniformLocation(def, "_CameraInverseProjection"), 1, false, &_CameraInverseProjection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(def, "_CameraToWorld"), 1, false, &_CameraToWorld[0][0]);
        glUniform1i(glGetUniformLocation(def, "n"), objects.size());
        glUniform3f(glGetUniformLocation(def, "lightDir"), lightDir[0], lightDir[1], lightDir[2]);

        // def.reload();

        glActiveTexture(GL_TEXTURE1);
        skybox.BindTexture();
        glUniform1i(glGetUniformLocation(def, "skybox"), 1);
        glActiveTexture(GL_TEXTURE0);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        auto printProgress = [&](int sample)
        {
            cout << "\rFrame " << frame << " ";

            float d = std::min(50.f / float(samples), 1.f);

            for (int i = 0; i < samples * d; i++)
            {
                if (i < sample * d)
                    cout << "#";
                else
                    cout << ".";
            }
            cout << " " << sample << " / " << samples;
            cout.flush();
        };

        printProgress(0);

        for (int sample = 0; sample < samples; sample++)
        {

            glUniform1f(glGetUniformLocation(def, "u_time"), frame * samples + sample);
            glUniform1i(glGetUniformLocation(def, "smple"), sample + 1);

            // glClear(GL_DEPTH_BUFFER_BIT);
            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glFinish();

            printProgress(sample + 1);
        }

        glDisableVertexAttribArray(0);

        sf::Image out = output.getTexture().copyToImage();
        out.flipVertically();
        out.saveToFile("Render/image" + to_string(frame) + ".jpg");

        // cout << "\n";
    }

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &objectbuffer);
}