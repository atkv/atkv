#include <at/gl.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLShaderPrivate{

}AtGLShaderPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(AtGLShader, at_gl_shader, G_TYPE_OBJECT)
static void
at_gl_shader_class_init(AtGLShaderClass *klass){

}

static void
at_gl_shader_init(AtGLShader *self){

}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLShader*
at_gl_shader_new(){
  return g_object_new(AT_TYPE_GL_SHADER, NULL);
}
AtGLShader*
at_gl_shader_new_from_file(char* file){
  AtGLShader* shader = at_gl_shader_new();
  return shader;
}
