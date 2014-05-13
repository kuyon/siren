/* THIS FILE IS PART OF *SIREN* SOURCE CODES.
 * WHEN YOU WANT THE LICENSE OF THIS FILE, YOU CAN GET IT
 * IN README OR LICENSE DOCUMENTATION OF THIS PROJECT.
 * ---------------------------------------------------------
 * AUTHOR: dyama <dyama@member.fsf.org>
 */

#include "Stdafx.h"
#include "OCCViewer.h"

#define NOREGIST 1

mrb_value my_class_print_name(mrb_state* mrb, mrb_value self)
{
	return mrb_fixnum_value(226);
}

/**
 * \brief 
 */
bool OCCViewer::mruby_init()
{
	// init mruby interpretor
	myMirb = new Mirb();

	// コマンドの登録

    // 実装予定コマンド
    // transparency obj val ... 不透明度設定。valを省略すると現在設定されている値を返す。
    // arc                  ... 円弧曲線を作成する
    // circle               ... 円曲線を作成する。内部的にはarcと同じ？
    // 視点制御系コマンド
    // ビュー管理系コマンド

	// General commands
	regcmd("help",      &help,      1,0, "Display help of command.",        "help(cmd) -> String[][name, dest, usage]");
	regcmd("version",   &version,   0,0, "",                                "version() -> String");
	regcmd("debug",     &debug,     0,0, "",                                "");

	// Infomation/Status commands
	regcmd("bndbox",    &bndbox,    1,0, "Get area of object exist.",       "bndbox(ObjID) -> Ary[min[X,Y,Z], max[X,Y,Z]]");
	regcmd("selected",  &selected,  0,0, "Get name of selected objects.",   "selected() -> Ary[ObjID, ...]");
	regcmd("type",      &type,      1,0, "Get type of object.",             "type(ObjID) -> Type");
	regcmd("exist",     &exist,     1,0, "Check exist.",                    "exist(ObjID) -> Boolean");
	regcmd("location",  &location,  1,1, "Get/Set location of shape in WCS.","location(ObjID, [X,Y,Z]) -> Ary[X,Y,Z]");
    regcmd("cparam",    &cparam,    2,0, "Get curve parameter at a point.", "cparam(obj, [X, Y, Z]) -> [int index, float param]");
    regcmd("cpoint",    &cpoint,    2,0, "Get point from parameter.",       "cpoint(obj, cparam) -> [X, Y, Z]");
    regcmd("ccurvature",&ccurvature,2,0, "Get curvature vector.",           "ccurvature(obj, cparam) -> [X, Y, Z]");
    regcmd("ctangent",  &ctangent  ,2,0, "Get tangent vector.",             "ctangent(obj, cparam) -> [X, Y, Z]");

	// Edit object commands
	regcmd("copy",      &copy,      1,0, "Copy specified object.",          "copy(ObjID) -> ObjID");
	regcmd("erase",     &erase,     1,0, "Erase specified object.",         "erase(ObjID) -> nil");
	regcmd("close",		&close,		1,0, "Close a wire.",                   "close( ObjID ) -> ObjID");

	// Group commands
	regcmd("compound",  &compound,  1,0, "Make compound model by objects.", "compound([ObjID, ObjID, ...]) -> ObjID");
	regcmd("sew",       &sew,       1,1, "Make shell model by objects.",    "sew([ObjID, ObjID, ...]) -> ObjID");
	regcmd("explode",   &explode,   2,0, "Explode object to children.",     "explode(type, ObjID) -> Ary");

	// Transform commands
	regcmd("translate", &translate, 2,0, "Translate specified object.",     "translate(obj, vector[X, Y, Z]) -> nil");
	regcmd("rotate",    &rotate,    4,0, "Rotate specified object.",        "rotate(obj, center[X, Y, Z], normal[X, Y, Z], angle) -> nil"); 
	regcmd("scale",     &scale,     3,0, "Scale specified object.",         "scale(obj, scale, center[X, Y, Z] = [0, 0, 0]) -> nil");
	regcmd("mirror",    &mirror,    3,0, "Mirror copy specified object.",   "mirror(obj, center[X, Y, Z], normal[X, Y, Z]) -> nil");

	// Visualization commands
	regcmd("display",   &display,   1,0, "Dislay object.",                  "display(obj) -> nil");
	regcmd("hide",      &hide,      1,0, "Hide object.",                    "hide(obj) -> nil");
	regcmd("fit",       &fit,       0,0, "Fit view to objects",             "fit() -> nil");
	regcmd("update",    &update,    0,0, "Update current viewer.",          "update() -> nil");
	regcmd("color",     &color,     4,0, "Set color of object.",            "color(obj, R, G, B) -> nil");
	regcmd("bgcolor",   &bgcolor,   3,3, "Set color of background.",        "bgcolor(topR, topG, topB, btmR, btmG, btmB) -> nil");

	// Boolean operation commands
	regcmd("common",    &common,    2,1, "Common boolean operation.",       "common(obj1, obj2) -> String");
	regcmd("cut",       &cut,       2,1, "Cut boolean operation.",          "cut(obj1, obj2) -> String");
	regcmd("fuse",      &fuse,      2,1, "Fuse boolean operation.",         "fuse(obj1, obj2) -> String");
	regcmd("volume",    &volume,    1,0, "Get volume of object.",           "volume(obj) -> float");
	regcmd("cog",       &cog,       1,0, "Get center position of gravity",  "cog(obj) -> float[X, Y, Z]");
	regcmd("intersect", &intersect, 2,0, "Get intersection line.",          "intersect(obj1, obj2) -> ObjID");
	regcmd("intcs",     &intcs,     2,1, "Intersection Curve x Surface",    "intcs(obj_curve, obj_surf, with_normal) -> [float[X, Y, Z], ...]");
	regcmd("intfe",     &intfe,     2,1, "Intersection Face x Edge",        "intfe(face, edge) -> [float[X, Y, Z], ...]");


	regcmd("split",     &split,     2,0, "",                                "");

	// Make object commands
	regcmd("vertex",    &vertex,    3,0, "Make a vertex.",                  "vertex(X, Y, Z) -> String");
	regcmd("line",      &line,      2,0, "Make a line.",                    "line(sp[X, Y, Z], tp[X, Y, Z]) -> String");
	regcmd("polyline",  &polyline,  1,0, "Make a polyline.",                "polyline(pts[[X, Y, Z], ...]) -> String");
	regcmd("curve",     &curve,     1,1, "Make a curve.",                   "curve(pts[[X, Y, Z], ...], vecs[[X, Y, Z], ...]) -> String");
	regcmd("box",       &box,       1,1, "Make a box.",                     "box(size[X, Y, Z], pos[X, Y, Z] = [0, 0, 0]) -> String");
	regcmd("sphere",    &sphere,    1,1, "Make a sphere.",                  "sphere(R, pos[X, Y, Z] = [0, 0, 0]) -> String");
	regcmd("cylinder",  &cylinder,  5,0, "Make a cylinder.",                "cylinder(pos[X, Y, Z], normal[X, Y, Z], R, height, angle) -> String");
	regcmd("cone",      &cone,      6,0, "Make a cone.",                    "cone(pos[X, Y, Z], normal[X, Y, Z], R1, R2, height, angle) -> String");
	regcmd("torus",     &torus,     7,0, "Make a torus.",                   "torus(pos[X, Y, Z], normal[X, Y, Z], R1, R2, angle) -> String");
	regcmd("plane",     &plane,     6,0, "Make a plane.",                   "plane(pos[X, Y, Z], normal[X, Y, Z], umin, umax, vmin, vmax) -> String");
	regcmd("wire",		&wire,		1,0, "Make a wire.",					"wire( Ary[edge or wire or comp obj] ) -> String");
	regcmd("sweep",     &sweep,		2,0, "Make a sweep model.",             "sweep( profile obj, vec[X, Y, Z] ) -> String | sweep( profile obj, path obj ) -> String");
	regcmd("loft",      &loft,      1,0, "Make a loft surface.",            "loft(Array[obj]) -> ObjID");
    regcmd("bzsurf",    &bzsurf,    1,1, "Make a bezier surface.",          "bzsurf([[pu, ...], [pv, ...]], [[wu, ...], [wv, ...]]) -> ObjID");
    regcmd("offset",    &offset,    1,1, "Make an offset surface.",         "offset(surface, offset_value) -> ObjID");

	// Convertion commands
	regcmd("wire2pts",  &wire2pts,  1,1, "Convert wire to points.",         "wire2pts(ObjID) -> Ary[[X, Y, Z], ...]");
	regcmd("wire2plane",&wire2plane,1,0, "Make a plane.",                   "wire2plane( Close wire ObjID ) -> String");
	regcmd("shell2solid",&shell2solid,1,0, "Make a solid by shell.",        "shell2solid(ObjID) -> ObjID");
    regcmd("triangle",  &triangle,  1,2, "Make triangle mesh from face.",   "triangle(ObjID, Deflection, Angle) -> ObjID");

	regcmd("obj2brep",  &obj2brep,  1,0, "Object to OpenCASCADE BRep.",     "obj2brep(obj) -> String");
	regcmd("brep2obj",  &brep2obj,  1,0, "Object from OpenCASCADE BRep.",   "obj2brep(string) -> obj");

	// I/O commands
	regcmd("bsave",     &savebrep,  2,0, "Save object to a file.",          "bsave(path, obj) -> nil");
	regcmd("bload",     &loadbrep,  1,0, "Load object from a file.",        "bload(path) -> ID");
	regcmd("isave",     &saveiges,  2,0, "Save object to an IGES.",         "isave(path, obj) -> nil");
	regcmd("iload",     &loadiges,  1,0, "Load object from an IGES.",       "iload(path) -> Ary");
	regcmd("stlload",   &loadstl,   1,0, "Load object from an STL file.",   "stlload(path)");

    //
	regcmd("selmode",   &selmode,   1,0, "Change selection mode.",          "");

	// デフォルトのグローバル変数定義
	myMirb->user_exec(
		"$DRAW=1;"
		"$tri10=[10,10,10];"
		);

    // バイトコードのロード
    loadbc(myMirb->mrb);

	return true;
}

