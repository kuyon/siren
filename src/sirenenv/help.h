/**
 * \brief Help string structure
 * THIS FILE IS PART OF *SIREN* SOURCE CODES.
 * WHEN YOU WANT THE LICENSE OF THIS FILE, YOU CAN GET IT
 * IN README OR LICENSE DOCMENTATION OF THIS PROJECT.
 * ---------------------------------------------------------
 * \author dyama <dyama@member.fsf.org>
 */

#ifndef _HELP_H_
#define _HELP_H_

struct structHelp
{
	std::string* desc;
	std::string* usage;
};

static std::map<std::string, structHelp*> Help;

#endif /* _HELP_H_ */