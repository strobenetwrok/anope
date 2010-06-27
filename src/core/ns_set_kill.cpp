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

class CommandNSSetKill : public Command
{
 public:
	CommandNSSetKill(const ci::string &cname) : Command(cname, 1)
	{
	}

	CommandReturn Execute(User *u, const std::vector<ci::string> &params)
	{
		if (params[0] == "ON")
		{
			u->Account()->SetFlag(NI_KILLPROTECT);
			u->Account()->UnsetFlag(NI_KILL_QUICK);
			u->Account()->UnsetFlag(NI_KILL_IMMED);
			notice_lang(Config.s_NickServ, u, NICK_SET_KILL_ON);
		}
		else if (params[0] == "QUICK")
		{
			u->Account()->SetFlag(NI_KILLPROTECT);
			u->Account()->SetFlag(NI_KILL_QUICK);
			u->Account()->UnsetFlag(NI_KILL_IMMED);
			notice_lang(Config.s_NickServ, u, NICK_SET_KILL_QUICK);
		}
		else if (params[0] == "IMMED")
		{
			if (Config.NSAllowKillImmed)
			{
				u->Account()->SetFlag(NI_KILLPROTECT);
				u->Account()->SetFlag(NI_KILL_IMMED);
				u->Account()->UnsetFlag(NI_KILL_QUICK);
				notice_lang(Config.s_NickServ, u, NICK_SET_KILL_IMMED);
			}
			else
				notice_lang(Config.s_NickServ, u, NICK_SET_KILL_IMMED_DISABLED);
		}
		else if (params[0] == "OFF")
		{
			u->Account()->UnsetFlag(NI_KILLPROTECT);
			u->Account()->UnsetFlag(NI_KILL_QUICK);
			u->Account()->UnsetFlag(NI_KILL_IMMED);
			notice_lang(Config.s_NickServ, u, NICK_SET_KILL_OFF);
		}
		else
			this->OnSyntaxError(u, "KILL");

		return MOD_CONT;
	}

	bool OnHelp(User *u, const ci::string &)
	{
		notice_help(Config.s_NickServ, u, NICK_HELP_SET_KILL);
		return true;
	}

	void OnSyntaxError(User *u, const ci::string &)
	{
		syntax_error(Config.s_NickServ, u, "SET KILL", Config.NSAllowKillImmed ? NICK_SET_KILL_IMMED_SYNTAX : NICK_SET_KILL_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		notice_lang(Config.s_NickServ, u, NICK_HELP_CMD_SET_KILL);
	}
};

class CommandNSSASetKill : public Command
{
 public:
	CommandNSSASetKill(const ci::string &cname) : Command(cname, 2, 2, "nickserv/saset/kill")
	{
	}

	CommandReturn Execute(User *u, const std::vector<ci::string> &params)
	{
		NickCore *nc = findcore(params[0]);
		assert(nc);

		ci::string param = params[1];

		if (param == "ON")
		{
			nc->SetFlag(NI_KILLPROTECT);
			nc->UnsetFlag(NI_KILL_QUICK);
			nc->UnsetFlag(NI_KILL_IMMED);
			notice_lang(Config.s_NickServ, u, NICK_SASET_KILL_ON, nc->display);
		}
		else if (param == "QUICK")
		{
			nc->SetFlag(NI_KILLPROTECT);
			nc->SetFlag(NI_KILL_QUICK);
			nc->UnsetFlag(NI_KILL_IMMED);
			notice_lang(Config.s_NickServ, u, NICK_SASET_KILL_QUICK, nc->display);
		}
		else if (param == "IMMED")
		{
			if (Config.NSAllowKillImmed)
			{
				nc->SetFlag(NI_KILLPROTECT);
				nc->SetFlag(NI_KILL_IMMED);
				nc->UnsetFlag(NI_KILL_QUICK);
				notice_lang(Config.s_NickServ, u, NICK_SASET_KILL_IMMED, nc->display);
			}
			else
				notice_lang(Config.s_NickServ, u, NICK_SASET_KILL_IMMED_DISABLED);
		}
		else if (param == "OFF")
		{
			nc->UnsetFlag(NI_KILLPROTECT);
			nc->UnsetFlag(NI_KILL_QUICK);
			nc->UnsetFlag(NI_KILL_IMMED);
			notice_lang(Config.s_NickServ, u, NICK_SASET_KILL_OFF, nc->display);
		}
		else
			this->OnSyntaxError(u, "KILL");

		return MOD_CONT;
	}

	bool OnHelp(User *u, const ci::string &)
	{
		notice_help(Config.s_NickServ, u, NICK_HELP_SASET_KILL);
		return true;
	}

	void OnSyntaxError(User *u, const ci::string &)
	{
		syntax_error(Config.s_NickServ, u, "SASET KILL", Config.NSAllowKillImmed ? NICK_SASET_KILL_IMMED_SYNTAX : NICK_SASET_KILL_SYNTAX);
	}

	void OnServHelp(User *u)
	{
		notice_lang(Config.s_NickServ, u, NICK_HELP_CMD_SASET_KILL);
	}
};

class NSSetKill : public Module
{
 public:
	NSSetKill(const std::string &modname, const std::string &creator) : Module(modname, creator)
	{
		this->SetAuthor("Anope");
		this->SetType(CORE);

		Command *c = FindCommand(NickServ, "SET");
		if (c)
			c->AddSubcommand(new CommandNSSetKill("KILL"));

		c = FindCommand(NickServ, "SASET");
		if (c)
			c->AddSubcommand(new CommandNSSASetKill("KILL"));
	}

	~NSSetKill()
	{
		Command *c = FindCommand(NickServ, "SET");
		if (c)
			c->DelSubcommand("KILL");

		c = FindCommand(NickServ, "SASET");
		if (c)
			c->DelSubcommand("KILL");
	}
};

MODULE_INIT(NSSetKill)