/**
 * \brief regist command with help string
 */
void OCCViewer::regcmd(const char* name, mrb_func_t func, int arg_req, int arg_opt, const char* desc, const char* usage)
{
	// mrb_define_method(ステータス, 登録先, コマンド名, 関数, コマンドが受け取る引数の情報);
	mrb_define_method(myMirb->mrb, myMirb->mrb->kernel_module, name, func, MRB_ARGS_REQ(arg_req) | MRB_ARGS_OPT(arg_opt));

	structHelp* myHelp = new structHelp;
	myHelp->desc = new std::string(desc);
	myHelp->usage = new std::string(usage);
	Help[std::string(name)] = myHelp;

	return;
}

/**
 * \brief 
 */
bool OCCViewer::mruby_cleenup()
{
	if (myMirb)
		free(myMirb);
	return true;
}

/**
 * \brief 
 */
int OCCViewer::mruby_exec(char* command)
{
	std::string errmsg;
	return myMirb->user_exec(command, errmsg);
}

/**
 * \brief 
 */
int OCCViewer::mruby_exec(char* command, std::string& errmsg)
{
	if (!myMirb)
		return -1;

	cur = this;
	if (cur == NULL)
		return -1;

	cur->aiscxt = aiscxt;
	if (cur->aiscxt.IsNull())
		return -1;
	cur->view = view;
	if (cur->view.IsNull())
		return -1;

	int res = myMirb->user_exec(command, errmsg);

	return res;
}

/**
 * \brief 
 */
int set(const TopoDS_Shape& shape)
{
	return ::set(shape, 1);
}

/**
 * \brief 
 */
int set(const TopoDS_Shape& shape, int draw)
{
	if (cur->aiscxt.IsNull()) {
		throw "No AIS Interactive Context.";
	}
	Handle(AIS_Shape) hashape = new AIS_Shape(shape);

	Handle(Graphic3d_ShaderProgram) myShader;
	myShader = new Graphic3d_ShaderProgram(Graphic3d_ShaderProgram::ShaderName_Phong);
	hashape->Attributes()->ShadingAspect()->Aspect()->SetShaderProgram(myShader);

	cur->aiscxt->SetMaterial(hashape, /*Graphic3d_NameOfMaterial::*/Graphic3d_NOM_DEFAULT);
	cur->aiscxt->SetColor(hashape, Quantity_NOC_WHITE, Standard_False);
	cur->aiscxt->SetDisplayMode(hashape, 1/* 0:wireframe, 1:shading */, Standard_False);

	cur->aiscxt->Display(hashape);
	cur->aiscxt->SetSelected(hashape, Standard_False);
	cur->aiscxt->UpdateCurrentViewer();

	return shape.HashCode(INT_MAX);
}

/**
 * \brief 
 */
Handle(AIS_Shape) get(int hashcode)
{
#if 0
	// これだと選択されたものから探すので
    cur->aiscxt->InitCurrent();
    for (; cur->aiscxt->MoreCurrent(); cur->aiscxt->NextCurrent()) {
		Handle(AIS_InteractiveObject) aisobj = cur->aiscxt->Current();
		Handle(AIS_Shape) hashape = Handle(AIS_Shape)::DownCast(aisobj);
        if (hashcode == hashape->Shape().HashCode(INT_MAX))
            return hashape;
    }
    return NULL;
#else
	// こっちが正解
	AIS_ListOfInteractive ar;
	cur->aiscxt->ObjectsInside(ar);
	AIS_ListIteratorOfListOfInteractive it(ar);
	for (; it.More(); it.Next()) {
		Handle(AIS_InteractiveObject) aisobj = it.Value();
		Handle(AIS_Shape) hashape = Handle(AIS_Shape)::DownCast(aisobj);
        if (hashcode == hashape->Shape().HashCode(INT_MAX))
            return hashape;
	}
	return NULL;
#endif
}

/**
 * \brief 
 */
void unset(int hashcode)
{
	Handle(AIS_Shape) myShape = ::get(hashcode);
	cur->aiscxt->Erase(myShape, Standard_True);
	return;
}

/**
 * \brief 
 */
bool has_object(int hashcode)
{
	Handle(AIS_Shape) hashape = get(hashcode);
	return !hashape.IsNull();
}

/**
 * \brief 
 */
void redisplay(const Handle(AIS_Shape)& hashape)
{
	cur->aiscxt->Redisplay(hashape, Standard_True);
}

// ------

/**
 * \brief 
 */
