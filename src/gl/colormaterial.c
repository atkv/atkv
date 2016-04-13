#include <at/gl.h>
#include <string.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
#define GET_PRIV(object) AtGLColorMaterialPrivate* priv = at_gl_colormaterial_get_instance_private(object)
typedef struct _AtGLColorMaterialPrivate{
  AtVec3 ambient;
  AtVec3 diffuse;
  AtVec3 specular;
  AtVec3 emissive;
}AtGLColorMaterialPrivate;
G_DEFINE_TYPE_WITH_PRIVATE(AtGLColorMaterial, at_gl_colormaterial, AT_TYPE_GL_MATERIAL)
static void
at_gl_colormaterial_class_init(AtGLColorMaterialClass *klass){

}

static void
at_gl_colormaterial_init(AtGLColorMaterial *self){
  GET_PRIV(self);
  memset(priv, 0, sizeof(AtGLColorMaterialPrivate));
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

AtGLColorMaterial*
at_gl_colormaterial_new(){
  return g_object_new(AT_TYPE_GL_COLORMATERIAL, NULL);
}

void
at_gl_colormaterial_set_ambient(AtGLColorMaterial* colormaterial,
                                AtVec3* ambient){
  GET_PRIV(colormaterial);
  priv->ambient = *ambient;
}

void
at_gl_colormaterial_set_diffuse(AtGLColorMaterial* colormaterial,
                                AtVec3* diffuse){
  GET_PRIV(colormaterial);
  priv->diffuse = *diffuse;
}

void
at_gl_colormaterial_set_specular(AtGLColorMaterial* colormaterial,
                                 AtVec3* specular){
  GET_PRIV(colormaterial);
  priv->specular = *specular;
}

void
at_gl_colormaterial_set_emissive(AtGLColorMaterial* colormaterial,
                                 AtVec3* emissive){
  GET_PRIV(colormaterial);
  priv->emissive = *emissive;
}

AtVec3
at_gl_colormaterial_get_ambient(AtGLColorMaterial* colormaterial){
  GET_PRIV(colormaterial);
  return priv->ambient;
}

AtVec3
at_gl_colormaterial_get_diffuse(AtGLColorMaterial* colormaterial){
  GET_PRIV(colormaterial);
  return priv->diffuse;
}

AtVec3
at_gl_colormaterial_get_specular(AtGLColorMaterial* colormaterial){
  GET_PRIV(colormaterial);
  return priv->specular;
}

AtVec3
at_gl_colormaterial_get_emissive(AtGLColorMaterial* colormaterial){
  GET_PRIV(colormaterial);
  return priv->emissive;
}
