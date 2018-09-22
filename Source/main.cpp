#include<SFML/Graphics.hpp>
#include<GL/glew.h>
#include"Map.h"
#include<iostream>
#include"BasicShader.h"
int main(){
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Template", sf::Style::Fullscreen, settings);
    glewExperimental = true;
    glewInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Map m;


    BasicShader shader("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag");
    shader.loadAspectRatio((float)window.getSize().y/window.getSize().x);

    sf::Image tile_texture;
    if(!tile_texture.loadFromFile("Textures/tiles.bmp")){
        std::cout<<"Couldnt load texture"<<std::endl;
        return 1;
    }
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tile_texture.getSize().x, tile_texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tile_texture.getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    sf::Clock clock;
    int counter = 0;
    float camX=0, camY =0;
    while(window.isOpen()){

        sf::Event e;
        while(window.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
            window.close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
            camY+=0.01;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
            camY-=0.01;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
            camX+=0.01;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
            camX-=0.01;
        }
        glClearColor(0.3f,0.5f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        shader.use();
        shader.loadCameraPosition(camX,camY);
        m.draw(shader);
        window.display();

        float currentTime = clock.restart().asSeconds();
        double fps = 1.0 / currentTime;
        if(counter++>100){
            //std::cout<<fps<<std::endl; //Display FPS
            counter = 0;
        }
    }
    return 0;
}