mrb_value update(mrb_state* mrb, mrb_value self)
{
	cur->aiscxt->UpdateCurrentViewer();
	return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value display(mrb_state* mrb, mrb_value self)
{
	if (cur->aiscxt.IsNull())
		return mrb_nil_value();

    mrb_int target;
	int argc = mrb_get_args(mrb, "i", &target);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified argument.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	cur->aiscxt->Display(hashape, Standard_False);
	return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value hide(mrb_state* mrb, mrb_value self)
{
	if (cur->aiscxt.IsNull())
		return mrb_nil_value();

    mrb_int target;
	int argc = mrb_get_args(mrb, "i", &target);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified argument.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	cur->aiscxt->Erase(hashape, Standard_False);
	return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value fit(mrb_state* mrb, mrb_value self)
{
	if (!cur->view.IsNull()) {
		cur->view->FitAll();
		cur->view->ZFitAll();
	}
	else {
		static const char m[] = "cur->view is NULL object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value color(mrb_state* mrb, mrb_value self)
{
    mrb_int target; 
	mrb_float r, g, b;
	int argc = mrb_get_args(mrb, "ifff", &target, &r, &g, &b);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	Quantity_Color col =  Quantity_Color((Standard_Real)r/255., (Standard_Real)g/255., (Standard_Real)b/255., Quantity_TOC_RGB);

	//cur->aiscxt->SetColor(hashape, col, Standard_True);
	//cur->aiscxt->SetMaterial(hashape, /*Graphic3d_NameOfMaterial::*/Graphic3d_NOM_DEFAULT);
	//cur->aiscxt->SetColor(hashape, Quantity_NOC_RED, Standard_True);
	Quantity_NameOfColor colname = col.Name();
	cur->aiscxt->SetColor(hashape, colname, Standard_True);

	return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value bgcolor(mrb_state* mrb, mrb_value self)
{
	mrb_float tr, tg, tb, br, bg, bb;
	int argc = mrb_get_args(mrb, "fff|fff", &tr, &tg, &tb, &br, &bg, &bb);

	if (argc == 6) {
	    Quantity_Color color_top(tr/255, tg/255, tb/255, Quantity_TOC_RGB);
	    Quantity_Color color_btm(br/255, bg/255, bb/255, Quantity_TOC_RGB);
	    cur->view->SetBgGradientColors(color_top, color_btm, Aspect_GFM_VER, Standard_True);
	}
	else if (argc == 3) {
	    Quantity_Color color(tr/255, tg/255, tb/255, Quantity_TOC_RGB);
		cur->view->SetBackgroundColor(color);
		cur->view->Redraw();
	}
	else {
		static const char m[] = "Incorrect number of arguments.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_nil_value();
}

// ------

/**
 * \brief help for commands
 */
mrb_value help(mrb_state* mrb, mrb_value self)
{
	mrb_value name;
	int argc = mrb_get_args(mrb, "S", &name);

#ifdef _REGEX_
	// prefixmatch
	std::string ptn = std::string("^") + std::string(RSTRING_PTR(name)) + std::string(".*");
	std::tr1::regex re(ptn);

	mrb_value ar = mrb_ary_new(mrb);

	std::map<std::string, structHelp*>::iterator it = Help.begin();
	for (; it != Help.end(); it++) {
		if (std::tr1::regex_match(it->first, re)) {
			mrb_value tar[3];
			tar[0] = mrb_str_new(mrb, it->first.c_str(), strlen(it->first.c_str()));
			tar[1] = mrb_str_new(mrb, it->second->desc->c_str(), strlen(it->second->desc->c_str()));
			tar[2] = mrb_str_new(mrb, it->second->usage->c_str(), strlen(it->second->usage->c_str()));
			mrb_ary_push(mrb, ar, mrb_ary_new_from_values(mrb, 3, tar));
		}
	}
#else
	mrb_value ar = mrb_ary_new(mrb);

	std::map<std::string, structHelp*>::iterator it = Help.begin();

	char* target = RSTRING_PTR(name);
	for (; it != Help.end(); it++) {
		// 前方一致検索
		const char* current = it->first.c_str();
		if (strlen(target) > strlen(current))
			continue;
		bool is_match = true;
		for (int i = 0; i<(signed int)strlen(target); i++) {
			if (current[i] != target[i]) {
				is_match = false;
				break;
			}
		}
		// 一致していた場合
		if (is_match) {
			mrb_value tar[3];
			tar[0] = mrb_str_new(mrb, it->first.c_str(), strlen(it->first.c_str()));
			tar[1] = mrb_str_new(mrb, it->second->desc->c_str(), strlen(it->second->desc->c_str()));
			tar[2] = mrb_str_new(mrb, it->second->usage->c_str(), strlen(it->second->usage->c_str()));
			mrb_ary_push(mrb, ar, mrb_ary_new_from_values(mrb, 3, tar));
		}
	}

#endif
	if ((int)mrb_ary_len(mrb, ar))
		return ar;
	else
		return mrb_nil_value();
}

// ------


/**
 * \brief Get selected object
 */
mrb_value selected(mrb_state* mrb, mrb_value self)
{
	mrb_value ar = mrb_ary_new(mrb);
	char* aname = NULL;

	cur->aiscxt->InitCurrent();
	for (; cur->aiscxt->MoreCurrent(); cur->aiscxt->NextCurrent()) {
		Handle(AIS_InteractiveObject) anIO = cur->aiscxt->Current();
		Handle(AIS_Shape) hashape = Handle(AIS_Shape)::DownCast(anIO);
		TopoDS_Shape shape = hashape->Shape();
		mrb_ary_push(mrb, ar, mrb_fixnum_value(shape.HashCode(INT_MAX)));
	}

	if ((int)mrb_ary_len(mrb, ar))
		return ar;
	else
		return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value selmode(mrb_state* mrb, mrb_value self)
{
	mrb_int type;
	int argc = mrb_get_args(mrb, "i", &type);

    cur->aiscxt->CloseAllContexts();
    cur->aiscxt->OpenLocalContext();
    cur->aiscxt->ActivateStandardMode((TopAbs_ShapeEnum)type);

    if (type == TopAbs_SHAPE) {
        cur->aiscxt->CloseAllContexts();
    }

    return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value translate(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
	mrb_value vec;
	int argc = mrb_get_args(mrb, "iA", &target, &vec);

	gp_Pnt pvec = *ar2pnt(mrb, vec);
	gp_Vec myvec(pvec.X(), pvec.Y(), pvec.Z());

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	gp_Trsf T;
    T.SetTranslation(myvec);

    BRepBuilderAPI_Transform trf(T);
	trf.Perform(hashape->Shape());

	mrb_value result;
	if (trf.IsDone()){
#if NOREGIST
		hashape->Set(trf.Shape());
		redisplay(hashape);
		result = mrb_nil_value();
#else
        result = mrb_fixnum_value(::set(trf.Shape()));
        ::unset(target);
#endif
	}
	else {
		static const char m[] = "Failed to translate object.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return result;
}

/**
 * \brief 
 */
mrb_value rotate(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
	mrb_float a;
	mrb_value pos, norm;
	int argc = mrb_get_args(mrb, "iAAf", &target, &pos, &norm, &a);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	gp_Trsf T;
	Standard_Real ang = (double)a * (M_PI / 180.0);
	gp_Ax1 ax = *ar2ax1(mrb, pos, norm);

    T.SetRotation(ax, ang);

    BRepBuilderAPI_Transform trf(T);
	trf.Perform(hashape->Shape());

	mrb_value result;
	if (trf.IsDone()){
#if NOREGIST
		hashape->Set(trf.Shape());
		redisplay(hashape);
		result = mrb_nil_value();
#else
        result = mrb_fixnum_value(::set(trf.Shape()));
        ::unset(target);
#endif
	}
	else {
		static const char m[] = "Failed to rotate object.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return result;
}

/**
 * \brief 
 */
mrb_value scale(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
	mrb_value pos;
	mrb_float s;
	int argc = mrb_get_args(mrb, "ifA", &target, &s, &pos);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	gp_Pnt p;
	if (argc == 2) {
		p = gp_Pnt(0, 0, 0);
	}
	else {
		p = *ar2pnt(mrb, pos);
	}

	gp_Trsf T;
    T.SetScale(p, (Standard_Real)s);

    BRepBuilderAPI_Transform trf(T);
	trf.Perform(hashape->Shape());

	mrb_value result;
	if (trf.IsDone()){
#if NOREGIST
		hashape->Set(trf.Shape());
		redisplay(hashape);
		result = mrb_nil_value();
#else
        result = mrb_fixnum_value(::set(trf.Shape()));
        ::unset(target);
#endif
	}
	else {
		static const char m[] = "Failed to scale object.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return result;
}

/**
 * \brief 
 */
mrb_value mirror(mrb_state* mrb, mrb_value self)
{
	mrb_int target;
    mrb_value pos, norm;
	int argc = mrb_get_args(mrb, "iAA", &target, &pos, &norm);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	gp_Trsf T;
	gp_Ax2 ax = *ar2ax2(mrb, pos, norm);
    T.SetMirror(ax);

    BRepBuilderAPI_Transform trf(T);
	trf.Perform(hashape->Shape());

	mrb_value result;
	if (trf.IsDone()){
#if NOREGIST && 0
		hashape->Set(trf.Shape());
		redisplay(hashape);
		result = mrb_nil_value();
#else
        result = mrb_fixnum_value(::set(trf.Shape()));
        ::unset(target);
#endif
	}
	else {
		static const char m[] = "Failed to mirror object.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return result;
}

/**
 * \brief make vertex
 */
mrb_value vertex(mrb_state* mrb, mrb_value self)
{
	mrb_float x, y, z;
	int argc = mrb_get_args(mrb, "fff", &x, &y, &z);

	gp_Pnt p((Standard_Real)x, (Standard_Real)y, (Standard_Real)z);
	TopoDS_Vertex v = BRepBuilderAPI_MakeVertex(p);

	return mrb_fixnum_value(::set(v));
}

/**
 * \brief make line
 */
mrb_value line(mrb_state* mrb, mrb_value self)
{
    mrb_value sp, tp;
    int argc = mrb_get_args(mrb, "AA", &sp, &tp);

	gp_Pnt _sp = *ar2pnt(mrb, sp);
	gp_Pnt _tp = *ar2pnt(mrb, tp);

	BRepBuilderAPI_MakeEdge line(_sp, _tp);
    TopoDS_Shape shape = line.Shape();

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make polyline
 */
mrb_value polyline(mrb_state* mrb, mrb_value self)
{
  mrb_value pts;
  int argc = mrb_get_args(mrb, "A", &pts);
	int psize = mrb_ary_len(mrb, pts);

	BRepBuilderAPI_MakePolygon poly;
	TopoDS_Shape shape;
	try {
		for (int i=0; i<psize; i++) {
			mrb_value pt = mrb_ary_ref(mrb, pts, i);
			gp_Pnt pnt = *ar2pnt(mrb, pt);
			poly.Add(pnt);
		}
		poly.Build();
        if (poly.IsDone()) {
            shape = poly.Wire();
        }

	}
    catch (...) {
		static const char m[] = "Failed to make a polyline.";
		return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	if ( shape.IsNull() ) {
		static const char m[] = "Failed to make a polyline.";
		return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make arc
 */
mrb_value arc(mrb_state* mrb, mrb_value self)
{
    return mrb_exc_new(mrb, E_NOTIMP_ERROR, NULL, 0);
}

/**
 * \brief make curve
 */
mrb_value curve(mrb_state* mrb, mrb_value self)
{
	mrb_value pts, vecs;
	int argc = mrb_get_args(mrb, "A|A", &pts, &vecs);

	int psize = mrb_ary_len(mrb, pts);
    Handle(TColgp_HArray1OfPnt) pary = new TColgp_HArray1OfPnt(1, psize);
	for (int i=0; i<psize; i++) {
		mrb_value pt = mrb_ary_ref(mrb, pts, i);
		gp_Pnt pnt = *ar2pnt(mrb, pt);
		pary->SetValue(i+1, pnt);
	}
    GeomAPI_Interpolate intp(pary, Standard_False, 1.0e-7);

	if (argc == 2) {
		TColgp_Array1OfVec vec(1, psize);
	    Handle(TColStd_HArray1OfBoolean) use = new TColStd_HArray1OfBoolean(1, psize);

		for (int i=0; i<psize; i++) {
			mrb_value avec = mrb_ary_ref(mrb, vecs, i);
			if (mrb_nil_p(avec)) {
				//vec.SetValue(i+1, gp_Dir(1, 0, 0)); // dummy
				use->SetValue(i+1, Standard_False);	
			}
			else {
				gp_Dir dir = *ar2dir(mrb, avec);
				vec.SetValue(i+1, dir);
				use->SetValue(i+1, Standard_True);	
			}
		}
		intp.Load(vec, use, Standard_True);
	}

    intp.Perform();
    Handle(Geom_BSplineCurve) geSpl = intp.Curve();
    TopoDS_Edge e = BRepBuilderAPI_MakeEdge(geSpl);
    TopoDS_Wire w = BRepBuilderAPI_MakeWire(e);

	delete(pary);

	return mrb_fixnum_value(::set(w));
}

/**
 * \brief make box
 */
mrb_value box(mrb_state* mrb, mrb_value self)
{
    mrb_value size;
    mrb_value pos;
    int argc = mrb_get_args(mrb, "A|A", &size, &pos);

	gp_Pnt s = *ar2pnt(mrb, size);
	gp_Pnt op = (argc == 2) ? *ar2pnt(mrb, pos) : gp_Pnt(0.0, 0.0, 0.0);

	TopoDS_Shape shape;
	try {
		BRepPrimAPI_MakeBox box(op, s.X(), s.Y(), s.Z());
	    shape = box.Shape();
	}
	catch (...) {
		static const char m[] = "Failed to make a box.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make sphere
 */
mrb_value sphere(mrb_state* mrb, mrb_value self)
{
    mrb_float r;
	mrb_value pos;
    int argc = mrb_get_args(mrb, "f|A", &r, &pos);

	gp_Pnt op
		= (argc == 2) ? *ar2pnt(mrb, pos) : gp_Pnt(0.0, 0.0, 0.0);

	TopoDS_Shape shape;
	try {
		BRepPrimAPI_MakeSphere sp(op, (Standard_Real)r); 
	    shape = sp.Shape();
	}
	catch (...) {
		static const char m[] = "Failed to make a sphere.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make cylinder
 */
mrb_value cylinder(mrb_state* mrb, mrb_value self)
{
	mrb_value pos, norm;
    mrb_float r, h, a;
	int argc = mrb_get_args(mrb, "AAfff", &pos, &norm, &r, &h, &a);

	gp_Ax2 ax = *ar2ax2(mrb, pos, norm);

	TopoDS_Shape shape;
	try {
		BRepPrimAPI_MakeCylinder cy(ax, (Standard_Real)r, (Standard_Real)h, (Standard_Real)a);
	    shape = cy.Shape();
	}
	catch (...) {
		static const char m[] = "Failed to make a cylinder.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make cone
 */
mrb_value cone(mrb_state* mrb, mrb_value self)
{
	mrb_value pos, norm;
	mrb_float r1, r2, h, ang;
	int argc = mrb_get_args(mrb, "AAffff", &pos, &norm, &r1, &r2, &h, &ang);

	gp_Ax2 ax = *ar2ax2(mrb, pos, norm);

	TopoDS_Shape shape;
	try {
		BRepPrimAPI_MakeCone prm(ax, (Standard_Real)r1, (Standard_Real)r2, (Standard_Real)h, (Standard_Real)ang);
	    shape = prm.Shape();
	}
	catch (...) {
		static const char m[] = "Failed to make a cone.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make torus
 */
mrb_value torus(mrb_state* mrb, mrb_value self)
{
	mrb_float r1, r2, ang;
	mrb_value pos, norm;
	int argc = mrb_get_args(mrb, "AAfff", &pos, &norm, &r1, &r2, &ang);

	gp_Ax2 ax = *ar2ax2(mrb, pos, norm);

	TopoDS_Shape shape;
	try {
		BRepPrimAPI_MakeTorus prm(ax, (Standard_Real)r1, (Standard_Real)r2, (Standard_Real)ang);
	    shape = prm.Shape();
	}
	catch (...) {
		static const char m[] = "Failed to make a torus.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make rectangle plane
 */
mrb_value plane(mrb_state* mrb, mrb_value self)
{
	mrb_value pos, dir;
	mrb_float umin, umax, vmin, vmax;
	int argc = mrb_get_args(mrb, "AAffff", &pos, &dir, &umin, &umax, &vmin, &vmax);

	gp_Pnt _pos = *ar2pnt(mrb, pos);
	gp_Pnt _dir = *ar2pnt(mrb, dir);
	gp_Dir __dir(_dir.X(), _dir.Y(), _dir.Z());
	gp_Pln _pln(_pos, __dir);

	TopoDS_Shape shape;
	try {
		BRepBuilderAPI_MakeFace face(_pln, (Standard_Real)umin, (Standard_Real)umax, (Standard_Real)vmin, (Standard_Real)vmax);
	    shape = face.Shape();
	}
	catch (...) {
		static const char m[] = "Failed to make a plane.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return mrb_fixnum_value(::set(shape));
}


/**
 * \brief make surface by profile-wire and path-wire
 */
mrb_value sweep(mrb_state* mrb, mrb_value self)
{
	mrb_int target;
	mrb_value obj;
	int argc = mrb_get_args(mrb, "io", &target, &obj);

	Handle(AIS_Shape) base = ::get(target);
	if (base.IsNull() ) {
		static const char m[] = "No such profile object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	TopoDS_Shape shape;
	TopoDS_Shape bs, ps;
	TopoDS_Wire pw;
	bs = base->Shape();

	try 
	{
		if (mrb_array_p(obj)) {
            // Vector
			gp_Pnt _vec = *ar2pnt(mrb, obj);
			gp_Pnt _pt = gp_Pnt(0.,0.,0.).Transformed(bs.Location());
			TopoDS_Edge pe = BRepBuilderAPI_MakeEdge(_pt,_vec);
			pw = BRepBuilderAPI_MakeWire(pe);
		} else if (mrb_fixnum_p(obj)) {
            // Profile
			Handle(AIS_Shape) path = ::get(mrb_fixnum(obj));
			if (path.IsNull()) {
				static const char m[] = "No such path object.";
						return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
			}
			ps = path->Shape();
			if (ps.ShapeType() == TopAbs_EDGE)
				pw = BRepBuilderAPI_MakeWire(TopoDS::Edge(path->Shape()));
			else
				pw = TopoDS::Wire(path->Shape());
		}

		BRepOffsetAPI_MakePipe mp(pw, bs);
		mp.Build();
		shape = mp.Shape();
		if (shape.IsNull()) {
			static const char m[] = "Failed to make a sweep model.";
			return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
	}
	catch (...) {
		static const char m[] = "Failed to make a sweep model.";
		return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief make wire by Edge, Wire, Compound object
 */
mrb_value wire(mrb_state* mrb, mrb_value self)
{
	mrb_value objs;
	int argc = mrb_get_args(mrb, "A", &objs);
	int osize = mrb_ary_len(mrb, objs);
	
	ShapeFix_Wire sfw;
	Handle( ShapeExtend_WireData ) wd = new ShapeExtend_WireData;
	BRepBuilderAPI_MakeWire mw;
	TopoDS_Shape shape;
	Standard_Real tol = 0.01;
	ShapeFix_ShapeTolerance FTol;
	try
	{
		for ( int i = 0; i < osize ; i++ )
		{
			mrb_value id = mrb_ary_ref(mrb, objs, i);
			Handle(AIS_Shape) ais = ::get(mrb_fixnum(id));
			TopoDS_Shape s = ais->Shape();
			if ( s.IsNull() ) continue;
			TopAbs_ShapeEnum se = s.ShapeType();
			if ( se != TopAbs_COMPOUND && se != TopAbs_EDGE && se != TopAbs_WIRE ) continue;
			for (TopExp_Explorer exp(s, TopAbs_EDGE); exp.More(); exp.Next())
			{
				if ( !exp.Current().IsNull() ) wd->Add(TopoDS::Edge(exp.Current()));
			}
		}
		sfw.Load( wd );
		sfw.Perform();
		for (int i = 1; i <= sfw.NbEdges(); i ++)
		{
			TopoDS_Edge e = sfw.WireData()->Edge(i);
			FTol.SetTolerance(e, tol, TopAbs_VERTEX);
			mw.Add(e);
		}
		shape = mw.Shape();
		if (shape.IsNull()) {
			delete(wd);
			static const char m[] = "Failed to make a wire.";
			return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
	}	catch (...) {
		delete(wd);
		static const char m[] = "Failed to make a wire.";
		return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	return mrb_fixnum_value(::set(shape));

}

mrb_value loft(mrb_state* mrb, mrb_value self)
{
	mrb_value objs;
	int argc = mrb_get_args(mrb, "A", &objs);
    int lsize = mrb_ary_len(mrb, objs);

    if (lsize < 2) {
        static const char m[] = "Number of objects must be over 2 lines.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    BRepOffsetAPI_ThruSections ts(/* isSolid = */ Standard_False, /* isRuled = */ Standard_True);

    for (int i=0; i<lsize; i++) {
		mrb_value line = mrb_ary_ref(mrb, objs, i);
        mrb_int target = mrb_fixnum(line);
    	Handle(AIS_Shape) hashape = ::get(target);
    	if (hashape.IsNull()) {
    		static const char m[] = "No such profile object.";
            return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    	}
        TopoDS_Shape shape = hashape->Shape();
        TopAbs_ShapeEnum type = shape.ShapeType();
        if (type == TopAbs_WIRE) {
            TopoDS_Wire w = TopoDS::Wire(shape);
            ts.AddWire(w);
        }
        else {
            // EdgeやCompoundなEdgeの場合
            // Wireを作る？
        }
    }

    mrb_value result;
    TopoDS_Shape shape;
    try {
        //ts.SetSmoothing(Standard_True);
        ts.Build();
		shape = ts.Shape();
    	result = mrb_fixnum_value(::set(shape));
    }
    catch (...) {
		static const char m[] = "Internal error.";
        result = mrb_exc_new(mrb, E_RUNTIME_ERROR, m, sizeof(m) - 1);
    }
    return result;
}

/**
 * \brief ポールとウェイトを指定してベジエ曲面を作成する
 */
mrb_value bzsurf(mrb_state* mrb, mrb_value self)
{
	mrb_value ptary, wtary;
	int argc = mrb_get_args(mrb, "A|A", &ptary, &wtary);

    int rlen = mrb_ary_len(mrb, ptary);
    int clen = mrb_ary_len(mrb, mrb_ary_ref(mrb, ptary, 0));

    TColgp_Array2OfPnt poles(0, rlen-1, 0, clen-1);

    for (int r=0; r<rlen; r++) {
        mrb_value ar = mrb_ary_ref(mrb, ptary, r);
        for (int c=0; c<clen; c++) {
            mrb_value p = mrb_ary_ref(mrb, ar, c);
            gp_Pnt pp = *::ar2pnt(mrb, p);
            poles.SetValue(r, c, pp);
        }
    }

    Handle(Geom_BezierSurface) s = NULL;

    if (argc == 2) {
        // ウェイトが指定された場合
        TColStd_Array2OfReal weights(0, rlen-1, 0, clen-1);

        for (int r=0; r<rlen; r++) {
            mrb_value ar = mrb_ary_ref(mrb, wtary, r);
            for (int c=0; c<clen; c++) {
                mrb_value val = mrb_ary_ref(mrb, ar, c);
                double value = mrb_float(val);
                weights.SetValue(r, c, (Standard_Real)value);
            }
        }
        s = new Geom_BezierSurface(poles, weights);
    }
    else {
        s = new Geom_BezierSurface(poles);
    }

    TopoDS_Face f = BRepBuilderAPI_MakeFace(s, 1.0e-7);

	return mrb_fixnum_value(::set(f));
}

/**
 * \brief オフセット曲面を作成する
 */
mrb_value offset(mrb_state* mrb, mrb_value self)
{
	mrb_int target;
	mrb_float offset;
	int argc = mrb_get_args(mrb, "if", &target, &offset);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull() ) {
		static const char m[] = "No such specified object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
    TopoDS_Shape shape = hashape->Shape();

	TopoDS_Compound	comp;
	BRep_Builder B;
	B.MakeCompound(comp);

    TopExp_Explorer exp(shape, TopAbs_FACE);

    for (; exp.More(); exp.Next()) {
        TopoDS_Face face = TopoDS::Face(exp.Current());
        Handle(Geom_Surface) gs = BRep_Tool::Surface(face);
        Handle(Geom_OffsetSurface) gos = new Geom_OffsetSurface(gs, (Standard_Real)offset);
        TopoDS_Face newface = BRepBuilderAPI_MakeFace(gos, 1.0e-7);        
        B.Add(comp, newface);
    }

	return mrb_fixnum_value(::set(comp));
}

/**
* \brief Check existed
*/
mrb_value exist(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    int argc = mrb_get_args(mrb, "i", &target);

    return has_object(target) ? mrb_true_value() : mrb_false_value();
}

/**
* \brief Get location of shape in WCS
*/
mrb_value location(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    mrb_value location;
    int argc = mrb_get_args(mrb, "i|A", &target, &location);

    Handle(AIS_Shape) hashape = ::get(target);
    if (hashape.IsNull()) {
        static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    TopoDS_Shape shape = hashape->Shape();

    if (argc == 2) {
        gp_Pnt p = *ar2pnt(mrb, location);
        gp_Trsf tr;
        tr.SetTranslation(gp_Vec(p.X(), p.Y(), p.Z()));
        shape.Location(TopLoc_Location(tr));
        hashape->Set(shape);
        redisplay(hashape);
        return mrb_nil_value();
    }
    else {
        gp_XYZ pos = shape.Location().Transformation().TranslationPart();
        return pnt2ar(mrb, gp_Pnt(pos.X(), pos.Y(), pos.Z()));
    }
}

/**
* \brief Get type of object
*/
mrb_value type(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    int argc = mrb_get_args(mrb, "i", &target);

    Handle(AIS_Shape) hashape = ::get(target);
    if (hashape.IsNull()) {
        static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    int type = (int)hashape->Shape().ShapeType();

    return mrb_fixnum_value(type);
}

/**
* \brief オブジェクトが存在しているエリアを取得する
* 
* \note グローバル座標系において、オブジェクトが存在しているエリアを
*       X軸、Y軸、Z軸に対してそれぞれ最小値、最大値で取得する。
*/
mrb_value bndbox(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    int argc = mrb_get_args(mrb, "i", &target);

    Handle(AIS_Shape) hashape = ::get(target);
    if (hashape.IsNull()) {
        static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    TopoDS_Shape shape = hashape->Shape();

    Bnd_Box box;
    BRepBndLib::Add(shape, box);
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

#if 0
    mrb_value rmin = ::pnt2ar(mrb, gp_Pnt(xmin, ymin, zmin));
    mrb_value rmax = ::pnt2ar(mrb, gp_Pnt(xmax, ymax, zmax));
#else
    RClass* prVec = mrb_class_get(mrb, "Vec");

    mrb_value arg[3];

    arg[0] = mrb_float_value(mrb, xmin);
    arg[1] = mrb_float_value(mrb, ymin);
    arg[2] = mrb_float_value(mrb, zmin);
    mrb_value rmin = mrb_class_new_instance(mrb, 3, arg, prVec);

    arg[0] = mrb_float_value(mrb, xmax);
    arg[1] = mrb_float_value(mrb, ymax);
    arg[2] = mrb_float_value(mrb, zmax);
    mrb_value rmax = mrb_class_new_instance(mrb, 3, arg, prVec);
#endif

    mrb_value res[2];
    res[0] = rmin;
    res[1] = rmax;	

    return mrb_ary_new_from_values(mrb, 2, res);
}

/**
* \brief 線上点における曲線パラメータを取得する
*/
mrb_value cparam(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    mrb_value xyz;
    int argc = mrb_get_args(mrb, "iA", &target, &xyz);

    Handle(AIS_Shape) hashape = ::get(target);
    if (hashape.IsNull()) {
        static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    gp_Pnt p = *::ar2pnt(mrb, xyz);
    TopoDS_Shape shape = hashape->Shape();

    Standard_Real tol = 1.0e-7;
    TopExp_Explorer exp(shape, TopAbs_EDGE);
    ShapeAnalysis_Curve ana;
    gp_Pnt pp;

    Standard_Real param, distance;
    int n; // index

    for (n = 0; exp.More(); n++, exp.Next()) {
        TopoDS_Edge e = TopoDS::Edge(exp.Current());
        if (e.IsNull())
            continue;

        BRepAdaptor_Curve gcurve(e);

        distance = ana.Project(gcurve, p, tol, pp, param);

        if (fabs(distance) <= tol) {
            break;
        }
    }

    mrb_value res[2];
    res[0] = mrb_fixnum_value(n);
    res[1] = mrb_float_value(mrb, param);

    return mrb_ary_new_from_values(mrb, 2, res);
}

/**
* \brief 曲線パラメータに対応する点座標を取得する
*/
mrb_value cpoint(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    mrb_value params;
    int argc = mrb_get_args(mrb, "iA", &target, &params);

    Handle(AIS_Shape) hashape = ::get(target);
    if (hashape.IsNull()) {
        static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }
    TopoDS_Shape shape = hashape->Shape();

    if (mrb_ary_len(mrb, params) != 2) {
        static const char m[] = "Illigal array of params.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    int index = mrb_fixnum(mrb_ary_ref(mrb, params, 0));
    double param = mrb_float(mrb_ary_ref(mrb, params, 1));

    TopExp_Explorer exp(shape, TopAbs_EDGE);
    for (int i=0; i<index && exp.More(); i++, exp.Next())
        ;
    TopoDS_Edge e = TopoDS::Edge(exp.Current());
    if (e.IsNull()) {
        static const char m[] = "Illigal index of params.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    BRepAdaptor_Curve c(e);

    gp_Pnt p;
    gp_Vec v1, v2;
    c.D2((Standard_Real)param, p, v1, v2);

    return ::pnt2ar(mrb, p);
}

/**
* \brief 線上点(パラメータ指定)における曲率線ベクトルを取得する
*/
mrb_value ccurvature(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    mrb_value params;
    int argc = mrb_get_args(mrb, "iA", &target, &params);

    Handle(AIS_Shape) hashape = ::get(target);
    if (hashape.IsNull()) {
        static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }
    TopoDS_Shape shape = hashape->Shape();

    if (mrb_ary_len(mrb, params) != 2) {
        static const char m[] = "Illigal array of params.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    int index = mrb_fixnum(mrb_ary_ref(mrb, params, 0));
    double param = mrb_float(mrb_ary_ref(mrb, params, 1));

    TopExp_Explorer exp(shape, TopAbs_EDGE);
    for (int i=0; i<index && exp.More(); i++, exp.Next())
        ;
    TopoDS_Edge e = TopoDS::Edge(exp.Current());
    if (e.IsNull()) {
        static const char m[] = "Illigal index of params.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    BRepAdaptor_Curve c(e);

    gp_Pnt p;
    gp_Vec v1, v2;
    c.D2((Standard_Real)param, p, v1, v2);

    return ::vec2ar(mrb, v2);
}

/**
* \brief 線上点(パラメータ指定)における接線ベクトルを取得する
*/
mrb_value ctangent(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
    mrb_value params;
    int argc = mrb_get_args(mrb, "iA", &target, &params);

    Handle(AIS_Shape) hashape = ::get(target);
    if (hashape.IsNull()) {
        static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }
    TopoDS_Shape shape = hashape->Shape();

    if (mrb_ary_len(mrb, params) != 2) {
        static const char m[] = "Illigal array of params.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    int index = mrb_fixnum(mrb_ary_ref(mrb, params, 0));
    double param = mrb_float(mrb_ary_ref(mrb, params, 1));

    TopExp_Explorer exp(shape, TopAbs_EDGE);
    for (int i=0; i<index && exp.More(); i++, exp.Next())
        ;
    TopoDS_Edge e = TopoDS::Edge(exp.Current());
    if (e.IsNull()) {
        static const char m[] = "Illigal index of params.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

    BRepAdaptor_Curve c(e);

    gp_Pnt p;
    gp_Vec v1, v2;
    c.D2((Standard_Real)param, p, v1, v2);

    return ::vec2ar(mrb, v1);
}

/**
 * \brief Make coupound from multiple objects.
 */
mrb_value compound(mrb_state* mrb, mrb_value self)
{
	mrb_value ar;
	int argc = mrb_get_args(mrb, "A", &ar);

	TopoDS_Compound	comp;
	BRep_Builder B;
	B.MakeCompound(comp);

	int psize = mrb_ary_len(mrb, ar);
	for (int i=0; i<psize; i++) {
		mrb_value item = mrb_ary_ref(mrb, ar, i);
		if (!mrb_fixnum_p(item)) {
			static const char m[] = "Incorrect argument specified.";
	        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
		mrb_int target = mrb_fixnum(item);
		Handle(AIS_Shape) hashape = ::get(target);
		if (hashape.IsNull()) {
			static const char m[] = "No such object name of specified at first.";
	        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
		TopoDS_Shape shape = hashape->Shape();
		B.Add(comp, shape);
        ::unset(target);
	}
	return mrb_fixnum_value(::set(comp));
}

/**
 * \brief Sew objects to shell.
 */
mrb_value sew(mrb_state* mrb, mrb_value self)
{
	mrb_value ar;
	mrb_float tol;
	int argc = mrb_get_args(mrb, "A|f", &ar, &tol);

	BRepBuilderAPI_Sewing sewer;
	sewer.Init();
	if (argc == 2 && tol >= 0)
		sewer.SetTolerance(tol);

	int psize = mrb_ary_len(mrb, ar);
	for (int i=0; i<psize; i++) {
		mrb_value item = mrb_ary_ref(mrb, ar, i);
		if (!mrb_fixnum_p(item)) {
			static const char m[] = "Incorrect argument specified.";
	        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
		mrb_int target = mrb_fixnum(item);
		Handle(AIS_Shape) hashape = ::get(target);
		if (hashape.IsNull()) {
#if 0
			static const char m[] = "No such object name of specified at first.";
	        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
#else
            continue;
#endif
		}
		TopoDS_Shape shape = hashape->Shape();
        TopExp_Explorer ex(shape, TopAbs_FACE);
        for (; ex.More(); ex.Next()) {
            sewer.Add(ex.Current());
        }
	}

	TopoDS_Shape result;
	try {
		sewer.Perform();
		result = sewer.SewedShape();
	}
	catch (...) {
		static const char m[] = "Failed to sew objects.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	return mrb_fixnum_value(::set(result));
}

/**
 * \brief Explode object to child elements.
 */
mrb_value explode(mrb_state* mrb, mrb_value self)
{
	mrb_int type;
	mrb_int target;
	int argc = mrb_get_args(mrb, "ii", &type, &target);

	if (type < 0 || type > (int)TopAbs_VERTEX) {
		static const char m[] = "Incorrect type specified.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	TopAbs_ShapeEnum shapetype = (TopAbs_ShapeEnum)type;

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	mrb_value ar = mrb_ary_new(mrb);
    TopExp_Explorer ex(hashape->Shape(), shapetype);

    for (; ex.More(); ex.Next()) {
        const TopoDS_Shape& Sx = ex.Current();
		mrb_value hc = mrb_fixnum_value(::set(Sx));
		mrb_ary_push(mrb, ar, hc);
    }

    ::unset(target);

	return ar;
}

/**
 * \brief Erase object
 */
mrb_value erase(mrb_state* mrb, mrb_value self)
{
    mrb_value obj;
    int argc = mrb_get_args(mrb, "o", &obj);
    if (mrb_fixnum_p(obj)) {
    	::unset(mrb_fixnum(obj));
    }
    else if (mrb_array_p(obj)) {
        for (int i=0; i<mrb_ary_len(mrb, obj); i++) {
            mrb_value item = mrb_ary_ref(mrb, obj, i);
            mrb_int hc = mrb_fixnum(item);
        	::unset(hc);
        }
    }
	return mrb_nil_value();
}

/**
 * \brief copy object
 */
mrb_value copy(mrb_state* mrb, mrb_value self)
{
	mrb_int src;
	int argc = mrb_get_args(mrb, "i", &src);

	Handle(AIS_Shape) hashape = ::get(src);
	if (hashape.IsNull()) {
		static const char m[] = "No such specified object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	// Deep copy
	BRepBuilderAPI_Copy Builder;
	Builder.Perform(hashape->Shape());
	TopoDS_Shape shape = Builder.Shape();

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief close wire / edge object
 */
mrb_value close(mrb_state* mrb, mrb_value self)
{
	mrb_int src;
	int argc = mrb_get_args(mrb, "i", &src);

	Handle(AIS_Shape) hashape = ::get(src);
	if (hashape.IsNull()) {
		static const char m[] = "No such specified object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	TopAbs_ShapeEnum se = hashape->Shape().ShapeType();
	if ( se != TopAbs_WIRE && se != TopAbs_EDGE && se != TopAbs_COMPOUND ) {
		static const char m[] = "Failed to make a closed wire.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	TopoDS_Shape shape;
	TopoDS_Shape sThis = hashape->Shape();
	try {
		BRepBuilderAPI_MakeWire mw;
		// find first point
		BRepAdaptor_Curve ac;
		gp_Pnt sp, ep;
		TopExp_Explorer exp( sThis, TopAbs_EDGE );
		double tol = 0.01;
		TopoDS_Edge e = TopoDS::Edge( exp.Current() );
		ac.Initialize( e );
		sp = ac.Value( ac.FirstParameter() );
		ep = ac.Value( ac.LastParameter() );
		mw.Add( e );
		if ( e.Orientation() == TopAbs_REVERSED ) sp = ep;
		//find last point
		int count = 0;
		for ( ; exp.More(); exp.Next() )
		{
			if ( exp.Current().IsNull() ) continue;
			e = TopoDS::Edge( exp.Current() );
			mw.Add( e );
			ac.Initialize( e );
			if (e.Orientation() == TopAbs_FORWARD )
				ep = ac.Value( ac.LastParameter() );
			else if (e.Orientation() == TopAbs_REVERSED )
				ep = ac.Value( ac.FirstParameter() );
		}
		if ( !sp.IsEqual( ep, tol ) ) {
			e = BRepBuilderAPI_MakeEdge( ep, sp );
			mw.Add(e);
		}
		ShapeFix_Wire sfw;
		sfw.Load( mw.Wire() );
		sfw.FixClosed();
		shape = sfw.Wire();
		if ( shape.IsNull() )
		{
			static const char m[] = "Failed to make a closed wire.";
			return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
	}	catch (...) {
		static const char m[] = "Failed to make a closed wire.";
		return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	return mrb_fixnum_value(::set(shape));
#if 0

        double qTolerance = 0.1e-3;

	Handle( ShapeExtend_WireData ) wd1 = new ShapeExtend_WireData(); //バッファ1
	gp_Pnt wsp,wep; //全体の終始点
	gp_Pnt sp,ep; //点
	gp_Pnt sp2,ep2; //点
	BRepAdaptor_Curve c1, c2; //カーブ
	int is = 0; //最初のEdgeのIndex
	int ie = 0;
	int num = 0; //エッジ総数
	int all = 0; //中線の数
	int hit = 0; //片方が繋がっていないEdgeの数
	bool fs, fe; //繋がっているかのフラグ

	//まずは全体の終始点を取得
	for ( TopExp_Explorer exp(sThis, TopAbs_EDGE ); exp.More(); exp.Next() )
	{
		TopoDS_Edge e = TopoDS::Edge(exp.Current());
		fs = false;
		fe = false;
		c1.Initialize( e );
		sp = c1.Value(c1.FirstParameter());
		ep = c1.Value(c1.LastParameter());
		for ( TopExp_Explorer exp2(sThis, TopAbs_EDGE ); exp2.More(); exp2.Next() )
		{
			//自身以外を検査
			if ( !e.IsEqual(exp2.Current()) )
			{
				const TopoDS_Edge e2 = TopoDS::Edge(exp2.Current());
				c2.Initialize( e2 );
				sp2 = c2.Value(c2.FirstParameter());
				ep2 = c2.Value(c2.LastParameter());
				if ( sp.IsEqual( sp2, qTolerance ) || sp.IsEqual( ep2, qTolerance ) ) fs = true;
				if ( ep.IsEqual( sp2, qTolerance ) || ep.IsEqual( ep2, qTolerance ) ) fe = true;
				if ( fs && fe ) { all++; break; }
			}
		}
		
		if (hit < 2 && ((!fs && fe) || (fs && !fe)) )
		{// hitカウンタが2未満なら設定
			if (!hit) {
				if (!fs && fe) wsp = sp; else wsp = ep; 
				is = num;
				hit++;
			} else {
				if (!fs && fe) wep = sp; else wep = ep; 
				ie = num;
				hit++;
			}
		}

		// 1本以外で独立したedgeの場合error
		if (num && !fs && !fe)
		{
            printf("");;
		}
		num++;
		e.Orientation( TopAbs_FORWARD ); //向きを統一しておく
		wd1->Add(e);
	}

	//総数が一本だけの場合はそのままセット
	if( num == 1 )
	{
        printf("");;
	}
	
	//修正点がない場合は閉区画として適当な位置から探す
	if( hit < 1 && num == all ) { wsp = sp; is = num; }

	//最初のEdgeを取得
	TopoDS_Edge se = wd1->Edge(is + 1); //!! OCCの要素は1からカウント !!
	num = wd1->NbEdges() - 1;
	BRepAdaptor_Curve sc( se );
	sp = sc.Value(sc.FirstParameter());
	ep = sc.Value(sc.LastParameter());
	//FirstParameterと始点が反転してたら逆にする
	if ( !wsp.IsEqual( sp, qTolerance ) && wsp.IsEqual( ep, qTolerance ) )
	{
		if (se.Orientation() == TopAbs_FORWARD )
		{
			se.Orientation( TopAbs_REVERSED );
		} else {
			se.Orientation( TopAbs_FORWARD );
		}
		gp_Pnt bak = sp;
		sp = ep;
		ep = bak;
	}
	Handle( ShapeExtend_WireData ) wd2 = new ShapeExtend_WireData(); //バッファ2
	wd2->Add( se, 1 );
	bool r; //エッジの反転フラグ
	bool h;
	hit = 0;
	//連続性のある順番で格納していく
	while ( num > 0 )
	{
		for ( int i=1; i <= wd1->NbEdges(); i++ )
		{
			r = false;
			h = false;
			TopoDS_Edge e = wd1->Edge( i );
			if ( !wd2->Index(e) )
			{ //すでに格納したエッジ以外を検査
				c1.Initialize( e );
				sp2 = c1.Value(c1.FirstParameter());
				ep2 = c1.Value(c1.LastParameter());
				if ( ep.IsEqual( sp2, qTolerance ) ) { h = true; }
				else if ( ep.IsEqual( ep2, qTolerance ) )	{ h = true; r = true; }

				if (h) //見つかった
				{
					if (r)
					{ //反転処理
						e.Orientation( TopAbs_REVERSED );
						gp_Pnt bak = sp2;
						sp2 = ep2;
						ep2 = bak;
					} 
					cout << sp.X() << "\t" << sp.Y() << "\t" << sp.Z() << endl;
					cout << ep.X() << "\t" << ep.Y() << "\t" << ep.Z() << "\t" << e.Orientation() << "\t" << r << endl;

					wd2->Add( e ); //バッファに追加
					sp = sp2;	//次に検査する値にセット
					ep = ep2;
					num--; //ひとつ減らす
					break; //for文を抜ける
				}
			}
		}
	hit++;
	if ( hit > 10 ) break; //無限ループ解除
	}
	shape = wd2->Wire();
	if ( shape.IsNull() )
		{
			static const char m[] = "Failed to make a closed wire.";
			return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
	}	catch (...) {
		static const char m[] = "Failed to make a closed wire.";
		return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	return mrb_fixnum_value(::set(shape));
#endif
}

/**
 * \brief wire to points
 */
mrb_value wire2pts(mrb_state* mrb, mrb_value self)
{
	mrb_int src;
	mrb_float deflect;
	int argc = mrb_get_args(mrb, "i|f", &src, &deflect);

	Handle(AIS_Shape) hashape = ::get(src);
	if (hashape.IsNull()) {
		static const char m[] = "No such specified object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	if (argc != 2)
		deflect = 1.0e-1;

	mrb_value result = mrb_ary_new(mrb);
	double first_param, last_param;

	TopExp_Explorer	exp(hashape->Shape(), TopAbs_EDGE);
	for(; exp.More(); exp.Next()) {
		const TopoDS_Edge& edge = TopoDS::Edge(exp.Current());
		try {
			BRepAdaptor_Curve adaptor(edge);
			first_param = adaptor.FirstParameter();
			last_param = adaptor.LastParameter();

			GCPnts_UniformDeflection unidef(adaptor, (Standard_Real)deflect);
			if (!unidef.IsDone())
				continue;

			mrb_value line = mrb_ary_new(mrb);

			// first point
			mrb_ary_push(mrb, line, pnt2ar(mrb, adaptor.Value(first_param)));

			for (int i=0; i<unidef.NbPoints(); i++)
				mrb_ary_push(mrb, line, pnt2ar(mrb, unidef.Value(i)));

			// last point
			mrb_ary_push(mrb, line, pnt2ar(mrb, adaptor.Value(last_param)));
			
			mrb_ary_push(mrb, result, line);
		}
		catch (...) {
			; // nothing to do.
		}
	}

    return result;
}

/**
 * \brief wire to plane
 */
mrb_value wire2plane(mrb_state* mrb, mrb_value self)
{
	//!!Curveを含んだWireには未対応!!
	mrb_int src;
	int argc = mrb_get_args(mrb, "i", &src);

	Handle(AIS_Shape) hashape = ::get(src);
	if (hashape.IsNull()) {
		static const char m[] = "No such specified object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	if ( hashape->Shape().ShapeType() != TopAbs_WIRE ) {
		static const char m[] = "Failed to make a plane.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	TopoDS_Shape shape;
	try {
		TopoDS_Wire w = TopoDS::Wire( hashape->Shape() );
		BRepBuilderAPI_MakeFace mf( w, Standard_True );
		mf.Build();
		if (  !mf.IsDone() ) {
			static const char m[] = "Failed to make a plane.";
			return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
		shape = mf.Shape();
		//TopAbs_ShapeEnum se = shape.ShapeType();
		if ( shape.IsNull() )
		{
			static const char m[] = "Failed to make a plane.";
			return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
	}	catch (...) {
		static const char m[] = "Failed to make a plane.";
		return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief Make a solid by shell.
 */
mrb_value shell2solid(mrb_state* mrb, mrb_value self)
{
    mrb_int obj;
	int argc = mrb_get_args(mrb, "i", &obj);

	Handle(AIS_Shape) hashape = ::get(obj);
	if (hashape.IsNull()) {
		static const char m[] = "No such specified object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

    mrb_value result;

    TopoDS_Solid solid;
    TopoDS_Shape shape = hashape->Shape();
    BRepBuilderAPI_MakeSolid solid_maker;
    
	for (TopExp_Explorer ex(shape, TopAbs_SHELL); ex.More(); ex.Next()) {
		TopoDS_Shell shell = TopoDS::Shell(ex.Current());
        solid_maker.Add(shell);
	}
	if (solid_maker.IsDone()) {
		solid = solid_maker.Solid();
        result = mrb_fixnum_value(::set(solid));
	}
	else {
		static const char m[] = "Failed to make a solid by shell.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

    return result;
}

/**
 * \brief Make triangle mesh from face.
 */
mrb_value triangle(mrb_state* mrb, mrb_value self)
{
    mrb_int obj;
    mrb_float defl, ang;
	int argc = mrb_get_args(mrb, "i|ff", &obj, &defl, &ang);

    Standard_Real deflection = (argc >= 2 ? (Standard_Real)defl : 1.0);
    Standard_Real angle      = (argc == 3 ? (Standard_Real)ang  : 0.5);

	Handle(AIS_Shape) hashape = ::get(obj);
	if (hashape.IsNull()) {
		static const char m[] = "No such specified object.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
    TopoDS_Shape shape = hashape->Shape();

    TopoDS_Compound comp;
    BRep_Builder B;
    B.MakeCompound(comp);

    for (TopExp_Explorer ex(shape, TopAbs_FACE); ex.More(); ex.Next()) {

        TopoDS_Face face = TopoDS::Face(ex.Current());
        BRepTools::Update(face);

        BRepMesh_IncrementalMesh imesh(face, deflection, Standard_False, angle);
        imesh.Perform();
        if (!imesh.IsDone())
            continue;

        face = TopoDS::Face(imesh.Shape());

        TopLoc_Location loc;
        // Do triangulation
        Handle(Poly_Triangulation) poly = BRep_Tool::Triangulation(face, loc);
        if (poly.IsNull()) {
            continue;
        }

        const Poly_Array1OfTriangle& tris = poly->Triangles();

        for (Standard_Integer i = tris.Lower(); i <= tris.Upper(); i++) {

            const Poly_Triangle& tri = tris.Value(i);

            // Node indexes
            Standard_Integer n1, n2, n3;

            if (face.Orientation() == TopAbs_REVERSED)
                tri.Get(n3, n2, n1);
            else
                tri.Get(n1, n2, n3);

            gp_Pnt p1 = poly->Nodes().Value(n1);
            gp_Pnt p2 = poly->Nodes().Value(n2);
            gp_Pnt p3 = poly->Nodes().Value(n3);

            gp_Vec u = gp_Vec(p2.XYZ() - p1.XYZ());                    
            gp_Vec v = gp_Vec(p3.XYZ() - p1.XYZ());

            gp_Vec norm(
                u.Y() * v.Z() - u.Z() * v.Y(),
                u.Z() * v.X() - u.X() * v.Z(),
                u.X() * v.Y() - u.Y() * v.X());
            if (norm.Magnitude() <= 0)
                continue;
            norm.Normalize();

            BRepBuilderAPI_MakePolygon polymaker;
            polymaker.Add(p1);
            polymaker.Add(p2);
            polymaker.Add(p3);
            polymaker.Add(p1);

            polymaker.Build();
            if (polymaker.IsDone()) {
                TopoDS_Wire w = polymaker.Wire();
                BRepBuilderAPI_MakeFace mf(w, Standard_True);
                mf.Build();
                if (mf.IsDone()) {
                    B.Add(comp, mf.Shape());
                }
            }
        }
    }
    hashape->Set(comp);

    return mrb_nil_value();
}

/**
 * \brief 
 */
mrb_value obj2brep(mrb_state* mrb, mrb_value self)
{
    return mrb_exc_new(mrb, E_NOTIMP_ERROR, NULL, 0);
}

/**
 * \brief 
 */
mrb_value brep2obj(mrb_state* mrb, mrb_value self)
{
    return mrb_exc_new(mrb, E_NOTIMP_ERROR, NULL, 0);
}

/**
 * \brief get across area of s1 by s2
 */
mrb_value common(mrb_state* mrb, mrb_value self)
{
	mrb_int s1, s2;
	int argc = mrb_get_args(mrb, "ii", &s1, &s2);

	Handle(AIS_Shape) haS1 = ::get(s1);
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = ::get(s2);
	if (haS2.IsNull()) {
		static const char m[] = "No such object name of specified at second.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	BRepAlgoAPI_Common bo(haS1->Shape(), haS2->Shape());
	bo.SetOperation(BOPAlgo_COMMON);
	bo.Build();

	mrb_value result;

	if (!bo.ErrorStatus()) {
	    TopoDS_Shape shape = bo.Shape();
		result = mrb_fixnum_value(::set(shape));
	}
	else {
		static const char m[] = "Failed to fuse operation of boolean.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	return result;
}

/**
 * \brief cut object (s1 by s2)
 */
mrb_value cut(mrb_state* mrb, mrb_value self)
{
    mrb_int s1, s2;
	int argc = mrb_get_args(mrb, "ii", &s1, &s2);

	Handle(AIS_Shape) haS1 = ::get(s1);
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = ::get(s2);
	if (haS2.IsNull()) {
		static const char m[] = "No such object name of specified at second.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	BRepAlgoAPI_Common bo(haS1->Shape(), haS2->Shape());
	bo.SetOperation(BOPAlgo_CUT);
	bo.Build();

	mrb_value result;

	if (!bo.ErrorStatus()) {
	    TopoDS_Shape shape = bo.Shape();
		result = mrb_fixnum_value(::set(shape));
	}
	else {
		static const char m[] = "Failed to fuse operation of boolean.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	return result;
}

/**
 * \brief add object
 */
mrb_value fuse(mrb_state* mrb, mrb_value self)
{
    mrb_int s1, s2;
	int argc = mrb_get_args(mrb, "ii", &s1, &s2);

	Handle(AIS_Shape) haS1 = ::get(s1);
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = ::get(s2);
	if (haS2.IsNull()) {
		static const char m[] = "No such object name of specified at second.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	BRepAlgoAPI_Common bo(haS1->Shape(), haS2->Shape());
	bo.SetOperation(BOPAlgo_FUSE);
	bo.Build();

	mrb_value result;

	if (!bo.ErrorStatus()) {
	    TopoDS_Shape shape = bo.Shape();
		result = mrb_fixnum_value(::set(shape));
	}
	else {
		static const char m[] = "Failed to fuse operation of boolean.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	return result;
}

/**
 * \brief get volume of solid object
 */
mrb_value volume(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
	int argc = mrb_get_args(mrb, "i", &target);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	GProp_GProps gprops;
	BRepGProp::VolumeProperties(hashape->Shape(), gprops);
	Standard_Real vol = gprops.Mass();

	return mrb_float_value(mrb, (mrb_float)vol);
}

/**
 * \brief get point of center of gravity
 */
mrb_value cog(mrb_state* mrb, mrb_value self)
{
    mrb_int target;
	int argc = mrb_get_args(mrb, "i", &target);

	Handle(AIS_Shape) hashape = ::get(target);
	if (hashape.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	GProp_GProps gprops;
	BRepGProp::VolumeProperties(hashape->Shape(), gprops);
	gp_Pnt cog = gprops.CentreOfMass();

	return ::pnt2ar(mrb, cog);
}

/**
 * \brief 2つのオブジェクトの交線を求め、作成する
 */
mrb_value intersect(mrb_state* mrb, mrb_value self)
{
    mrb_int s1, s2;
	int argc = mrb_get_args(mrb, "ii", &s1, &s2);

	Handle(AIS_Shape) haS1 = ::get(s1);
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = ::get(s2);
	if (haS2.IsNull()) {
		static const char m[] = "No such object name of specified at second.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	BRepAlgoAPI_Section	sect(haS1->Shape(), haS2->Shape(), Standard_False );
	sect.ComputePCurveOn1(Standard_True);
	sect.Approximation(Standard_True);
	sect.Build();

	if (!sect.IsDone()) {
		static const char m[] = "Failed to intersection.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	TopoDS_Shape shape = sect.Shape();

	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief 分割する
 */
mrb_value split(mrb_state* mrb, mrb_value self)
{
    mrb_int s1, s2;
	int argc = mrb_get_args(mrb, "ii", &s1, &s2);

	Handle(AIS_Shape) haS1 = ::get(s1);
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = ::get(s2);
	if (haS2.IsNull()) {
		static const char m[] = "No such object name of specified at second.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

    // Intersection
    BRepAlgoAPI_Section asect(haS1->Shape(), haS2->Shape(), FALSE);
    asect.ComputePCurveOn1(TRUE);
    asect.Approximation(TRUE);
    asect.Build();
    if(!asect.IsDone())
        return mrb_nil_value();
    TopoDS_Shape curve = asect.Shape();

    BRepFeat_SplitShape splitter(haS1->Shape());
    TopExp_Explorer exp(curve, TopAbs_EDGE);

    for (; exp.More(); exp.Next()) {
        TopoDS_Shape e = exp.Current();     
        TopoDS_Shape f;
        if (asect.HasAncestorFaceOn1(e, f)) {
             TopoDS_Edge ee = TopoDS::Edge(e);
             TopoDS_Face ff = TopoDS::Face(f);
             splitter.Add(ee, ff);
        }
    }
    try {
        splitter.Build();
    	if (!splitter.IsDone()) {
    		static const char m[] = "Failed to intersection.";
            return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    	}
    }
    catch (...) {
		static const char m[] = "Failed to intersection.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }

	TopoDS_Shape shape = splitter.Shape();
	return mrb_fixnum_value(::set(shape));
}

/**
 * \brief サーフェスとカーブの交点を得る
 *
 * \note サーフェスの外形を考慮せず無限曲面の交点を得る
 */
/*
 * GeomAPI_IntCS
 * IntCurvesFace_ShapeIntersector
 * IntCurvesFace_Intersector
 */
mrb_value intcs(mrb_state* mrb, mrb_value self)
{
    mrb_int c, s;
    mrb_bool with_normal = FALSE;
	int argc = mrb_get_args(mrb, "ii|b", &c, &s, &with_normal);

	Handle(AIS_Shape) hacurve = ::get(c);
	if (hacurve.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) hasurf = ::get(s);
	if (hasurf.IsNull()) {
		static const char m[] = "No such object name of specified at second.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

    mrb_value result = mrb_ary_new(mrb);

    Standard_Real first, last;
    TopExp_Explorer exc, exs;

    for (exc.Init(hacurve->Shape(), TopAbs_EDGE); exc.More(); exc.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(exc.Current());
        Handle(Geom_Curve) gcurve = BRep_Tool::Curve(edge, first, last);

        for (exs.Init(hasurf->Shape(), TopAbs_FACE); exs.More(); exs.Next()) {
            TopoDS_Face face = TopoDS::Face(exs.Current());
            Handle(Geom_Surface) gsurf  = BRep_Tool::Surface(face);
            GeomAPI_IntCS isc = GeomAPI_IntCS(gcurve, gsurf);
            if (!isc.IsDone())
                continue;
            for (int i = 1; i <= isc.NbPoints() ; i++ ) {
               // Intersect point
               gp_Pnt p = isc.Point(i);
               mrb_value item = pnt2ar(mrb, p);
        	   mrb_ary_push(mrb, result, item);

               // Normal vector on a point
               if (with_normal) {
                   Quantity_Parameter u, v, w;
                   isc.Parameters(i, u, v, w);
                   BRepAdaptor_Surface aSurface(face);
                   gp_Vec ut, vt;
                   gp_Pnt pt;
                   aSurface.D1(u, v, pt, ut, vt);
                   gp_Vec normal = ut.Crossed(vt);
                   normal.Normalize();
                   gp_Pnt np(normal.X(), normal.Y(), normal.Z());
                   mrb_value a = pnt2ar(mrb, np);
            	   mrb_ary_push(mrb, result, a);
               }
            }
        }
    }

    return result;
}

/**
 * \brief フェイスとエッジの交点を得る
 */
mrb_value intfe(mrb_state* mrb, mrb_value self)
{
    mrb_int c, s;
    mrb_bool with_normal = FALSE;
	int argc = mrb_get_args(mrb, "ii|b", &s, &c, &with_normal);

	Handle(AIS_Shape) hacurve = ::get(c);
	if (hacurve.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) hasurf = ::get(s);
	if (hasurf.IsNull()) {
		static const char m[] = "No such object name of specified at second.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

    mrb_value result = mrb_ary_new(mrb);

    TopExp_Explorer exc, exs;

    for (exc.Init(hacurve->Shape(), TopAbs_EDGE); exc.More(); exc.Next()) {
        TopoDS_Edge edge = TopoDS::Edge(exc.Current());

        for (exs.Init(hasurf->Shape(), TopAbs_FACE); exs.More(); exs.Next()) {
            TopoDS_Face face = TopoDS::Face(exs.Current());

            TopOpeBRep_FaceEdgeIntersector feint;
            feint.Perform(face, edge);

            for (feint.InitPoint(); feint.MorePoint(); feint.NextPoint()) {
                gp_Pnt p = feint.Value();

                mrb_value item = pnt2ar(mrb, p);
        	    mrb_ary_push(mrb, result, item);
            }
        }
    }

    return result;
}

/**
 * \brief Save object to BRep file
 */
mrb_value savebrep(mrb_state* mrb, mrb_value self)
{
    mrb_value path;
	mrb_int target; 
	int argc = mrb_get_args(mrb, "Si", &path, &target);

	mrb_value result;

	Handle(AIS_Shape) hashape = ::get((int)target);

	if (hashape.IsNull()) {
		static const char m[] = "No such named object.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	else {
		Standard_Boolean res = BRepTools::Write(hashape->Shape(), (Standard_CString)RSTRING_PTR(path));

		if (res) {
			result = mrb_nil_value();
		}
		else {
			static const char m[] = "Failed to save BRep file.";
	        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
		}
	}

	return result;
}

/**
 * \brief Load object from BRep file
 */
mrb_value loadbrep(mrb_state* mrb, mrb_value self)
{
    mrb_value path;
	int argc = mrb_get_args(mrb, "S", &path);

	TopoDS_Shape shape;
    BRep_Builder aBuilder;
    Standard_Boolean res = BRepTools::Read(shape, (Standard_CString)RSTRING_PTR(path), aBuilder);

	mrb_value result;

	if (res) {
		//if(AISContext->HasOpenedContext())
		//	AISContext->CloseLocalContext();
		result = mrb_fixnum_value(::set(shape));
	}
	else {
		static const char m[] = "Failed to load BRep file.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return result;
}

/**
 * \brief Save object to IGES file
 */
mrb_value saveiges(mrb_state* mrb, mrb_value self)
{
    return mrb_exc_new(mrb, E_NOTIMP_ERROR, NULL, 0);
}

/**
 * \brief Load object from IGES file
 */
mrb_value loadiges(mrb_state* mrb, mrb_value self)
{
    mrb_value path;
	int argc = mrb_get_args(mrb, "S", &path);

    IGESControl_Reader iges_reader;
    int stat = iges_reader.ReadFile((Standard_CString)RSTRING_PTR(path));
	mrb_value result;

    if (stat == IFSelect_RetDone) {
	    iges_reader.TransferRoots();
	    TopoDS_Shape shape = iges_reader.OneShape();
		result = mrb_fixnum_value(::set(shape));
	}
	else {
		static const char m[] = "Failed to load IGES file.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
    return result;
}

/**
 * \brief Save object to STEP file
 */
mrb_value savestep(mrb_state* mrb, mrb_value self)
{
    return mrb_exc_new(mrb, E_NOTIMP_ERROR, NULL, 0);
}

/**
 * \brief Load object from STEP file
 */
mrb_value loadstep(mrb_state* mrb, mrb_value self)
{
    return mrb_exc_new(mrb, E_NOTIMP_ERROR, NULL, 0);
}

/**
 * \brief Load object from STL file
 */
mrb_value loadstl(mrb_state* mrb, mrb_value self)
{
    mrb_value path;
	int argc = mrb_get_args(mrb, "S", &path);

    // OSD_Path aFile();
    // Handle(StlMesh_Mesh) aSTLMesh = RWStl::ReadFile(aFile);

    TopoDS_Shape shape;
    // StlAPI_Reader::Read(shape, (Standard_CString)RSTRING_PTR(path));

    StlAPI_Reader reader;
    reader.Read(shape, (Standard_CString)RSTRING_PTR(path));

    mrb_value result;

    if (shape.IsNull()) {
	 	static const char m[] = "Failed to load STL file.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
    }
    else {
	 	result = mrb_fixnum_value(::set(shape));
    }

    return result;
}

/**
 * \brief version information
 */
mrb_value version(mrb_state* mrb, mrb_value self)
{
    return mrb_exc_new(mrb, E_NOTIMP_ERROR, NULL, 0);
}

mrb_value debug2(mrb_state* mrb, mrb_value self)
{
    return mrb_fixnum_value(123);
}

/**
 * \brief debug command
 */
mrb_value debug(mrb_state* mrb, mrb_value self)
{
    //RClass* my_class = mrb_define_class(mrb, "Shape", mrb->object_class);
    //mrb_define_method(mrb, my_class, "type", debug2, ARGS_NONE());
    //RClass* a_class = mrb_class_get(myMirb->mrb, "MyClass");
    // mrb_value args[2];
    // args[0] = mrb_fixnum_value(LED_ORANGE);     //pin Number
    // args[1] = mrb_fixnum_value(1000);   //interval

    //return mrb_funcall(mrb, a_obj, "type", 0);

    //  mrb_funcall(mrb, blinker_obj,"run",0);
    // return a_obj;

    RClass* my_class = mrb_define_class(mrb, "Test", mrb->object_class);
    mrb_value obj = mrb_class_new_instance(mrb, 0, NULL, my_class);
    mrb_sym sym = mrb_intern(mrb, "asdf", strlen("asdf"));
    RObject* pobj = mrb_obj_ptr(obj);
    mrb_obj_iv_set(mrb, pobj, sym, mrb_fixnum_value(1244));
    return mrb_obj_iv_get(mrb, pobj, sym);
}
