#include <jansson.h>
#include <string.h>
#include "bspwm.h"
#include "query.h"
#include "tree.h"
#include "types.h"
#include "subscribe.h"

// Add more if __VA_ARGS__ > 30
#define FE_1(WHAT, X) WHAT(X)
#define FE_2(WHAT, X, ...) WHAT(X)FE_1(WHAT, __VA_ARGS__)
#define FE_3(WHAT, X, ...) WHAT(X)FE_2(WHAT, __VA_ARGS__)
#define FE_4(WHAT, X, ...) WHAT(X)FE_3(WHAT, __VA_ARGS__)
#define FE_5(WHAT, X, ...) WHAT(X)FE_4(WHAT, __VA_ARGS__)
#define FE_6(WHAT, X, ...) WHAT(X)FE_5(WHAT, __VA_ARGS__)
#define FE_7(WHAT, X, ...) WHAT(X)FE_6(WHAT, __VA_ARGS__)
#define FE_8(WHAT, X, ...) WHAT(X)FE_7(WHAT, __VA_ARGS__)
#define FE_9(WHAT, X, ...) WHAT(X)FE_8(WHAT, __VA_ARGS__)
#define FE_10(WHAT, X, ...) WHAT(X)FE_9(WHAT, __VA_ARGS__)
#define FE_11(WHAT, X, ...) WHAT(X)FE_10(WHAT, __VA_ARGS__)
#define FE_12(WHAT, X, ...) WHAT(X)FE_11(WHAT, __VA_ARGS__)
#define FE_13(WHAT, X, ...) WHAT(X)FE_12(WHAT, __VA_ARGS__)
#define FE_14(WHAT, X, ...) WHAT(X)FE_13(WHAT, __VA_ARGS__)
#define FE_15(WHAT, X, ...) WHAT(X)FE_14(WHAT, __VA_ARGS__)
#define FE_16(WHAT, X, ...) WHAT(X)FE_15(WHAT, __VA_ARGS__)
#define FE_17(WHAT, X, ...) WHAT(X)FE_16(WHAT, __VA_ARGS__)
#define FE_18(WHAT, X, ...) WHAT(X)FE_17(WHAT, __VA_ARGS__)
#define FE_19(WHAT, X, ...) WHAT(X)FE_18(WHAT, __VA_ARGS__)
#define FE_20(WHAT, X, ...) WHAT(X)FE_19(WHAT, __VA_ARGS__)
#define FE_21(WHAT, X, ...) WHAT(X)FE_20(WHAT, __VA_ARGS__)
#define FE_22(WHAT, X, ...) WHAT(X)FE_21(WHAT, __VA_ARGS__)
#define FE_23(WHAT, X, ...) WHAT(X)FE_22(WHAT, __VA_ARGS__)
#define FE_24(WHAT, X, ...) WHAT(X)FE_23(WHAT, __VA_ARGS__)
#define FE_25(WHAT, X, ...) WHAT(X)FE_24(WHAT, __VA_ARGS__)
#define FE_26(WHAT, X, ...) WHAT(X)FE_25(WHAT, __VA_ARGS__)
#define FE_27(WHAT, X, ...) WHAT(X)FE_26(WHAT, __VA_ARGS__)
#define FE_28(WHAT, X, ...) WHAT(X)FE_27(WHAT, __VA_ARGS__)
#define FE_29(WHAT, X, ...) WHAT(X)FE_28(WHAT, __VA_ARGS__)
#define FE_30(WHAT, X, ...) WHAT(X)FE_29(WHAT, __VA_ARGS__)
#define GET_MACRO( \
	_1, _2, _3, _4, _5, \
	_6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, \
	_16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, \
	_26, _27, _28, _29, _30, \
	NAME, ... \
) NAME
#define FOR_EACH(ACTION, ...) \
	GET_MACRO( \
		__VA_ARGS__, \
		FE_30, FE_29, FE_28, FE_27, FE_26, \
		FE_25, FE_24, FE_23, FE_22, FE_21, \
		FE_20, FE_19, FE_18, FE_17, FE_16, \
		FE_15, FE_14, FE_13, FE_12, FE_11, \
		FE_10, FE_9, FE_8, FE_7, FE_6, \
		FE_5, FE_4, FE_3, FE_2, FE_1 \
	)(ACTION, __VA_ARGS__)

