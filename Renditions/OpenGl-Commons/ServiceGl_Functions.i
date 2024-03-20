// programs
Define(PFNGLGETPROGRAMIVPROC, glGetProgramiv)
Define(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)
Define(PFNGLCREATEPROGRAMPROC, glCreateProgram)
Define(PFNGLLINKPROGRAMPROC, glLinkProgram)
Define(PFNGLDELETEPROGRAMPROC, glDeleteProgram)
Define(PFNGLUSEPROGRAMPROC, glUseProgram)

// uniforms
Define(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)
Define(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)
Define(PFNGLUNIFORM1IPROC, glUniform1i)
Define(PFNGLUNIFORM4FVPROC, glUniform4fv)
Define(PFNGLUNIFORM3FVPROC, glUniform3fv)

// shaders
Define(PFNGLGETSHADERIVPROC, glGetShaderiv)
Define(PFNGLDELETESHADERPROC, glDeleteShader)
Define(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)
Define(PFNGLATTACHSHADERPROC, glAttachShader)
Define(PFNGLCREATESHADERPROC, glCreateShader)
Define(PFNGLSHADERSOURCEPROC, glShaderSource)
Define(PFNGLCOMPILESHADERPROC, glCompileShader)

// buffers
Define(PFNGLGENBUFFERSPROC, glGenBuffers)
Define(PFNGLBINDBUFFERPROC, glBindBuffer)
Define(PFNGLBUFFERDATAPROC, glBufferData)
Define(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)

// vertex arrays
Define(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)
Define(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)
Define(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)
Define(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)
Define(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)

// new vertex arrays
Define(PFNGLVERTEXATTRIBFORMATPROC, glVertexAttribFormat)
Define(PFNGLVERTEXATTRIBBINDINGPROC, glVertexAttribBinding)
Define(PFNGLVERTEXBINDINGDIVISORPROC, glVertexBindingDivisor)
Define(PFNGLBINDVERTEXBUFFERPROC, glBindVertexBuffer)