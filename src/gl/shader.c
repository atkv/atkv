#include <at/gl.h>
#include <epoxy/gl.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLShaderPrivate{
  gint     id;
  char*    name;
  char*    code;
  int32_t  type;
  uint32_t code_length;
}AtGLShaderPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(AtGLShader, at_gl_shader, G_TYPE_OBJECT)

static void
at_gl_shader_dispose(GObject* object){
  AtGLShaderPrivate *priv = at_gl_shader_get_instance_private(AT_GL_SHADER(object));
  glDeleteShader(priv->id);
  G_OBJECT_CLASS(at_gl_shader_parent_class)->dispose(object);
}
static void
at_gl_shader_finalize(GObject* object){
  AtGLShaderPrivate *priv = at_gl_shader_get_instance_private(AT_GL_SHADER(object));
  g_free(priv->code);
  g_free(priv->name);
  G_OBJECT_CLASS(at_gl_shader_parent_class)->finalize(object);
}

static void
at_gl_shader_class_init(AtGLShaderClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = at_gl_shader_dispose;
  object_class->finalize = at_gl_shader_finalize;
}

static void
at_gl_shader_init(AtGLShader *self){
  AtGLShaderPrivate* priv = at_gl_shader_get_instance_private(self);
  priv->id = -1;
  priv->name = NULL;
  priv->code = NULL;
  priv->type = -1;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLShader*
at_gl_shader_new(){
  return g_object_new(AT_TYPE_GL_SHADER, NULL);
}
AtGLShader*
at_gl_shader_new_from_file(int32_t type, char* file, GError** error){

  g_return_val_if_fail(error == NULL || *error == NULL, NULL);

  char* code = NULL;
  uint64_t code_length = 0;

  // Read content and create the shader
  if(g_file_get_contents(file, &code, &code_length, error)){
    AtGLShader* shader      = at_gl_shader_new();
    AtGLShaderPrivate* priv = at_gl_shader_get_instance_private(shader);
    priv->code = code;
    priv->code_length = code_length;
    priv->type = type;
    priv->id = glCreateShader(type);
    at_gl_shader_compile(shader, error);
    if(*error != NULL) return NULL;
    return shader;
  }

  // If the was a problem
  g_free(code);
  return NULL;
}

void
at_gl_shader_compile(AtGLShader *shader, GError** error){

  g_return_if_fail(error == NULL || *error == NULL);

  AtGLShaderPrivate* priv = at_gl_shader_get_instance_private(shader);
  g_autofree char* info_log_str = NULL;
  int32_t info_log_length = 0;

  // Compile
  glShaderSource(priv->id, 1, (const char* const*)&priv->code, &priv->code_length);
  glCompileShader(priv->id);

  // Verify error
  if(!at_gl_shader_is_compiled(shader)){
    glGetShaderiv(priv->id, GL_INFO_LOG_LENGTH, &info_log_length);
    info_log_str = g_malloc(sizeof(char) * (info_log_length+1));
    glGetShaderInfoLog(priv->id, info_log_length, NULL, info_log_str);
    g_set_error(error, AT_GL_SHADER_ERROR, AT_GL_SHADER_ERROR_COMPILE, info_log_str);
  }
}

gboolean
at_gl_shader_is_compiled(AtGLShader* shader){
  AtGLShaderPrivate* priv = at_gl_shader_get_instance_private(shader);
  GLint result = GL_FALSE;
  glGetShaderiv(priv->id, GL_COMPILE_STATUS, &result);
  return result;
}
GQuark
at_gl_shader_error_quark(void){
  return g_quark_from_static_string("at-gl-shader-error-quark");
}

gint
at_gl_shader_get_id(AtGLShader* shader){
  AtGLShaderPrivate* priv = at_gl_shader_get_instance_private(shader);
  return priv->id;
}
int32_t
at_gl_shader_get_shader_type(AtGLShader* shader){
  AtGLShaderPrivate* priv = at_gl_shader_get_instance_private(shader);
  return priv->type;
}
