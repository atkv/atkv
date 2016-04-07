#include <at/gl.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLScenePrivate{
  char* name;
}AtGLScenePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AtGLScene, at_gl_scene, AT_TYPE_GL_CONTAINER)
static void
at_gl_scene_class_init(AtGLSceneClass *klass){

}
static void
at_gl_scene_init(AtGLScene *self){
  AtGLScenePrivate* priv = at_gl_scene_get_instance_private(self);
  priv->name = NULL;
}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
AtGLScene*
at_gl_scene_new(){
  return g_object_new(AT_TYPE_GL_SCENE, NULL);
}

void
at_gl_scene_add_mesh(AtGLScene* scene, AtGLMesh* mesh){
  at_gl_container_add_object(AT_GL_CONTAINER(scene),AT_GL_OBJECT(mesh));
}
