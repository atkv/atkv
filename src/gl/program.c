#include <at/gl.h>

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
typedef struct _AtGLProgramPrivate{
  AtGLShader* vertex_shader;
  AtGLShader* fragment_shader;
}AtGLProgramPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLProgram, at_gl_program, G_TYPE_OBJECT)
static void
at_gl_program_class_init(AtGLProgramClass *klass){

}
static void
at_gl_program_init(AtGLProgram *self){
  AtGLProgramPrivate* priv = at_gl_program_get_instance_private(self);
  priv->fragment_shader = NULL;
  priv->vertex_shader = NULL;
}

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
AtGLProgram*
at_gl_program_new(){
  return g_object_new(AT_TYPE_GL_PROGRAM, NULL);
}
AtGLProgram*
at_gl_program_new_from_shaders(AtGLShader* vertexShader,
                               AtGLShader* fragmentShader){
  AtGLProgram* program = at_gl_program_new();
  return program;
}