#define COMMA ,
#define SERIALIZE_CAT(X) SERIALIZE_##X
#define SERIALIZATION(TYPE, ...) \
	SERIALIZE_BEGIN(TYPE) \
	FOR_EACH(SERIALIZE_CAT, __VA_ARGS__) \
	SERIALIZE_END
//
// Enums
//

#define SERIALIZE_BEGIN(TYPE) \
	json_t* json_serialize_##TYPE##_type(TYPE##_t *obj) \
	{ \
		if (!obj) \
			return NULL;
#define SERIALIZE_IF(ENUM, VALUE) \
		else if (*obj == ENUM) \
			return json_string(VALUE);
#define SERIALIZE_END \
		else \
			return NULL; \
	}

SERIALIZATION(split_type,
	IF(TYPE_HORIZONTAL, "horizontal"),
	IF(TYPE_VERTICAL, "vertical")
)

SERIALIZATION(client_state,
	IF(STATE_TILED, "tiled"),
	IF(STATE_PSEUDO_TILED, "pseudo_tiled"),
	IF(STATE_FLOATING, "floating"),
	IF(STATE_FULLSCREEN, "fullscreen")
)

SERIALIZATION(stack_layer,
	IF(LAYER_BELOW, "below"),
	IF(LAYER_NORMAL, "normal"),
	IF(LAYER_ABOVE, "above")
)

SERIALIZATION(direction,
	IF(DIR_NORTH, "north"),
	IF(DIR_WEST, "west"),
	IF(DIR_SOUTH, "south"),
	IF(DIR_EAST, "east")
)

SERIALIZATION(layout,
	IF(LAYOUT_TILED, "tiled"),
	IF(LAYOUT_MONOCLE, "monocle")
)
SERIALIZATION(subscriber_mask,
	IF(SBSC_MASK_MONITOR_ADD, "monitorAdd"),
	IF(SBSC_MASK_MONITOR_RENAME, "monitorRename"),
	IF(SBSC_MASK_MONITOR_REMOVE, "monitorRemove"),
	IF(SBSC_MASK_MONITOR_SWAP, "monitorSwap"),
	IF(SBSC_MASK_MONITOR_FOCUS, "monitorFocus"),
	IF(SBSC_MASK_MONITOR_GEOMETRY, "monitorGeometry"),
	IF(SBSC_MASK_DESKTOP_ADD, "desktopAdd"),
	IF(SBSC_MASK_DESKTOP_RENAME, "desktopRename"),
	IF(SBSC_MASK_DESKTOP_REMOVE, "desktopRemove"),
	IF(SBSC_MASK_DESKTOP_SWAP, "desktopSwap"),
	IF(SBSC_MASK_DESKTOP_TRANSFER, "desktopTransfer"),
	IF(SBSC_MASK_DESKTOP_FOCUS, "desktopFocus"),
	IF(SBSC_MASK_DESKTOP_ACTIVATE, "desktopActivate"),
	IF(SBSC_MASK_DESKTOP_LAYOUT, "desktopLayout"),
	IF(SBSC_MASK_NODE_MANAGE, "nodeManage"),
	IF(SBSC_MASK_NODE_UNMANAGE, "nodeUnmanage"),
	IF(SBSC_MASK_NODE_SWAP, "nodeSwap"),
	IF(SBSC_MASK_NODE_TRANSFER, "nodeTransfer"),
	IF(SBSC_MASK_NODE_FOCUS, "nodeFocus"),
	IF(SBSC_MASK_NODE_PRESEL, "nodePresel"),
	IF(SBSC_MASK_NODE_STACK, "nodeStack"),
	IF(SBSC_MASK_NODE_ACTIVATE, "nodeActivate"),
	IF(SBSC_MASK_NODE_GEOMETRY, "nodeGeometry"),
	IF(SBSC_MASK_NODE_STATE, "nodeState"),
	IF(SBSC_MASK_NODE_FLAG, "nodeFlag"),
	IF(SBSC_MASK_NODE_LAYER, "nodeLayer"),
	IF(SBSC_MASK_MONITOR, "monitor"),
	IF(SBSC_MASK_DESKTOP, "desktop"),
	IF(SBSC_MASK_NODE, "node"),
	IF(SBSC_MASK_ALL, "all")
)

