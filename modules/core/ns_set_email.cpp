/* NickServ core functions
 *
 * (C) 2003-2010 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church.
 */

/*************************************************************************/

#include "module.h"

class CommandNSSetEmail : public Command
{
 public:
	CommandNSSetEmail(const Anope::string &spermission = "") : Command("EMAIL", 1, 2, spermission)
	{
	}

	CommandReturn Execute(User *u, const std::vector<Anope::string> &params)
	{
		NickCore *nc = findcore(params[0]);
		if (!nc)
			throw CoreException("NULL nc in CommandNSSetEmail");

		Anope::string param = params.size() > 1 ? params[1] : "";

		if (param.empty() && Config->NSForceEmail)
		{
			u->SendMessage(NickServ, NICK_SET_EMAIL_UNSET_IMPOSSIBLE);
			return MOD_CONT;
		}
		else if (Config->NSSecureAdmins && u->Account() != nc && nc->IsServicesOper())
		{
			u->SendMessage(NickServ, ACCESS_DENIED);
			return MOD_CONT;
		}
		else if (!param.empty() && !MailValidate(param))
		{
			u->SendMessage(NickServ, MAIL_X_INVALID, param.c_str());
			return MOD_CONT;
		}

		if (!param.empty())
		{
			nc->email = param;
			u->SendMessage(NickServ, NICK_SASET_EMAIL_CHANGED, nc->display.c_str(), param.c_str());
		}
		else
		{
			nc->email.clear();
			u->SendMessage(NickServ, NICK_SASET_EMAIL_UNSET, nc->display.c_str());
		}

		return MOD_CONT;
	}

	bool OnHelp(User *u, const Anope::string &)
	{
		u->SendMessage(NickServ, NICK_HELP_SET_EMAIL);
		return true;
	}

	void OnServHelp(User *u)
	{
		u->SendMessage(NickServ, NICK_HELP_CMD_SET_EMAIL);
	}
};

class CommandNSSASetEmail : public CommandNSSetEmail
{
 public:
	CommandNSSASetEmail() : CommandNSSetEmail("nickserv/saset/email")
	{
	}

	bool OnHelp(User *u, const Anope::string &)
	{
		u->SendMessage(NickServ, NICK_HELP_SASET_EMAIL);
		return true;
	}

	void OnServHelp(User *u)
	{
		u->SendMessage(NickServ, NICK_HELP_CMD_SASET_EMAIL);
	}
};

class NSSetEmail : public Module
{
	CommandNSSetEmail commandnssetemail;
	CommandNSSASetEmail commandnssasetemail;

 public:
	NSSetEmail(const Anope::string &modname, const Anope::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		Command *c = FindCommand(NickServ, "SET");
		if (c)
			c->AddSubcommand(&commandnssetemail);

		c = FindCommand(NickServ, "SASET");
		if (c)
			c->AddSubcommand(&commandnssasetemail);
	}

	~NSSetEmail()
	{
		Command *c = FindCommand(NickServ, "SET");
		if (c)
			c->DelSubcommand(&commandnssetemail);

		c = FindCommand(NickServ, "SASET");
		if (c)
			c->DelSubcommand(&commandnssasetemail);
	}
};

MODULE_INIT(NSSetEmail)