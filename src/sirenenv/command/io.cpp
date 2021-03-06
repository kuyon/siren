/**
 * \brief The I/O operation commands
 * THIS FILE IS PART OF *SIREN* SOURCE CODES.
 * WHEN YOU WANT THE LICENSE OF THIS FILE, YOU CAN GET IT
 * IN README OR LICENSE DOCMENTATION OF THIS PROJECT.
 * ---------------------------------------------------------
 * \author dyama <dyama@member.fsf.org>
 */

#include "StdAfx.h"
#include "OCCViewer.h"

mrbcmddef(savebrep)
{
    mrb_value path, name;
	int argc = mrb_get_args(mrb, "SS", &path, &name);

	mrb_value result;

	Handle(AIS_Shape) hashape = OCCViewer::get(RSTRING_PTR(name));

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

mrbcmddef(loadbrep)
{
    mrb_value path, name;

	int argc = mrb_get_args(mrb, "S|S", &path, &name);

	TopoDS_Shape shape;
    BRep_Builder aBuilder;
    Standard_Boolean res = BRepTools::Read(shape, (Standard_CString)RSTRING_PTR(path), aBuilder);

	mrb_value result;

	if (res) {
		//if(AISContext->HasOpenedContext())
		//	AISContext->CloseLocalContext();
		const char* rname = OCCViewer::set(shape, mrb_string_p(name) ? RSTRING_PTR(name) : NULL);
		result = mrb_str_new(mrb, rname, strlen(rname));
	}
	else {
		static const char m[] = "Failed to load BRep file.";
        result = mrb_exc_new(mrb, E_ARGUMENT_ERROR, m, sizeof(m) - 1);
	}

	return result;
}