SERIALIZATION(wm_flags,
	IF(WM_FLAG_MODAL, "modal"),
	IF(WM_FLAG_STICKY, "sticky"),
	IF(WM_FLAG_MAXIMIZED_VERT, "maximizedVertical"),
	IF(WM_FLAG_MAXIMIZED_HORZ, "maximizedHorizontal"),
	IF(WM_FLAG_SHADED, "shaded"),
	IF(WM_FLAG_SKIP_TASKBAR, "skipTaskbar"),
	IF(WM_FLAG_SKIP_PAGER, "skipPager"),
	IF(WM_FLAG_HIDDEN, "hidden"),
	IF(WM_FLAG_FULLSCREEN, "fullscreen"),
	IF(WM_FLAG_ABOVE, "above"),
	IF(WM_FLAG_BELOW, "below"),
	IF(WM_FLAG_DEMANDS_ATTENTION, "demandsAttention")
)

#undef SERIALIZE_BEGIN
#undef SERIALIZE_IF
#undef SERIALIZE_END

//
// Structs
//

#define SERIALIZE_INIT_OBJECT(TYPE) \
		json_t *json = json_object(); \
		if (!json) { \
			warn("JSON serialize failed: "#TYPE": init_object\n"); \
			return NULL; \
		}
#define SERIALIZE_INIT_ARRAY(TYPE) \
		json_t *json = json_array(); \
		if (!json) { \
			warn("JSON serialize failed: "#TYPE": init_array\n"); \
			return NULL; \
		}


#define SERIALIZE_SET_JSONNULL(TYPE, KEY) \
		if (json_is_null(set)) { \
			json_decref(set); \
			json_decref(json); \
			warn("JSON serialize failed: "#TYPE": "KEY": json_is_null\n"); \
			return NULL; \
		}
#define SERIALIZE_SET_OBJECT_NEW(TYPE, KEY) \
		if (json_object_set_new(json, KEY, set) == -1) { \
			json_decref(json); \
			warn("JSON serialize failed: "#TYPE": "KEY": json_object_set_new\n"); \
			return NULL; \
		}

#define SERIALIZE_BEGIN(TYPE) \
	json_t* json_serialize_##TYPE##_type(TYPE##_t *obj) \
	{ \
		if (!obj) \
			return json_null(); \
		SERIALIZE_INIT_OBJECT(TYPE) \
		json_t *set;
#define SERIALIZE_INTEGER(KEY, TYPE, MEMBER) \
		set = json_integer(MEMBER); \
		SERIALIZE_SET_OBJECT_NEW(integer, KEY)
#define SERIALIZE_REAL(KEY, TYPE, MEMBER) \
		set = json_real(MEMBER); \
		SERIALIZE_SET_OBJECT_NEW(real, KEY)
#define SERIALIZE_STRING(KEY, MEMBER) \
		set = json_string(MEMBER); \
		SERIALIZE_SET_OBJECT_NEW(string, KEY)
#define SERIALIZE_BOOLEAN(KEY, MEMBER) \
		set = json_boolean(MEMBER); \
		SERIALIZE_SET_OBJECT_NEW(boolean, KEY)
#define SERIALIZE_OBJECT(KEY, VAR, TYPE, FUNCTION, MEMBER) \
		set = json_serialize_##FUNCTION(&MEMBER); \
		SERIALIZE_SET_JSONNULL(object, KEY) \
		SERIALIZE_SET_OBJECT_NEW(object, KEY)
#define SERIALIZE_ENUM(KEY, FUNCTION, MEMBER) \
		set = json_serialize_##FUNCTION(&MEMBER); \
		SERIALIZE_SET_OBJECT_NEW(enum, KEY)
#define SERIALIZE_ENUM_NULLABLE(KEY, FUNCTION, MEMBER) \
		set = json_serialize_##FUNCTION(&MEMBER); \
		if (set == NULL) \
			set = json_null(); \
		SERIALIZE_SET_OBJECT_NEW(enum_nullable, KEY)
#define SERIALIZE_POINTER(KEY, FUNCTION, MEMBER) \
		set = json_serialize_##FUNCTION(MEMBER); \
		SERIALIZE_SET_JSONNULL(pointer, KEY) \
		SERIALIZE_SET_OBJECT_NEW(pointer, KEY)
#define SERIALIZE_POINTER_NULLABLE(KEY, FUNCTION, MEMBER) \
		set = json_serialize_##FUNCTION(MEMBER); \
		SERIALIZE_SET_OBJECT_NEW(pointer_nullable, KEY)
#define SERIALIZE_FUNCTION(FUNCTION, MEMBER) \
		json_serialize_##FUNCTION(MEMBER);
#define SERIALIZE_END \
		return json; \
	}
#define SERIALIZE_SERONLY(WHAT) WHAT

SERIALIZATION(xcb_rectangle,
	INTEGER("x", int16_t, obj->x),
	INTEGER("y", int16_t, obj->y),
	INTEGER("height", uint16_t, obj->height),
	INTEGER("width", uint16_t, obj->width)
)

SERIALIZATION(icccm_props,
	BOOLEAN("takeFocus", obj->take_focus),
	BOOLEAN("inputHint", obj->input_hint),
	BOOLEAN("deleteWindow", obj->delete_window)
)

SERIALIZATION(client,
	STRING("className", obj->class_name),
	STRING("instanceName", obj->instance_name),
	INTEGER("borderWidth", unsigned int, obj->border_width),
	BOOLEAN("urgent", obj->urgent),
	BOOLEAN("shown", obj->shown),
	ENUM("state", client_state_type, obj->state),
	ENUM("lastState", client_state_type, obj->last_state),
	ENUM("layer", stack_layer_type, obj->layer),
	ENUM("lastLayer", stack_layer_type, obj->last_layer),
	OBJECT("floatingRectangle", floating_rectangle, xcb_rectangle_t, xcb_rectangle_type, obj->floating_rectangle),
	OBJECT("tiledRectangle", tiled_rectangle, xcb_rectangle_t, xcb_rectangle_type, obj->tiled_rectangle),
	OBJECT("icccmProps", icccm_props, icccm_props_t, icccm_props_type, obj->icccm_props),
	ENUM_NULLABLE("wmFlags", wm_flags_type, obj->wm_flags)
)

json_t* json_serialize_node_id(node_t *obj)
{
	if (!obj)
		return json_null();
	return json_integer(obj->id);
}

json_t* json_serialize_node_focused(node_t *obj)
{
	return obj == mon->desk->focus ? json_true() : json_false();
}

SERIALIZATION(presel,
	REAL("splitRatio", double, obj->split_ratio),
	ENUM("splitDir", direction_type, obj->split_dir),
	INTEGER("feedback", xcb_window_t, obj->feedback)
)

SERIALIZATION(constraints,
	INTEGER("min_width", uint16_t, obj->min_height),
	INTEGER("min_height", uint16_t, obj->min_width)
)

SERIALIZATION(node,
	INTEGER("id", uint32_t, obj->id),
	ENUM("splitType", split_type_type, obj->split_type),
	REAL("splitRatio", double, obj->split_ratio),
	POINTER_NULLABLE("presel", presel_type, obj->presel),
	OBJECT("rectangle", rectangle, xcb_rectangle_t, xcb_rectangle_type, obj->rectangle),
	OBJECT("constraints", constraints, constraints_t, constraints_type, obj->constraints),
	BOOLEAN("vacant", obj->vacant),
	BOOLEAN("hidden", obj->hidden),
	BOOLEAN("sticky", obj->sticky),
	BOOLEAN("private", obj->private),
	BOOLEAN("locked", obj->locked),
	BOOLEAN("marked", obj->marked),
	POINTER_NULLABLE("firstChild", node_type, obj->first_child),
	POINTER_NULLABLE("secondChild", node_type, obj->second_child),
	// parent
	POINTER_NULLABLE("client", client_type, obj->client),
	SERONLY(SERIALIZE_POINTER("focused", node_focused, obj))
)

json_t* json_serialize_desktop_name(desktop_t *obj)
{
	if (!obj)
		return json_null();
	return json_string(obj->name);
}

json_t* json_serialize_desktop_focused(desktop_t *obj)
{
	if (!obj)
		return NULL;
	return obj == mon->desk ? json_true() : json_false();
}

SERIALIZATION(padding,
	INTEGER("top", int, obj->top),
	INTEGER("right", int, obj->right),
	INTEGER("bottom", int, obj->bottom),
	INTEGER("left", int, obj->left)
)

SERIALIZATION(desktop,
	STRING("name", obj->name),
	INTEGER("id", uint32_t, obj->id),
	ENUM("layout", layout_type, obj->layout),
	ENUM("userLayout", layout_type, obj->user_layout),
	POINTER_NULLABLE("root", node_type, obj->root),
	POINTER_NULLABLE("focusId", node_id, obj->focus),
	POINTER_NULLABLE("prevName", desktop_name, obj->prev),
	POINTER_NULLABLE("nextName", desktop_name, obj->next),
	OBJECT("padding", padding, padding_t, padding_type, obj->padding),
	INTEGER("windowGap", int, obj->window_gap),
	INTEGER("borderWidth", unsigned int, obj->border_width),
	SERONLY(SERIALIZE_POINTER("focused", desktop_focused, obj))
)

json_t* json_serialize_monitor_name(monitor_t *obj)
{
	if (!obj)
		return json_null();
	return json_string(obj->name);
}

json_t* json_serialize_monitor_id(monitor_t *obj)
{
	if (!obj)
		return json_null();
	return json_integer(obj->id);
}

json_t* json_serialize_monitor_desktops(monitor_t *obj)
{
	SERIALIZE_INIT_ARRAY(monitor_desktops)
	for (desktop_t *d = obj->desk_head; d; d = d->next) {
		json_array_append_new(json, json_string(d->name));
	}
	return json;
}

json_t* json_serialize_monitor_focused(monitor_t *obj)
{
	if (!obj)
		return NULL;
	return obj == mon ? json_true() : json_false();
}

SERIALIZATION(monitor,
	STRING("name", obj->name),
	INTEGER("id", uint32_t, obj->id),
	INTEGER("randrId", xcb_randr_output_t, obj->randr_id),
	INTEGER("rootWindow", xcb_window_t, obj->root),
	BOOLEAN("wired", obj->wired),
	OBJECT("padding", padding, padding_t, padding_type, obj->padding),
	INTEGER("stickyCount", unsigned int, obj->sticky_count),
	INTEGER("windowGap", int, obj->window_gap),
	INTEGER("borderWidth", unsigned int, obj->border_width),
	OBJECT("rectangle", rectangle, xcb_rectangle_t, xcb_rectangle_type, obj->rectangle),
	POINTER("deskName", desktop_name, obj->desk),
	POINTER("deskHeadName", desktop_name, obj->desk_head),
	POINTER("deskTailName", desktop_name, obj->desk_tail),
	POINTER_NULLABLE("prevName", monitor_name, obj->prev),
	SERONLY(SERIALIZE_POINTER_NULLABLE("prevId", monitor_id, obj->prev)),
	POINTER_NULLABLE("nextName", monitor_name, obj->next),
	SERONLY(SERIALIZE_POINTER_NULLABLE("nextId", monitor_id, obj->next)),
	SERONLY(SERIALIZE_POINTER("desktops", monitor_desktops, obj)),
	SERONLY(SERIALIZE_POINTER("focused", monitor_focused, obj))
)

SERIALIZATION(coordinates,
	POINTER_NULLABLE("monitorName", monitor_name, obj->monitor),
	POINTER_NULLABLE("desktopName", desktop_name, obj->desktop),
	POINTER_NULLABLE("nodeId", node_id, obj->node)
)

#undef SERIALIZE_BEGIN
#undef SERIALIZE_END
#undef SERIALIZE_SERONLY

#undef SERIALIZE_CAT
#undef SERIALIZATION

//
// Misc
//

#define SERIALIZE_IS_OBJECT(TYPE) \
	if (!json_is_object(json)) { \
		json_decref(json); \
		warn("JSON serialize failed: "#TYPE": !json_is_object\n"); \
		return NULL; \
	}
#define SERIALIZE_IS_OBJECT_NULLABLE(TYPE) \
	if (!json_is_object(json)) { \
		json_decref(json); \
		json = json_object(); \
		if (!json) { \
			warn("JSON serialize failed: "#TYPE": !json\n"); \
			return NULL; \
		} \
	}
#define SERIALIZE_POINTER_EQUALS(KEY1, TYPE, OBJ1, KEY2, OBJ2) \
	if (OBJ1 == OBJ2) { \
		json_object_set(json, KEY1, json_object_get(json, KEY2)); \
	} else { \
		SERIALIZE_POINTER(KEY1, TYPE, OBJ1) \
	}
#define SERIALIZE_STRING_NESTED(KEY1, OBJ, KEY2) \
	if (json_object_set_new(json_object_get(json, KEY2), KEY1, json_string(OBJ)) == -1) { \
		json_decref(json); \
		warn("JSON serialize failed: string: "KEY1": json_object_set_new\n"); \
		return NULL; \
	}

json_t* json_serialize_status_node(monitor_t *m, desktop_t *d, node_t *n)
{
	SERIALIZE_INIT_OBJECT(status_node)
	json_t *set;
	SERIALIZE_POINTER("monitor", monitor_type, m)
	SERIALIZE_POINTER("desktop", desktop_type, d)
	SERIALIZE_POINTER("node", node_type, n)
	return json;
}

json_t* json_serialize_status_node_nullable(monitor_t *m, desktop_t *d, node_t *n)
{
	SERIALIZE_INIT_OBJECT(status_node_nullable)
	json_t *set;
	SERIALIZE_POINTER("monitor", monitor_type, m)
	SERIALIZE_POINTER("desktop", desktop_type, d)
	SERIALIZE_POINTER_NULLABLE("node", node_type, n)
	return json;
}

json_t* json_serialize_status_node_swap(monitor_t *m1, desktop_t *d1, node_t *n1, monitor_t *m2, desktop_t *d2, node_t *n2)
{
	SERIALIZE_INIT_OBJECT(status_node_swap)
	json_t *set;
	SERIALIZE_POINTER("monitor1", monitor_type, m1)
	SERIALIZE_POINTER("desktop1", desktop_type, d1)
	SERIALIZE_POINTER("node1", node_type, n2)
	SERIALIZE_POINTER_EQUALS("monitor2", monitor_type, m2, "monitor1", m1)
	SERIALIZE_POINTER_EQUALS("desktop2", desktop_type, d2, "desktop1", d1)
	SERIALIZE_POINTER("node2", node_type, n1)
	return json;
}

json_t* json_serialize_status_node_transfer(monitor_t *ms, desktop_t *ds, node_t *ns, monitor_t *md, desktop_t *dd)
{
	SERIALIZE_INIT_OBJECT(status_node_transfer)
	json_t *set;
	SERIALIZE_POINTER("monitor", monitor_type, md)
	SERIALIZE_POINTER("desktop", desktop_type, dd)
	SERIALIZE_POINTER("node", node_type, ns)
	SERIALIZE_POINTER_EQUALS("monitorLast", monitor_type, ms, "monitor", md)
	SERIALIZE_POINTER("desktopLast", desktop_type, ds)
	return json;
}

json_t* json_serialize_status_desktop(monitor_t *m, desktop_t *d)
{
	SERIALIZE_INIT_OBJECT(status_desktop)
	json_t *set;
	SERIALIZE_POINTER("monitor", monitor_type, m)
	SERIALIZE_POINTER("desktop", desktop_type, d)
	return json;
}

json_t* json_serialize_status_desktop_transfer(monitor_t *ms, monitor_t *md, desktop_t *d)
{
	SERIALIZE_INIT_OBJECT(status_desktop_transfer)
	json_t *set;
	SERIALIZE_POINTER("monitor", monitor_type, md)
	SERIALIZE_POINTER("desktop", desktop_type, d)
	SERIALIZE_POINTER("monitorLast", monitor_type, ms)
	return json;
}

json_t* json_serialize_status_desktop_rename(monitor_t *m, desktop_t *d, const char *name_last)
{
	SERIALIZE_INIT_OBJECT(status_desktop_rename)
	json_t *set;
	SERIALIZE_POINTER("monitor", monitor_type, m)
	SERIALIZE_POINTER("desktop", desktop_type, d)
	SERIALIZE_STRING_NESTED("nameLast", name_last, "desktop")
	return json;
}

json_t* json_serialize_status_desktop_swap(monitor_t *m1, desktop_t *d1, monitor_t *m2, desktop_t *d2)
{
	SERIALIZE_INIT_OBJECT(status_desktop_swap)
	json_t *set;
	SERIALIZE_POINTER("monitor1", monitor_type, m1)
	SERIALIZE_POINTER("desktop1", desktop_type, d2)
	SERIALIZE_POINTER_EQUALS("monitor2", monitor_type, m2, "monitor1", m1)
	SERIALIZE_POINTER("desktop2", desktop_type, d1)
	return json;
}

json_t* json_serialize_status_monitor_rename(monitor_t *m, const char *name_last)
{
	SERIALIZE_INIT_OBJECT(status_monitor_rename)
	json_t *set;
	SERIALIZE_POINTER("monitor", monitor_type, m)
	SERIALIZE_STRING_NESTED("nameLast", name_last, "monitor")
	return json;
}

json_t* json_serialize_status_monitor_swap(monitor_t *m1, monitor_t *m2)
{
	SERIALIZE_INIT_OBJECT(status_monitor_swap)
	json_t *set;
	SERIALIZE_POINTER("monitor1", monitor_type, m2)
	SERIALIZE_POINTER("monitor2", monitor_type, m1)
	return json;
}

json_t* json_serialize_query_window(coordinates_t loc)
{
	json_t *json = json_serialize_node_type(loc.node);
	SERIALIZE_IS_OBJECT(query_window)
	json_t *set;
	SERIALIZE_POINTER("monitorName", monitor_name, loc.monitor)
	SERIALIZE_POINTER("desktopName", desktop_name, loc.desktop)
	return json;
}

json_t* json_serialize_query_windows(coordinates_t loc)
{
	SERIALIZE_INIT_OBJECT(query_windows)
	char id[11];
	json_t *jmonitor, *jdesktop, *jnode;
	for (monitor_t *m = mon_head; m; m = m->next) {
		if (loc.monitor && m != loc.monitor)
			continue;
		jmonitor = json_serialize_monitor_name(m);
		for (desktop_t *d = m->desk_head; d; d = d->next) {
			if (loc.desktop && d != loc.desktop)
				continue;
			jdesktop = json_serialize_desktop_name(d);
			for (node_t *n = first_extrema(d->root); n; n = next_leaf(n, d->root)) {
				if (loc.node && n != loc.node)
					continue;
				jnode = json_serialize_node_type(n);
				sprintf(id, "%d", n->id);
				if (json_object_set(jnode, "monitorName", jmonitor) == -1
						|| json_object_set(jnode, "desktopName", jdesktop) == -1
						|| json_object_set(json, id, jnode) == -1) {
					json_decref(jnode);
					json_decref(jdesktop);
					json_decref(jmonitor);
					json_decref(json);
					return NULL;
				}
				json_decref(jnode);
			}
			json_decref(jdesktop);
		}
		json_decref(jmonitor);
	}
	return json;
}

json_t* json_serialize_query_desktop(coordinates_t loc)
{
	json_t *json = json_serialize_desktop_type(loc.desktop);
	SERIALIZE_IS_OBJECT(query_desktop)
	json_t *set;
	SERIALIZE_POINTER("monitorName", monitor_name, loc.monitor)
	return json;
}

json_t* json_serialize_query_desktops(coordinates_t loc)
{
	SERIALIZE_INIT_OBJECT(query_desktops)
	json_t *jmonitor, *jdesktop;
	for (monitor_t *m = mon_head; m; m = m->next) {
		if (loc.monitor && m != loc.monitor)
			continue;
		jmonitor = json_serialize_monitor_name(m);
		for (desktop_t *d = m->desk_head; d; d = d->next) {
			if (loc.desktop && d != loc.desktop)
				continue;
			jdesktop = json_serialize_desktop_type(d);
			if (json_object_set(jdesktop, "monitorName", jmonitor) == -1
					|| json_object_set(json, d->name, jdesktop) == -1) {
				json_decref(jdesktop);
				json_decref(jmonitor);
				json_decref(json);
				return NULL;
			}
			json_decref(jdesktop);
		}
		json_decref(jmonitor);
	}
	return json;
}

json_t* json_serialize_query_monitor(coordinates_t loc)
{
	json_t *json = json_serialize_monitor_type(loc.monitor);
	SERIALIZE_IS_OBJECT(query_monitor)
	return json;
}

json_t* json_serialize_query_monitors(coordinates_t loc)
{
	SERIALIZE_INIT_OBJECT(query_monitors)
	for (monitor_t *m = mon_head; m; m = m->next) {
		if (loc.monitor && m != loc.monitor)
			continue;
		if (json_object_set_new(json, m->name, json_serialize_monitor_type(m)) == -1) {
			json_decref(json);
			return NULL;
		}
	}
	return json;
}

json_t* json_serialize_query_tree(coordinates_t loc)
{
	SERIALIZE_INIT_OBJECT(query_tree)
	json_t *jmonitor, *jdesktops;
	for (monitor_t *m = mon_head; m; m = m->next) {
		if (loc.monitor && m != loc.monitor)
			continue;
		jmonitor = json_serialize_monitor_type(m);
		jdesktops = json_array();
		for (desktop_t *d = m->desk_head; d; d = d->next) {
			if (loc.desktop && d != loc.desktop)
				continue;
			if (json_array_append_new(jdesktops, json_serialize_desktop_type(d)) == -1) {
				json_decref(jdesktops);
				json_decref(jmonitor);
				json_decref(json);
				return NULL;
			}
		}
		if (json_object_set_new(jmonitor, "desktops", jdesktops) == -1) {
			json_decref(jmonitor);
			json_decref(json);
			return NULL;
		}
		if (json_object_set_new(json, m->name, jmonitor) == -1) {
			json_decref(json);
			return NULL;
		}
	}
	return json;
}

json_t* json_serialize_query_history(coordinates_t loc)
{
	SERIALIZE_INIT_ARRAY(query_history)
	for (history_t *h = history_head; h; h = h->next) {
		if ((loc.monitor && h->loc.monitor != loc.monitor)
				|| (loc.desktop && h->loc.desktop != loc.desktop))
			continue;
		if (json_array_append_new(json, json_serialize_coordinates_type(&h->loc)) == -1) {
			json_decref(json);
			return NULL;
		}
	}
	return json;
}

json_t* json_serialize_query_stack()
{
	SERIALIZE_INIT_ARRAY(query_stack)
	for (stacking_list_t *s = stack_head; s; s = s->next) {
		if (json_array_append_new(json, json_serialize_node_id(s->node)) == -1) {
			json_decref(json);
			return NULL;
		}
	}
	return json;
}

#undef SERIALIZE_IS_OBJECT
#undef SERIALIZE_IS_OBJECT_NULLABLE

#undef SERIALIZE_INTEGER
#undef SERIALIZE_REAL
#undef SERIALIZE_STRING
#undef SERIALIZE_BOOLEAN
#undef SERIALIZE_OBJECT
