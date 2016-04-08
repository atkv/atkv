#include <at/gl.h>
#include <epoxy/gl.h>

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
typedef struct _AtGLProgramPrivate{
  int32_t id;
  char* name;
  AtGLShader* vertex_shader;
  AtGLShader* fragment_shader;
  AtGLShader* geometry_shader;
}AtGLProgramPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLProgram, at_gl_program, G_TYPE_OBJECT)


static void
at_gl_program_dispose(GObject* object){
  AtGLProgram*        program = AT_GL_PROGRAM(object);
  AtGLProgramPrivate* priv    = at_gl_program_get_instance_private(program);
  int current_program_id = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM,&current_program_id);
  if(current_program_id == priv->id) glUseProgram(0);
  glDeleteProgram(priv->id);
  g_clear_object(&priv->vertex_shader);
  g_clear_object(&priv->fragment_shader);
  g_clear_object(&priv->geometry_shader);
  G_OBJECT_CLASS(at_gl_program_parent_class)->dispose(object);
}

static void
at_gl_program_finalize(GObject* object){
  AtGLProgram*        program = AT_GL_PROGRAM(object);
  AtGLProgramPrivate* priv    = at_gl_program_get_instance_private(program);
  g_free(priv->name);
  G_OBJECT_CLASS(at_gl_program_parent_class)->finalize(object);
}
static void
at_gl_program_class_init(AtGLProgramClass *klass){
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  object_class->finalize = at_gl_program_finalize;
  object_class->dispose  = at_gl_program_dispose;
}

static void
at_gl_program_init(AtGLProgram *self){
  AtGLProgramPrivate* priv = at_gl_program_get_instance_private(self);
  priv->fragment_shader = NULL;
  priv->vertex_shader   = NULL;
  priv->geometry_shader = NULL;
  priv->id = -1;
}

static AtGLProgram*
at_gl_program_new_linking(AtGLShader* vertex_shader,
                          AtGLShader* fragment_shader,
                          gboolean should_link,
                          GError** error){
  g_return_val_if_fail(error == NULL || *error == NULL, NULL);

  AtGLProgram*        program = at_gl_program_new();
  AtGLProgramPrivate* priv    = at_gl_program_get_instance_private(program);

  // Create the program
  priv->id = glCreateProgram();
  g_set_object(&priv->vertex_shader, vertex_shader);
  g_set_object(&priv->fragment_shader, fragment_shader);

  glAttachShader(priv->id, at_gl_shader_get_id(vertex_shader));
  glAttachShader(priv->id, at_gl_shader_get_id(fragment_shader));
  if(should_link) {
    at_gl_program_link(program, error);
    if(*error != NULL) return NULL;
  }
  return program;
}



/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
AtGLProgram*
at_gl_program_new(){
  return g_object_new(AT_TYPE_GL_PROGRAM, NULL);
}
AtGLProgram*
at_gl_program_new_from_shaders(AtGLShader* vertex_shader,
                               AtGLShader* fragment_shader, GError** error){
  return at_gl_program_new_linking(vertex_shader, fragment_shader, TRUE, error);
}
AtGLProgram*
at_gl_program_new_from_shaders_with_geometry(AtGLShader* vertex_shader,
                                             AtGLShader* fragment_shader,
                                             AtGLShader* geometry_shader,
                                             GError** error){
  g_return_val_if_fail(error == NULL || *error == NULL, NULL);

  AtGLProgram* program = at_gl_program_new_linking(vertex_shader,
                                                   fragment_shader,
                                                   TRUE,
                                                   error);
  if(error != NULL) return NULL;

  AtGLProgramPrivate* priv = at_gl_program_get_instance_private(program);
  g_set_object(&priv->geometry_shader, geometry_shader);
  glAttachShader(priv->id, at_gl_shader_get_id(geometry_shader));
  at_gl_program_link(program, error);
  if(error != NULL) return NULL;
}

void
at_gl_program_link(AtGLProgram* program, GError** error){
  AtGLProgramPrivate* priv = at_gl_program_get_instance_private(program);
  glLinkProgram(priv->id);

  // Verify error
  if(!at_gl_program_is_linked(program)){
    int info_log_length;
    glGetProgramiv(priv->id, GL_INFO_LOG_LENGTH, &info_log_length);
    g_autofree char* info_log_str = g_malloc(sizeof(char) * (info_log_length+1));
    glGetShaderInfoLog(priv->id, info_log_length, NULL, info_log_str);
    g_set_error(error, AT_GL_PROGRAM_ERROR, AT_GL_PROGRAM_ERROR_LINK, info_log_str);
  }
}
void
at_gl_program_set_shader(AtGLProgram* program, AtGLShader* shader){
  AtGLProgramPrivate* priv = at_gl_program_get_instance_private(program);
  switch(at_gl_shader_get_shader_type(shader)){
    case GL_VERTEX_SHADER:   priv->vertex_shader = shader;break;
    case GL_FRAGMENT_SHADER: priv->fragment_shader = shader;break;
    case GL_GEOMETRY_SHADER: break;
  }
}

gboolean
at_gl_program_is_linked(AtGLProgram* program){
  AtGLProgramPrivate* priv = at_gl_program_get_instance_private(program);
  GLint result = GL_FALSE;
  glGetProgramiv(priv->id, GL_LINK_STATUS, &result);
  return result;
}

GQuark
at_gl_program_error_quark(void){
  return g_quark_from_static_string("at-gl-program-error-quark");
}
