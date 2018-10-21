#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include<vector>

class TextureManager{
    public:
        TextureManager();
        ~TextureManager();
        void loadTexture(const char* fileName);
        void useTexture(int id);
        

    private:
        std::vector<unsigned int> textureHandles;
};
#endif
