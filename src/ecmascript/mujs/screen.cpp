/* The MuJS screen object implementation. */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elinks.h"

#include "bfu/dialog.h"
#include "cache/cache.h"
#include "cookies/cookies.h"
#include "dialogs/menu.h"
#include "dialogs/status.h"
#include "document/html/frames.h"
#include "document/document.h"
#include "document/forms.h"
#include "document/view.h"
#include "ecmascript/ecmascript.h"
#include "ecmascript/mujs.h"
#include "ecmascript/mujs/screen.h"
#include "ecmascript/mujs/window.h"
#include "intl/libintl.h"
#include "main/select.h"
#include "osdep/newwin.h"
#include "osdep/sysname.h"
#include "protocol/http/http.h"
#include "protocol/uri.h"
#include "session/history.h"
#include "session/location.h"
#include "session/session.h"
#include "session/task.h"
#include "terminal/tab.h"
#include "terminal/terminal.h"
#include "util/conv.h"
#include "util/memory.h"
#include "util/string.h"
#include "viewer/text/draw.h"
#include "viewer/text/form.h"
#include "viewer/text/link.h"
#include "viewer/text/vs.h"


static void
mjs_screen_get_property_availHeight(js_State *J)
{
#ifdef ECMASCRIPT_DEBUG
	fprintf(stderr, "%s:%s\n", __FILE__, __FUNCTION__);
#endif
	struct ecmascript_interpreter *interpreter = (struct ecmascript_interpreter *)js_touserdata(J, 0, "screen");

	assert(interpreter);
	struct view_state *vs = interpreter->vs;
	struct document_view *doc_view = vs->doc_view;

	if (!doc_view) {
		js_pushundefined(J);
		return;
	}
	js_pushnumber(J, doc_view->box.height * 16);
}

static void
mjs_screen_get_property_availWidth(js_State *J)
{
#ifdef ECMASCRIPT_DEBUG
	fprintf(stderr, "%s:%s\n", __FILE__, __FUNCTION__);
#endif
	struct ecmascript_interpreter *interpreter = (struct ecmascript_interpreter *)js_touserdata(J, 0, "screen");

	assert(interpreter);
	struct view_state *vs = interpreter->vs;
	struct document_view *doc_view = vs->doc_view;

	if (!doc_view) {
		js_pushundefined(J);
		return;
	}
	js_pushnumber(J, doc_view->box.width * 8);
}

static void
mjs_screen_get_property_height(js_State *J)
{
#ifdef ECMASCRIPT_DEBUG
	fprintf(stderr, "%s:%s\n", __FILE__, __FUNCTION__);
#endif
	struct ecmascript_interpreter *interpreter = (struct ecmascript_interpreter *)js_touserdata(J, 0, "screen");

	assert(interpreter);
	struct view_state *vs = interpreter->vs;
	struct document_view *doc_view = vs->doc_view;

	if (!doc_view) {
		js_pushundefined(J);
		return;
	}
	struct session *ses = doc_view->session;

	if (!ses) {
#ifdef ECMASCRIPT_DEBUG
	fprintf(stderr, "%s:%s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
		js_pushundefined(J);
		return;
	}
	js_pushnumber(J, ses->tab->term->height * 16);
}

static void
mjs_screen_get_property_width(js_State *J)
{
#ifdef ECMASCRIPT_DEBUG
	fprintf(stderr, "%s:%s\n", __FILE__, __FUNCTION__);
#endif
	struct ecmascript_interpreter *interpreter = (struct ecmascript_interpreter *)js_touserdata(J, 0, "screen");

	assert(interpreter);
	struct view_state *vs = interpreter->vs;
	struct document_view *doc_view = vs->doc_view;

	if (!doc_view) {
		js_pushundefined(J);
		return;
	}
	struct session *ses = doc_view->session;

	if (!ses) {
#ifdef ECMASCRIPT_DEBUG
	fprintf(stderr, "%s:%s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
		js_pushundefined(J);
		return;
	}
	js_pushnumber(J, ses->tab->term->width * 8);
}

static void
mjs_screen_toString(js_State *J)
{
	js_pushstring(J, "[screen object]");
}

int
mjs_screen_init(struct ecmascript_interpreter *interpreter, js_State *J)
{
	js_getglobal(J, "Object");
	js_getproperty(J, -1, "prototype");	// window.prototype.[[Prototype]] = Object.prototype
	js_newuserdata(J, "screen", interpreter, NULL);

	js_newcfunction(J, mjs_screen_toString, "screen.prototype.toString", 0);
	js_defproperty(J, -2, "toString", JS_DONTENUM);

	js_newcfunction(J, mjs_screen_get_property_availHeight, "screen.prototype.availHeight", 0);
	js_pushnull(J);
	js_defaccessor(J, -3, "availHeight", JS_READONLY | JS_DONTENUM | JS_DONTCONF);

	js_newcfunction(J, mjs_screen_get_property_availWidth, "screen.prototype.availWidth", 0);
	js_pushnull(J);
	js_defaccessor(J, -3, "availWidth", JS_READONLY | JS_DONTENUM | JS_DONTCONF);

	js_newcfunction(J, mjs_screen_get_property_height, "screen.prototype.height", 0);
	js_pushnull(J);
	js_defaccessor(J, -3, "height", JS_READONLY | JS_DONTENUM | JS_DONTCONF);

	js_newcfunction(J, mjs_screen_get_property_width, "screen.prototype.width", 0);
	js_pushnull(J);
	js_defaccessor(J, -3, "width", JS_READONLY | JS_DONTENUM | JS_DONTCONF);

	js_defglobal(J, "screen", JS_DONTENUM);

	return 0;
}