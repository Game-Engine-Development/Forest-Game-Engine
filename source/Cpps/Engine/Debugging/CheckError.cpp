#include "Headers/Engine/Debugging/CheckError.h"

GLenum glCheckErrorThenExit_(const char *file, int line)
{
    GLenum errorCode = glGetError();
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_NO_ERROR:                      error = "NO_ERROR"; break;
            }

        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
        std::exit(-1);

    return errorCode;
}

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    errorCode = glGetError();
    std::string error;
    switch (errorCode)
    {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        case GL_NO_ERROR:                      error = "NO_ERROR"; break;
    }
    std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    if(errorCode != GL_NO_ERROR) {
        std::exit(-1);
    }

    return errorCode;
}




