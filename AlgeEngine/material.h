#include "AlgeEngine.h"

class Material {
public:
    Material(const char* filename1, const char* filename2 = nullptr); // Allow optional second texture
    ~Material();
    void use();

private:
    unsigned int texture, texture2;
    bool hasSecondTexture; // Track if texture2 is used
};
