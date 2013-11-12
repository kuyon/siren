/**
 * \brief The boolean operation commands
 * THIS FILE IS PART OF *SIREN* SOURCE CODES.
 * WHEN YOU WANT THE LICENSE OF THIS FILE, YOU CAN GET IT
 * IN README OR LICENSE DOCMENTATION OF THIS PROJECT.
 * ---------------------------------------------------------
 * \author dyama <dyama@member.fsf.org>
 */

#include "StdAfx.h"
#include "OCCViewer.h"

mrbcmddef(common)
{
	return mrb_nil_value();	
    mrb_value s1, s2, name;

	int argc = mrb_get_args(mrb, "SS|S", &s1, &s2, &name);

	Handle(AIS_Shape) haS1 = OCCViewer::get(RSTRING_PTR(s1));
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = OCCViewer::get(RSTRING_PTR(s2));
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
		const char* rname = OCCViewer::set(shape, mrb_string_p(name) ? RSTRING_PTR(name) : NULL);
		result = mrb_str_new(mrb, rname, strlen(rname));
	}
	else {
		static const char m[] = "Failed to fuse operation of boolean.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	return result;
}

mrbcmddef(cut)
{
	return mrb_nil_value();	
    mrb_value s1, s2, name;

	int argc = mrb_get_args(mrb, "SS|S", &s1, &s2, &name);

	Handle(AIS_Shape) haS1 = OCCViewer::get(RSTRING_PTR(s1));
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = OCCViewer::get(RSTRING_PTR(s2));
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
		const char* rname = OCCViewer::set(shape, mrb_string_p(name) ? RSTRING_PTR(name) : NULL);
		result = mrb_str_new(mrb, rname, strlen(rname));
	}
	else {
		static const char m[] = "Failed to fuse operation of boolean.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	return result;
}

mrbcmddef(fuse)
{
    mrb_value s1, s2, name;

	int argc = mrb_get_args(mrb, "SS|S", &s1, &s2, &name);

	Handle(AIS_Shape) haS1 = OCCViewer::get(RSTRING_PTR(s1));
	if (haS1.IsNull()) {
		static const char m[] = "No such object name of specified at first.";
        return mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	Handle(AIS_Shape) haS2 = OCCViewer::get(RSTRING_PTR(s2));
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
		const char* rname = OCCViewer::set(shape, mrb_string_p(name) ? RSTRING_PTR(name) : NULL);
		result = mrb_str_new(mrb, rname, strlen(rname));
	}
	else {
		static const char m[] = "Failed to fuse operation of boolean.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}
	
	return result;
}
