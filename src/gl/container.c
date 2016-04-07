#include <at/gl.h>
#include <at/core.h>
#include <string.h>
/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
typedef struct _AtGLContainerPrivate{
  AtGLObject** children;
  uint32_t num_children;
  uint64_t num_allocated;
}AtGLContainerPrivate;

static void
at_gl_container_object_interface_init(AtGLObject *iface);

G_DEFINE_TYPE_WITH_PRIVATE_AND_CODE(AtGLContainer, at_gl_container, G_TYPE_OBJECT, G_IMPLEMENT_INTERFACE(AT_TYPE_GL_OBJECT, at_gl_container_object_interface_init))

static void
at_gl_container_dispose(GObject* object){
  AtGLContainerPrivate* priv = at_gl_container_get_instance_private(AT_GL_CONTAINER(object));
  uint32_t i;
  for(i = 0; i < priv->num_children;i++)
    g_clear_object(&priv->children[i]);
  g_free(priv->children);
  G_OBJECT_CLASS(at_gl_container_parent_class)->dispose(object);
}
static void
at_gl_container_finalize(GObject* object){
  G_OBJECT_CLASS(at_gl_container_parent_class)->finalize(object);
}

static void
at_gl_container_class_init(AtGLContainerClass *klass){
  GObjectClass* object_class = G_OBJECT_CLASS(klass);
  object_class->dispose = at_gl_container_dispose;
  object_class->finalize = at_gl_container_finalize;
}

static void
at_gl_container_init(AtGLContainer *self){
  AtGLContainerPrivate* priv = at_gl_container_get_instance_private(self);
  priv->children = NULL;
  priv->num_children = 0;
  priv->num_allocated = 0;
}
static void
at_gl_container_object_interface_init(AtGLObject *iface){

}

/*===========================================================================
 * PUBLIC API
 *===========================================================================*/
uint8_t
at_gl_container_get_num_children(AtGLContainer* container){
  AtGLContainerPrivate* priv = at_gl_container_get_instance_private(container);
  return priv->num_children;
}

void
at_gl_container_add_object(AtGLContainer* container, AtGLObject* object){
  AtGLContainerPrivate* priv = at_gl_container_get_instance_private(container);
  // If there is not any memory
  if(priv->children == NULL){
    priv->children = g_malloc(sizeof(AtGLObject*));
    priv->num_allocated++;
  // If there is no sufficient memory
  }else if(priv->num_children == priv->num_allocated){
    priv->num_allocated <<= 1;
    priv->children = g_realloc(priv->children, priv->num_allocated * sizeof(AtGLObject*));
  }
  memset(&priv->children[priv->num_children],0,(priv->num_allocated-priv->num_children)*sizeof(AtGLObject*));
  g_set_object(&priv->children[priv->num_children], object);
  priv->num_children++;
}

int32_t
at_gl_container_index_of(AtGLContainer* container, AtGLObject* object){
  AtGLContainerPrivate* priv = at_gl_container_get_instance_private(container);
  uint32_t i;
  for(i = 0; i < priv->num_children; i++){
    if(priv->children[i] == object) return i;
  }
  return -1;
}

void
at_gl_container_remove_object(AtGLContainer* container, AtGLObject* object){
  int32_t index = at_gl_container_index_of(container, object);
  if(index != -1){
    AtGLContainerPrivate* priv = at_gl_container_get_instance_private(container);
    g_clear_object(&priv->children[index]);
    priv->children--;
    priv->children[index] = priv->children[priv->num_children];
  }
}
