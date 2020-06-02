#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

GLenum glCheckErrorThenExit_(const char *file, int line);

GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)
#define glCheckErrorThenExit() glCheckErrorThenExit_(__FILE__, __LINE__)