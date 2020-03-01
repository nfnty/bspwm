#include <jansson.h>
#include "subscribe.h"

#define SERIALIZE(TYPE) \
json_t* json_serialize_##TYPE##_type(TYPE##_t *obj);

#define SERIALIZATION(TYPE) SERIALIZE(TYPE)

// Enums
SERIALIZATION(subscriber_mask)

// Structs
SERIALIZATION(monitor)

#undef SERIALIZE
#undef DESERALIZE
#undef SERIALIZATION

// Misc
json_t* json_serialize_node_window(node_t *obj);

json_t* json_serialize_desktop_name(desktop_t *obj);

json_t* json_serialize_monitor_name(monitor_t *obj);

json_t* json_serialize_status_node(monitor_t *m, desktop_t *d, node_t *n);
json_t* json_serialize_status_node_nullable(monitor_t *m, desktop_t *d, node_t *n);
json_t* json_serialize_status_node_transfer(monitor_t *ms, desktop_t *ds, node_t *ns, monitor_t *md, desktop_t *dd);
json_t* json_serialize_status_node_swap(monitor_t *m1, desktop_t *d1, node_t *n1, monitor_t *m2, desktop_t *d2, node_t *n2);
json_t* json_serialize_status_desktop(monitor_t *m, desktop_t *d);
json_t* json_serialize_status_desktop_transfer(monitor_t *ms, monitor_t *md, desktop_t *d);
json_t* json_serialize_status_desktop_rename(monitor_t *m, desktop_t *d, const char *name_last);
json_t* json_serialize_status_desktop_swap(monitor_t *m1, desktop_t *d1, monitor_t *m2, desktop_t *d2);
json_t* json_serialize_status_monitor_rename(monitor_t *m, const char *name_last);
json_t* json_serialize_status_monitor_swap(monitor_t *m1, monitor_t *m2);

json_t* json_serialize_query_windows(coordinates_t loc);
json_t* json_serialize_query_window(coordinates_t loc);
json_t* json_serialize_query_desktops(coordinates_t loc);
json_t* json_serialize_query_desktop(coordinates_t loc);
json_t* json_serialize_query_monitors(coordinates_t loc);
json_t* json_serialize_query_monitor(coordinates_t loc);
json_t* json_serialize_query_tree(coordinates_t loc);
json_t* json_serialize_query_history(coordinates_t loc);
json_t* json_serialize_query_stack();
