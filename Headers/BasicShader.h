#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H
#include "ShaderProgram.h"
#include<glm/matrix.hpp>
class Camera;
class BasicShader: public ShaderProgram{
    public:
        BasicShader(const char* vertexPath, const char*fragmentPath);
        void loadAspectRatio(float aspectRatio);
        void loadCameraPosition(float x, float y);
        void setTranslation(float x, float y);
    private:
        int aspect_ratio_uniform_location, cam_x_uniform_location, cam_y_uniform_location, trans_x_uniform_location, trans_y_uniform_location;
};

#endif
